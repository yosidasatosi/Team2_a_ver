//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : GP11B341 吉田　知史
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TITLE		"data/TEXTURE/bg000.jpg"		// 読み込むテクスチャファイル名
#define	TEXTURE_TITLE_LOGO	"data/TEXTURE/cooltext324405949980514.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_LOGO_START	"data/TEXTURE/cooltext327070171852684.png"	// 読み込むテクスチャファイル名

#define	TEXTURE_LOGO_SOLO	"data/TEXTURE/cooltext327070646178839.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_LOGO_DUO	"data/TEXTURE/cooltext327070681684319.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_LOGO_EXIT	"data/TEXTURE/cooltext327070699165474.png"	// 読み込むテクスチャファイル名



//#define	TITLE_LOGO_POS_X		(320)		// タイトルロゴの位置(X座標)
//#define	TITLE_LOGO_POS_Y		(200)		// タイトルロゴの位置(Y座標)
//#define	TITLE_LOGO_WIDTH		(640)		// タイトルロゴの幅
//#define	TITLE_LOGO_HEIGHT		(240)		// タイトルロゴの高さ
//
//#define	START_POS_X				(400)		// スタートボタンの位置(X座標)
//#define	START_POS_Y				(720)		// スタートボタンの位置(Y座標)
//#define	START_WIDTH				(480)		// スタートボタンの幅
//#define	START_HEIGHT			(120)		// スタートボタンの高さ
//
//#define	SOLO_POS_X				(420)		// スタートボタンの位置(X座標)
//#define	SOLO_POS_Y				(620)		// スタートボタンの位置(Y座標)
//#define	SOLO_WIDTH				(400)		// スタートボタンの幅
//#define	SOLO_HEIGHT				(100)		// スタートボタンの高さ
//
//#define	DUO_POS_X				(420)		// スタートボタンの位置(X座標)
//#define	DUO_POS_Y				(720)		// スタートボタンの位置(Y座標)
//#define	DUO_WIDTH				(400)		// スタートボタンの幅
//#define	DUO_HEIGHT				(100)		// スタートボタンの高さ
//
//#define	EXIT_POS_X				(480)		// スタートボタンの位置(X座標)
//#define	EXIT_POS_Y				(820)		// スタートボタンの位置(Y座標)
//#define	EXIT_WIDTH				(280)		// スタートボタンの幅
//#define	EXIT_HEIGHT				(100)		// スタートボタンの高さ

#define	TITLE_LOGO_POS_X		(100)		// タイトルロゴの位置(X座標)
#define	TITLE_LOGO_POS_Y		(50)		// タイトルロゴの位置(Y座標)
#define	TITLE_LOGO_WIDTH		(600)		// タイトルロゴの幅
#define	TITLE_LOGO_HEIGHT		(200)		// タイトルロゴの高さ

#define	START_POS_X				(180)		// スタートボタンの位置(X座標)
#define	START_POS_Y				(400)		// スタートボタンの位置(Y座標)
#define	START_WIDTH				(440)		// スタートボタンの幅
#define	START_HEIGHT			(120)		// スタートボタンの高さ

#define	SOLO_POS_X				(200)		// スタートボタンの位置(X座標)
#define	SOLO_POS_Y				(300)		// スタートボタンの位置(Y座標)
#define	SOLO_WIDTH				(400)		// スタートボタンの幅
#define	SOLO_HEIGHT				(80)		// スタートボタンの高さ

#define	DUO_POS_X				(200)		// スタートボタンの位置(X座標)
#define	DUO_POS_Y				(400)		// スタートボタンの位置(Y座標)
#define	DUO_WIDTH				(400)		// スタートボタンの幅
#define	DUO_HEIGHT				(80)		// スタートボタンの高さ

#define	EXIT_POS_X				(260)		// スタートボタンの位置(X座標)
#define	EXIT_POS_Y				(500)		// スタートボタンの位置(Y座標)
#define	EXIT_WIDTH				(280)		// スタートボタンの幅
#define	EXIT_HEIGHT				(80)		// スタートボタンの高さ

