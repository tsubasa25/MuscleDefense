#include "stdafx.h"
#include "Material.h"

//ルートシグネチャとパイプラインステート周りはカリカリカリ
enum {
	enDescriptorHeap_CB,
	enDescriptorHeap_SRV,
	enNumDescriptorHeap
};
	
void Material::InitTexture(const TkmFile::SMaterial& tkmMat)
{
	const auto& nullTextureMaps = g_graphicsEngine->GetNullTextureMaps();

	const char* filePath = nullptr;
	char* map = nullptr;
	unsigned int mapSize;

	/*if (tkmMat.albedoMap != nullptr) {
		m_albedoMap->InitFromMemory(tkmMat.albedoMap->data.get(), tkmMat.albedoMap->dataSize);
	}
	else {
		m_albedoMap->InitFromMemory(
		nullTextureMaps.GetAlbedoMap().get(), 
		nullTextureMaps.GetAlbedoMapSize());
	}*/
	//auto albedoMap = nsMuscle::ResourceBankManager::GetInstance()->GetTextureFromBank(tkmMat);

	//アルベドマップ。
	{
		if (tkmMat.albedoMap != nullptr)
		{
			filePath = tkmMat.albedoMap->filePath.c_str();
			map = tkmMat.albedoMap->data.get();
			mapSize = tkmMat.albedoMap->dataSize;
		}
		else
		{
			filePath = nullTextureMaps.GetAlbedoMapFilePath();
			map = nullTextureMaps.GetAlbedoMap().get();
			mapSize = nullTextureMaps.GetAlbedoMapSize();
		}
		auto albedoMap = nsMuscle::ResourceBankManager::GetInstance()->GetTextureFromBank(filePath);

		if (albedoMap == nullptr)
		{
			albedoMap = new Texture();
			albedoMap->InitFromMemory(map, mapSize);
			nsMuscle::ResourceBankManager::GetInstance()->RegistTextureToBank(filePath, albedoMap);
		}
		m_albedoMap = albedoMap;
	}

	//法線マップ。
	{
		if (tkmMat.normalMap != nullptr)
		{
			filePath = tkmMat.normalMap->filePath.c_str();
			map = tkmMat.normalMap->data.get();
			mapSize = tkmMat.normalMap->dataSize;
		}
		else
		{
			filePath = nullTextureMaps.GetNormalMapFilePath();
			map = nullTextureMaps.GetNormalMap().get();
			mapSize = nullTextureMaps.GetNormalMapSize();
		}
		auto normalMap = nsMuscle::ResourceBankManager::GetInstance()->GetTextureFromBank(filePath);

		if (normalMap == nullptr)
		{
			normalMap = new Texture();
			normalMap->InitFromMemory(map, mapSize);
			nsMuscle::ResourceBankManager::GetInstance()->RegistTextureToBank(filePath, normalMap);
		}
		m_normalMap = normalMap;
	}



	//スペキュラマップ。
	{
		if (tkmMat.specularMap != nullptr)
		{
			filePath = tkmMat.specularMap->filePath.c_str();
			map = tkmMat.specularMap->data.get();
			mapSize = tkmMat.specularMap->dataSize;
		}
		else
		{
			filePath = nullTextureMaps.GetSpecularMapFilePath();
			map = nullTextureMaps.GetSpecularMap().get();
			mapSize = nullTextureMaps.GetSpecularMapSize();
		}
		auto specularMap = nsMuscle::ResourceBankManager::GetInstance()->GetTextureFromBank(filePath);

		if (specularMap == nullptr)
		{
			specularMap = new Texture();
			specularMap->InitFromMemory(map, mapSize);
			nsMuscle::ResourceBankManager::GetInstance()->RegistTextureToBank(filePath, specularMap);
		}
		m_specularMap = specularMap;
	}

	//反射マップ。
	{
		if (tkmMat.reflectionMap != nullptr)
		{
			filePath = tkmMat.reflectionMap->filePath.c_str();
			map = tkmMat.reflectionMap->data.get();
			mapSize = tkmMat.reflectionMap->dataSize;
		}
		else
		{
			filePath = nullTextureMaps.GetReflectionMapFilePath();
			map = nullTextureMaps.GetReflectionMap().get();
			mapSize = nullTextureMaps.GetReflectionMapSize();
		}
		auto reflectionMap = nsMuscle::ResourceBankManager::GetInstance()->GetTextureFromBank(filePath);

		if (reflectionMap == nullptr)
		{
			reflectionMap = new Texture();
			reflectionMap->InitFromMemory(map, mapSize);
			nsMuscle::ResourceBankManager::GetInstance()->RegistTextureToBank(filePath, reflectionMap);
		}
		m_reflectionMap = reflectionMap;
	}

	//屈折マップ。
	{
		if (tkmMat.refractionMap != nullptr)
		{
			filePath = tkmMat.refractionMap->filePath.c_str();
			map = tkmMat.refractionMap->data.get();
			mapSize = tkmMat.refractionMap->dataSize;
		}
		else
		{
			filePath = nullTextureMaps.GetRefractionMapFilePath();
			map = nullTextureMaps.GetRefractionMap().get();
			mapSize = nullTextureMaps.GetRefractionMapSize();
		}
		auto refractionMap = nsMuscle::ResourceBankManager::GetInstance()->GetTextureFromBank(filePath);

		if (refractionMap == nullptr)
		{
			refractionMap = new Texture();
			refractionMap->InitFromMemory(map, mapSize);
			nsMuscle::ResourceBankManager::GetInstance()->RegistTextureToBank(filePath, refractionMap);
		}
		m_refractionMap = refractionMap;
	}

}
void Material::InitFromTkmMaterila(
	const TkmFile::SMaterial& tkmMat,
	const wchar_t* fxFilePath,
	const char* vsEntryPointFunc,
	const char* vsSkinEntryPointFunc,
	const char* psEntryPointFunc)
{
	//テクスチャをロード。
	InitTexture(tkmMat);
	
	//定数バッファを作成。
	SMaterialParam matParam;
	matParam.hasNormalMap = m_normalMap->IsValid() ? 1 : 0;
	matParam.hasSpecMap = m_specularMap->IsValid() ? 1 : 0;
	m_constantBuffer.Init(sizeof(SMaterialParam), &matParam);

	//ルートシグネチャを初期化。
	m_rootSignature.Init(
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP);

	if (wcslen(fxFilePath) > 0) {
		//シェーダーを初期化。
		InitShaders(fxFilePath, vsEntryPointFunc, vsSkinEntryPointFunc, psEntryPointFunc);
		//パイプラインステートを初期化。
		InitPipelineState();
	}
}
void Material::InitPipelineState()
{
	// 頂点レイアウトを定義する。
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 88, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

	};

	//パイプラインステートを作成。
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel->GetCompiledBlob());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psModel->GetCompiledBlob());
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = TRUE;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 3;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//アルベドカラー出力用。
#ifdef SAMPE_16_02
	psoDesc.RTVFormats[1] = DXGI_FORMAT_R16G16B16A16_FLOAT;	//法線出力用。	
	psoDesc.RTVFormats[2] = DXGI_FORMAT_R32_FLOAT;						//Z値。
