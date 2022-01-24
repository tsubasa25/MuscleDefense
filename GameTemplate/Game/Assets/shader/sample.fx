///////////////////////////////////////////
// 定数バッファー
///////////////////////////////////////////
// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};
///////////////////////////////////////////
// 構造体
///////////////////////////////////////////
//ライト用の構造体たち
struct DirectionLigData
{
    float3 ligDir;
    float3 ligColor;
};

struct PointLigData
{
    float3 ligPos;
    float3 ligColor;
    float ligRange;
};
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn {
    int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos      : POSITION;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD0;
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos      : SV_POSITION;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD0;
    float3 worldPos : TEXCOORD1;
    float3 normalInView : TEXCOORD2;//カメラ空間の法線
};



// ライトデータにアクセスするための定数バッファーを用意する
cbuffer DirectionLightCb : register(b1)
{
    DirectionLigData directionLigData;//ディレクションライトの情報
    PointLigData pointLigData[20];//ポイントライトの情報
    
    int pointLigNum;//ポイントライトの数
    float3 eyePos;          // 視点の位置
    float3 ambientLight;    // アンビエントライト

    //半球ライト用
    float3 groundColor;		//照り返しのライト。
    float3 skyColor;		//天球ライト。
    float3 groundNormal;	//地面の法線
};

///////////////////////////////////////////
// 関数宣言
///////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);

///////////////////////////////////////////
// シェーダーリソース
///////////////////////////////////////////
// モデルテクスチャ
Texture2D<float4> g_texture : register(t0);

///////////////////////////////////////////
// サンプラーステート
///////////////////////////////////////////
sampler g_sampler : register(s0);

/// <summary>
/// モデル用の頂点シェーダーのエントリーポイント
/// </summary>
SPSIn VSMain(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;

    psIn.pos = mul(mWorld, vsIn.pos);   // モデルの頂点をワールド座標系に変換
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);    // ワールド座標系からカメラ座標系に変換
    psIn.pos = mul(mProj, psIn.pos);    // カメラ座標系からスクリーン座標系に変換

    // 頂点法線をピクセルシェーダーに渡す
    psIn.normal = mul(mWorld, vsIn.normal); // 法線を回転させる
    psIn.uv = vsIn.uv;

    //カメラ空間の法線を求める
    psIn.normalInView = mul(mView, psIn.normal);
    return psIn;
}
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMain(vsIn, true);
}
/// <summary>
/// モデル用のピクセルシェーダーのエントリーポイント
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{

    //float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);

    float4 finalColor = 0.0f;
    finalColor.a = 1.0f;

    // ディレクションライトによるLambert拡散反射光を計算する
    float3 diffDirection = CalcLambertDiffuse(directionLigData.ligDir, directionLigData.ligColor, psIn.normal);
    // ディレクションライトによるPhong鏡面反射光を計算する
    float3 specDirection = CalcPhongSpecular(directionLigData.ligDir, directionLigData.ligColor, psIn.worldPos, psIn.normal);
    float3 dirfinalLig = diffDirection + specDirection;
    finalColor.xyz += dirfinalLig;
   
    // ポイントライトによるLambert拡散反射光とPhong鏡面反射光を計算する   
    for (int i = 0; i < pointLigNum; i++)
    {
        // サーフェイスに入射するポイントライトの光の向きを計算する
        float3 ligDir = psIn.worldPos - pointLigData[i].ligPos;
        //正規化して大きさ１のベクトルにする。
        ligDir = normalize(ligDir);
        //ポイントライトによる減衰なしのLambert拡散反射光を計算する
        float3 diffPoint = CalcLambertDiffuse(
            ligDir, 		//ライトの方向
            pointLigData[i].ligColor,	 	//ライトのカラー
            psIn.normal		//サーフェイスの法線
        );
        //ポイントライトによる減衰なしのPhong鏡面反射光を計算する
        float3 specPoint = CalcPhongSpecular(
            ligDir, 			//ライトの方向。
            pointLigData[i].ligColor,		 	//ライトのカラー。
            psIn.worldPos, 		//サーフェイズのワールド座標。
            psIn.normal			//サーフェイズの法線。
        );
        //距離による影響率を計算する
        //ポイントライトとの距離を計算する。
        float distance = length(psIn.worldPos - pointLigData[i].ligPos);

        //影響率は距離に比例して小さくなっていく。
        float affect = 1.0f - 1.0f / pointLigData[i].ligRange * distance;
        //影響力がマイナスにならないように補正をかける。
        if (affect < 0.0f) {
            affect = 0.0f;
        }
        //影響の仕方を指数関数的にする。今回のサンプルでは3乗している。
        affect = pow(affect, 3.0f);
        //拡散反射光と鏡面反射光に影響率を乗算して影響を弱める
        diffPoint *= affect;
        specPoint *= affect;
        //2つの反射光を合算して最終的な反射光を求める        
        float3 ptfinalLig = diffPoint + specPoint;

        finalColor.xyz += ptfinalLig;
    }
    //半球ライトを計算する
    //サーフェイスの法線と地面の法線との内積を計算する
    float t = dot(psIn.normal, groundNormal);
    //内積の結果を０～１の範囲に変換する。
    t = (t + 1.0f) / 2.0f;
    //地面色と天球色を補完率ｔで線形補完する。
    float3 hemiLight = lerp(groundColor, skyColor, t);
    finalColor.xyz += hemiLight;

    //リムライトの強さを求める
    // サーフェイスの法線と光の入射方向に依存するリムの強さを求める
    float power1 = 1.0f - max(0.0f, dot(directionLigData.ligDir, psIn.normal));
    // step-4 サーフェイスの法線と視線の方向に依存するリムの強さを求める
    float power2 = 1.0f - max(0.0f, psIn.normalInView.z * -1.0f);
    // step-5 最終的なリムの強さを求める
    float limPower = power1 * power2;
    //pow()を使用して、強さの変化を指数関数的にする。
    limPower = pow(limPower, 1.3f);
    // 最終的な反射光にリムライトの反射光を合算する
    //まずはリムライトのカラーを計算する
    float3 limColor = limPower * directionLigData.ligColor;
    finalColor.xyz += limColor;

    //最終的な反射光にリムの反射光を合算する    
    // テクスチャカラーに求めた光を乗算して最終出力カラーを求める   
    float4 albedoColor = g_texture.Sample(g_sampler, psIn.uv);      
    finalColor *= albedoColor;
    return finalColor;
}

/// <summary>
/// Lambert拡散反射光を計算する
/// </summary>
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    // ピクセルの法線とライトの方向の内積を計算する
    float t = dot(normal, lightDirection) * -1.0f;

    // 内積の値を0以上の値にする
    t = max(0.0f, t);

    // 拡散反射光を計算する
    return lightColor * t;
}

/// <summary>
/// Phong鏡面反射光を計算する
/// </summary>
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
    // 反射ベクトルを求める
    float3 refVec = reflect(lightDirection, normal);

    // 光が当たったサーフェイスから視点に伸びるベクトルを求める
    float3 toEye = eyePos - worldPos;
    toEye = normalize(toEye);

    // 鏡面反射の強さを求める
    float t = dot(refVec, toEye);

    // 鏡面反射の強さを0以上の数値にする
    t = max(0.0f, t);

    // 鏡面反射の強さを絞る
    t = pow(t, 10.0f);

    // 鏡面反射光を求める
    return lightColor * t;
}
