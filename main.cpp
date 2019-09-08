//=============================================================================
//
// メイン処理 [main.cpp]
// Author : aaa
//
//=============================================================================
#include <stdio.h>

#include "main.h"
#include "input.h"
#include "light.h"
#include "camera.h"
#include "field.h"
#include "player.h"
#include "star1.h"
#include "render.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"カメラ処理"		// ウインドウのキャプション名

//*****************************************************************************
// 構造体定義
//*****************************************************************************
//struct COLOR {
//	int r;
//	int g;
//	int b;
//};
//
//struct VIEW {
//	D3DXVECTOR3 vEyePt;
//	D3DXVECTOR3 vLookatPt;
//	D3DXVECTOR3 vUpVec;
//	D3DXMATRIXA16 matView;
//};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//void Object(void);
//void UI(void);
//void Viewport(int no);

void DrawDebug(void);

//void DrawGame(void);

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

//COLOR g_color[] = {
//	{ 20, 60, 20 },
//	{ 20, 60, 20 },
//};
//
//D3DVIEWPORT9 g_port[CAMERA_MAX];
//VIEW g_view[CAMERA_MAX];

bool	g_sw = true;

MODE				g_mode = MODE_TITLE;	// モード

// D3DVIEWPORT9
// 第1引数: X		左上の頂点(X)
// 第2引数: Y		左上の頂点(Y)
// 第3引数: Width	横幅
// 第4引数: Height	縦幅
// 第5引数: MinZ	基本0.0f
// 第6引数: MaxZ	基本1.0f

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

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / FRAME_MAX))
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
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 裏面をカリングしない
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

	// フェードの初期化処理
	InitFade();

	// 最初はタイトル画面に
	SetMode(MODE_TITLE);

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

	// フェードの終了処理
	UninitFade();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	// 入力更新
	UpdateInput();

	switch (g_mode)
	{
	case MODE_TITLE:		// タイトル画面の更新
		UpdateTitle();
		break;

	case MODE_GAME:			// ゲーム画面の更新
		UpdateGame();
		break;

	case MODE_RESULT:		// リザルト画面の更新
		UpdateResult();
		break;
	}

	// フェード処理の更新
	UpdateFade();

	if (GetKeyboardTrigger(DIK_F1))
	{// デバッグ表示ON/OFF
		g_bDispDebug = g_bDispDebug ? false : true;
	}

	//if (GetKeyboardTrigger(DIK_F3))
	//{
	//	g_sw = g_sw ? false : true;
	//}
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (g_mode)
		{
		case MODE_TITLE:		// タイトル画面の描画
			DrawTitle();
			break;

		case MODE_GAME:			// ゲーム画面の描画
			DrawGame();
			break;

		case MODE_RESULT:		// リザルト画面の描画
			DrawResult();
			break;
		}

		// フェード描画
		DrawFade();

#ifdef _DEBUG
		// デバッグ表示
		DrawDebug();
#endif

		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
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
	//STARS_BASE *base = GetStarsBase(0);
	STAR1_BASE *base = GetStar1Base(0);

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];

	wsprintf(str, "FPS:%d\n", g_nCountFPS);

	// テキスト描画
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 20;
	wsprintf(str, "カメラの向き:%d\n", (int)(camera->rot.y * 100));
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top += 20;
	wsprintf(str, "ビュー行列:%d\n", g_sw);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top += 20;
	wsprintf(str, "カメラ0の位置 X:%d Y:%d Z:%d\n", (int)camera->posEye.x * 1000, (int)camera->posEye.y * 1000, (int)camera->posEye.z * 1000);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top += 20;
	wsprintf(str, "カメラ1の位置 X:%d Y:%d Z:%d\n", (int)camera1P->posEye.x * 1000, (int)camera1P->posEye.y * 1000, (int)camera1P->posEye.z * 1000);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top += 20;
	wsprintf(str, "カメラ2の位置 X:%d Y:%d Z:%d\n", (int)camera2P->posEye.x * 1000, (int)camera2P->posEye.y * 1000, (int)camera2P->posEye.z * 1000);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top += 20;
	wsprintf(str, "プレイヤー1 turn?:%d プレイヤー2 turn?:%d\n", GetTurn(0), GetTurn(1));
	g_pD3DXFont->DrawTextA(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	D3DVIEWPORT9 *port1 = GetPort(0);
	D3DVIEWPORT9 *port2 = GetPort(1);

	rect.top += 20;
	//wsprintf(str, "1P用ビューポート X:%d Y:%d WIDTH:%d HEIGHT:%d\n", g_port[0].X, g_port[0].Y, g_port[0].Width, g_port[0].Height);
	wsprintf(str, "1P用ビューポート X:%d Y:%d WIDTH:%d HEIGHT:%d\n", port1->X, port1->Y, port1->Width, port1->Height);
	g_pD3DXFont->DrawTextA(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top += 20;
	//wsprintf(str, "2P用ビューポート X:%d Y:%d WIDTH:%d HEIGHT:%d\n", g_port[1].X, g_port[1].Y, g_port[1].Width, g_port[1].Height);
	wsprintf(str, "2P用ビューポート X:%d Y:%d WIDTH:%d HEIGHT:%d\n", port2->X, port2->Y, port2->Width, port2->Height);
	g_pD3DXFont->DrawTextA(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top += 20;
	wsprintf(str, "カメラ0の注視点 X:%d Y:%d Z:%d\n", (int)camera->posAt.x * 1000, (int)camera->posAt.y * 1000, (int)camera->posAt.z * 1000);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top += 20;
	wsprintf(str, "αスピード:%d.%d%d\n", (int)(GetStar1AlphaSpd()), (int)(GetStar1AlphaSpd() * 10), (int)(GetStar1AlphaSpd() * 10000));
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top += 20;
	wsprintf(str, "角度:%d\n", (int)(GetStar1Angle() * 1000));
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top += 20;
	wsprintf(str, "α値:%d.%d\n", (int)GetStar1Alpha(), (int)(GetStar1Alpha() * 10));
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top += 20;
	if (GetClear())
	{
		wsprintf(str, "クリア:true\n");
	}
	else
	{
		wsprintf(str, "クリア:false\n");
	}
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

//=============================================================================
// BCによる当たり判定処理
// サイズは半径
// 戻り値：当たってたらtrue
//=============================================================================
bool CheckHitBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2)
{
	if ((size1 + size2) * (size1 + size2) >
		(pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y) + (pos1.z - pos2.z) * (pos1.z - pos2.z))
	{
		return true;
	}

	return false;
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=============================================================================
// モードの設定
//=============================================================================
void SetMode(MODE mode)
{
	g_mode = mode;

	switch (g_mode)
	{
	case MODE_TITLE:
		// リザルト画面の終了処理
		UninitResult();

		// タイトル画面の初期化
		InitTitle();

		break;

	case MODE_GAME:
		// タイトル画面の終了処理
		UninitTitle();

		// ゲーム画面の初期化
		InitGame();

		break;

	case MODE_RESULT:
		// ゲーム画面の終了処理
		UninitGame();

		// リザルト画面の初期化
		InitResult();

		break;
	}
}

//=============================================================================
// モードの取得
//=============================================================================
MODE GetMode(void)
{
	return g_mode;
}