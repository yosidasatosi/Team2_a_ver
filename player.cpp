//=============================================================================
//
// モデル処理 [Player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "camera.h"
#include "input.h"
#include "physic.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_CAR			"data/MODEL/car000.x"	// 読み込むモデルファイル名
#define	VALUE_MOVE_MODEL	(5.0f)					// 移動速度
#define	VALUE_ROTATE_MODEL	(D3DX_PI * 0.02f)		// 回転速度
#define	PLAYER_SIZE_X		(20.0f)							// ビルボードの幅
#define	PLAYER_SIZE_Y		(20.0f)							// ビルボードの高さ
#define	TEXTURE_PLAYER		"data/TEXTURE/hasiri.png"	// 読み込むテクスチャファイル名
#define TEXTURE_PATTERN_DIVIDE_X	(4)	// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)	// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define TIME_ANIMATION				(4)	// アニメーションの切り替わるカウント

#define DEFAULT_POS		(500.0f)	// プレイヤーの初期位置(1P)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice, int no);
void SetTexturePlayer(int no, int cntPattern);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
PLAYER				playerWk[PLAYER_MAX];		// 構造体

D3DXMATRIX			g_mtxWorldPlayer;			// ワールドマトリックス
int					g_player;					// プレイヤーナンバー

bool				g_update_x[PLAYER_MAX];
bool				g_update_z[PLAYER_MAX];

