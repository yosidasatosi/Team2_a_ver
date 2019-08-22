//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_MAX		(3) // 0:合流時用　1:1P用　2:2P用

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3		posEye;				// カメラの視点
	D3DXVECTOR3		posAt;				// カメラの注視点
	D3DXVECTOR3		vecUp;				// カメラの上方向
	D3DXVECTOR3		rot;				// カメラの回転
} CAMERA;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

void SetUseCamera(int no);
void SetCamera(int no);
void SetCameraX(int num);
void SetCameraZ(int num);

void TurnCamera(int playerno);

int GetCameraNum(void);
D3DXVECTOR3 GetRotCamera(int no);
D3DXMATRIX GetMtxView(void);
CAMERA *GetCamera(int no);

bool GetGouryu(void);
bool CheckTurn(void);

void Viewport(int no);
void SetViewport(void);

D3DVIEWPORT9 *GetPort(int no);

#endif