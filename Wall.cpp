//=============================================================================
//
// �n�ʏ��� [field.cpp]
// Author : 
//
//=============================================================================
#include "wall.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_WALL	"data/TEXTURE/field000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexWall(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureWall = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffWall = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

D3DXMATRIX				g_mtxWorldWall;			// ���[���h�}�g���b�N�X
D3DXVECTOR3				g_posWall;					// ���݂̈ʒu
D3DXVECTOR3				g_rotWall;					// ���݂̌���
D3DXVECTOR3				g_sclWall;					// ���݂̈ʒu


//=============================================================================
// ����������
//=============================================================================
HRESULT InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 
	HRESULT hr;

	// �ʒu�A�����̏����ݒ�
	g_posWall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotWall = D3DXVECTOR3(D3DX_PI*0.5f, 0.0f, 0.0f);
	g_sclWall = D3DXVECTOR3(1.0f, 1.0f, 1.0f)*20.0f;


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_WALL,			// �t�@�C���̖��O
								&g_pD3DTextureWall);	// �ǂݍ��ރ������[

	// ���_���̍쐬
	hr = MakeVertexWall(pDevice);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitWall(void)
{
	if(g_pD3DTextureWall != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureWall->Release();
		g_pD3DTextureWall = NULL;
	}

	if(g_pD3DVtxBuffWall != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffWall->Release();
		g_pD3DVtxBuffWall = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateWall(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 
	D3DXMATRIX mtxRot, mtxTranslate, mtxScl;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldWall);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, g_sclWall.x,g_sclWall.y,g_sclWall.z);
	D3DXMatrixMultiply(&g_mtxWorldWall,&g_mtxWorldWall, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotWall.y, g_rotWall.x, g_rotWall.z);
	D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_posWall.x, g_posWall.y, g_posWall.z);
	D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldWall);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pD3DVtxBuffWall, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureWall);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexWall(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffWall,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))						// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
		pVtx[1].vtx = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
		pVtx[2].vtx = D3DXVECTOR3(-100.0f, 0.0f, -100.0f);
		pVtx[3].vtx = D3DXVECTOR3(100.0f, 0.0f, -100.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffWall->Unlock();
	}

	return S_OK;
}

