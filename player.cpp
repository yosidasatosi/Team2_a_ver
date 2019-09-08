//=============================================================================
//
// ���f������ [Player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "camera.h"
#include "input.h"
#include "physic.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_CAR			"data/MODEL/car000.x"	// �ǂݍ��ރ��f���t�@�C����
#define	VALUE_MOVE_MODEL	(5.0f)					// �ړ����x
#define	VALUE_ROTATE_MODEL	(D3DX_PI * 0.02f)		// ��]���x
#define	PLAYER_SIZE_X		(20.0f)							// �r���{�[�h�̕�
#define	PLAYER_SIZE_Y		(20.0f)							// �r���{�[�h�̍���
#define	TEXTURE_PLAYER		"data/TEXTURE/hasiri.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_PATTERN_DIVIDE_X	(4)	// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)	// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define TIME_ANIMATION				(4)	// �A�j���[�V�����̐؂�ւ��J�E���g

#define DEFAULT_POS		(500.0f)	// �v���C���[�̏����ʒu(1P)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice, int no);
void SetTexturePlayer(int no, int cntPattern);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
PLAYER				playerWk[PLAYER_MAX];		// �\����

D3DXMATRIX			g_mtxWorldPlayer;			// ���[���h�}�g���b�N�X
int					g_player;					// �v���C���[�i���o�[

bool				g_update_x[PLAYER_MAX];
bool				g_update_z[PLAYER_MAX];

