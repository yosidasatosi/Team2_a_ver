//=============================================================================
//
// ���f������ [star1.h]
// Author : 
//
//=============================================================================
#ifndef _STAR1_H_
#define _STAR1_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define STAR1_MAX			(3)

#define STAR1_BASE_MAX		(200)
#define STAR1_BASE_FULL		(STAR1_MAX * STAR1_BASE_MAX)

#define STAR1_SECN			(5)
#define STAR1_SEC_MAX		(STAR1_BASE_MAX/STAR1_SECN)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	bool						use;			// 
	D3DXVECTOR3					pos;			// �ʒu
	int							secNo;			// �Ή�����STARS_SEC�̔ԍ�
}STAR1_BASE;

typedef struct
{
	D3DXVECTOR3			rot;					// ��]
	float				size;					// �T�C�Y
	D3DXCOLOR			col;					// �F
	float				angle;					// �p�x
	float				rotSpd;					// ��]�̑���
	float				alphaSpd;				// �_�ł̑��x
	

	LPDIRECT3DVERTEXBUFFER9		pD3DVtxBuff;	// �}�e���A�����ւ̃|�C���^

}STAR1_SEC;				// �p�����[�^���L

typedef struct
{
	D3DXVECTOR3			pos;			// ���̃J�[�e���̈ʒu

	STAR1_BASE			*base;			// 
}STAR1;


//*****************************************************************************
// �v���g�^�C�v�錾
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
