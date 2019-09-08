//=============================================================================
//
// モデル処理 [star1.h]
// Author : 
//
//=============================================================================
#ifndef _STAR1_H_
#define _STAR1_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STAR1_MAX			(3)

#define STAR1_BASE_MAX		(200)
#define STAR1_BASE_FULL		(STAR1_MAX * STAR1_BASE_MAX)

#define STAR1_SECN			(5)
#define STAR1_SEC_MAX		(STAR1_BASE_MAX/STAR1_SECN)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	bool						use;			// 
	D3DXVECTOR3					pos;			// 位置
	int							secNo;			// 対応するSTARS_SECの番号
}STAR1_BASE;

typedef struct
{
	D3DXVECTOR3			rot;					// 回転
	float				size;					// サイズ
	D3DXCOLOR			col;					// 色
	float				angle;					// 角度
	float				rotSpd;					// 回転の速さ
	float				alphaSpd;				// 点滅の速度
	

	LPDIRECT3DVERTEXBUFFER9		pD3DVtxBuff;	// マテリアル情報へのポインタ

}STAR1_SEC;				// パラメータ共有

typedef struct
{
	D3DXVECTOR3			pos;			// 星のカーテンの位置

	STAR1_BASE			*base;			// 
}STAR1;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitStar1(void);
void UninitStar1(void);
void UpdateStar1(void);
void DrawStar1(void);

STAR1 *GetStar1(int no);
STAR1_BASE *GetStar1Base(int no);
STAR1_SEC *GetStar1Sec(int no);

float GetStar1AlphaSpd(void);
float GetStar1Angle(void);
float GetStar1Alpha(void);
#endif
