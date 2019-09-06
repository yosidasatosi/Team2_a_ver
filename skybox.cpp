//=============================================================================
//
// 背景処理 [skybox.cpp]
// Author : 
//
//=============================================================================
#include "skybox.h"
#include "Camera.h"

//*****************************************************************************
// マクロ定義
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
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexSkybox(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSkybox;
LPDIRECT3DTEXTURE9  g_pTexSkybox[6];



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSkybox(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点、情報の作成
	MakeVertexSkybox();
	// テクスチャの読み込み
	LoadSkyboxTexture();

	return S_OK;
}

//=============================================================================
// 終了処理
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
// 更新処理
//=============================================================================
void UpdateSkybox(void)
{


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSkybox(int no)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX  mtxTranslate;
	CAMERA *camera = GetCamera(no);

	// ワールドマトリクスの初期化
	D3DXMatrixIdentity(&mtxTranslate);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, camera->posEye.x, camera->posEye.y, camera->posEye.z);
	// ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxTranslate);
	
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffSkybox, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int i = 0; i < 6; i++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexSkybox[i]);
		// ポリゴンの描画
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
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			SkyboxTextureName[i],			// ファイルの名前
			&g_pTexSkybox[i]);				// 読み込むメモリー
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexSkybox(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * 6,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pVtxBuffSkybox,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	VERTEX_3D *pVtxBuff, *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffSkybox->Lock(0, 0, (void**)&pVtxBuff, 0);

	for (int i = 0; i < 6; i++)
	{

		pVtx = pVtxBuff + 4 * i;

		// 法線ベクトルの設定
		pVtx[0].nor =
		pVtx[1].nor =
		pVtx[2].nor =
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 反射光の設定
		pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = D3DXCOLOR(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	}

	// 前
	pVtx = pVtxBuff;
	pVtx[0].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[1].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[2].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[3].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_X);


	// 後
	pVtx = pVtxBuff + 4;
	pVtx[0].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[1].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[2].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[3].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);

	// 左
	pVtx = pVtxBuff + 4 * 2;
	pVtx[0].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[1].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[2].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[3].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_X);

	// 右
	pVtx = pVtxBuff + 4 * 3;
	pVtx[0].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[1].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);
	pVtx[2].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_X);
	pVtx[3].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_X);


	// 上
	pVtx = pVtxBuff + 4 * 4;
	pVtx[0].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_Y);
	pVtx[1].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, -SKYBOX_SIZE_Y);
	pVtx[2].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_Y);
	pVtx[3].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, SKYBOX_SIZE_Y, SKYBOX_SIZE_Y);


	// 下
	pVtx = pVtxBuff + 4 * 5;
	pVtx[0].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_Y);
	pVtx[1].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, SKYBOX_SIZE_Y);
	pVtx[2].vtx = D3DXVECTOR3(-SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_Y);
	pVtx[3].vtx = D3DXVECTOR3(SKYBOX_SIZE_X, -SKYBOX_SIZE_Y, -SKYBOX_SIZE_Y);

	// 頂点データをアンロックする
	g_pVtxBuffSkybox->Unlock();

	return S_OK;
}



