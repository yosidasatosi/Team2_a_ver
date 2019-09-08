//=============================================================================
//
// ���f������ [Player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_MAX	(2)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3			pos;				// �ʒu
	D3DXVECTOR3			rot;				// ��]
	D3DXVECTOR3			scl;				// �傫��

	bool				jump;
	bool				Turn;				// �J������] true:��]��ԁ@false:���]���
	bool				sayuu;				// ���E����
	float				speed;
	float				a;

	int				PatternAnim;				// �A�j���[�V�����p�^�[���i���o�[
	int				CountAnim;					// �A�j���[�V�����J�E���g

	LPDIRECT3DTEXTURE9	pD3DTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		g_pD3DVtxBuff;		// �}�e���A�����ւ̃|�C���^

} PLAYER;

//*****************************************************************************
// �v���g�^�C�v�錾
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
