//=============================================================================
//
// ���C������ [main.cpp]
// Author : 
//
//=============================================================================
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "light.h"
#include "camera.h"
#include "field.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"�J��������"		// �E�C���h�E�̃L���v�V������

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

void DrawDebug(void);

void SetBackGroundPolygon(int num);

HRESULT InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3D�I�u�W�F�N�g
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Device�I�u�W�F�N�g(�`��ɕK�v)
#ifdef _DEBUG
LPD3DXFONT			g_pD3DXFont = NULL;		// �t�H���g�ւ̃|�C���^
int					g_nCountFPS;			// FPS�J�E���^
#endif
bool				g_bDispDebug = true;	// �f�o�b�O�\��ON/OFF

COLOR g_color[] = {
	{ 20, 60, 20 },
	{ 20, 60, 20 },
};

D3DVIEWPORT9 g_port[CAMERA_MAX];
VIEW g_view[CAMERA_MAX];

bool	g_sw = true;

// D3DVIEWPORT9
// ��1����: X		����̒��_(X)
// ��2����: Y		����̒��_(Y)
// ��3����: Width	����
// ��4����: Height	�c��
// ��5����: MinZ	��{0.0f
// ��6����: MaxZ	��{1.0f

//****************************************************************************
//�����_�����O�e�N�X�`���p�C���^�[�t�F�[�X��
//****************************************************************************

LPDIRECT3DSURFACE9		pRenderDepthBuffer;
LPDIRECT3DTEXTURE9		pRenderTexture[PLAYER_MAX];

D3DVIEWPORT9			RenderViewport;

LPDIRECT3DSURFACE9		pDefaultRenderTargetSurface;
LPDIRECT3DSURFACE9		pDefaultDepthBufferSurface;
LPDIRECT3DSURFACE9		pRenderTextureSurface;
D3DVIEWPORT9			DefaultViewport;

//�w�i�|���S��
HRESULT InitBackGroundPolygon(void);
void UninitBackGroundPolygon(void);
void DrawBackGroundPolygon(int no);

LPDIRECT3DTEXTURE9		g_pD3DBackGround = NULL;	// �e�N�X�`���ւ̃|�C���^

LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBackGround[PLAYER_MAX] = { NULL, NULL };	// ���_�o�b�t�@�ւ̃|�C���^

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND hWnd;
	MSG msg;

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		NULL,
		NULL,
		hInstance,
		NULL);

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//�t���[���J�E���g������
	timeBeginPeriod(1);				// ����\��ݒ�
	dwExecLastTime =
		dwFPSLastTime = timeGetTime();
	dwCurrentTime =
		dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
#ifdef _DEBUG
				//PrintDebugProc("FPS:%d\n", g_nCountFPS);
#endif
				dwExecLastTime = dwCurrentTime;

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++;
			}
		}
	}

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	timeEndPeriod(1);				// ����\��߂�

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;						// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;				// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed = bWindow;					// �E�B���h�E���[�h
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	if (bWindow)
	{// �E�B���h�E���[�h
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	else
	{// �t���X�N���[�����[�h
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

																			// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

																			// �e�N�X�`���X�e�[�W���Z��������
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

#ifdef _DEBUG
																			// ���\���p�t�H���g��ݒ�
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DXFont);
#endif

	// ���͂̏���������
	InitInput(hInstance, hWnd);

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

	//�����_�����O�e�N�X�`���֘A
	InitBackGroundPolygon();
	InitRendertexture();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
#ifdef _DEBUG
	if (g_pD3DXFont != NULL)
	{// ���\���p�t�H���g�̊J��
		g_pD3DXFont->Release();
		g_pD3DXFont = NULL;
	}
#endif

	if (g_pD3DDevice != NULL)
	{// �f�o�C�X�̊J��
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if (g_pD3D != NULL)
	{// Direct3D�I�u�W�F�N�g�̊J��
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// ���͂̏I������
	UninitInput();

	// ���C�g�̏I������
	UninitLight();

	// �J�����̏I������
	UninitCamera();

	// �n�ʂ̏I������
	UninitField();

	// ���f���̏I������
	UninitPlayer();

	UninitBackGroundPolygon();
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
	if (GetKeyboardTrigger(DIK_F1))
	{// �f�o�b�O�\��ON/OFF
		g_bDispDebug = g_bDispDebug ? false : true;
	}

	if (GetKeyboardTrigger(DIK_F3))
	{
		g_sw = g_sw ? false : true;
	}

	// ���͂̍X�V����
	UpdateInput();

	// ���C�g�̍X�V����
	UpdateLight();

	// �J�����̍X�V����
	UpdateCamera();

	// �n�ʂ̍X�V����
	UpdateField();

	// ���f���̍X�V����
	UpdatePlayer();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	int camera_num = GetCameraNum();

	if (CheakTurn())
	{
		for (int i = 0; i < camera_num; i++)
		{
			// �r���[�|�[�g�̈��ݒ�(�ύX)		
			g_pD3DDevice->SetViewport(&g_port[i]);

			// �J�����̍��W��ύX(�r���[�s��)
			if (g_sw)
			{
				g_pD3DDevice->SetTransform(D3DTS_VIEW, &g_view[i].matView);
			}

			// �`��̈�ɂ����Ƃ����C�ӂ̐F�ŃN���A
			g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(g_color[i].r, g_color[i].b, g_color[i].g), 1.0f, 0);

			// Direct3D�ɂ��`��̊J�n
			if (SUCCEEDED(g_pD3DDevice->BeginScene()))
			{
				if (camera_num == 1)
				{
					SetCamera(i);
				}
				else
				{
					// �J�����̐ݒ菈��
					SetCamera(i + 1);
				}

				//
				if (GetTurn(i))
				{
					if (camera_num == 1)
					{
						SetBackGroundPolygon(i);
					}
					else
					{
						SetBackGroundPolygon(i + 1);
					}

					//�����_�����O�^�[�Q�b�g�̐؂�ւ�
					ChangeRenderTarget(i);
				}

				// �n�ʂ̕`�揈��
				DrawField();

				// ���f���̕`�揈��
				DrawPlayer();


				if (GetTurn(i))
				{
					//�����_�����O�^�[�Q�b�g�̕��A
					RecoverRenderTarget();
				}
			}

			if (i == camera_num - 1)
			{
				for (int j = 0; j < camera_num; j++)
				{
					if (GetTurn(j))
					{
						//�����_�����O�����w�i��\��
						DrawBackGroundPolygon(j);
					}
				}
			}

#ifdef _DEBUG
			// FPS�\��
			DrawDebug();
#endif

			// Direct3D�ɂ��`��̏I��
			g_pD3DDevice->EndScene();
		}
	}
	else
	{
		if (GetTurn(0))
		{
			for (int i = 0; i < camera_num; i++)
			{
				// �r���[�|�[�g�̈��ݒ�(�ύX)		
				g_pD3DDevice->SetViewport(&g_port[i]);

				// �J�����̍��W��ύX(�r���[�s��)
				if (g_sw)
				{
					g_pD3DDevice->SetTransform(D3DTS_VIEW, &g_view[i].matView);
				}

				// �`��̈�ɂ����Ƃ����C�ӂ̐F�ŃN���A
				g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(g_color[i].r, g_color[i].b, g_color[i].g), 1.0f, 0);

				// Direct3D�ɂ��`��̊J�n
				if (SUCCEEDED(g_pD3DDevice->BeginScene()))
				{
					if (camera_num == 1)
					{
						SetCamera(i);
					}
					else
					{
						// �J�����̐ݒ菈��
						SetCamera(i + 1);
					}

					//
					if (GetTurn(i))
					{
						if (camera_num == 1)
						{
							SetBackGroundPolygon(i);
						}
						else
						{
							SetBackGroundPolygon(i + 1);
						}

						//�����_�����O�^�[�Q�b�g�̐؂�ւ�
						ChangeRenderTarget(i);
					}

					// �n�ʂ̕`�揈��
					DrawField();

					// ���f���̕`�揈��
					DrawPlayer();



					if (GetTurn(i))
					{
						//�����_�����O�^�[�Q�b�g�̕��A
						RecoverRenderTarget();

						//�����_�����O�����w�i��\��
						DrawBackGroundPolygon(i);
					}
				}

#ifdef _DEBUG
				// FPS�\��
				DrawDebug();
#endif

				// Direct3D�ɂ��`��̏I��
				g_pD3DDevice->EndScene();
			}
		}
		else
		{
			for (int i = camera_num - 1; i > -1; i--)
			{
				// �r���[�|�[�g�̈��ݒ�(�ύX)		
				g_pD3DDevice->SetViewport(&g_port[i]);

				// �J�����̍��W��ύX(�r���[�s��)
				if (g_sw)
				{
					g_pD3DDevice->SetTransform(D3DTS_VIEW, &g_view[i].matView);
				}

				// �`��̈�ɂ����Ƃ����C�ӂ̐F�ŃN���A
				g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(g_color[i].r, g_color[i].b, g_color[i].g), 1.0f, 0);

				// Direct3D�ɂ��`��̊J�n
				if (SUCCEEDED(g_pD3DDevice->BeginScene()))
				{
					if (camera_num == 1)
					{
						SetCamera(i);
					}
					else
					{
						// �J�����̐ݒ菈��
						SetCamera(i + 1);
					}

					//
					if (GetTurn(i))
					{
						if (camera_num == 1)
						{
							SetBackGroundPolygon(i);
						}
						else
						{
							SetBackGroundPolygon(i + 1);
						}

						//�����_�����O�^�[�Q�b�g�̐؂�ւ�
						ChangeRenderTarget(i);
					}



					// �n�ʂ̕`�揈��
					DrawField();

					// ���f���̕`�揈��
					DrawPlayer();

					if (GetTurn(i))
					{
						//�����_�����O�^�[�Q�b�g�̕��A
						RecoverRenderTarget();

						//�����_�����O�����w�i��\��
						DrawBackGroundPolygon(i);
					}
				}

#ifdef _DEBUG
				// FPS�\��
				DrawDebug();
#endif

				// Direct3D�ɂ��`��̏I��
				g_pD3DDevice->EndScene();
			}
		}
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// �f�o�b�O�\��
//=============================================================================
void DrawDebug(void)
{
	CAMERA *camera = GetCamera(0);
	CAMERA *camera1P = GetCamera(1);
	CAMERA *camera2P = GetCamera(2);

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];

	wsprintf(str, "FPS:%d\n", g_nCountFPS);

	// �e�L�X�g�`��
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 20;
	wsprintf(str, "�J�����̌���:%d\n", (int)(camera->rot.y * 100));
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 40;
	wsprintf(str, "�r���[�s��:%d\n", g_sw);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 60;
	wsprintf(str, "�J����0�̈ʒu X:%d Y:%d Z:%d\n", (int)camera->posEye.x * 1000, (int)camera->posEye.y * 1000, (int)camera->posEye.z * 1000);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 80;
	wsprintf(str, "�J����1�̈ʒu X:%d Y:%d Z:%d\n", (int)camera1P->posEye.x * 1000, (int)camera1P->posEye.y * 1000, (int)camera1P->posEye.z * 1000);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 100;
	wsprintf(str, "�J����2�̈ʒu X:%d Y:%d Z:%d\n", (int)camera2P->posEye.x * 1000, (int)camera2P->posEye.y * 1000, (int)camera2P->posEye.z * 1000);
	g_pD3DXFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 120;
	wsprintf(str, "�v���C���[1 turn?:%d �v���C���[2 turn?:%d\n", GetTurn(0), GetTurn(1));
	g_pD3DXFont->DrawTextA(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 140;
	wsprintf(str, "1P�p�r���[�|�[�g X:%d Y:%d WIDTH:%d HEIGHT:%d\n", g_port[0].X, g_port[0].Y, g_port[0].Width, g_port[0].Height);
	g_pD3DXFont->DrawTextA(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	rect.top = 160;
	wsprintf(str, "2P�p�r���[�|�[�g X:%d Y:%d WIDTH:%d HEIGHT:%d\n", g_port[1].X, g_port[1].Y, g_port[1].Width, g_port[1].Height);
	g_pD3DXFont->DrawTextA(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
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
			g_port[1] = { SCREEN_CENTER_X, 0, SCREEN_CENTER_X, SCREEN_HEIGHT, 0.0f, 1.0f };
		}

		break;
	}
}

//=============================================================================
// �����_�����O�e�N�X�`���쐬����
//=============================================================================
void InitRendertexture()
{
	//�����_�����O�e�N�X�`���p�r���[�|�[�g
	RenderViewport.X = 0;
	RenderViewport.Y = 0;
	RenderViewport.Width = 800;
	RenderViewport.Height = 600;
	RenderViewport.MinZ = 0.0f;
	RenderViewport.MaxZ = 1.0f;

	//�f�v�X�o�b�t�@�쐬(2�ׂ̂���T�C�Y)
	g_pD3DDevice->CreateDepthStencilSurface(1024, 1024,		//�T�C�Y
		D3DFMT_D24S8,	//�s�N�Z���t�H�[�}�b�g
		D3DMULTISAMPLE_NONE,//marutisanpuringu OFF
		0,
		FALSE,
		&pRenderDepthBuffer,
		NULL);

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		//�����_�[�^�[�Q�b�g�쐬(2�ׂ̂���T�C�Y)
		D3DXCreateTexture(g_pD3DDevice,
			800, 1024,		//�T�C�Y
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
	//�����_�[�^�[�Q�b�g��؂�ւ�

	//�f�t�H���g�r���[�|�[�g�ۑ�
	g_pD3DDevice->GetViewport(&DefaultViewport);

	//�f�t�H���g�̃T�[�t�F�[�X��ۑ�
	g_pD3DDevice->GetRenderTarget(0, &pDefaultRenderTargetSurface);
	g_pD3DDevice->GetDepthStencilSurface(&pDefaultDepthBufferSurface);
	//�����_�����O�e�N�X�`���̃T�[�t�F�[�X�擾
	pRenderTexture[no]->GetSurfaceLevel(0, &pRenderTextureSurface);

	//�V���������_�����O�^�[�Q�b�g���Z�b�g
	g_pD3DDevice->SetRenderTarget(0, pRenderTextureSurface);
	//�V�����f�v�X�o�b�t�@���Z�b�g
	g_pD3DDevice->SetDepthStencilSurface(pRenderDepthBuffer);

	//�r���[�|�[�g�̐؂�ւ�
	g_pD3DDevice->SetViewport(&RenderViewport);

	//�N���A
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
}

//=============================================================================
// �����_�[�^�[�Q�b�g���A����
//=============================================================================
void RecoverRenderTarget()
{
	//�����_�[�^�[�Q�b�g�̕��A
	g_pD3DDevice->SetRenderTarget(0, pDefaultRenderTargetSurface);
	g_pD3DDevice->SetDepthStencilSurface(pDefaultDepthBufferSurface);

	//�T�[�t�F�[�X�̉��
	pDefaultRenderTargetSurface->Release();
	pDefaultDepthBufferSurface->Release();
	pRenderTextureSurface->Release();

	//�r���[�|�[�g�̕��A
	g_pD3DDevice->SetViewport(&DefaultViewport);
}

//=============================================================================
// �����_�����O�e�N�X�`���w�i������
//=============================================================================
HRESULT	InitBackGroundPolygon()
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		g_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
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
			pVtx[1].vtx = D3DXVECTOR3(1024.0f, 0.0f, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(0.0f, 1024.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(1024.0f, 1024.0f, 0.0f);

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
void	DrawBackGroundPolygon(int no)
{
	// ���_�t�H�[�}�b�g�̐ݒ�
	g_pD3DDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	g_pD3DDevice->SetTexture(0, pRenderTexture[no]);

	//
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	g_pD3DDevice->SetStreamSource(0, g_pD3DVtxBuffBackGround[no], 0, sizeof(VERTEX_2D));

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �|���S���̕`��
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void SetBackGroundPolygon(int num)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	int no;
	if (num == 0)
	{
		no = 0;
	}
	else
	{
		no = num - 1;
	}

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffBackGround[no]->Lock(0, 0, (void**)&pVtx, 0);

	switch (num)
	{
	case 0:
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(800.0f, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, 1024.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(800.0f, 1024.0f, 0.0f);
		break;
	case 1:
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(400.0f, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, 1024.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(400.0f, 1024.0f, 0.0f);
		break;
	case 2:
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(400.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(800.0f, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(400.0f, 1024.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(800.0f, 1024.0f, 0.0f);
		break;
	}

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffBackGround[no]->Unlock();
}

//=============================================================================
// �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

