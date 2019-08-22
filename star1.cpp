//=============================================================================
//
// ���f������ [Player.cpp]
// Author : 
//
//=============================================================================
#include "star1.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_STAR1				("data/TEXTURE/star.png")				// �g�p����e�N�X�`��

#define STAR1_FALLSPEED				(0.4f)									// �������x

#define STAR1_POS_X					(0.0f)									// �ʒu(X)
#define STAR1_POS_Y					(60.0f)									// �ʒu(Y)
#define STAR1_POS_Z					(-10.0f)									// �ʒu(Z)

#define STAR1_SIZE					(20.0f)									// �e�N�X�`���T�C�Y

#define STAR1_POS_X_RANGE_MAX		(80)									// �\���͈�(X)
#define STAR1_POS_X_RANGE_MID		(STAR1_POS_X_RANGE_MAX / 2)				// �\���͈͂̒��S(X)

#define STAR1_POS_Y_RANGE_MAX		(150)									// �\���͈�(Y)
#define STAR1_POS_Y_RANGE_MID		(STAR1_POS_Y_RANGE_MAX / 2)				// �\���͈͂̒��S(Y)

#define STAR1_POS_Y_CEIL			(STAR1_POS_Y + STAR1_POS_Y_RANGE_MID)	// �\���͈͂̏��(Y)
#define STAR1_POS_Y_LOW				(STAR1_POS_Y - STAR1_POS_Y_RANGE_MID)	// �\���͈͂̉���(Y)

#define STAR1_ROTSPEED_RAND_MAX		(300 + 1)								// ��]���x�����_���ő吔
#define STAR1_ROTSPEED_RAND_ADD		(200)									// ��]���x�����_���ɉ��Z���鐔��

#define STAR1_COLOR_RAND_MAX		(20 + 1)								// �F�����_���ő吔
#define STAR1_COLOR_RAND_ADD		(235)									// �F�����_���ɉ��Z���鐔��

#define STAR1_USE_RAND_MAX			(120)									// use��true�ɂ���m��

#define STAR1_FRAME_ANGLE			(D3DX_PI / (FRAME_MAX * STAR1_SECN * 2))		// 1�t���[���ɑ��₷�l

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexStar1(LPDIRECT3DDEVICE9 pDevice, int no);
void	SetColorStar1(STAR1_SEC *sec);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureStar1 = NULL;

STAR1_BASE			star1BaseWk[STAR1_BASE_MAX];		// �\����
STAR1_SEC			star1SecWk[STAR1_SEC_MAX];			// �\����

D3DXMATRIX			g_mtxWorldStar1;					// ���[���h�}�g���b�N�X

