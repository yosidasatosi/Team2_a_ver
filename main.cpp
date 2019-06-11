//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 
//
//=============================================================================
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "light.h"
#include "camera.h"
#include "field.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"カメラ処理"		// ウインドウのキャプション名

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

void DrawDebug(void);

void SetBackGroundPolygon(int num);

HRESULT InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3Dオブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)
#ifdef _DEBUG
LPD3DXFONT			g_pD3DXFont = NULL;		// フォントへのポインタ
int					g_nCountFPS;			// FPSカウンタ
#endif
bool				g_bDispDebug = true;	// デバッグ表示ON/OFF

COLOR g_color[] = {
	{ 20, 60, 20 },
	{ 20, 60, 20 },
};

D3DVIEWPORT9 g_port[CAMERA_MAX];
VIEW g_view[CAMERA_MAX];

bool	g_sw = true;

// D3DVIEWPORT9
// 第1引数: X		左上の頂点(X)
// 第2引数: Y		左上の頂点(Y)
// 第3引数: Width	横幅
// 第4引数: Height	縦幅
// 第5引数: MinZ	基本0.0f
// 第6引数: MaxZ	基本1.0f

//****************************************************************************
//レンダリングテクスチャ用インターフェース等
//****************************************************************************

LPDIRECT3DSURFACE9		pRenderDepthBuffer;
LPDIRECT3DTEXTURE9		pRenderTexture[PLAYER_MAX];

D3DVIEWPORT9			RenderViewport;

LPDIRECT3DSURFACE9		pDefaultRenderTargetSurface;
LPDIRECT3DSURFACE9		pDefaultDepthBufferSurface;
LPDIRECT3DSURFACE9		pRenderTextureSurface;
D3DVIEWPORT9			DefaultViewport;

//背景ポリゴン
HRESULT InitBackGroundPolygon(void);
void UninitBackGroundPolygon(void);
void DrawBackGroundPolygon(int no);

LPDIRECT3DTEXTURE9		g_pD3DBackGround = NULL;	// テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBackGround[PLAYER_MAX] = { NULL, NULL };	// 頂点バッファへのポインタ

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND hWnd;
	MSG msg;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		NULL,
		NULL,
		hInstance,
		NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	dwExecLastTime =
		dwFPSLastTime = timeGetTime();
	dwCurrentTime =
		dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
#ifdef _DEBUG
				//PrintDebugProc("FPS:%d\n", g_nCountFPS);
#endif
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	timeEndPeriod(1);				// 分解能を戻す

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;						// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed = bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;				// デプスバッファとして16bitを使う

	if (bWindow)
	{// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダリングステートパラメータの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

																			// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

																			// テクスチャステージ加算合成処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

#ifdef _DEBUG
																			// 情報表示用フォントを設定
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DXFont);
#endif

	// 入力の初期化処理
	InitInput(hInstance, hWnd);

	// モデルの初期化処理
	InitPlayer();

	// ライトの初期化処理
	InitLight();

	// カメラの初期化処理
	InitCamera();

	// 地面の初期化処理
	InitField();

	// ビューポート設定
	SetViewport();

	//レンダリングテクスチャ関連
	InitBackGroundPolygon();
	InitRendertexture();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
#ifdef _DEBUG
	if (g_pD3DXFont != NULL)
	{// 情報表示用フォントの開放
		g_pD3DXFont->Release();
		g_pD3DXFont = NULL;
	}