bool				g_clear;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		player->pD3DTexture = NULL;
		player->g_pD3DVtxBuff = NULL;

		// ���_���̍쐬
		MakeVertexPlayer(pDevice,i);

		// �傫���A�ʒu�A�����̏����ݒ�
		player->pos = D3DXVECTOR3(-DEFAULT_POS + i * (DEFAULT_POS * 2), 10.0f, 0.0f);
		player->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		player->scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		
		player->Turn = false;

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
			TEXTURE_PLAYER,		// �t�@�C���̖��O
									&player->pD3DTexture);	// �ǂݍ��ރ������[

	}

	g_player = 0;

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_update_x[i] = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	PLAYER *player = GetPlayer(0);

	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		if (player->pD3DTexture != NULL)
		{// �e�N�X�`���̊J��
			player->pD3DTexture->Release();
			player->pD3DTexture = NULL;
		}

		if (player->g_pD3DVtxBuff != NULL)
		{// ���_�o�b�t�@�̊J��
			player->g_pD3DVtxBuff->Release();
			player->g_pD3DVtxBuff = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	//if (GetKeyboardTrigger(DIK_F2))
	//{
	//	g_player++;
	//	g_player %= PLAYER_MAX;
	//}

	//PLAYER *player = GetPlayer(g_player);

	PLAYER *player1 = GetPlayer(0);
	PLAYER *player2 = GetPlayer(1);
	D3DXVECTOR3 rotCamera;
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		PLAYER *player = GetPlayer(i);
		// �A�j���[�V����
		player->CountAnim++;
		if ((player->CountAnim % TIME_ANIMATION) == 0)
		{
			// �p�^�[���̐؂�ւ�
			player->PatternAnim = (player->PatternAnim + 1) % ANIM_PATTERN_NUM;

			// �e�N�X�`�����W��ݒ�
			SetTexturePlayer(i, player->PatternAnim);
		}

	}
	// �J�����̌����擾
	rotCamera = GetRotCamera(0);

	// �v���C���[1��X�X�V
	if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D))
	{
		g_update_x[0] = true;
	}
	// �v���C���[1��Z�X�V
	if (GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_S))
	{
		g_update_z[0] = true;
	}

	// �v���C���[2��X�X�V
	if (GetKeyboardPress(DIK_LEFT) || GetKeyboardPress(DIK_RIGHT))
	{
		g_update_x[1] = true;
	}
	// �v���C���[2��Z�X�V
	if (GetKeyboardPress(DIK_UP) || GetKeyboardPress(DIK_DOWN))
	{
		g_update_z[1] = true;
	}

	// �v���C���[1�ړ�����
	if(GetKeyboardPress(DIK_D))
	{// �E�ړ�
		player1->pos.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_MODEL;
		player1->pos.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_MODEL;
		player1->sayuu = true;
	}
	else if (GetKeyboardPress(DIK_A))
	{// ���ړ�
		player1->pos.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_MODEL;
		player1->pos.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_MODEL;
		player1->sayuu = false;
	}
	else if (GetKeyboardPress(DIK_W))
	{// ���ړ�
		player1->pos.z += VALUE_MOVE_MODEL;
	}
	else if (GetKeyboardPress(DIK_S))
	{// ��O�ړ�
		player1->pos.z -= VALUE_MOVE_MODEL;
	}

	// �v���C���[1
	if (player1->jump == true)
	{// �W�����v

		FreeFallMotion(&player1->pos.y, &player1->speed, &player1->a);

		if (player1->pos.y < 10)
		{
			player1->jump = false;
			player1->pos.y = 10;

		}

	}
	else
	{
		if (GetKeyboardTrigger(DIK_SPACE))
		{

			player1->jump = true;
			player1->speed = 500;
			player1->a = -9.8f * 3;
		}
	}



	if (GetKeyboardPress(DIK_RIGHT))
	{
		player2->pos.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_MODEL;
		player2->sayuu = true;

	}
	else if (GetKeyboardPress(DIK_LEFT))
	{
		player2->pos.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_MODEL;
		player2->sayuu = false;

	}
	else if (GetKeyboardPress(DIK_UP))
	{
		player2->pos.z += VALUE_MOVE_MODEL;


	}
	else if (GetKeyboardPress(DIK_DOWN))
	{
		player2->pos.z -= VALUE_MOVE_MODEL;

	}

	if (player2->jump == true)
	{

		FreeFallMotion(&player2->pos.y, &player2->speed, &player2->a);

		if (player2->pos.y < 10)
		{
			player2->jump = false;
			player2->pos.y = 10;

		}

	}
	else
	{
		if (GetKeyboardTrigger(DIK_C))
		{

			player2->jump = true;
			player2->speed = 500;
			player2->a = -9.8f * 3;
		}
	}


	if (!GetGouryu())
	{
		if (GetKeyboardTrigger(DIK_1))
		{
			if (player1->Turn)
			{
				player1->Turn = false;
			}
			else
			{
				player1->Turn = true;
			}
			TurnCamera(1);
			SetCameraZ(1);
		}

		if (GetKeyboardTrigger(DIK_2))
		{
			if (player2->Turn)
			{
				player2->Turn = false;
			}
			else
			{
				player2->Turn = true;
			}
			TurnCamera(2);
			SetCameraZ(2);
		}
		SetViewport();
	}
	
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (g_update_x[i])
		{
			SetCameraX(0);
			SetCameraX(i + 1);
			g_update_x[i] = false;
		}
		if (g_update_z[i])
		{
			SetCameraZ(0);
			SetCameraZ(i + 1);
			g_update_z[i] = false;
		}
	}

	if (CheckTurn())
	{
		if (CheckClear())
		{
			SetFade(FADE_OUT);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	PLAYER *player = GetPlayer(0);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	for (int i = 0; i < PLAYER_MAX; i++, player++)
	{
		// ���C���e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// �r���[�}�g���b�N�X���擾
		mtxView = GetMtxView();

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldPlayer);

		// �|���S���𐳖ʂɌ�����
		g_mtxWorldPlayer._11 = mtxView._11;
		g_mtxWorldPlayer._12 = mtxView._21;
		g_mtxWorldPlayer._13 = mtxView._31;
		g_mtxWorldPlayer._21 = mtxView._12;
		g_mtxWorldPlayer._22 = mtxView._22;
		g_mtxWorldPlayer._23 = mtxView._32;
		g_mtxWorldPlayer._31 = mtxView._13;
		g_mtxWorldPlayer._32 = mtxView._23;
		g_mtxWorldPlayer._33 = mtxView._33;




		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScale, player->scl.x,
			player->scl.y,
			player->scl.z);
		D3DXMatrixMultiply(&g_mtxWorldPlayer,
			&g_mtxWorldPlayer, &mtxScale);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTranslate, player->pos.x,
			player->pos.y,
			player->pos.z);
		D3DXMatrixMultiply(&g_mtxWorldPlayer,
			&g_mtxWorldPlayer, &mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPlayer);

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, player->g_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, player->pD3DTexture);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		
		// ���C���e�B���O��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice ,int no)
{

	PLAYER *player = GetPlayer(no);

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&player->g_pD3DVtxBuff,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))						// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		player->g_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-PLAYER_SIZE_X / 2, PLAYER_SIZE_Y / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(PLAYER_SIZE_X / 2, PLAYER_SIZE_Y / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-PLAYER_SIZE_X / 2, -PLAYER_SIZE_Y / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(PLAYER_SIZE_X / 2, -PLAYER_SIZE_Y / 2, 0.0f);

		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// ���_�f�[�^���A�����b�N����
		player->g_pD3DVtxBuff->Unlock();
	}

	return S_OK;
}
//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void SetTexturePlayer(int no, int cntPattern)
{
	PLAYER *player = &playerWk[no];

	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X;
	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X;
	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X;
	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		player->g_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		if (player->sayuu)
		{
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		else
		{

			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		}

		// ���_�f�[�^���A�����b�N����
		player->g_pD3DVtxBuff->Unlock();

	}
}

bool CheckClear(void)
{
	PLAYER *player1 = GetPlayer(0);
	PLAYER *player2 = GetPlayer(1);

	g_clear = CheckHitBC(player1->pos, player2->pos, PLAYER_SIZE_X / 2, PLAYER_SIZE_X / 2);

	return g_clear;
}

//=============================================================================
// �v���C���[��Turn�̎擾
//=============================================================================
bool GetTurn(int no)
{
	PLAYER *player = GetPlayer(no);
	return player->Turn;
}

//*****************************************************************************
// �v���C���[�Q�b�g�֐�
//*****************************************************************************
PLAYER *GetPlayer(int no)
{
	return (&playerWk[no]);
}

bool GetClear(void)
{
	return g_clear;
}