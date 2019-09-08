//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : chanp
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CAM_POS_P_X			(0.0f)											// カメラの視点初期位置(X座標)
#define	CAM_POS_P_Y			(50.0f)										// カメラの視点初期位置(Y座標)
#define	CAM_POS_P_Z			(-200.0f)										// カメラの視点初期位置(Z座標)
#define	CAM_POS_R_X			(0.0f)											// カメラの注視点初期位置(X座標)
#define	CAM_POS_R_Y			(0.0f)											// カメラの注視点初期位置(Y座標)
#define	CAM_POS_R_Z			(0.0f)											// カメラの注視点初期位置(Z座標)
#define	VIEW_ANGLE			(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比
#define	VIEW_NEAR_Z			(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z			(10000.0f)										// ビュー平面のFarZ値
#define	VALUE_MOVE_CAMERA	(2.0f)											// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)								// カメラの回転量

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct COLOR {
	int r;
	int g;
	int b;
};

struct VIEW {
	D3DXVECTOR3 vEyePt;
	D3DXVECTOR3 vLookatPt;
	D3DXVECTOR3 vUpVec;
	D3DXMATRIXA16 matView;
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
float SetAspect(void);

void SetCamera0PosAt(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DXMATRIX		g_mtxView;					// ビューマトリックス
D3DXMATRIX		g_mtxProjection;			// プロジェクションマトリックス

float			g_Aspect;					// カメラのアスペクト比
int				g_camera;					// 使用するカメラの変更

int				g_cameramax;				// 画面に同時描画するカメラの最大数
bool			g_gouryu;					// true:1つ　false:2つ (使用するカメラ)
bool			g_up;						// true:処理に入る　false:処理に入らない

CAMERA cameraWk[CAMERA_MAX];

COLOR g_color[] = {
	{ 20, 60, 20 },
{ 20, 60, 20 },
};

D3DVIEWPORT9 g_port[PLAYER_MAX];
VIEW g_view[PLAYER_MAX];

//=============================================================================
// カメラの初期化処理
//=============================================================================
HRESULT InitCamera(void)
{
	CAMERA *camera = GetCamera(0);

	// カメラの最大数分の初期化
	for (int i = 0; i < CAMERA_MAX; i++, camera++)
	{
		if (i == 0)
		{ // カメラ0(分割しない時用カメラ)の設定
			camera->posEye = D3DXVECTOR3(GetPlayer(1)->pos.x + GetPlayer(0)->pos.x, CAM_POS_P_Y, CAM_POS_P_Z);
			camera->posAt = D3DXVECTOR3(camera->posEye.x, 0.0f, 0.0f);
		}
		else
		{ // カメラ1・2(各プレイヤー用カメラ)の設定
			camera->posEye = D3DXVECTOR3(GetPlayer(i - 1)->pos.x, CAM_POS_P_Y, CAM_POS_P_Z);
			camera->posAt = GetPlayer(i - 1)->pos;
		}

		camera->vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		camera->rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	}

	// 各種グローバルへ数の初期化
	g_gouryu = false;
	g_up = false;
	g_camera = 0;
	g_cameramax = PLAYER_MAX;
	g_Aspect = SetAspect();

	return S_OK;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	PLAYER *player1 = GetPlayer(0);
	PLAYER *player2 = GetPlayer(1);

	if (player2->pos.x - player1->pos.x <= 200.0f && CheckTurn())
	{
		g_gouryu = true;
	}
	else
	{
		g_gouryu = false;
	}

	if (g_gouryu)
	{// カメラが一つならば
		SetCamera0PosAt();	// カメラ0の注視点を変更
	}

	if (!g_up)
	{	// 一度だけ処理を通す
		if (g_gouryu)
		{	// 合流時に行う処理
			g_cameramax = 1;			//カメラの使用最大数を1つにする

			g_Aspect = SetAspect();		// カメラのアスペクト比の更新

			SetViewport();				// ビューポートの更新

			SetCameraZ(0);

			g_up = true;				// この処理に入らないよう更新
		}
	}
	else
	{	// 一度だけ処理を通す
		if (!g_gouryu)
		{	// 合流後、離れた時に行う処理
			g_cameramax = 2;			// カメラの使用最大数を2つにする

			g_Aspect = SetAspect();		// カメラのアスペクト比の更新

			SetViewport();				// ビューポートの更新

			g_up = false;				// この処理に入らないよう更新
		}
	}
}

//=============================================================================
// 使用するカメラの設定処理
//=============================================================================
void SetUseCamera(int no)
{
	if (g_cameramax == 1)
	{ // 使用しているカメラが1台の時
		SetCamera(no);		// カメラの設定処理
	}
	else
	{ // 使用しているカメラが2台の時
		SetCamera(no + 1);	// カメラの設定処理
	}
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(int no)
{
	CAMERA *camera = GetCamera(no);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_mtxView,
		&camera->posEye,		// カメラの視点
		&camera->posAt,			// カメラの注視点
		&camera->vecUp);		// カメラの上方向

								// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
		VIEW_ANGLE,			// ビュー平面の視野角
		g_Aspect,			// ビュー平面のアスペクト比
		VIEW_NEAR_Z,		// ビュー平面のNearZ値
		VIEW_FAR_Z);		// ビュー平面のFarZ値

							// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);
}

//=============================================================================
// カメラのアスペクト比の設定
//=============================================================================
float SetAspect(void)
{
	float aspect;

	switch (g_cameramax)
	{
	case 1:
		aspect = ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
		break;
	case 2:
		aspect = ((float)SCREEN_CENTER_X / (float)SCREEN_HEIGHT);
		break;
	}

	return aspect;
}

//=============================================================================
// カメラの向きの取得
//=============================================================================
D3DXVECTOR3 GetRotCamera(int no)
{
	return GetCamera(no)->rot;
}

//=============================================================================
// カメラの位置(X)の設定
//=============================================================================
void SetCameraX(int num)
{
	if (num == 0)
	{
		SetCamera0PosAt();
	}
	else
	{
		PLAYER *player = GetPlayer(num - 1);
		CAMERA *camera = GetCamera(num);

		camera->posEye.x = player->pos.x;
		camera->posAt.x = player->pos.x;
	}
}

//=============================================================================
// カメラの位置(Z)の設定
//=============================================================================
void SetCameraZ(int num)
{
	PLAYER *player;
	CAMERA *camera;

	if (num == 0)
	{
		PLAYER *player1 = GetPlayer(0);
		PLAYER *player2 = GetPlayer(1);

		camera = GetCamera(num);

		if (player1->Turn)
		{
			if (player1->pos.z > player2->pos.z)
			{
				player = player1;
			}
			else
			{
				player = player2;
			}
		}
		else
		{
			if (player1->pos.z < player2->pos.z)
			{
				player = player1;
			}
			else
			{
				player = player2;
			}
		}
	}
	else
	{
		player = GetPlayer(num - 1);
		camera = GetCamera(num);

		num--;
	}

	//if (g_gouryu)
	//{
	//	PLAYER *player1 = GetPlayer(num);
	//	PLAYER *player2 = GetPlayer((num + 1) % 2);

	//	camera = GetCamera(0);

	//	if (player1->pos.z < player2->pos.z)
	//	{
	//		player = player1;
	//	}
	//	else
	//	{
	//		player = player2;
	//	}
	//}
	//else
	//{
	//	player = GetPlayer(num);
	//	camera = GetCamera(num + 1);
	//}

	if (GetTurn(num))
	{
		camera->posEye.z = player->pos.z - CAM_POS_P_Z;
		if (camera->posEye.z < -CAM_POS_P_Z)
		{
			camera->posEye.z = -CAM_POS_P_Z;
			camera->posAt.z = CAM_POS_R_Z;
		}
		else
		{
			camera->posAt.z = player->pos.z;
		}
	}
	else
	{
		camera->posEye.z = player->pos.z + CAM_POS_P_Z;
		if (camera->posEye.z > CAM_POS_P_Z)
		{
			camera->posEye.z = CAM_POS_P_Z;
			camera->posAt.z = CAM_POS_R_Z;
		}
		else
		{
			camera->posAt.z = player->pos.z;
		}
	}
}

//=============================================================================
// ビューマトリックスの取得
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	return g_mtxView;
}