#else
	psoDesc.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;			//法線出力用。	
	psoDesc.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;	//Z値。
#endif
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	m_skinModelPipelineState.Init(psoDesc);

	//続いてスキンなしモデル用を作成。
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel->GetCompiledBlob());
	m_nonSkinModelPipelineState.Init(psoDesc);

	//続いて半透明マテリアル用。
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel->GetCompiledBlob());
	psoDesc.BlendState.IndependentBlendEnable = TRUE;
	psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
	psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;

	
	m_transSkinModelPipelineState.Init(psoDesc);

	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel->GetCompiledBlob());
	m_transNonSkinModelPipelineState.Init(psoDesc);

}
void Material::InitShaders(
	const wchar_t* fxFilePath,
	const char* vsEntryPointFunc,
	const char* vsSkinEntriyPointFunc,
	const char* psEntryPointFunc
)
{
	char cfxFilePath[256];
	wcstombs(cfxFilePath, fxFilePath, sizeof(cfxFilePath));
	////スキンなしモデル用のシェーダーをロードする。
	//m_vsNonSkinModel.LoadVS(fxFilePath, vsEntryPointFunc);
	////スキンありモデル用のシェーダーをロードする。
	//m_vsSkinModel.LoadVS(fxFilePath, vsSkinEntriyPointFunc);
	//
	//m_psModel.LoadPS(fxFilePath, psEntryPointFunc);

	//スキンなしモデル用のシェーダーをロードする。
	m_vsNonSkinModel = nsMuscle::ResourceBankManager::GetInstance()->GetShaderFromBank(cfxFilePath, vsEntryPointFunc);
	if (m_vsNonSkinModel == nullptr) {
		m_vsNonSkinModel = new Shader;
		m_vsNonSkinModel->LoadVS(fxFilePath, vsEntryPointFunc);
		nsMuscle::ResourceBankManager::GetInstance()->RegistShaderToBank(cfxFilePath, vsEntryPointFunc, m_vsNonSkinModel);
	}
	//スキンありモデル用のシェーダーをロードする。
	m_vsSkinModel = nsMuscle::ResourceBankManager::GetInstance()->GetShaderFromBank(cfxFilePath, vsSkinEntriyPointFunc);
	if (m_vsSkinModel == nullptr) {
		m_vsSkinModel = new Shader;
		m_vsSkinModel->LoadVS(fxFilePath, vsSkinEntriyPointFunc);
		nsMuscle::ResourceBankManager::GetInstance()->RegistShaderToBank(cfxFilePath, vsSkinEntriyPointFunc, m_vsSkinModel);
	}

	m_psModel = nsMuscle::ResourceBankManager::GetInstance()->GetShaderFromBank(cfxFilePath, psEntryPointFunc);
	if (m_psModel == nullptr) {
		m_psModel = new Shader;
		m_psModel->LoadPS(fxFilePath, psEntryPointFunc);
		nsMuscle::ResourceBankManager::GetInstance()->RegistShaderToBank(cfxFilePath, psEntryPointFunc, m_psModel);
	}
}
void Material::BeginRender(RenderContext& rc, int hasSkin)
{
	rc.SetRootSignature(m_rootSignature);
	
	if (hasSkin) {
	//	rc.SetPipelineState(m_skinModelPipelineState);
		rc.SetPipelineState(m_transSkinModelPipelineState);
	}
	else {
	//	rc.SetPipelineState(m_nonSkinModelPipelineState);
		rc.SetPipelineState(m_transNonSkinModelPipelineState);
	}
}
