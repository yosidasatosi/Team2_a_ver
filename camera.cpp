//=============================================================================
//
// �J�������� [camera.cpp]
// Author : 
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CAM_POS_P_X			(0.0f)					// �J�����̎��_�����ʒu(X���W)
#define	CAM_POS_P_Y			(50.0f)				// �J�����̎��_�����ʒu(Y���W)
#define	CAM_POS_P_Z			(-200.0f)				// �J�����̎��_�����ʒu(Z���W)
#define	CAM_POS_R_X			(0.0f)					// �J�����̒����_�����ʒu(X���W)
#define	CAM_POS_R_Y			(0.0f)					// �J�����̒����_�����ʒu(Y���W)
#define	CAM_POS_R_Z			(0.0f)					// �J�����̒����_�����ʒu(Z���W)
#define	VIEW_ANGLE			(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��
#define	VIEW_NEAR_Z			(30.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z			(2000.0f)										// �r���[���ʂ�FarZ�l
#define	VALUE_MOVE_CAMERA	(2.0f)											// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)								// �J�����̉�]��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
float GetAspect(void);
float SetAspect(void);

void SetCamera0PosAt(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DXMATRIX		g_mtxView;					// �r���[�}�g���b�N�X
D3DXMATRIX		g_mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X

D3DXMATRIX		g_m;

float			g_Aspect;					// �J�����̃A�X�y�N�g��
int				g_camera;					// �g�p����J�����̕ύX

int				g_cameramax;				// ��ʂɓ����`�悷��J�����̍ő吔
bool			g_gouryu;
bool			g_up;

CAMERA cameraWk[CAMERA_MAX];

//=============================================================================
// �J�����̏���������
//=============================================================================
HRESULT InitCamera(void)
{
	CAMERA *camera = GetCamera(0);

	for (int i = 0; i < CAMERA_MAX; i++, camera++)
	{
		if (i == 0)
		{
			camera->posEye = D3DXVECTOR3(GetPlayer(1)->pos.x + GetPlayer(0)->pos.x, CAM_POS_P_Y, CAM_POS_P_Z);
			camera->posAt = D3DXVECTOR3(camera->posEye.x, 0.0f, 0.0f);
		}
		else
		{
			camera->posEye = D3DXVECTOR3(GetPlayer(i - 1)->pos.x, CAM_POS_P_Y, CAM_POS_P_Z);
			camera->posAt = GetPlayer(i - 1)->pos;
		}
		camera->vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		camera->rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);

		float fVecX, fVecZ;
		fVecX = camera->posEye.x - camera->posAt.x;
		fVecZ = camera->posEye.z - camera->posAt.z;
		camera->fLengthInterval = sqrtf(fVecX * fVecX + fVecZ * fVecZ);
		camera->turn = false;
	}

	g_gouryu = false;
	g_up = false;
	g_camera = 0;
	g_cameramax = PLAYER_MAX;
	g_Aspect = GetAspect();

	return S_OK;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	PLAYER *player1 = GetPlayer(0);
	PLAYER *player2 = GetPlayer(1);

	if (CheakTurn())
	{
		if (player2->pos.x - player1->pos.x <= 200.0f)
		{
			g_gouryu = true;
		}
		else
		{
			g_gouryu = false;
		}
	}

	if (g_gouryu)
	{// �͈͓��Ȃ�J�����̒����_���X�V
		SetCamera0PosAt();
	}

	if (!g_up)
	{	// ��x����������ʂ�
		if (g_gouryu)
		{	// �������ɍs������
			g_cameramax = 1;

			g_Aspect = SetAspect();

			SetViewport();

			g_up = true;
		}
	}
	else
	{	// ��x����������ʂ�
		if (!g_gouryu)
		{	// ������A���ꂽ���ɍs������
			g_cameramax = 2;

			g_Aspect = SetAspect();

			SetViewport();

			g_up = false;
		}
	}

	for (int i = 0; i < g_cameramax; i++)
	{
		CAMERA *camera = GetCamera(i + 1);

		if (camera->turn)
		{

		}
	}

	//if (GetKeyboardTrigger(DIK_F1))
	//{
	//	g_cameramax--;
	//	if (g_cameramax < 1)
	//	{
	//		g_cameramax = 2;
	//	}
	//	
	//	g_Aspect = SetAspect();

	//	SetViewport();
	//}

	//if (GetKeyboardTrigger(DIK_F1))
	//{
	//	g_camera++;
	//	g_camera %= PLAYER_MAX;
	//}

	//CAMERA *camera = GetCamera(g_camera + 1);

	////for (int i = 0; i < CAMERA_MAX; i++, camera++)
	////{
	//	if (GetKeyboardPress(DIK_LEFT))
	//	{// ���_����u���v
	//		camera->rot.y -= VALUE_ROTATE_CAMERA;
	//		if (camera->rot.y < -D3DX_PI)
	//		{
	//			camera->rot.y += D3DX_PI * 2.0f;
	//		}

	//		//camera->posEye.x = camera->posAt.x - sinf(camera->rot.y) * camera->fLengthInterval;
	//		//camera->posEye.z = camera->posAt.z - cosf(camera->rot.y) * camera->fLengthInterval;

	//		//camera->posAt.x = camera->posEye.x - sinf(camera->rot.y) * camera->fLengthInterval;
	//		//camera->posAt.z = camera->posEye.z - cosf(camera->rot.y) * camera->fLengthInterval;
	//	}
	//	if (GetKeyboardPress(DIK_RIGHT))
	//	{// ���_����u�E�v
	//		camera->rot.y += VALUE_ROTATE_CAMERA;
	//		if (camera->rot.y > D3DX_PI)
	//		{
	//			camera->rot.y -= D3DX_PI * 2.0f;
	//		}

	//		//camera->posAt.x = camera->posEye.x - sinf(camera->rot.y) * camera->fLengthInterval;
	//		//camera->posAt.z = camera->posEye.z - cosf(camera->rot.y) * camera->fLengthInterval;

	//		//camera->posEye.x = camera->posAt.x - sinf(camera->rot.y) * camera->fLengthInterval;
	//		//camera->posEye.z = camera->posAt.z - cosf(camera->rot.y) * camera->fLengthInterval;
	//	}

	//	//if (GetKeyboardPress(DIK_UP))
	//	//{// �����_�Ǝ��_�Ƃ̋���
	//	//	camera->fLengthInterval -= 0.5f;
	//	//	camera->posEye.x = camera->posAt.x - sinf(camera->rot.y) * camera->fLengthInterval;
	//	//	camera->posEye.z = camera->posAt.z - cosf(camera->rot.y) * camera->fLengthInterval;
	//	//}

	//	//if (GetKeyboardPress(DIK_DOWN))
	//	//{// �����_�Ǝ��_�Ƃ̋���
	//	//	camera->fLengthInterval += 0.5f;
	//	//	camera->posEye.x = camera->posAt.x - sinf(camera->rot.y) * camera->fLengthInterval;
	//	//	camera->posEye.z = camera->posAt.z - cosf(camera->rot.y) * camera->fLengthInterval;
	//	//}

	//	// ������܂��(�J�����̈ʒu������)
	//	//camera->posEye.x = camera->posAt.x - sinf(camera->rot.y) * camera->fLengthInterval;
	//	//camera->posEye.z = camera->posAt.z - cosf(camera->rot.y) * camera->fLengthInterval;

	//	// ��������n��(�J�����̒����_������)
	//	camera->posAt.x = camera->posEye.x - sinf(camera->rot.y) * camera->fLengthInterval;
	//	camera->posAt.z = camera->posEye.z - cosf(camera->rot.y) * camera->fLengthInterval;
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(int no)
{
	CAMERA *camera = GetCamera(no);

	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_mtxView,
		&camera->posEye,		// �J�����̎��_
		&camera->posAt,			// �J�����̒����_
		&camera->vecUp);		// �J�����̏����

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
		VIEW_ANGLE,			// �r���[���ʂ̎���p
		g_Aspect,			// �r���[���ʂ̃A�X�y�N�g��
		VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l
	
	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);
}

