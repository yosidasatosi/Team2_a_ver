//=============================================================================
//
// 背景処理 [skybox.h]
// Author : 
//
//=============================================================================
#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "main.h"

#define	SKYBOX_SIZE_X		(50)
#define	SKYBOX_SIZE_Y		(50)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSkybox(void);
void UninitSkybox(void);
void UpdateSkybox(void);
void DrawSkybox(int no);
void LoadSkyboxTexture(void);

#endif