//=============================================================================
// ����������
//=============================================================================
HRESULT InitStar1(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	STAR1_BASE *base = GetStar1Base(0);
	STAR1_SEC *sec = GetStar1Sec(0);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
		TEXTURE_STAR1,					// �t�@�C���̖��O
		&g_pD3DTextureStar1);					// �ǂݍ��ރ������[

	// �p�����[�^�̐ݒ�
	for (int i = 0; i < STAR1_BASE_MAX; i++, base++)
	{
		if ((rand() % STAR1_USE_RAND_MAX) == 0)
		{
			base->use = true;
		}
		else
		{
			base->use = false;
		}
		base->pos = D3DXVECTOR3(STAR1_POS_X, STAR1_POS_Y, STAR1_POS_Z);
		base->pos.x += (rand() % STAR1_POS_X_RANGE_MAX - STAR1_POS_X_RANGE_MID);
		base->pos.y += (rand() % STAR1_POS_Y_RANGE_MAX - STAR1_POS_Y_RANGE_MID);
		base->secNo = (i / STAR1_SECN);
	}

	for (int i = 0; i < STAR1_SEC_MAX; i++, sec++)
	{
		sec->pD3DVtxBuff = NULL;

		sec->col = D3DCOLOR_RGBA(rand() % STAR1_COLOR_RAND_MAX + STAR1_COLOR_RAND_ADD, 0, 0, 0);
		sec->col.g = sec->col.r;
		sec->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		sec->size = STAR1_SIZE;
		sec->angle = /*(rand() % 314) / 100.0f*/1.0f;
		sec->rotSpd = 1.0f / (rand() % STAR1_ROTSPEED_RAND_MAX + STAR1_ROTSPEED_RAND_ADD);
		sec->alphaSpd = /*STAR1_FRAME_ANGLE*/0.0f;

		// ���_�t�H�[�}�b�g(���S���W)
		MakeVertexStar1(pDevice, i);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitStar1(void)
{
	STAR1_SEC *sec = GetStar1Sec(0);

	if (g_pD3DTextureStar1 != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureStar1->Release();
		g_pD3DTextureStar1 = NULL;
	}

	for (int i = 0; i < STAR1_SEC_MAX; i++, sec++)
	{
		if (sec->pD3DVtxBuff != NULL)
		{// ���_�o�b�t�@�̊J��
			sec->pD3DVtxBuff->Release();
			sec->pD3DVtxBuff = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateStar1(void)
{
	STAR1_BASE *base = GetStar1Base(0);

	for (int i = 0; i < STAR1_BASE_MAX; i++, base++)
	{
		STAR1_SEC *sec = GetStar1Sec(base->secNo);

		base->pos.y -= STAR1_FALLSPEED;
		if (base->pos.y < STAR1_POS_Y_LOW)
		{
			base->pos.y = STAR1_POS_Y_CEIL;
		}

		sec->rot.z += sec->rotSpd;
		if (sec->rot.z > D3DX_PI)
		{
			sec->rot.z -= D3DX_PI * 2.0f;
		}
		if (sec->rot.z < -D3DX_PI)
		{
			sec->rot.z += D3DX_PI * 2.0f;
		}

		if (!base->use)
		{
			if ((rand() % STAR1_USE_RAND_MAX) == 0)
			{
				base->use = true;
			}
		}
		
		if (base->use)
		{
			sec->angle += sec->alphaSpd;

			if (sec->angle > D3DX_PI * 2)
			{
				sec->angle = 0.0f;
				base->use = false;
			}

			// �����g�œ_�ŏ���
			sec->col.a = sinf(sec->angle);

			// �J���[�̍X�V
			SetColorStar1(sec);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawStar1(void)
{
	STAR1_BASE *base = GetStar1Base(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale,mtxRot, mtxTranslate;

	for (int i = 0; i < STAR1_BASE_MAX; i++, base++)
	{
		if (base->use)
		{
			STAR1_SEC *sec = GetStar1Sec(base->secNo);

			// ���C���e�B���O�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldStar1);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, sec->rot.y, sec->rot.x, sec->rot.z);
			D3DXMatrixMultiply(&g_mtxWorldStar1, &g_mtxWorldStar1, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, base->pos.x,
				base->pos.y,
				base->pos.z);
			D3DXMatrixMultiply(&g_mtxWorldStar1,
				&g_mtxWorldStar1, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldStar1);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, sec->pD3DVtxBuff, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureStar1);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

			// ���C���e�B���O��L���ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexStar1(LPDIRECT3DDEVICE9 pDevice, int no)
{
	STAR1_SEC *sec = GetStar1Sec(no);

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&sec->pD3DVtxBuff,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		sec->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-sec->size / 2, sec->size / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(sec->size / 2, sec->size / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-sec->size / 2, -sec->size / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(sec->size / 2, -sec->size / 2, 0.0f);

		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		SetColorStar1(sec);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		sec->pD3DVtxBuff->Unlock();
	}

	return S_OK;
}

void SetColorStar1(STAR1_SEC *sec)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		sec->pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = sec->col;
		pVtx[1].diffuse = sec->col;
		pVtx[2].diffuse = sec->col;
		pVtx[3].diffuse = sec->col;

		// ���_�f�[�^���A�����b�N����
		sec->pD3DVtxBuff->Unlock();
	}
}

//=============================================================================
// ���̃x�[�X�����擾
//=============================================================================
STAR1_BASE *GetStar1Base(int no)
{
	return (&star1BaseWk[no]);
}

//=============================================================================
// ����Sec�����擾
//=============================================================================
STAR1_SEC *GetStar1Sec(int no)
{
	return (&star1SecWk[no]);
}

//=============================================================================
// ���l�̕ω������擾
//=============================================================================
float GetStar1AlphaSpd(void)
{
	STAR1_SEC *sec = GetStar1Sec(0);

	return sec->alphaSpd;
}

float GetStar1Angle(void)
{
	STAR1_SEC *sec = GetStar1Sec(0);

	return sec->angle;
}

float GetStar1Alpha(void)
{
	STAR1_SEC *sec = GetStar1Sec(0);

	return sec->col.a;
}