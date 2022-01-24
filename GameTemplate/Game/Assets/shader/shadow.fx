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
//ライトカメラの情報
cbuffer LightCameraCb : register(b1)
{
	float4x4 mLVP;
	float3 lightCameraPos;
	float3 lightCameraDir;
	//筋肉用
	
	float interporateRate;
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
	float4 posInMax : TEXCOORD1;    //筋肉量最大のときの頂点座標。    

	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn {
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//法線
	float2 uv 			: TEXCOORD0;	//uv座標。	
	float3 worldPos		: TEXCOORD1;
	//float3 normalInView : TEXCOORD2;
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。
StructuredBuffer<float> g_muscleRateArray:register(t11);//筋肉量配列

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;
	float w = 0.0f;
	[unroll]
	for (int i = 0; i < 3; i++)
	{
		skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
		w += skinVert.Weights[i];
	}

	skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);

	return skinning;
}
/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if (hasSkin) {
		m = CalcSkinMatrix(vsIn.skinVert);
	}
	else {
		m = mWorld;
	}
	 // 頂点モーフ
    float4 pos = vsIn.pos;
    if (hasSkin) {
        float musleRate = 0;
        float w = 0.0f;
        for (int i = 0; i < 3; i++)
        {
            musleRate += g_muscleRateArray[vsIn.skinVert.Indices[i]] * vsIn.skinVert.Weights[i];
            w += vsIn.skinVert.Weights[i];
        }
        musleRate += g_muscleRateArray[vsIn.skinVert.Indices[3]] * (1.0f - w);

        pos = lerp(
            vsIn.pos,
            vsIn.posInMax,
            musleRate      //interporateRate    // 補間率。定数バッファで送る。interporateRate         
        );
    }
	psIn.pos = mul(m, pos);

	psIn.worldPos = psIn.pos;

	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);
	//psIn.normal = mul(m, vsIn.normal);
	//psIn.normal = normalize(psIn.normal);

	//psIn.normalInView = mul(mView, psIn.normal);

	psIn.uv = vsIn.uv;
	
	return psIn;
}
/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	// シャドウマップ描画用の頂点シェーダーを実装
	return VSMainCore(vsIn, true);
}

SPSIn VSSkinMain(SVSIn vsIn)//とりあえずおなじことする
{
	return VSMainCore(vsIn, true);
}
/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	float depth = length(psIn.worldPos - lightCameraPos) / 1000.0f;//深度値を1000で割って0.0～1.0にする

	return float4(depth, 0.0f, 0.0f, 1.0f);
}
