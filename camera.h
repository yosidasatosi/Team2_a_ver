//=============================================================================
//
// �J�������� [camera.h]
// Author : 
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CAMERA_MAX		(3) // 0:�������p�@1:1P�p�@2:2P�p

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3		posEye;				// �J�����̎��_
	D3DXVECTOR3		posAt;				// �J�����̒����_
	D3DXVECTOR3		vecUp;				// �J�����̏����
	D3DXVECTOR3		rot;				// �J�����̉�]
} CAMERA;

//*****************************************************************************
// �v���g�^�C�v�錾
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