//=============================================================================
//
// メイン処理 [render.cpp]
// Author : 
//
//=============================================================================
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "player.h"

// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetBackGroundPolygon(int num);
void ChangeRenderTarget(int no);
void RecoverRenderTarget();

void InitRendertexture();
HRESULT InitBackGroundPolygon(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3DSURFACE9		pRenderDepthBuffer;
LPDIRECT3DTEXTURE9		pRenderTexture[PLAYER_MAX];

D3DVIEWPORT9			RenderViewport;

//*****************************************************************************
LPDIRECT3DSURFACE9		pDefaultRenderTargetSurface;
LPDIRECT3DSURFACE9		pDefaultDepthBufferSurface;
LPDIRECT3DSURFACE9		pRenderTextureSurface;
D3DVIEWPORT9			DefaultViewport;

LPDIRECT3DTEXTURE9		g_pD3DBackGround = NULL;	// テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBackGround[PLAYER_MAX] = { NULL, NULL };	// 頂点バッファへのポインタ


//=============================================================================
// レンダリングテクスチャ作成処理
//=============================================================================
void InitRendertexture()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//レンダリングテクスチャ用ビューポート
	RenderViewport.X = 0;
	RenderViewport.Y = 0;
	RenderViewport.Width = SCREEN_WIDTH;
	RenderViewport.Height = SCREEN_HEIGHT;
	RenderViewport.MinZ = 0.0f;
	RenderViewport.MaxZ = 1.0f;

	//デプスバッファ作成(2のべき乗サイズ)
	pDevice->CreateDepthStencilSurface(1024, 1024,		//サイズ
		D3DFMT_D24S8,	//ピクセルフォーマット
		D3DMULTISAMPLE_NONE,//marutisanpuringu OFF
		0,
		FALSE,
		&pRenderDepthBuffer,
		NULL);

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		//レンダーターゲット作成(2のべき乗サイズ)
		D3DXCreateTexture(pDevice,
			SCREEN_WIDTH, 1024,		//サイズ
			1,				//みっぷマップ数
			D3DUSAGE_RENDERTARGET,	//siyou目的
			D3DFMT_X8R8G8B8,
			D3DPOOL_DEFAULT,
			&pRenderTexture[i]);
	}
}

//=============================================================================
// レンダーターゲット切り替え処理
//=============================================================================
void ChangeRenderTarget(int no)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

	//レンダーターゲットを切り替え

	//デフォルトビューポート保存
	pD3DDevice->GetViewport(&DefaultViewport);

	//デフォルトのサーフェースを保存
	pD3DDevice->GetRenderTarget(0, &pDefaultRenderTargetSurface);
	pD3DDevice->GetDepthStencilSurface(&pDefaultDepthBufferSurface);
	//レンダリングテクスチャのサーフェース取得
	pRenderTexture[no]->GetSurfaceLevel(0, &pRenderTextureSurface);

	//新しいレンダリングターゲットをセット
	pD3DDevice->SetRenderTarget(0, pRenderTextureSurface);
	//新しいデプスバッファをセット
	pD3DDevice->SetDepthStencilSurface(pRenderDepthBuffer);

	//ビューポートの切り替え
	pD3DDevice->SetViewport(&RenderViewport);

	//クリア
	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
}

//=============================================================================
// レンダーターゲット復帰処理
//=============================================================================
void RecoverRenderTarget()
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

	//レンダーターゲットの復帰
	pD3DDevice->SetRenderTarget(0, pDefaultRenderTargetSurface);
	pD3DDevice->SetDepthStencilSurface(pDefaultDepthBufferSurface);

	//サーフェースの解放
	pDefaultRenderTargetSurface->Release();
	pDefaultDepthBufferSurface->Release();
	pRenderTextureSurface->Release();

	//ビューポートの復帰
	pD3DDevice->SetViewport(&DefaultViewport);
}

//=============================================================================
// レンダリングテクスチャ背景初期化
//=============================================================================
HRESULT	InitBackGroundPolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// オブジェクトの頂点バッファを生成
		pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,		// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_2D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&g_pD3DVtxBuffBackGround[i],	// 頂点バッファインターフェースへのポインタ
			NULL);						// NULLに設定

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_pD3DVtxBuffBackGround[i]->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(0.0f, 1024, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, 1024, 0.0f);

			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;


			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);

			// 頂点データをアンロックする
			g_pD3DVtxBuffBackGround[i]->Unlock();
		}
	}

	return S_OK;
}

//=============================================================================
// レンダリングテクスチャ背景終了処理
//=============================================================================
void	UninitBackGroundPolygon()
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_pD3DVtxBuffBackGround[i] != NULL)
		{// 頂点バッファの開放
			g_pD3DVtxBuffBackGround[i]->Release();
			g_pD3DVtxBuffBackGround[i] = NULL;
		}
	}
}

//=============================================================================
// レンダリングテクスチャ背景描画
//=============================================================================
void DrawBackGroundPolygon(int no)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

	// 頂点フォーマットの設定
	pD3DDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pD3DDevice->SetTexture(0, pRenderTexture[no]);

	//
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 頂点バッファをデバイスのデータストリームにバインド
	pD3DDevice->SetStreamSource(0, g_pD3DVtxBuffBackGround[no], 0, sizeof(VERTEX_2D));

	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ポリゴンの描画
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void InitRender(void)
{
	//レンダリングテクスチャ関連
	InitBackGroundPolygon();
	InitRendertexture();
}

void SetRender(int num, int no)
{
	if (num == 1)
	{ // カメラの使用数が1のとき
		SetBackGroundPolygon(no);
	}
	else
	{
		SetBackGroundPolygon(no + 1);
	}

	//レンダリングターゲットの切り替え
	ChangeRenderTarget(no);
}

void DrawRender(int no, bool flag)
{
	//レンダリングターゲットの復帰
	RecoverRenderTarget();

	if (flag)
	{
		//レンダリングした背景を表示
		DrawBackGroundPolygon(no);
	}
}

//=============================================================================
// レンダリングテクスチャ背景設定
//=============================================================================
void SetBackGroundPolygon(int num)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	int no = 0;		// 配列のナンバーの初期化

	if (num != 0)
	{// カメラナンバーが0じゃないとき
		no = num - 1;	// 配列のナンバーを設定
	}

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffBackGround[no]->Lock(0, 0, (void**)&pVtx, 0);

	switch (num)
	{
	case 0:
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, 1024.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, 1024.0f, 0.0f);
		break;
	case 1:
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_CENTER_X, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, 1024.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_CENTER_X, 1024.0f, 0.0f);
		break;
	case 2:
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(SCREEN_CENTER_X, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(SCREEN_CENTER_X, 1024.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, 1024.0f, 0.0f);
		break;
	}

	// 頂点データをアンロックする
	g_pD3DVtxBuffBackGround[no]->Unlock();
}