//=============================================================================
// カメラの取得
//=============================================================================
CAMERA *GetCamera(int no)
{
	return (&cameraWk[no]);
}

//=============================================================================
// カメラのアスペクト比の取得
//=============================================================================
float GetAspect(void)
{
	float aspect;

	switch (g_cameramax)
	{
	case 2:
		aspect = ((float)SCREEN_CENTER_X / (float)SCREEN_HEIGHT);
		break;

	case 1:
	case 3:
	case 4:
		aspect = ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
		break;
	}

	return aspect;
}

//=============================================================================
// 使用中のカメラのナンバー取得処理
//=============================================================================
int GetCameraNum(void)
{
	return g_cameramax;
}

//=============================================================================
// カメラ0の位置の設定処理
//=============================================================================
void SetCamera0PosAt(void)
{
	PLAYER *player1 = GetPlayer(0);
	PLAYER *player2 = GetPlayer(1);

	CAMERA *camera = GetCamera(0);

	if (player1->pos.x * player2->pos.x > -1)
	{
		camera->posEye.x = (player2->pos.x + player1->pos.x) / 2;
		camera->posAt.x = (player2->pos.x + player1->pos.x) / 2;
	}
	else
	{
		float max, min;

		if (player2->pos.x > player1->pos.x * -1)
		{
			max = player2->pos.x;
			min = player1->pos.x;
		}
		else
		{
			max = player1->pos.x;
			min = player2->pos.x;
		}

		camera->posAt.x = max - (max - min) / 2;
		camera->posEye.x = max - (max - min) / 2;
	}
}
//=============================================================================
// カメラの位置(Z)を反転
//=============================================================================
void TurnCamera(int playerno)
{
	CAMERA *camera = GetCamera(playerno);

	camera->posEye.z *= -1.0f;
}