bool				g_clear;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		player->pD3DTexture = NULL;
		player->g_pD3DVtxBuff = NULL;

		// 頂点情報の作成
		MakeVertexPlayer(pDevice,i);

		// 大きさ、位置、向きの初期設定
		player->pos = D3DXVECTOR3(-DEFAULT_POS + i * (DEFAULT_POS * 2), 10.0f, 0.0f);
		player->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		player->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		
		player->Turn = false;

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
			TEXTURE_PLAYER,		// ファイルの名前
									&player->pD3DTexture);	// 読み込むメモリー

	}

	g_player = 0;

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_update_x[i] = false;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->pD3DTexture != NULL)
		{// テクスチャの開放
			player->pD3DTexture->Release();
			player->pD3DTexture = NULL;
		}

		if (player->g_pD3DVtxBuff != NULL)
		{// 頂点バッファの開放
			player->g_pD3DVtxBuff->Release();
			player->g_pD3DVtxBuff = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	//if (GetKeyboardTrigger(DIK_F2))
	//{
	//	g_player++;
	//	g_player %= PLAYER_MAX;
	//}

	//PLAYER *player = GetPlayer(g_player);

	PLAYER *player1 = GetPlayer(0);
	PLAYER *player2 = GetPlayer(1);
	D3DXVECTOR3 rotCamera;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		PLAYER *player = GetPlayer(i);
		// アニメーション
		player->CountAnim++;
		if ((player->CountAnim % TIME_ANIMATION) == 0)
		{
			// パターンの切り替え
			player->PatternAnim = (player->PatternAnim + 1) % ANIM_PATTERN_NUM;

			// テクスチャ座標を設定
			SetTexturePlayer(i, player->PatternAnim);
		}

	}
	// カメラの向き取得
	rotCamera = GetRotCamera(0);

	// プレイヤー1のX更新
	if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D))
	{
		g_update_x[0] = true;
	}
	// プレイヤー1のZ更新
	if (GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S))
	{
		g_update_z[0] = true;
	}

	// プレイヤー2のX更新
	if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT))
	{
		g_update_x[1] = true;
	}
	// プレイヤー2のZ更新
	if (GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
	{
		g_update_z[1] = true;
	}

	// プレイヤー1移動処理
	if(GetKeyboardPress(DIK_D))
	{// 右移動
		player1->pos.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_MODEL;
		player1->pos.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_MODEL;
		player1->sayuu = true;
	}
	else if (GetKeyboardPress(DIK_A))
	{// 左移動
		player1->pos.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_MODEL;
		player1->pos.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_MODEL;
		player1->sayuu = false;
	}
	else if (GetKeyboardPress(DIK_W))
	{// 奥移動
		player1->pos.z += VALUE_MOVE_MODEL;
	}
	else if (GetKeyboardPress(DIK_S))
	{// 手前移動
		player1->pos.z -= VALUE_MOVE_MODEL;
	}

	// プレイヤー1
	if (player1->jump == true)
	{// ジャンプ

		FreeFallMotion(&player1->pos.y, &player1->speed, &player1->a);

		if (player1->pos.y < 10)
		{
			player1->jump = false;
			player1->pos.y = 10;

		}

	}
	else
	{
		if (GetKeyboardTrigger(DIK_SPACE))
		{

			player1->jump = true;
			player1->speed = 500;
			player1->a = -9.8f * 3;
		}
	}



	if (GetKeyboardPress(DIK_RIGHT))
	{
		player2->pos.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_MODEL;
		player2->sayuu = true;

	}
	else if (GetKeyboardPress(DIK_LEFT))
	{
		player2->pos.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_MODEL;
		player2->sayuu = false;

	}
	else if (GetKeyboardPress(DIK_UP))
	{
		player2->pos.z += VALUE_MOVE_MODEL;


	}
	else if (GetKeyboardPress(DIK_DOWN))
	{
		player2->pos.z -= VALUE_MOVE_MODEL;

	}

	if (player2->jump == true)
	{

		FreeFallMotion(&player2->pos.y, &player2->speed, &player2->a);

		if (player2->pos.y < 10)
		{
			player2->jump = false;
			player2->pos.y = 10;

		}

	}
	else
	{
		if (GetKeyboardTrigger(DIK_C))
		{

			player2->jump = true;
			player2->speed = 500;
			player2->a = -9.8f * 3;
		}
	}


	if (!GetGouryu())
	{
		if (GetKeyboardTrigger(DIK_1))
		{
			if (player1->Turn)
			{
				player1->Turn = false;
			}
			else
			{
				player1->Turn = true;
			}
			TurnCamera(1);
			SetCameraZ(1);
		}

		if (GetKeyboardTrigger(DIK_2))
		{
			if (player2->Turn)
			{
				player2->Turn = false;
			}
			else
			{
				player2->Turn = true;
			}
			TurnCamera(2);
			SetCameraZ(2);
		}
		SetViewport();
	}
	
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_update_x[i])
		{
			SetCameraX(0);
			SetCameraX(i + 1);
			g_update_x[i] = false;
		}
		if (g_update_z[i])
		{
			SetCameraZ(0);
			SetCameraZ(i + 1);
			g_update_z[i] = false;
		}
	}

	if (CheckTurn())
	{
		if (CheckClear())
		{
			SetFade(FADE_OUT);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	PLAYER *player = GetPlayer(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		// ラインティングを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// ビューマトリックスを取得
		mtxView = GetMtxView();

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldPlayer);

		// ポリゴンを正面に向ける
		g_mtxWorldPlayer._11 = mtxView._11;
		g_mtxWorldPlayer._12 = mtxView._21;
		g_mtxWorldPlayer._13 = mtxView._31;
		g_mtxWorldPlayer._21 = mtxView._12;
		g_mtxWorldPlayer._22 = mtxView._22;
		g_mtxWorldPlayer._23 = mtxView._32;
		g_mtxWorldPlayer._31 = mtxView._13;
		g_mtxWorldPlayer._32 = mtxView._23;
		g_mtxWorldPlayer._33 = mtxView._33;




		// スケールを反映
		D3DXMatrixScaling(&mtxScale, player->scl.x,
			player->scl.y,
			player->scl.z);
		D3DXMatrixMultiply(&g_mtxWorldPlayer,
			&g_mtxWorldPlayer, &mtxScale);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, player->pos.x,
			player->pos.y,
			player->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldPlayer,
			&g_mtxWorldPlayer, &mtxTranslate);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPlayer);

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, player->g_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, player->pD3DTexture);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		
		// ラインティングを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice ,int no)
{

	PLAYER *player = GetPlayer(no);

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&player->g_pD3DVtxBuff,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		player->g_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-PLAYER_SIZE_X / 2, PLAYER_SIZE_Y / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(PLAYER_SIZE_X / 2, PLAYER_SIZE_Y / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-PLAYER_SIZE_X / 2, -PLAYER_SIZE_Y / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(PLAYER_SIZE_X / 2, -PLAYER_SIZE_Y / 2, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 頂点データをアンロックする
		player->g_pD3DVtxBuff->Unlock();
	}

	return S_OK;
}
//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void SetTexturePlayer(int no, int cntPattern)
{
	PLAYER *player = &playerWk[no];

	// テクスチャ座標の設定
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		player->g_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		if (player->sayuu)
		{
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		else
		{

			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		}

		// 頂点データをアンロックする
		player->g_pD3DVtxBuff->Unlock();

	}
}

bool CheckClear(void)
{
	PLAYER *player1 = GetPlayer(0);
	PLAYER *player2 = GetPlayer(1);

	g_clear = CheckHitBC(player1->pos, player2->pos, PLAYER_SIZE_X / 2, PLAYER_SIZE_X / 2);

	return g_clear;
}

//=============================================================================
// プレイヤーのTurnの取得
//=============================================================================
bool GetTurn(int no)
{
	PLAYER *player = GetPlayer(no);
	return player->Turn;
}

//*****************************************************************************
// プレイヤーゲット関数
//*****************************************************************************
PLAYER *GetPlayer(int no)
{
	return (&playerWk[no]);
}

bool GetClear(void)
{
	return g_clear;
}