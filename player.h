//=============================================================================
//
// モデル処理 [Player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MAX	(2)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3			pos;				// 位置
	D3DXVECTOR3			rot;				// 回転
	D3DXVECTOR3			scl;				// 大きさ

	bool				jump;
	bool				Turn;				// カメラ回転 true:回転状態　false:非回転状態
	bool				sayuu;				// 左右向き
	float				speed;
	float				a;

	int				PatternAnim;				// アニメーションパターンナンバー
	int				CountAnim;					// アニメーションカウント

	LPDIRECT3DTEXTURE9	pD3DTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		g_pD3DVtxBuff;		// マテリアル情報へのポインタ

} PLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

bool GetTurn(int no);
bool CheckClear(void);
PLAYER *GetPlayer(int no);
bool GetClear(void);

#endif
