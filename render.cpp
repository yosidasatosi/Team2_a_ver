//=============================================================================
//
// ���C������ [render.cpp]
// Author : 
//
//=============================================================================
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "player.h"

// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void SetBackGroundPolygon(int num);
void ChangeRenderTarget(int no);
void RecoverRenderTarget();

void InitRendertexture();
HRESULT InitBackGroundPolygon(void);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3DSURFACE9		pRenderDepthBuffer;
LPDIRECT3DTEXTURE9		pRenderTexture[PLAYER_MAX];

D3DVIEWPORT9			RenderViewport;

//*****************************************************************************
LPDIRECT3DSURFACE9		pDefaultRenderTargetSurface;
LPDIRECT3DSURFACE9		pDefaultDepthBufferSurface;
LPDIRECT3DSURFACE9		pRenderTextureSurface;
D3DVIEWPORT9			DefaultViewport;

LPDIRECT3DTEXTURE9		g_pD3DBackGround = NULL;	// �e�N�X�`���ւ̃|�C���^

LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBackGround[PLAYER_MAX] = { NULL, NULL };	// ���_�o�b�t�@�ւ̃|�C���^


//=============================================================================
// �����_�����O�e�N�X�`���쐬����
//=============================================================================
void InitRendertexture()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�����_�����O�e�N�X�`���p�r���[�|�[�g
	RenderViewport.X = 0;
	RenderViewport.Y = 0;
	RenderViewport.Width = SCREEN_WIDTH;
	RenderViewport.Height = SCREEN_HEIGHT;
	RenderViewport.MinZ = 0.0f;
	RenderViewport.MaxZ = 1.0f;

	//�f�v�X�o�b�t�@�쐬(2�ׂ̂���T�C�Y)
	pDevice->CreateDepthStencilSurface(1024, 1024,		//�T�C�Y
		D3DFMT_D24S8,	//�s�N�Z���t�H�[�}�b�g
		D3DMULTISAMPLE_NONE,//marutisanpuringu OFF
		0,
		FALSE,
		&pRenderDepthBuffer,
		NULL);

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		//�����_�[�^�[�Q�b�g�쐬(2�ׂ̂���T�C�Y)
		D3DXCreateTexture(pDevice,
			SCREEN_WIDTH, 1024,		//�T�C�Y
			1,				//�݂��Ճ}�b�v��
			D3DUSAGE_RENDERTARGET,	//siyou�ړI
			D3DFMT_X8R8G8B8,
			D3DPOOL_DEFAULT,
			&pRenderTexture[i]);
	}
}

//=============================================================================
// �����_�[�^�[�Q�b�g�؂�ւ�����
//=============================================================================
void ChangeRenderTarget(int no)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

	//�����_�[�^�[�Q�b�g��؂�ւ�

	//�f�t�H���g�r���[�|�[�g�ۑ�
	pD3DDevice->GetViewport(&DefaultViewport);

	//�f�t�H���g�̃T�[�t�F�[�X��ۑ�
	pD3DDevice->GetRenderTarget(0, &pDefaultRenderTargetSurface);
	pD3DDevice->GetDepthStencilSurface(&pDefaultDepthBufferSurface);
	//�����_�����O�e�N�X�`���̃T�[�t�F�[�X�擾
	pRenderTexture[no]->GetSurfaceLevel(0, &pRenderTextureSurface);

	//�V���������_�����O�^�[�Q�b�g���Z�b�g
	pD3DDevice->SetRenderTarget(0, pRenderTextureSurface);
	//�V�����f�v�X�o�b�t�@���Z�b�g
	pD3DDevice->SetDepthStencilSurface(pRenderDepthBuffer);

	//�r���[�|�[�g�̐؂�ւ�
	pD3DDevice->SetViewport(&RenderViewport);

	//�N���A
	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
}

//=============================================================================
// �����_�[�^�[�Q�b�g���A����
//=============================================================================
void RecoverRenderTarget()
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

	//�����_�[�^�[�Q�b�g�̕��A
	pD3DDevice->SetRenderTarget(0, pDefaultRenderTargetSurface);
	pD3DDevice->SetDepthStencilSurface(pDefaultDepthBufferSurface);

	//�T�[�t�F�[�X�̉��
	pDefaultRenderTargetSurface->Release();
	pDefaultDepthBufferSurface->Release();
	pRenderTextureSurface->Release();

	//�r���[�|�[�g�̕��A
	pD3DDevice->SetViewport(&DefaultViewport);
}

//=============================================================================
// �����_�����O�e�N�X�`���w�i������
//=============================================================================
HRESULT	InitBackGroundPolygon(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&g_pD3DVtxBuffBackGround[i],	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL);						// NULL�ɐݒ�

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_2D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_pD3DVtxBuffBackGround[i]->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(0.0f, 1024, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, 1024, 0.0f);

			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;


			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);

			// ���_�f�[�^���A�����b�N����
			g_pD3DVtxBuffBackGround[i]->Unlock();
		}
	}

	return S_OK;
}

//=============================================================================
// �����_�����O�e�N�X�`���w�i�I������
//=============================================================================
void	UninitBackGroundPolygon()
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_pD3DVtxBuffBackGround[i] != NULL)
		{// ���_�o�b�t�@�̊J��
			g_pD3DVtxBuffBackGround[i]->Release();
			g_pD3DVtxBuffBackGround[i] = NULL;
		}
	}
}

//=============================================================================
// �����_�����O�e�N�X�`���w�i�`��
//=============================================================================
void DrawBackGroundPolygon(int no)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pD3DDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pD3DDevice->SetTexture(0, pRenderTexture[no]);

	//
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pD3DDevice->SetStreamSource(0, g_pD3DVtxBuffBackGround[no], 0, sizeof(VERTEX_2D));

	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �|���S���̕`��
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void InitRender(void)
{
	//�����_�����O�e�N�X�`���֘A
	InitBackGroundPolygon();
	InitRendertexture();
}

void SetRender(int num, int no)
{
	if (num == 1)
	{ // �J�����̎g�p����1�̂Ƃ�
		SetBackGroundPolygon(no);
	}
	else
	{
		SetBackGroundPolygon(no + 1);
	}

	//�����_�����O�^�[�Q�b�g�̐؂�ւ�
	ChangeRenderTarget(no);
}

void DrawRender(int no, bool flag)
{
	//�����_�����O�^�[�Q�b�g�̕��A
	RecoverRenderTarget();

	if (flag)
	{
		//�����_�����O�����w�i��\��
		DrawBackGroundPolygon(no);
	}
}

//=============================================================================
// �����_�����O�e�N�X�`���w�i�ݒ�
//=============================================================================
void SetBackGroundPolygon(int num)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	int no = 0;		// �z��̃i���o�[�̏�����

	if (num != 0)
	{// �J�����i���o�[��0����Ȃ��Ƃ�
		no = num - 1;	// �z��̃i���o�[��ݒ�
	}

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffBackGround[no]->Lock(0, 0, (void**)&pVtx, 0);

	switch (num)
	{
	case 0:
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, 1024.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, 1024.0f, 0.0f);
		break;
	case 1:
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_CENTER_X, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, 1024.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_CENTER_X, 1024.0f, 0.0f);
		break;
	case 2:
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(SCREEN_CENTER_X, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(SCREEN_CENTER_X, 1024.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, 1024.0f, 0.0f);
		break;
	}

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffBackGround[no]->Unlock();
}