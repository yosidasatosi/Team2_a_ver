//=============================================================================
//
// �J�������� [camera.cpp]
// Author : chanp
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CAM_POS_P_X			(0.0f)											// �J�����̎��_�����ʒu(X���W)
#define	CAM_POS_P_Y			(50.0f)										// �J�����̎��_�����ʒu(Y���W)
#define	CAM_POS_P_Z			(-200.0f)										// �J�����̎��_�����ʒu(Z���W)
#define	CAM_POS_R_X			(0.0f)											// �J�����̒����_�����ʒu(X���W)
#define	CAM_POS_R_Y			(0.0f)											// �J�����̒����_�����ʒu(Y���W)
#define	CAM_POS_R_Z			(0.0f)											// �J�����̒����_�����ʒu(Z���W)
#define	VIEW_ANGLE			(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��
#define	VIEW_NEAR_Z			(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z			(10000.0f)										// �r���[���ʂ�FarZ�l
#define	VALUE_MOVE_CAMERA	(2.0f)											// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)								// �J�����̉�]��

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct COLOR {
	int r;
	int g;
	int b;
};

struct VIEW {
	D3DXVECTOR3 vEyePt;
	D3DXVECTOR3 vLookatPt;
	D3DXVECTOR3 vUpVec;
	D3DXMATRIXA16 matView;
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
float SetAspect(void);

void SetCamera0PosAt(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DXMATRIX		g_mtxView;					// �r���[�}�g���b�N�X
D3DXMATRIX		g_mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X

float			g_Aspect;					// �J�����̃A�X�y�N�g��
int				g_camera;					// �g�p����J�����̕ύX

int				g_cameramax;				// ��ʂɓ����`�悷��J�����̍ő吔
bool			g_gouryu;					// true:1�@false:2�� (�g�p����J����)
bool			g_up;						// true:�����ɓ���@false:�����ɓ���Ȃ�

CAMERA cameraWk[CAMERA_MAX];

COLOR g_color[] = {
	{ 20, 60, 20 },
{ 20, 60, 20 },
};

D3DVIEWPORT9 g_port[PLAYER_MAX];
VIEW g_view[PLAYER_MAX];

//=============================================================================
// �J�����̏���������
//=============================================================================
HRESULT InitCamera(void)
{
	CAMERA *camera = GetCamera(0);

	// �J�����̍ő吔���̏�����
	for (int i = 0; i < CAMERA_MAX; i++, camera++)
	{
		if (i == 0)
		{ // �J����0(�������Ȃ����p�J����)�̐ݒ�
			camera->posEye = D3DXVECTOR3(GetPlayer(1)->pos.x + GetPlayer(0)->pos.x, CAM_POS_P_Y, CAM_POS_P_Z);
			camera->posAt = D3DXVECTOR3(camera->posEye.x, 0.0f, 0.0f);
		}
		else
		{ // �J����1�E2(�e�v���C���[�p�J����)�̐ݒ�
			camera->posEye = D3DXVECTOR3(GetPlayer(i - 1)->pos.x, CAM_POS_P_Y, CAM_POS_P_Z);
			camera->posAt = GetPlayer(i - 1)->pos;
		}

		camera->vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		camera->rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	}

	// �e��O���[�o���֐��̏�����
	g_gouryu = false;
	g_up = false;
	g_camera = 0;
	g_cameramax = PLAYER_MAX;
	g_Aspect = SetAspect();

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

	if (player2->pos.x - player1->pos.x <= 200.0f && CheckTurn())
	{
		g_gouryu = true;
	}
	else
	{
		g_gouryu = false;
	}

	if (g_gouryu)
	{// �J��������Ȃ��
		SetCamera0PosAt();	// �J����0�̒����_��ύX
	}

	if (!g_up)
	{	// ��x����������ʂ�
		if (g_gouryu)
		{	// �������ɍs������
			g_cameramax = 1;			//�J�����̎g�p�ő吔��1�ɂ���

			g_Aspect = SetAspect();		// �J�����̃A�X�y�N�g��̍X�V

			SetViewport();				// �r���[�|�[�g�̍X�V

			SetCameraZ(0);

			g_up = true;				// ���̏����ɓ���Ȃ��悤�X�V
		}
	}
	else
	{	// ��x����������ʂ�
		if (!g_gouryu)
		{	// ������A���ꂽ���ɍs������
			g_cameramax = 2;			// �J�����̎g�p�ő吔��2�ɂ���

			g_Aspect = SetAspect();		// �J�����̃A�X�y�N�g��̍X�V

			SetViewport();				// �r���[�|�[�g�̍X�V

			g_up = false;				// ���̏����ɓ���Ȃ��悤�X�V
		}
	}
}

//=============================================================================
// �g�p����J�����̐ݒ菈��
//=============================================================================
void SetUseCamera(int no)
{
	if (g_cameramax == 1)
	{ // �g�p���Ă���J������1��̎�
		SetCamera(no);		// �J�����̐ݒ菈��
	}
	else
	{ // �g�p���Ă���J������2��̎�
		SetCamera(no + 1);	// �J�����̐ݒ菈��
	}
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
// �J�����̈ʒu(X)�̐ݒ�
//=============================================================================
void SetCameraX(int num)
{
	if (num == 0)
	{
		SetCamera0PosAt();
	}
	else
	{
		PLAYER *player = GetPlayer(num - 1);
		CAMERA *camera = GetCamera(num);

		camera->posEye.x = player->pos.x;
		camera->posAt.x = player->pos.x;
	}
}

//=============================================================================
// �J�����̈ʒu(Z)�̐ݒ�
//=============================================================================
void SetCameraZ(int num)
{
	PLAYER *player;
	CAMERA *camera;

	if (num == 0)
	{
		PLAYER *player1 = GetPlayer(0);
		PLAYER *player2 = GetPlayer(1);

		camera = GetCamera(num);

		if (player1->Turn)
		{
			if (player1->pos.z > player2->pos.z)
			{
				player = player1;
			}
			else
			{
				player = player2;
			}
		}
		else
		{
			if (player1->pos.z < player2->pos.z)
			{
				player = player1;
			}
			else
			{
				player = player2;
			}
		}
	}
	else
	{
		player = GetPlayer(num - 1);
		camera = GetCamera(num);

		num--;
	}

	//if (g_gouryu)
	//{
	//	PLAYER *player1 = GetPlayer(num);
	//	PLAYER *player2 = GetPlayer((num + 1) % 2);

	//	camera = GetCamera(0);

	//	if (player1->pos.z < player2->pos.z)
	//	{
	//		player = player1;
	//	}
	//	else
	//	{
	//		player = player2;
	//	}
	//}
	//else
	//{
	//	player = GetPlayer(num);
	//	camera = GetCamera(num + 1);
	//}

	if (GetTurn(num))
	{
		camera->posEye.z = player->pos.z - CAM_POS_P_Z;
		if (camera->posEye.z < -CAM_POS_P_Z)
		{
			camera->posEye.z = -CAM_POS_P_Z;
			camera->posAt.z = CAM_POS_R_Z;
		}
		else
		{
			camera->posAt.z = player->pos.z;
		}
	}
	else
	{
		camera->posEye.z = player->pos.z + CAM_POS_P_Z;
		if (camera->posEye.z > CAM_POS_P_Z)
		{
			camera->posEye.z = CAM_POS_P_Z;
			camera->posAt.z = CAM_POS_R_Z;
		}
		else
		{
			camera->posAt.z = player->pos.z;
		}
	}
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

//=============================================================================
// �g�p���̃J�����̃i���o�[�擾����
//=============================================================================
int GetCameraNum(void)
{
	return g_cameramax;
}

//=============================================================================
// �J����0�̈ʒu�̐ݒ菈��
//=============================================================================
void SetCamera0PosAt(void)
{
	PLAYER *player1 = GetPlayer(0);
	PLAYER *player2 = GetPlayer(1);

	CAMERA *camera = GetCamera(0);

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
//=============================================================================
// �J�����̈ʒu(Z)�𔽓]
//=============================================================================
void TurnCamera(int playerno)
{
	CAMERA *camera = GetCamera(playerno);

	camera->posEye.z *= -1.0f;
}

//=============================================================================
// 2�l�̃v���C���[���t�������Ă��邩�ǂ���
//=============================================================================
bool CheckTurn(void)
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


//=============================================================================
// �J�������������Ă��邩�擾
//=============================================================================
bool GetGouryu(void)
{
	return g_gouryu;
}

//=============================================================================
// �r���[�|�[�g�֘A
//=============================================================================
void Viewport(int no)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetDevice();

	// �r���[�|�[�g�̈��ݒ�(�ύX)		
	pD3DDevice->SetViewport(&g_port[no]);

	// �`��̈�ɂ����Ƃ����C�ӂ̐F�ŃN���A
	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(g_color[no].r, g_color[no].b, g_color[no].g), 1.0f, 0);
}

//=============================================================================
// �r���[�|�[�g�ݒ�
//=============================================================================
void SetViewport(void)
{
	// ��1����: X		����̒��_(X)
	// ��2����: Y		����̒��_(Y)
	// ��3����: Width	����
	// ��4����: Height	�c��
	// ��5����: MinZ	��{0.0f
	// ��6����: MaxZ	��{1.0f

	switch (GetCameraNum())
	{
	case 1:
		g_port[0] = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };

		break;

	case 2:
		bool player1_turn = GetTurn(0);
		bool player2_turn = GetTurn(1);

		if (GetTurn(0))
		{
			g_port[0] = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };
		}
		else
		{
			g_port[0] = { 0, 0, SCREEN_CENTER_X, SCREEN_HEIGHT, 0.0f, 1.0f };
		}

		if (GetTurn(1))
		{
			g_port[1] = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };
		}
		else
		{
			//g_port[1] = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f };

			g_port[1] = { SCREEN_CENTER_X, 0, SCREEN_CENTER_X, SCREEN_HEIGHT, 0.0f, 1.0f };
		}

		break;
	}
}

D3DVIEWPORT9 *GetPort(int no)
{
	return &g_port[no];
}