//=============================================================================
//
// �w�i���� [skybox.cpp]
// Author : 
//
//=============================================================================
#include "skybox.h"
#include "Camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
	


const char* SkyboxTextureName[6] =
{
	"data/TEXTURE/sky_front.png",
	"data/TEXTURE/sky_back.png",
	"data/TEXTURE/sky_left.png",
	"data/TEXTURE/sky_right.png",
	"data/TEXTURE/sky_top.png",
	"data/TEXTURE/sky_down.png",
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexSkybox(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSkybox;
LPDIRECT3DTEXTURE9  g_pTexSkybox[6];



//=============================================================================
// ����������
//=============================================================================
HRESULT InitSkybox(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�A���̍쐬
	MakeVertexSkybox();
	// �e�N�X�`���̓ǂݍ���
	LoadSkyboxTexture();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSkybox(void)
{
	for (int i = 0; i < 6; i++)
	{
		if (g_pVtxBuffSkybox != NULL)
		{
			g_pVtxBuffSkybox->Release();
		}		
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSkybox(void)
{


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawSkybox(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX  mtxTranslate;
	CAMERA *camera = GetCamera(no);

	// ���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&mtxTranslate);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, camera->posEye.x, camera->posEye.y, camera->posEye.z);
	// ���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxTranslate);
	
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffSkybox, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int i = 0; i < 6; i++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexSkybox[i]);
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i*NUM_VERTEX, NUM_POLYGON);

	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

}

void LoadSkyboxTexture(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	for (int i = 0; i < 6; i++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,	// �f�o�C�X�ւ̃|�C���^
			SkyboxTextureName[i],			// �t�@�C���̖��O
			&g_pTexSkybox[i]);				// �ǂݍ��ރ������[
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexSkybox(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * 6,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pVtxBuffSkybox,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	VERTEX_3D *pVtxBuff, *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffSkybox->Lock(0, 0, (void**)&pVtxBuff, 0);

	for (int i = 0; i < 6; i++)
	{

		pVtx = pVtxBuff + 4 * i;

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor =
		pVtx[1].nor =
		pVtx[2].nor =
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = D3DXCOLOR(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	// �O
	pVtx = pVtxBuff;
	pVtx[0].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[1].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[2].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[3].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_X);


	// ��
	pVtx = pVtxBuff + 4;
	pVtx[0].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[1].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[2].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[3].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);

	// ��
	pVtx = pVtxBuff + 4 * 2;
	pVtx[0].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[1].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[2].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[3].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_X);

	// �E
	pVtx = pVtxBuff + 4 * 3;
	pVtx[0].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[1].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[2].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[3].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);


	// ��
	pVtx = pVtxBuff + 4 * 4;
	pVtx[0].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_Y);
	pVtx[1].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_Y);
	pVtx[2].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_Y);
	pVtx[3].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_Y);


	// ��
	pVtx = pVtxBuff + 4 * 5;
	pVtx[0].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_Y);
	pVtx[1].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_Y);
	pVtx[2].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_Y);
	pVtx[3].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_Y);

	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffSkybox->Unlock();

	return S_OK;
}