#endif

	if (g_pD3DDevice != NULL)
	{// デバイスの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if (g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// 入力の終了処理
	UninitInput();

	// ライトの終了処理
	UninitLight();

	// カメラの終了処理
	UninitCamera();

	// 地面の終了処理
	UninitField();

	// モデルの終了処理
	UninitPlayer();

	UninitBackGroundPolygon();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	if (GetKeyboardTrigger(DIK_F1))
	{// デバッグ表示ON/OFF
		g_bDispDebug = g_bDispDebug ? false : true;
	}

	if (GetKeyboardTrigger(DIK_F3))
	{
		g_sw = g_sw ? false : true;
	}

	// 入力の更新処理
	UpdateInput();

	// ライトの更新処理
	UpdateLight();

	// カメラの更新処理
	UpdateCamera();

	// 地面の更新処理
	UpdateField();

	// モデルの更新処理
	UpdatePlayer();
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	int camera_num = GetCameraNum();

	if (CheakTurn())
	{
		for (int i = 0; i < camera_num; i++)
		{
			// ビューポート領域を設定(変更)		
			g_pD3DDevice->SetViewport(&g_port[i]);

			// カメラの座標を変更(ビュー行列)
			if (g_sw)
			{
				g_pD3DDevice->SetTransform(D3DTS_VIEW, &g_view[i].matView);
			}

			// 描画領域にしたところを任意の色でクリア
			g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(g_color[i].r, g_color[i].b, g_color[i].g), 1.0f, 0);

			// Direct3Dによる描画の開始
			if (SUCCEEDED(g_pD3DDevice->BeginScene()))
			{
				if (camera_num == 1)
				{
					SetCamera(i);
				}
				else
				{
					// カメラの設定処理
					SetCamera(i + 1);
				}

				//
				if (GetTurn(i))
				{
					if (camera_num == 1)
					{
						SetBackGroundPolygon(i);
					}
					else
					{
						SetBackGroundPolygon(i + 1);
					}

					//レンダリングターゲットの切り替え
					ChangeRenderTarget(i);
				}

				// 地面の描画処理
				DrawField();

				// モデルの描画処理
				DrawPlayer();


				if (GetTurn(i))
				{
					//レンダリングターゲットの復帰
					RecoverRenderTarget();
				}
			}

			if (i == camera_num - 1)
			{
				for (int j = 0; j < camera_num; j++)
				{
					if (GetTurn(j))
					{
						//レンダリングした背景を表示
						DrawBackGroundPolygon(j);
					}
				}
			}

#ifdef _DEBUG
			// FPS表示
			DrawDebug();
#endif

			// Direct3Dによる描画の終了
			g_pD3DDevice->EndScene();
		}
	}
	else
	{
		if (GetTurn(0))
		{
			for (int i = 0; i < camera_num; i++)
			{
				// ビューポート領域を設定(変更)		
				g_pD3DDevice->SetViewport(&g_port[i]);

				// カメラの座標を変更(ビュー行列)
				if (g_sw)
				{
					g_pD3DDevice->SetTransform(D3DTS_VIEW, &g_view[i].matView);
				}

				// 描画領域にしたところを任意の色でクリア
				g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(g_color[i].r, g_color[i].b, g_color[i].g), 1.0f, 0);

				// Direct3Dによる描画の開始
				if (SUCCEEDED(g_pD3DDevice->BeginScene()))
				{
					if (camera_num == 1)
					{
						SetCamera(i);
					}
					else
					{
						// カメラの設定処理
						SetCamera(i + 1);
					}

					//
					if (GetTurn(i))
					{
						if (camera_num == 1)
						{
							SetBackGroundPolygon(i);
						}
						else
						{
							SetBackGroundPolygon(i + 1);
						}

						//レンダリングターゲットの切り替え
						ChangeRenderTarget(i);
					}

					// 地面の描画処理
					DrawField();

					// モデルの描画処理
					DrawPlayer();



					if (GetTurn(i))
					{
						//レンダリングターゲットの復帰
						RecoverRenderTarget();

						//レンダリングした背景を表示
						DrawBackGroundPolygon(i);
					}
				}

#ifdef _DEBUG
				// FPS表示
				DrawDebug();
#endif

				// Direct3Dによる描画の終了
				g_pD3DDevice->EndScene();
			}
		}
		else
		{
			for (int i = camera_num - 1; i > -1; i--)
			{
				// ビューポート領域を設定(変更)		
				g_pD3DDevice->SetViewport(&g_port[i]);

				// カメラの座標を変更(ビュー行列)
				if (g_sw)
				{
					g_pD3DDevice->SetTransform(D3DTS_VIEW, &g_view[i].matView);
				}

				// 描画領域にしたところを任意の色でクリア
				g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(g_color[i].r, g_color[i].b, g_color[i].g), 1.0f, 0);

				// Direct3Dによる描画の開始
				if (SUCCEEDED(g_pD3DDevice->BeginScene()))
				{
					if (camera_num == 1)
					{
						SetCamera(i);
					}
					else
					{
						// カメラの設定処理
						SetCamera(i + 1);
					}

					//
					if (GetTurn(i))
					{
						if (camera_num == 1)
						{
							SetBackGroundPolygon(i);
						}
						else
						{
							SetBackGroundPolygon(i + 1);
						}

						//レンダリングターゲットの切り替え
						ChangeRenderTarget(i);
					}



					// 地面の描画処理
					DrawField();

					// モデルの描画処理
					DrawPlayer();

					if (GetTurn(i))
					{
						//レンダリングターゲットの復帰
						RecoverRenderTarget();

						//レンダリングした背景を表示
						DrawBackGroundPolygon(i);
					}
				}

#ifdef _DEBUG
				// FPS表示
				DrawDebug();
#endif

				// Direct3Dによる描画の終了
				g_pD3DDevice->EndScene();
			}
		}
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバッグ表示
//=============================================================================
void DrawDebug(void)
{
	CAMERA *camera = GetCamera(0);
	CAMERA *camera1P = GetCamera(1);
	CAMERA *camera2P = GetCamera(2);

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];

	wsprintf(str, "FPS:%d\n", g_nCountFPS);

	// テキスト描画
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 20;
	wsprintf(str, "カメラの向き:%d\n", (int)(camera->rot.y * 100));
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 40;
	wsprintf(str, "ビュー行列:%d\n", g_sw);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 60;
	wsprintf(str, "カメラ0の位置 X:%d Y:%d Z:%d\n", (int)camera->posEye.x * 1000, (int)camera->posEye.y * 1000, (int)camera->posEye.z * 1000);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 80;
	wsprintf(str, "カメラ1の位置 X:%d Y:%d Z:%d\n", (int)camera1P->posEye.x * 1000, (int)camera1P->posEye.y * 1000, (int)camera1P->posEye.z * 1000);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 100;
	wsprintf(str, "カメラ2の位置 X:%d Y:%d Z:%d\n", (int)camera2P->posEye.x * 1000, (int)camera2P->posEye.y * 1000, (int)camera2P->posEye.z * 1000);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 120;
	wsprintf(str, "プレイヤー1 turn?:%d プレイヤー2 turn?:%d\n", GetTurn(0), GetTurn(1));
	g_pD3DXFont->DrawTextA(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 140;
	wsprintf(str, "1P用ビューポート X:%d Y:%d WIDTH:%d HEIGHT:%d\n", g_port[0].X, g_port[0].Y, g_port[0].Width, g_port[0].Height);
	g_pD3DXFont->DrawTextA(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 160;
	wsprintf(str, "2P用ビューポート X:%d Y:%d WIDTH:%d HEIGHT:%d\n", g_port[1].X, g_port[1].Y, g_port[1].Width, g_port[1].Height);
	g_pD3DXFont->DrawTextA(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
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
			g_port[1] = { SCREEN_CENTER_X, 0, SCREEN_CENTER_X, SCREEN_HEIGHT, 0.0f, 1.0f };
		}

		break;
	}
}

