//=============================================================================
//
// �t�F�[�h���� [fade.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

//*************************************
// �t�F�[�h�̏��
//*************************************
typedef enum
{
	FADE_NONE = 0,		// �����Ȃ����
	FADE_IN,			// �t�F�[�h�C������
	FADE_OUT,			// �t�F�[�h�A�E�g����
	FADE_MAX
} FADE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFade(void);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void SetFade(FADE fade);
FADE GetFade(void);

#endif
