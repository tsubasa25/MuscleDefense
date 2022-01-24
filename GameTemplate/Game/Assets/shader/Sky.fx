/*!
 * @brief	シンプルなモデルシェーダー。
 */


 ////////////////////////////////////////////////
 // 定数バッファ。
 ////////////////////////////////////////////////
 //モデル用の定数バッファ
cbuffer ModelCb : register(b0) {
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

cbuffer SkyScrollValue : register(b1)
{
	float textureSize;
	float scrollingValue;
};

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn {
	int4  Indices  	: BLENDINDICES0;
	float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn {
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal	: NORMAL;		//法線
	float2 uv 		: TEXCOORD0;	//UV座標。	
};
//ピクセルシェーダーへの入力。
struct SPSIn {
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//法線
	float2 uv 			: TEXCOORD0;	//uv座標。
	float3 worldPos		: TEXCOORD1;
	float3 normalInView : TEXCOORD2;
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。
TextureCube<float4> g_skyCubeMap : register(t10);
////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////


/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	
	psIn.pos = mul(mWorld, vsIn.pos);
	
	psIn.worldPos = psIn.pos;

	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);
	psIn.normal = mul(mWorld, vsIn.normal);
	psIn.normal = normalize(psIn.normal);

	psIn.normalInView = mul(mView, psIn.normal);

	psIn.uv = vsIn.uv;

	return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, true);
}


//TextureCube<float4> skyCubeMap : register(t0);	//スカイキューブマップ。

/*!
 *@brief	空用のシェーダー。
 */
float4 PSMain_SkyCube(SPSIn In) : SV_Target0
{
 	float4 color = g_skyCubeMap.Sample(g_sampler, In.normal);
 	color.xyz += 0.2f;
	
 	return color;
 }