//=============================================================================
// レンダリングテクスチャ作成処理
//=============================================================================
void InitRendertexture()
{
	//レンダリングテクスチャ用ビューポート
	RenderViewport.X = 0;
	RenderViewport.Y = 0;
	RenderViewport.Width = 800;
	RenderViewport.Height = 600;
	RenderViewport.MinZ = 0.0f;
	RenderViewport.MaxZ = 1.0f;

	//デプスバッファ作成(2のべき乗サイズ)
	g_pD3DDevice->CreateDepthStencilSurface(1024, 1024,		//サイズ
		D3DFMT_D24S8,	//ピクセルフォーマット
		D3DMULTISAMPLE_NONE,//marutisanpuringu OFF
		0,
		FALSE,
		&pRenderDepthBuffer,
		NULL);

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		//レンダーターゲット作成(2のべき乗サイズ)
		D3DXCreateTexture(g_pD3DDevice,
			800, 1024,		//サイズ
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
	//レンダーターゲットを切り替え

	//デフォルトビューポート保存
	g_pD3DDevice->GetViewport(&DefaultViewport);

	//デフォルトのサーフェースを保存
	g_pD3DDevice->GetRenderTarget(0, &pDefaultRenderTargetSurface);
	g_pD3DDevice->GetDepthStencilSurface(&pDefaultDepthBufferSurface);
	//レンダリングテクスチャのサーフェース取得
	pRenderTexture[no]->GetSurfaceLevel(0, &pRenderTextureSurface);

	//新しいレンダリングターゲットをセット
	g_pD3DDevice->SetRenderTarget(0, pRenderTextureSurface);
	//新しいデプスバッファをセット
	g_pD3DDevice->SetDepthStencilSurface(pRenderDepthBuffer);

	//ビューポートの切り替え
	g_pD3DDevice->SetViewport(&RenderViewport);

	//クリア
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
}

//=============================================================================
// レンダーターゲット復帰処理
//=============================================================================
void RecoverRenderTarget()
{
	//レンダーターゲットの復帰
	g_pD3DDevice->SetRenderTarget(0, pDefaultRenderTargetSurface);
	g_pD3DDevice->SetDepthStencilSurface(pDefaultDepthBufferSurface);

	//サーフェースの解放
	pDefaultRenderTargetSurface->Release();
	pDefaultDepthBufferSurface->Release();
	pRenderTextureSurface->Release();

	//ビューポートの復帰
	g_pD3DDevice->SetViewport(&DefaultViewport);
}

//=============================================================================
// レンダリングテクスチャ背景初期化
//=============================================================================
HRESULT	InitBackGroundPolygon()
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// オブジェクトの頂点バッファを生成
		g_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,		// 頂点データ用に確保するバッファサイズ(バイト単位)
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
			pVtx[1].vtx = D3DXVECTOR3(1024.0f, 0.0f, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(0.0f, 1024.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(1024.0f, 1024.0f, 0.0f);

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
void	DrawBackGroundPolygon(int no)
{
	// 頂点フォーマットの設定
	g_pD3DDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	g_pD3DDevice->SetTexture(0, pRenderTexture[no]);

	//
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 頂点バッファをデバイスのデータストリームにバインド
	g_pD3DDevice->SetStreamSource(0, g_pD3DVtxBuffBackGround[no], 0, sizeof(VERTEX_2D));

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ポリゴンの描画
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void SetBackGroundPolygon(int num)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	int no;
	if (num == 0)
	{
		no = 0;
	}
	else
	{
		no = num - 1;
	}

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffBackGround[no]->Lock(0, 0, (void**)&pVtx, 0);

	switch (num)
	{
	case 0:
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(800.0f, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, 1024.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(800.0f, 1024.0f, 0.0f);
		break;
	case 1:
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(400.0f, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, 1024.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(400.0f, 1024.0f, 0.0f);
		break;
	case 2:
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(400.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(800.0f, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(400.0f, 1024.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(800.0f, 1024.0f, 0.0f);
		break;
	}

	// 頂点データをアンロックする
	g_pD3DVtxBuffBackGround[no]->Unlock();
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

