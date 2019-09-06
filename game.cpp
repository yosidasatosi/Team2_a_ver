//=============================================================================
//
// メイン処理 [main.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "game.h"
#include "light.h"
#include "camera.h"
#include "field.h"
#include "player.h"
#include "star1.h"
#include "render.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "skybox.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 構造体定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void GameObject(void);
void GameUI(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************


HRESULT InitGame(void)
{
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

	//レンダリング初期化
	InitRender();

	// ギミック(スター1)
	InitStar1();

	// スカイボックス初期化
	InitSkybox();

	return S_OK;
}

void UninitGame(void)
{
	// ライトの終了処理
	UninitLight();

	// カメラの終了処理
	UninitCamera();

	// 地面の終了処理
	UninitField();

	// モデルの終了処理
	UninitPlayer();

	UninitBackGroundPolygon();

	// ギミック(スター1)の終了処理
	UninitStar1();

	// スカイボックス終了処理
	UninitSkybox();
}

void UpdateGame(void)
{
	// ライトの更新処理
	UpdateLight();

	// カメラの更新処理
	UpdateCamera();

	// 地面の更新処理
	UpdateField();

	// モデルの更新処理
	UpdatePlayer();

	// ギミック(スター1)の更新処理
	UpdateStar1();

	// 空更新処理
	UpdateSkybox();
}

void DrawGame(void)
{
	int camera_num = GetCameraNum();

	// カメラが2つとも回っている・回っていない時
	if (CheckTurn())
	{
		for (int i = 0; i < camera_num; i++)
		{
			Viewport(i);

			SetUseCamera(i);

			// カメラを回していないときはレンダリング関連の処理は行わない
			if (GetTurn(i))
			{ // プレイヤー(i)がカメラを回しているとき
			  // レンダリング描画処理
				SetRender(camera_num, i);
			}

			// オブジェクト描画処理
			GameObject();

			if (GetTurn(i))
			{
				DrawRender(i, false);
			}

			// テクスチャの設定が終わってから描画を行う
			if (i == camera_num - 1)
			{ // カメラの台数分処理が行われた時
				for (int j = 0; j < camera_num; j++)
				{ // カメラの台数分描画を行う
					if (GetTurn(j))
					{ // カメラが回されていたら
						DrawBackGroundPolygon(j);	//レンダリングした背景を表示
					}
				}
				// UI描画処理
				GameUI();
			}
		}
	}
	else
	{ // カメラが片方だけ回っているとき
		if (GetTurn(0))
		{ // プレイヤー1が回っているか判定
			for (int i = 0; i < camera_num; i++)
			{ // プレイヤー1から順に描画を行う
				Viewport(i);

				SetUseCamera(i);

				if (GetTurn(i))
				{
					// レンダリング描画処理
					SetRender(camera_num, i);
				}
				// オブジェクト描画処理
				GameObject();

				if (GetTurn(i))
				{
					// レンダリング描画処理
					DrawRender(i, true);
				}

				// 最後にUI描画を行う
				if (i == camera_num - 1)
				{
					// UI描画処理
					GameUI();
				}
			}
		}
		else
		{
			// プレイヤー２から順に描画を行う
			for (int i = camera_num - 1; i > -1; i--)
			{
				Viewport(i);

				SetUseCamera(i);

				if (GetTurn(i))
				{
					// レンダリング描画処理
					SetRender(camera_num, i);
				}

				// オブジェクト描画処理
				GameObject();

				if (GetTurn(i))
				{
					DrawRender(i, true);
				}

				// 最後にUI描画を行う
				if (i == camera_num)
				{
					// UI描画処理
					GameUI();
				}
			}
		}
	}
}

void GameObject(void)
{
	for (int i = 0; i < CAMERA_MAX; i++)
	{
		// 空描画
		DrawSkybox(i);
	}

	// フィールドの描画処理
	DrawField();

	// プレイヤーの描画処理
	DrawPlayer();

	// スター1の描画処理
	DrawStar1();
}

void GameUI(void)
{

}