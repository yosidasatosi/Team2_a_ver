//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CAM_POS_P_X			(0.0f)					// カメラの視点初期位置(X座標)
#define	CAM_POS_P_Y			(50.0f)				// カメラの視点初期位置(Y座標)
#define	CAM_POS_P_Z			(-200.0f)				// カメラの視点初期位置(Z座標)
#define	CAM_POS_R_X			(0.0f)					// カメラの注視点初期位置(X座標)
#define	CAM_POS_R_Y			(0.0f)					// カメラの注視点初期位置(Y座標)
#define	CAM_POS_R_Z			(0.0f)					// カメラの注視点初期位置(Z座標)
#define	VIEW_ANGLE			(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比
#define	VIEW_NEAR_Z			(30.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z			(2000.0f)										// ビュー平面のFarZ値
#define	VALUE_MOVE_CAMERA	(2.0f)											// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)								// カメラの回転量

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
float GetAspect(void);
float SetAspect(void);

void SetCamera0PosAt(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DXMATRIX		g_mtxView;					// ビューマトリックス
D3DXMATRIX		g_mtxProjection;			// プロジェクションマトリックス

D3DXMATRIX		g_m;

float			g_Aspect;					// カメラのアスペクト比
int				g_camera;					// 使用するカメラの変更

int				g_cameramax;				// 画面に同時描画するカメラの最大数
bool			g_gouryu;
bool			g_up;

CAMERA cameraWk[CAMERA_MAX];

//=============================================================================
// カメラの初期化処理
//=============================================================================
HRESULT InitCamera(void)
{
	CAMERA *camera = GetCamera(0);

	for (int i = 0; i < CAMERA_MAX; i++, camera++)
	{
		if (i == 0)
		{
			camera->posEye = D3DXVECTOR3(GetPlayer(1)->pos.x + GetPlayer(0)->pos.x, CAM_POS_P_Y, CAM_POS_P_Z);
			camera->posAt = D3DXVECTOR3(camera->posEye.x, 0.0f, 0.0f);
		}
		else
		{
			camera->posEye = D3DXVECTOR3(GetPlayer(i - 1)->pos.x, CAM_POS_P_Y, CAM_POS_P_Z);
			camera->posAt = GetPlayer(i - 1)->pos;
		}
		camera->vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		camera->rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

		float fVecX, fVecZ;
		fVecX = camera->posEye.x - camera->posAt.x;
		fVecZ = camera->posEye.z - camera->posAt.z;
		camera->fLengthInterval = sqrtf(fVecX * fVecX + fVecZ * fVecZ);
		camera->turn = false;
	}

	g_gouryu = false;
	g_up = false;
	g_camera = 0;
	g_cameramax = PLAYER_MAX;
	g_Aspect = GetAspect();

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

	if (CheakTurn())
	{
		if (player2->pos.x - player1->pos.x <= 200.0f)
		{
			g_gouryu = true;
		}
		else
		{
			g_gouryu = false;
		}
	}

	if (g_gouryu)
	{// 範囲内ならカメラの注視点を更新
		SetCamera0PosAt();
	}

	if (!g_up)
	{	// 一度だけ処理を通す
		if (g_gouryu)
		{	// 合流時に行う処理
			g_cameramax = 1;

			g_Aspect = SetAspect();

			SetViewport();

			g_up = true;
		}
	}
	else
	{	// 一度だけ処理を通す
		if (!g_gouryu)
		{	// 合流後、離れた時に行う処理
			g_cameramax = 2;

			g_Aspect = SetAspect();

			SetViewport();

			g_up = false;
		}
	}

	for (int i = 0; i < g_cameramax; i++)
	{
		CAMERA *camera = GetCamera(i + 1);

		if (camera->turn)
		{

		}
	}

	//if (GetKeyboardTrigger(DIK_F1))
	//{
	//	g_cameramax--;
	//	if (g_cameramax < 1)
	//	{
	//		g_cameramax = 2;
	//	}
	//	
	//	g_Aspect = SetAspect();

	//	SetViewport();
	//}

	//if (GetKeyboardTrigger(DIK_F1))
	//{
	//	g_camera++;
	//	g_camera %= PLAYER_MAX;
	//}

	//CAMERA *camera = GetCamera(g_camera + 1);

	////for (int i = 0; i < CAMERA_MAX; i++, camera++)
	////{
	//	if (GetKeyboardPress(DIK_LEFT))
	//	{// 視点旋回「左」
	//		camera->rot.y -= VALUE_ROTATE_CAMERA;
	//		if (camera->rot.y < -D3DX_PI)
	//		{
	//			camera->rot.y += D3DX_PI * 2.0f;
	//		}

	//		//camera->posEye.x = camera->posAt.x - sinf(camera->rot.y) * camera->fLengthInterval;
	//		//camera->posEye.z = camera->posAt.z - cosf(camera->rot.y) * camera->fLengthInterval;

	//		//camera->posAt.x = camera->posEye.x - sinf(camera->rot.y) * camera->fLengthInterval;
	//		//camera->posAt.z = camera->posEye.z - cosf(camera->rot.y) * camera->fLengthInterval;
	//	}
	//	if (GetKeyboardPress(DIK_RIGHT))
	//	{// 視点旋回「右」
	//		camera->rot.y += VALUE_ROTATE_CAMERA;
	//		if (camera->rot.y > D3DX_PI)
	//		{
	//			camera->rot.y -= D3DX_PI * 2.0f;
	//		}

	//		//camera->posAt.x = camera->posEye.x - sinf(camera->rot.y) * camera->fLengthInterval;
	//		//camera->posAt.z = camera->posEye.z - cosf(camera->rot.y) * camera->fLengthInterval;

	//		//camera->posEye.x = camera->posAt.x - sinf(camera->rot.y) * camera->fLengthInterval;
	//		//camera->posEye.z = camera->posAt.z - cosf(camera->rot.y) * camera->fLengthInterval;
	//	}

	//	//if (GetKeyboardPress(DIK_UP))
	//	//{// 注視点と視点との距離
	//	//	camera->fLengthInterval -= 0.5f;
	//	//	camera->posEye.x = camera->posAt.x - sinf(camera->rot.y) * camera->fLengthInterval;
	//	//	camera->posEye.z = camera->posAt.z - cosf(camera->rot.y) * camera->fLengthInterval;
	//	//}

	//	//if (GetKeyboardPress(DIK_DOWN))
	//	//{// 注視点と視点との距離
	//	//	camera->fLengthInterval += 0.5f;
	//	//	camera->posEye.x = camera->posAt.x - sinf(camera->rot.y) * camera->fLengthInterval;
	//	//	camera->posEye.z = camera->posAt.z - cosf(camera->rot.y) * camera->fLengthInterval;
	//	//}

	//	// 周りをまわる(カメラの位置が動く)
	//	//camera->posEye.x = camera->posAt.x - sinf(camera->rot.y) * camera->fLengthInterval;
	//	//camera->posEye.z = camera->posAt.z - cosf(camera->rot.y) * camera->fLengthInterval;

	//	// 周りを見渡す(カメラの注視点が動く)
	//	camera->posAt.x = camera->posEye.x - sinf(camera->rot.y) * camera->fLengthInterval;
	//	camera->posAt.z = camera->posEye.z - cosf(camera->rot.y) * camera->fLengthInterval;
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
// カメラの向きの取得
//=============================================================================
void SetCameraRot(void)
{
	D3DXVECTOR3 player_pos = GetPlayer(0)->pos;

	CAMERA *camera = GetCamera(0);
}

//=============================================================================
// カメラの位置・注視点の設定
//=============================================================================
void SetCameraPos(int num)
{
	PLAYER *player = GetPlayer(num);
	CAMERA *camera = GetCamera(num + 1);

	camera->posEye.x = player->pos.x;
	camera->posAt.x = player->pos.x;
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

int GetCameraNum(void)
{
	return g_cameramax;
}

void SetCamera0PosAt(void)
{
	PLAYER *player1 = GetPlayer(0);
	PLAYER *player2 = GetPlayer(1);

	CAMERA *camera = GetCamera(0);

	if (player1->Turn && player2->Turn)
	{
		camera->posEye.z = -CAM_POS_P_Z;
	}
	else
	{
		camera->posEye.z = CAM_POS_P_Z;
	}

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

void TurnCamera(int playerno)
{
	CAMERA *camera = GetCamera(playerno);

	camera->posEye.z *= -1.0f;
}

bool CheakTurn(void)
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

bool GetGouryu(void)
{
	return g_gouryu;
}