//=============================================================================
// 2人のプレイヤーが逆を向いているかどうか
//=============================================================================
bool CheckTurn(void)
{
	PLAYER *player1 = GetPlayer(0);
	PLAYER *player2 = GetPlayer(1);

	if (player1->Turn && player2->Turn || !(player1->Turn) && !(player2->Turn))
	{
		return true;
	}
	else
	{
		return false;
	}
}


//=============================================================================
// カメラが合流しているか取得
//=============================================================================
bool GetGouryu(void)
{
	return g_gouryu;
}

//=============================================================================
// ビューポート関連
//=============================================================================
void Viewport(int no)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

	// ビューポート領域を設定(変更)		
	pD3DDevice->SetViewport(&g_port[no]);

	// 描画領域にしたところを任意の色でクリア
	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(g_color[no].r, g_color[no].b, g_color[no].g), 1.0f, 0);
}

//=============================================================================
// ビューポート設定
//=============================================================================
void SetViewport(void)
{
	// 第1引数: X		左上の頂点(X)
	// 第2引数: Y		左上の頂点(Y)
	// 第3引数: Width	横幅
	// 第4引数: Height	縦幅
	// 第5引数: MinZ	基本0.0f
	// 第6引数: MaxZ	基本1.0f

	switch (GetCameraNum())
	{
	case 1:
		g_port[0] = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };

		break;

	case 2:
		bool player1_turn = GetTurn(0);
		bool player2_turn = GetTurn(1);

		if (GetTurn(0))
		{
			g_port[0] = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };
		}
		else
		{
			g_port[0] = { 0, 0, SCREEN_CENTER_X, SCREEN_HEIGHT, 0.0f, 1.0f };
		}

		if (GetTurn(1))
		{
			g_port[1] = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };
		}
		else
		{
			//g_port[1] = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };

			g_port[1] = { SCREEN_CENTER_X, 0, SCREEN_CENTER_X, SCREEN_HEIGHT, 0.0f, 1.0f };
		}

		break;
	}
}

D3DVIEWPORT9 *GetPort(int no)
{
	return &g_port[no];
}