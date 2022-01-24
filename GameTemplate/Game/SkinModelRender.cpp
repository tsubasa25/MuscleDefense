#include "stdafx.h"
#include "SkinModelRender.h"
#include "GameScene.h"
namespace nsMuscle {
	SkinModelRender::~SkinModelRender()
	{
	}
	void SkinModelRender::Update()
	{
		if (m_isActive == false) {
			return;
		}
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed, m_onPostCalcLocalMatrix);
		UpdateModel();
	}
	void SkinModelRender::InitSkeleton(const char* skeletonPath, AnimationClip* animClips, int animClipNum)
	{
		m_skeleton.Init(skeletonPath);
		m_initData[eModel].m_skeleton = &m_skeleton;
		m_initData[eModel_Shadow].m_skeleton = &m_skeleton;
		//アニメーション関連の初期化
		m_animationClips = animClips;
		m_animationClipNum = animClipNum;
		m_animation.Init(m_skeleton, m_animationClips, m_animationClipNum);		
	}
	void SkinModelRender::Init(const char* modelPath, const char* maxModelPath, StructuredBuffer* muscleRateArraySB)
	{
		//モデルのファイルパスの指定
		m_initData[eModel].m_tkmFilePath = modelPath;		
		//ムキムキモデルのファイルパスの指定
		if (maxModelPath != nullptr) {
			m_initData[eModel].m_maxTkmFilePath = maxModelPath;
		}
		//シェーダーパスの指定
		m_initData[eModel].m_fxFilePath = "Assets/shader/model.fx";		
		//シェーダーの頂点シェーダーのエントリー関数名の指定
		m_initData[eModel].m_vsEntryPointFunc = "VSMain";		
		//シェーダーのスキンありマテリアル用の頂点シェーダーのエントリー関数名の指定
		m_initData[eModel].m_vsSkinEntryPointFunc = "VSSkinMain";		
		//ピクセルシェーダー。
		m_initData[eModel].m_psEntryPointFunc = "PSMain";		
		
		//カラーバッファのフォーマットを指定
		m_initData[eModel].m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;		
		//モデルデータの上方向の軸を指定
		m_initData[eModel].m_modelUpAxis = enModelUpAxisZ;		
		//モデルに影を落とすために影のテクスチャを紐付ける
		m_initData[eModel].m_expandShaderResoruceView[0]= &PostEffectManager::GetInstance()->GetShadowMap();		
		//定数バッファをモデルに紐付ける
		m_initData[eModel].m_expandConstantBufferSize[0] = LightManager::GetInstance()->GetLigDataSize();
		
		m_initData[eModel].m_expandConstantBuffer[0] = LightManager::GetInstance()->GetLigDatas();
		
		m_initData[eModel].m_expandConstantBufferSize[1] = LightManager::GetInstance()->GetLigCameraDataSize();
		m_initData[eModel].m_expandConstantBuffer[1] = LightManager::GetInstance()->GetLigCameraDatas();

		m_initData[eModel].m_muscleRateArraySB = muscleRateArraySB;
		if (m_isWipeModel) {
			m_initData[eModel_Wipe0] = m_initData[eModel];//ワイプ用のデータにコピー
			m_initData[eModel_Wipe1] = m_initData[eModel];//ワイプ用のデータにコピー
			m_initData[eModel_Wipe2] = m_initData[eModel];//ワイプ用のデータにコピー
			
		}
		m_initData[eModel_Shadow] = m_initData[eModel];//影用のデータにコピー

		m_initData[eModel_Shadow].m_fxFilePath = "Assets/shader/shadow.fx";
		m_initData[eModel_Shadow].m_colorBufferFormat = DXGI_FORMAT_R32_FLOAT;
		m_initData[eModel_Shadow].m_expandConstantBufferSize[0] = LightManager::GetInstance()->GetLigCameraDataSize();
		m_initData[eModel_Shadow].m_expandConstantBuffer[0] = LightManager::GetInstance()->GetLigCameraDatas();
		
		//モデルの初期化
		if (m_isWipeModel[0]) {
			m_model[eModel_Wipe0].Init(m_initData[eModel_Wipe0]);
		}
		if(m_isWipeModel[1]) {
			m_model[eModel_Wipe1].Init(m_initData[eModel_Wipe1]);
		}
		if (m_isWipeModel[2]) {
			m_model[eModel_Wipe2].Init(m_initData[eModel_Wipe2]);
		}		
		if (m_isMainModel) {
			m_model[eModel].Init(m_initData[eModel]);			
		}
		if (m_isShadowCaster) {
			m_model[eModel_Shadow].Init(m_initData[eModel_Shadow]);
		}
	}
	
	void SkinModelRender::Init(const char* modelPath)
	{
		Init(modelPath, nullptr,nullptr);
	}
	void SkinModelRender::Render(RenderContext& rc,Camera*camera)
	{	
		if (m_isActive == false) {
			return;
		}
		//レンダーコンテキストの描画先で分岐
		switch (rc.GetRenderStep()) {
		case RenderContext::eStep_Render:
			//画面1に描画
			if (m_isMainModel) {
				m_model[eModel].Draw(rc, *camera);
			}
			break;
		case RenderContext::eStep_RenderWipe0:
			//ワイプ画面に描画
			if (m_isWipeModel[0]) {
				m_model[eModel_Wipe0].Draw(rc, *camera);
			}
			break;
		case RenderContext::eStep_RenderWipe1:
			//ワイプ画面に描画
			if (m_isWipeModel[1]) {
				m_model[eModel_Wipe1].Draw(rc, *camera);
			}
			break;
		case RenderContext::eStep_RenderWipe2:
			//ワイプ画面に描画
			if (m_isWipeModel[2]) {
				m_model[eModel_Wipe2].Draw(rc, *camera);
			}
			break;		
		case RenderContext::eStep_RenderShadowMap:
			//影を作るモデルの時だけ影を描画
			if (m_isShadowCaster)
			{
				m_model[eModel_Shadow].Draw(rc,*camera);
			}
			break;
		}
	
	}
	void SkinModelRender::UpdateModel()
	{
		//モデルをアップデート
		for (auto& model : m_model) {
			model.UpdateWorldMatrix(m_position, m_qRot, m_scale);
		}
		m_skeleton.Update(m_model[eModel_Shadow].GetWorldMatrix());
	}

	void SkinModelRender::SetPosition(Vector3 pos)
	{
		m_position = pos;
	}
	
	void SkinModelRender::SetRotation(Quaternion qRot)
	{
		m_qRot = qRot;
	}

	void SkinModelRender::SetScale(Vector3 scale)
	{
		m_scale = scale;
	}

	Matrix SkinModelRender::GetWorldMatrixFromBoneName(const wchar_t* boneName)
	{
		int boneNo = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneNo)->GetWorldMatrix();
	}
}