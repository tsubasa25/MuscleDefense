#include "stdafx.h"
#include "Model.h"
#include "Material.h"

void Model::Init(const ModelInitData& initData)
{
	MY_ASSERT(
		initData.m_fxFilePath, 
		"error : initData.m_fxFilePathが指定されていません。"
	);
	MY_ASSERT(
		initData.m_tkmFilePath,
		"error : initData.m_tkmFilePathが指定されていません。"
	);
	/*MY_ASSERT(
		initData.m_maxTkmFilePath,
		"error : initData.m_tkmFilePathMaxMuscleが指定されていません。"
	);*/
	//内部のシェーダーをロードする処理が求めているのが
	//wchar_t型の文字列なので、ここで変換しておく。
	wchar_t wfxFilePath[256] = {L""};
	if (initData.m_fxFilePath != nullptr) {
		//MessageBoxA(nullptr, "fxファイルパスが指定されていません。", "エラー", MB_OK);
		//std::abort();
		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	}

	
	if (initData.m_skeleton != nullptr) {
		//スケルトンが指定されている。
		m_meshParts.BindSkeleton(*initData.m_skeleton);
	}
	
	m_modelUpAxis = initData.m_modelUpAxis;

	//リソースバンクに登録
	m_tkmFile = nsMuscle::ResourceBankManager::GetInstance()->GetTkmFileFromBank(initData.m_tkmFilePath);

	if (m_tkmFile == nullptr)
	{
		//未登録
		m_tkmFile = new TkmFile;
		if (initData.m_maxTkmFilePath != nullptr) {
			m_tkmFile->Load(initData.m_tkmFilePath, initData.m_maxTkmFilePath);
		}
		else {
			m_tkmFile->Load(initData.m_tkmFilePath);//
		}
		//m_tkmFile->Load(initData.m_tkmFilePath);
		nsMuscle::ResourceBankManager::GetInstance()->RegistTkmFileToBank(initData.m_tkmFilePath, m_tkmFile);
	}
	
	//m_tkmFile = new TkmFile;	
	//m_tkmFile->Load(initData.m_tkmFilePath);

	m_meshParts.InitFromTkmFile(
		*m_tkmFile, 
		wfxFilePath,
		initData.m_vsEntryPointFunc,
		initData.m_vsSkinEntryPointFunc,
		initData.m_psEntryPointFunc,
		initData.m_expandConstantBuffer,
		initData.m_expandConstantBufferSize,
		initData.m_expandShaderResoruceView[0],
		initData.m_colorBufferFormat,
		initData.m_muscleRateArraySB
	);

	UpdateWorldMatrix(g_vec3Zero, g_quatIdentity, g_vec3One);
	
}

void Model::UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
{
	Matrix mBias;
	if (m_modelUpAxis == enModelUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(Math::PI * -0.5f);
	}
	Matrix mTrans, mRot, mScale;
	mTrans.MakeTranslation(pos);
	mRot.MakeRotationFromQuaternion(rot);
	mScale.MakeScaling(scale);
	m_world = mBias * mScale * mRot * mTrans;
}
void Model::UpdateWorldMatrix(Matrix world)
{
	m_world = world;
}

void Model::ChangeAlbedoMap(const char* materialName, Texture& albedoMap)
{
	m_meshParts.QueryMeshs([&](const SMesh& mesh) {
		//todo マテリアル名をtkmファイルに出力したなかった・・・。
		//todo 今は全マテリアル差し替えます
		for (Material* material : mesh.m_materials) {
			material->GetAlbedoMap().InitFromD3DResource(albedoMap.Get());
		}
	});
	//ディスクリプタヒープの再作成。
	m_meshParts.CreateDescriptorHeaps();
	
}
void Model::Draw(RenderContext& rc)
{
	m_meshParts.Draw(
		rc, 
		m_world, 
		g_camera3D[0]->GetViewMatrix(),
		g_camera3D[0]->GetProjectionMatrix()
	);
}
void Model::Draw(RenderContext& rc, Camera& camera)
{
	m_meshParts.Draw(rc, m_world, camera.GetViewMatrix(), camera.GetProjectionMatrix());
}