//=============================================================================
//
// モデル処理 [Player.cpp]
// Author : 
//
//=============================================================================
#include "star1.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_STAR1				("data/TEXTURE/star.png")				// 使用するテクスチャ

#define STAR1_FALLSPEED				(0.4f)									// 落下速度

#define STAR1_POS_X					(0.0f)									// 位置(X)
#define STAR1_POS_Y					(60.0f)									// 位置(Y)
#define STAR1_POS_Z					(-10.0f)									// 位置(Z)

#define STAR1_SIZE					(20.0f)									// テクスチャサイズ

#define STAR1_POS_X_RANGE_MAX		(80)									// 表示範囲(X)
#define STAR1_POS_X_RANGE_MID		(STAR1_POS_X_RANGE_MAX / 2)				// 表示範囲の中心(X)

#define STAR1_POS_Y_RANGE_MAX		(150)									// 表示範囲(Y)
#define STAR1_POS_Y_RANGE_MID		(STAR1_POS_Y_RANGE_MAX / 2)				// 表示範囲の中心(Y)

#define STAR1_POS_Y_CEIL			(STAR1_POS_Y + STAR1_POS_Y_RANGE_MID)	// 表示範囲の上限(Y)
#define STAR1_POS_Y_LOW				(STAR1_POS_Y - STAR1_POS_Y_RANGE_MID)	// 表示範囲の下限(Y)

#define STAR1_ROTSPEED_RAND_MAX		(300 + 1)								// 回転速度ランダム最大数
#define STAR1_ROTSPEED_RAND_ADD		(200)									// 回転速度ランダムに加算する数字

#define STAR1_COLOR_RAND_MAX		(20 + 1)								// 色ランダム最大数
#define STAR1_COLOR_RAND_ADD		(235)									// 色ランダムに加算する数字

#define STAR1_USE_RAND_MAX			(120)									// useをtrueにする確率

#define STAR1_FRAME_ANGLE			(D3DX_PI / (FRAME_MAX * STAR1_SECN * 2))		// 1フレームに増やす値

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexStar1(LPDIRECT3DDEVICE9 pDevice, int no);
void	SetColorStar1(STAR1_SEC *sec);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureStar1 = NULL;

STAR1_BASE			star1BaseWk[STAR1_BASE_MAX];		// 構造体
STAR1_SEC			star1SecWk[STAR1_SEC_MAX];			// 構造体

