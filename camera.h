//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_MAX		(PLAYER_MAX + 1) // 0:合流時用　1:1P用　2:2P用

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3		posEye;				// カメラの視点
	D3DXVECTOR3		posAt;				// カメラの注視点
	D3DXVECTOR3		vecUp;				// カメラの上方向
	D3DXVECTOR3		rot;				// カメラの回転

	bool			turn;				// true:回転動作状態　false:非回転動作状態
	float			fLengthInterval;	// カメラの視点と注視点の距離
} CAMERA;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

void SetCamera(int no);
void SetCameraPos(int num);

void TurnCamera(int playerno);

int GetCameraNum(void);
D3DXVECTOR3 GetRotCamera(int no);
D3DXMATRIX GetMtxView(void);
CAMERA *GetCamera(int no);

bool GetGouryu(void);
bool CheakTurn(void);

#endif
