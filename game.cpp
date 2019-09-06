//=============================================================================
//
// ���C������ [main.cpp]
// Author : AKIRA TANAKA
//
//=============================================================================
#include "game.h"
#include "light.h"
#include "camera.h"
#include "field.h"
#include "player.h"
#include "star1.h"
#include "render.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "skybox.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void GameObject(void);
void GameUI(void);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************


HRESULT InitGame(void)
{
	// ���f���̏���������
	InitPlayer();

	// ���C�g�̏���������
	InitLight();

	// �J�����̏���������
	InitCamera();

	// �n�ʂ̏���������
	InitField();

	// �r���[�|�[�g�ݒ�
	SetViewport();

	//�����_�����O������
	InitRender();

	// �M�~�b�N(�X�^�[1)
	InitStar1();

	// �X�J�C�{�b�N�X������
	InitSkybox();

	return S_OK;
}

void UninitGame(void)
{
	// ���C�g�̏I������
	UninitLight();

	// �J�����̏I������
	UninitCamera();

	// �n�ʂ̏I������
	UninitField();

	// ���f���̏I������
	UninitPlayer();

	UninitBackGroundPolygon();

	// �M�~�b�N(�X�^�[1)�̏I������
	UninitStar1();

	// �X�J�C�{�b�N�X�I������
	UninitSkybox();
}

void UpdateGame(void)
{
	// ���C�g�̍X�V����
	UpdateLight();

	// �J�����̍X�V����
	UpdateCamera();

	// �n�ʂ̍X�V����
	UpdateField();

	// ���f���̍X�V����
	UpdatePlayer();

	// �M�~�b�N(�X�^�[1)�̍X�V����
	UpdateStar1();

	// ��X�V����
	UpdateSkybox();
}

void DrawGame(void)
{
	int camera_num = GetCameraNum();

	// �J������2�Ƃ�����Ă���E����Ă��Ȃ���
	if (CheckTurn())
	{
		for (int i = 0; i < camera_num; i++)
		{
			Viewport(i);

			SetUseCamera(i);

			// �J�������񂵂Ă��Ȃ��Ƃ��̓����_�����O�֘A�̏����͍s��Ȃ�
			if (GetTurn(i))
			{ // �v���C���[(i)���J�������񂵂Ă���Ƃ�
			  // �����_�����O�`�揈��
				SetRender(camera_num, i);
			}

			// �I�u�W�F�N�g�`�揈��
			GameObject();

			if (GetTurn(i))
			{
				DrawRender(i, false);
			}

			// �e�N�X�`���̐ݒ肪�I����Ă���`����s��
			if (i == camera_num - 1)
			{ // �J�����̑䐔���������s��ꂽ��
				for (int j = 0; j < camera_num; j++)
				{ // �J�����̑䐔���`����s��
					if (GetTurn(j))
					{ // �J�������񂳂�Ă�����
						DrawBackGroundPolygon(j);	//�����_�����O�����w�i��\��
					}
				}
				// UI�`�揈��
				GameUI();
			}
		}
	}
	else
	{ // �J�������Е���������Ă���Ƃ�
		if (GetTurn(0))
		{ // �v���C���[1������Ă��邩����
			for (int i = 0; i < camera_num; i++)
			{ // �v���C���[1���珇�ɕ`����s��
				Viewport(i);

				SetUseCamera(i);

				if (GetTurn(i))
				{
					// �����_�����O�`�揈��
					SetRender(camera_num, i);
				}
				// �I�u�W�F�N�g�`�揈��
				GameObject();

				if (GetTurn(i))
				{
					// �����_�����O�`�揈��
					DrawRender(i, true);
				}

				// �Ō��UI�`����s��
				if (i == camera_num - 1)
				{
					// UI�`�揈��
					GameUI();
				}
			}
		}
		else
		{
			// �v���C���[�Q���珇�ɕ`����s��
			for (int i = camera_num - 1; i > -1; i--)
			{
				Viewport(i);

				SetUseCamera(i);

				if (GetTurn(i))
				{
					// �����_�����O�`�揈��
					SetRender(camera_num, i);
				}

				// �I�u�W�F�N�g�`�揈��
				GameObject();

				if (GetTurn(i))
				{
					DrawRender(i, true);
				}

				// �Ō��UI�`����s��
				if (i == camera_num)
				{
					// UI�`�揈��
					GameUI();
				}
			}
		}
	}
}

void GameObject(void)
{
	for (int i = 0; i < CAMERA_MAX; i++)
	{
		// ��`��
		DrawSkybox(i);
	}

	// �t�B�[���h�̕`�揈��
	DrawField();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �X�^�[1�̕`�揈��
	DrawStar1();
}

void GameUI(void)
{

}