//=============================================================================
// �J�����̌����̎擾
//=============================================================================
void SetCameraRot(void)
{
	D3DXVECTOR3 player_pos = GetPlayer(0)->pos;

	CAMERA *camera = GetCamera(0);
}

//=============================================================================
// �J�����̈ʒu�E�����_�̐ݒ�
//=============================================================================
void SetCameraPos(int num)
{
	PLAYER *player = GetPlayer(num);
	CAMERA *camera = GetCamera(num + 1);

	camera->posEye.x = player->pos.x;
	camera->posAt.x = player->pos.x;
}

//=============================================================================
// �J�����̃A�X�y�N�g��̐ݒ�
//=============================================================================
float SetAspect(void)
{
	float aspect;

	switch (g_cameramax)
	{
	case 1:
		aspect = ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
		break;
	case 2:
		aspect = ((float)SCREEN_CENTER_X / (float)SCREEN_HEIGHT);
		break;
	}

	return aspect;
}

//=============================================================================
// �J�����̌����̎擾
//=============================================================================
D3DXVECTOR3 GetRotCamera(int no)
{
	return GetCamera(no)->rot;
}

//=============================================================================
// �r���[�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	return g_mtxView;
}

//=============================================================================
// �J�����̎擾
//=============================================================================
CAMERA *GetCamera(int no)
{
	return (&cameraWk[no]);
}

//=============================================================================
// �J�����̃A�X�y�N�g��̎擾
//=============================================================================
float GetAspect(void)
{
	float aspect;

	switch (g_cameramax)
	{
	case 2:
		aspect = ((float)SCREEN_CENTER_X / (float)SCREEN_HEIGHT);
		break;

	case 1:
	case 3:
	case 4:
		aspect = ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
		break;
	}

	return aspect;
}

int GetCameraNum(void)
{
	return g_cameramax;
}

void SetCamera0PosAt(void)
{
	PLAYER *player1 = GetPlayer(0);
	PLAYER *player2 = GetPlayer(1);

	CAMERA *camera = GetCamera(0);

	if (player1->Turn && player2->Turn)
	{
		camera->posEye.z = -CAM_POS_P_Z;
	}
	else
	{
		camera->posEye.z = CAM_POS_P_Z;
	}

	if (player1->pos.x * player2->pos.x > -1)
	{
		camera->posEye.x = (player2->pos.x + player1->pos.x) / 2;
		camera->posAt.x = (player2->pos.x + player1->pos.x) / 2;
	}
	else
	{
		float max, min;

		if (player2->pos.x > player1->pos.x * -1)
		{
			max = player2->pos.x;
			min = player1->pos.x;
		}
		else
		{
			max = player1->pos.x;
			min = player2->pos.x;
		}

		camera->posAt.x = max - (max - min) / 2;
		camera->posEye.x = max - (max - min) / 2;
	}
}

void TurnCamera(int playerno)
{
	CAMERA *camera = GetCamera(playerno);

	camera->posEye.z *= -1.0f;
}

bool CheakTurn(void)
{
	PLAYER *player1 = GetPlayer(0);
	PLAYER *player2 = GetPlayer(1);

	if (player1->Turn && player2->Turn || !(player1->Turn) && !(player2->Turn))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GetGouryu(void)
{
	return g_gouryu;
}