D3DXMATRIX			g_mtxWorldStar1;					// ワールドマトリックス

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitStar1(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	STAR1_BASE *base = GetStar1Base(0);
	STAR1_SEC *sec = GetStar1Sec(0);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
		TEXTURE_STAR1,					// ファイルの名前
		&g_pD3DTextureStar1);					// 読み込むメモリー

	// パラメータの設定
	for (int i = 0; i < STAR1_BASE_MAX; i++, base++)
	{
		if ((rand() % STAR1_USE_RAND_MAX) == 0)
		{
			base->use = true;
		}
		else
		{
			base->use = false;
		}
		base->pos = D3DXVECTOR3(STAR1_POS_X, STAR1_POS_Y, STAR1_POS_Z);
		base->pos.x += (rand() % STAR1_POS_X_RANGE_MAX - STAR1_POS_X_RANGE_MID);
		base->pos.y += (rand() % STAR1_POS_Y_RANGE_MAX - STAR1_POS_Y_RANGE_MID);
		base->secNo = (i / STAR1_SECN);
	}

	for (int i = 0; i < STAR1_SEC_MAX; i++, sec++)
	{
		sec->pD3DVtxBuff = NULL;

		sec->col = D3DCOLOR_RGBA(rand() % STAR1_COLOR_RAND_MAX + STAR1_COLOR_RAND_ADD, 0, 0, 0);
		sec->col.g = sec->col.r;
		sec->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		sec->size = STAR1_SIZE;
		sec->angle = /*(rand() % 314) / 100.0f*/1.0f;
		sec->rotSpd = 1.0f / (rand() % STAR1_ROTSPEED_RAND_MAX + STAR1_ROTSPEED_RAND_ADD);
		sec->alphaSpd = /*STAR1_FRAME_ANGLE*/0.0f;

		// 頂点フォーマット(中心座標)
		MakeVertexStar1(pDevice, i);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStar1(void)
{
	STAR1_SEC *sec = GetStar1Sec(0);

	if (g_pD3DTextureStar1 != NULL)
	{// テクスチャの開放
		g_pD3DTextureStar1->Release();
		g_pD3DTextureStar1 = NULL;
	}

	for (int i = 0; i < STAR1_SEC_MAX; i++, sec++)
	{
		if (sec->pD3DVtxBuff != NULL)
		{// 頂点バッファの開放
			sec->pD3DVtxBuff->Release();
			sec->pD3DVtxBuff = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateStar1(void)
{
	STAR1_BASE *base = GetStar1Base(0);

	for (int i = 0; i < STAR1_BASE_MAX; i++, base++)
	{
		STAR1_SEC *sec = GetStar1Sec(base->secNo);

		base->pos.y -= STAR1_FALLSPEED;
		if (base->pos.y < STAR1_POS_Y_LOW)
		{
			base->pos.y = STAR1_POS_Y_CEIL;
		}

		sec->rot.z += sec->rotSpd;
		if (sec->rot.z > D3DX_PI)
		{
			sec->rot.z -= D3DX_PI * 2.0f;
		}
		if (sec->rot.z < -D3DX_PI)
		{
			sec->rot.z += D3DX_PI * 2.0f;
		}

		if (!base->use)
		{
			if ((rand() % STAR1_USE_RAND_MAX) == 0)
			{
				base->use = true;
			}
		}
		
		if (base->use)
		{
			sec->angle += sec->alphaSpd;

			if (sec->angle > D3DX_PI * 2)
			{
				sec->angle = 0.0f;
				base->use = false;
			}

			// 正弦波で点滅処理
			sec->col.a = sinf(sec->angle);

			// カラーの更新
			SetColorStar1(sec);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawStar1(void)
{
	STAR1_BASE *base = GetStar1Base(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale,mtxRot, mtxTranslate;

	for (int i = 0; i < STAR1_BASE_MAX; i++, base++)
	{
		if (base->use)
		{
			STAR1_SEC *sec = GetStar1Sec(base->secNo);

			// ラインティングを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// ビューマトリックスを取得
			mtxView = GetMtxView();

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldStar1);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, sec->rot.y, sec->rot.x, sec->rot.z);
			D3DXMatrixMultiply(&g_mtxWorldStar1, &g_mtxWorldStar1, &mtxRot);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, base->pos.x,
				base->pos.y,
				base->pos.z);
			D3DXMatrixMultiply(&g_mtxWorldStar1,
				&g_mtxWorldStar1, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldStar1);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, sec->pD3DVtxBuff, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureStar1);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

			// ラインティングを有効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexStar1(LPDIRECT3DDEVICE9 pDevice, int no)
{
	STAR1_SEC *sec = GetStar1Sec(no);

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&sec->pD3DVtxBuff,			// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		sec->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-sec->size / 2, sec->size / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(sec->size / 2, sec->size / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-sec->size / 2, -sec->size / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(sec->size / 2, -sec->size / 2, 0.0f);

		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		SetColorStar1(sec);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		sec->pD3DVtxBuff->Unlock();
	}

	return S_OK;
}

void SetColorStar1(STAR1_SEC *sec)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		sec->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].diffuse = sec->col;
		pVtx[1].diffuse = sec->col;
		pVtx[2].diffuse = sec->col;
		pVtx[3].diffuse = sec->col;

		// 頂点データをアンロックする
		sec->pD3DVtxBuff->Unlock();
	}
}

//=============================================================================
// 星のベース情報を取得
//=============================================================================
STAR1_BASE *GetStar1Base(int no)
{
	return (&star1BaseWk[no]);
}

//=============================================================================
// 星のSec情報を取得
//=============================================================================
STAR1_SEC *GetStar1Sec(int no)
{
	return (&star1SecWk[no]);
}

//=============================================================================
// α値の変化数を取得
//=============================================================================
float GetStar1AlphaSpd(void)
{
	STAR1_SEC *sec = GetStar1Sec(0);

	return sec->alphaSpd;
}

float GetStar1Angle(void)
{
	STAR1_SEC *sec = GetStar1Sec(0);

	return sec->angle;
}

float GetStar1Alpha(void)
{
	STAR1_SEC *sec = GetStar1Sec(0);

	return sec->col.a;
}