#define	COUNT_APPERA_START		(60)		// スタートボタン出現までの時間
#define	INTERVAL_DISP_START		(60)		// スタートボタン点滅の時間

#define	COUNT_WAIT_DEMO			(60 * 5)	// デモまでの待ち時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice);
void SetColorTitleLogo(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureTitle = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitle = NULL;		// 頂点バッファインターフェースへのポインタ

LPDIRECT3DTEXTURE9		g_pD3DTextureTitleLogo = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTitleLogo = NULL;	// 頂点バッファインターフェースへのポインタ

LPDIRECT3DTEXTURE9		g_pD3DTextureSoloLogo = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffSoloLogo = NULL;	// 頂点バッファインターフェースへのポインタ

LPDIRECT3DTEXTURE9		g_pD3DTextureDuoLogo = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffDuoLogo = NULL;	// 頂点バッファインターフェースへのポインタ

LPDIRECT3DTEXTURE9		g_pD3DTextureExitLogo = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffExitLogo = NULL;	// 頂点バッファインターフェースへのポインタ

LPDIRECT3DTEXTURE9		g_pD3DTextureStart = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffStart = NULL;		// 頂点バッファインターフェースへのポインタ

int						g_nCountAppearStart = 0;		//
float					g_fAlphaLogo = 0.0f;			//
int						g_nCountDisp = 0;				//
bool					g_bDispStart = false;			//
int						g_nConutDemo = 0;				//
bool					sentaku = true;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nCountAppearStart = 0;
	g_fAlphaLogo = 0.0f;
	g_nCountDisp = 0;
	g_bDispStart = false;
	g_nConutDemo = 0;
	sentaku = true;

	// 頂点情報の作成
	MakeVertexTitle(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEXTURE_TITLE,				// ファイルの名前
								&g_pD3DTextureTitle);		// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEXTURE_TITLE_LOGO,			// ファイルの名前
								&g_pD3DTextureTitleLogo);	// 読み込むメモリー


	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEXTURE_LOGO_START,			// ファイルの名前
								&g_pD3DTextureStart);		// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_LOGO_SOLO,			// ファイルの名前
		&g_pD3DTextureSoloLogo);		// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_LOGO_DUO,			// ファイルの名前
		&g_pD3DTextureDuoLogo);		// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_LOGO_EXIT,			// ファイルの名前
		&g_pD3DTextureExitLogo);		// 読み込むメモリー

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	if(g_pD3DTextureTitle != NULL)
	{// テクスチャの開放
		g_pD3DTextureTitle->Release();
		g_pD3DTextureTitle = NULL;
	}

	if(g_pD3DVtxBuffTitle != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffTitle->Release();
		g_pD3DVtxBuffTitle = NULL;
	}

	if(g_pD3DTextureTitleLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureTitleLogo->Release();
		g_pD3DTextureTitleLogo = NULL;
	}

	if(g_pD3DVtxBuffTitleLogo != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffTitleLogo->Release();
		g_pD3DVtxBuffTitleLogo = NULL;
	}

	if(g_pD3DTextureStart != NULL)
	{// テクスチャの開放
		g_pD3DTextureStart->Release();
		g_pD3DTextureStart = NULL;
	}

	if(g_pD3DVtxBuffStart != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffStart->Release();
		g_pD3DVtxBuffStart = NULL;
	}



	if (g_pD3DTextureSoloLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureSoloLogo->Release();
		g_pD3DTextureSoloLogo = NULL;
	}

	if (g_pD3DVtxBuffSoloLogo != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffSoloLogo->Release();
		g_pD3DVtxBuffSoloLogo = NULL;
	}

	if (g_pD3DTextureDuoLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureDuoLogo->Release();
		g_pD3DTextureDuoLogo = NULL;
	}

	if (g_pD3DVtxBuffDuoLogo != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffDuoLogo->Release();
		g_pD3DVtxBuffDuoLogo = NULL;
	}

	if (g_pD3DTextureExitLogo != NULL)
	{// テクスチャの開放
		g_pD3DTextureExitLogo->Release();
		g_pD3DTextureExitLogo = NULL;
	}

	if (g_pD3DVtxBuffExitLogo != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffExitLogo->Release();
		g_pD3DVtxBuffExitLogo = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{

#if 0
	if(g_nCountAppearStart >= COUNT_APPERA_START)
	{
		g_nConutDemo++;
		if(g_nConutDemo > COUNT_WAIT_DEMO)
		{
			SetFade(FADE_OUT, MODE_TITLE);
		}
	}
#endif
	if (sentaku == true)
	{
		if (g_fAlphaLogo < 1.0f)
		{
			g_fAlphaLogo += 0.005f;
			if (g_fAlphaLogo >= 1.0f)
			{
				g_fAlphaLogo = 1.0f;
			}
			SetColorTitleLogo();
		}
		else
		{
			g_nCountAppearStart++;
			if (g_nCountAppearStart > COUNT_APPERA_START)
			{
				g_nCountDisp = (g_nCountDisp + 1) % 80;
				if (g_nCountDisp > INTERVAL_DISP_START)
				{
					g_bDispStart = false;
				}
				else
				{
					g_bDispStart = true;
				}
			}
		}
	}

	if (GetKeyboardTrigger(DIK_RETURN))
	{
		if (sentaku)
		{
			g_bDispStart = false;
			sentaku = false;
		}
		else
		{
			SetFade(FADE_OUT);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//// 頂点バッファをデバイスのデータストリームにバインド
 //   pDevice->SetStreamSource(0, g_pD3DVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//// 頂点フォーマットの設定
	//pDevice->SetFVF(FVF_VERTEX_2D);

	//// テクスチャの設定
	//pDevice->SetTexture(0, g_pD3DTextureTitle);

	//// ポリゴンの描画
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pD3DVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureTitleLogo);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	if(g_bDispStart == true)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffStart, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureStart);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}


	if (sentaku == false)
	{

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffSoloLogo, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureSoloLogo);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);



		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffDuoLogo, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureDuoLogo);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);



		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pD3DVtxBuffExitLogo, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureExitLogo);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffTitle,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffTitle->Unlock();
	}





	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffTitleLogo,	// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y + TITLE_LOGO_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(TITLE_LOGO_POS_X + TITLE_LOGO_WIDTH, TITLE_LOGO_POS_Y + TITLE_LOGO_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffTitleLogo->Unlock();
	}


	// オブジェクトの頂点バッファを生成
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
												FVF_VERTEX_2D,				// 使用する頂点フォーマット
												D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffStart,		// 頂点バッファインターフェースへのポインタ
												NULL)))						// NULLに設定
	{
        return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(START_POS_X, START_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(START_POS_X, START_POS_Y + START_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(START_POS_X + START_WIDTH, START_POS_Y + START_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffStart->Unlock();
	}










	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffSoloLogo,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffSoloLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(SOLO_POS_X, SOLO_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SOLO_POS_X + SOLO_WIDTH, SOLO_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(SOLO_POS_X, SOLO_POS_Y + SOLO_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SOLO_POS_X + SOLO_WIDTH, SOLO_POS_Y + SOLO_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffSoloLogo->Unlock();
	}




	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffDuoLogo,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffDuoLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(DUO_POS_X, DUO_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(DUO_POS_X + DUO_WIDTH, DUO_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(DUO_POS_X, DUO_POS_Y + DUO_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(DUO_POS_X + DUO_WIDTH, DUO_POS_Y + DUO_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffDuoLogo->Unlock();
	}




	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffExitLogo,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffExitLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(EXIT_POS_X, EXIT_POS_Y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(EXIT_POS_X + EXIT_WIDTH, EXIT_POS_Y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(EXIT_POS_X, EXIT_POS_Y + EXIT_HEIGHT, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(EXIT_POS_X + EXIT_WIDTH, EXIT_POS_Y + EXIT_HEIGHT, 0.0f);

		// テクスチャのパースペクティブコレクト用
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffExitLogo->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点の作成
//=============================================================================
void SetColorTitleLogo(void)
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaLogo);

		// 頂点データをアンロックする
		g_pD3DVtxBuffTitleLogo->Unlock();
	}

}

