#pragma once
namespace nsMuscle {
	class SkinModelRender : public IGameObject
	{
	public: 
		//モデルの状態
		enum EModel {
			eModel,			//画面に描画されるモデル		
			eModel_Wipe0,	//ワイプ画面に描画されるモデル
			eModel_Wipe1,
			eModel_Wipe2,			
			eModel_Shadow,	//影を描画する用モデル
			eModel_Num,		//モデルの状態の数
		};
		SkinModelRender() :m_position(Vector3::Zero), m_qRot(g_quatIdentity), m_scale(Vector3::One) {}
		~SkinModelRender();
		void Update();
		/**
		 * @brief モデルの情報を更新する
		*/
		void UpdateModel();
		void Render(RenderContext& rc,Camera*camera)override;
		void InitSkeleton(const char* skeletonPath, AnimationClip* animClips, int animClipNum);
		void Init(const char* modelPath, const char* maxModelPath,StructuredBuffer* muscleRateArraySB);
		
		/**
		 * @brief モデルの初期化関数　モデルパスだけ版
		 * @param modelPath モデルファイルのパス(.tkm)
		*/
		void Init(const char* modelPath);
		/**
		 * @brief モデルの座標を設定する
		 * @param pos 座標
		*/
		void SetPosition(Vector3 pos);
		/**
		 * @brief モデルの回転を設定する
		 * @param qRot 回転
		*/
		void SetRotation(Quaternion qRot);
		/**
		 * @brief モデルの拡大率を設定する
		 * @param scale 拡大率
		*/
		void SetScale(Vector3 scale);

		/**
		 * @brief モデルのワールド行列を直接セットする
		 * @param world ワールド行列
		*/
		void SetMatrix(Matrix world)
		{
			for (auto& model : m_model) {
				model.UpdateWorldMatrix(world);
				m_skeleton.Update(world);
			}
		}
		/**
		 * @brief モデルの座標を取得
		 * @return 座標
		*/
		Vector3 GetPosition()
		{
			return m_position;
		}
		/**
		 * @brief モデルの回転を取得
		 * @return 回転
		*/
		Quaternion GetRotation()
		{
			return m_qRot;
		}

		/**
		 * @brief モデルの拡大率を取得
		 * @return 拡大率
		*/
		Vector3 GetScale()
		{
			return m_scale;
		}
		/**
		* @brief モデルを取得
		* @param model ビューポート番号(カメラ番号)
		* @return 番号から取得したモデルへの参照
		*/
		Model& GetModel(EModel model = eModel)
		{
			return m_model[model];
		}
		/**
		 * @brief ボーンの名前からそのボーンのワールド行列を取得する
		 * @param boneName ボーンの名前
		 * @return ボーンのワールド行列
		*/
		//Matrix GetWorldMatrixFromBoneName(const wchar_t* boneName);

		/**
		 * @brief アニメーションの再生
		 * @param animationNo アニメーション番号
		 * @param interpolateTime アニメーションの補完時間
		*/
		void PlayAnimation(
			int animationNo, 
			float interpolateTime = 0.0f,
			std::function<void(int boneNo, Bone* bone)> onPostCalcLocalMatrix = nullptr)
		{
			m_animation.Play(animationNo, interpolateTime);
			m_onPostCalcLocalMatrix = onPostCalcLocalMatrix;
		}
		void PlayAnimation(
			int animationNo,
			bool isRePlay,
			float interpolateTime = 0.0f,
			std::function<void(int boneNo, Bone* bone)> onPostCalcLocalMatrix = nullptr)
		{
			m_animation.Play(animationNo,isRePlay, interpolateTime);
			m_onPostCalcLocalMatrix = onPostCalcLocalMatrix;
		}
		/**
		 * @brief アニメーションの再生速度を設定する
		 * @param animationSpeed 再生速度
		*/
		void SetAnimationSpeed(float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

		/**
		 * @brief アニメーションを再生しているか?
		 * @return 再生している:true 再生していない:false
		*/
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}
		/**
		 * @brief モデルが影を作るかどうかを設定する
		 * @param isShadowCaster 作る:true 作らない:false
		*/
		void SetShadowCasterFlag(bool isShadowCaster)
		{
			m_isShadowCaster = isShadowCaster;
		}
		/// <summary>
		/// ワイプ画面表示ようかどうか
		/// </summary>
		/// <param name="flag"></param>
		void SetIsWipeModel(bool flag,int num)
		{
			m_isWipeModel[num] = flag;
		}
		/// <summary>
		/// メイン画面表示かどうか
		/// </summary>
		/// <param name="flag"></param>
		void SetIsMainModel(bool flag)
		{
			m_isMainModel = flag;
		}
		/**
		 * @brief ボーンの名前からそのボーンのワールド行列を取得する
		 * @param boneName ボーンの名前
		 * @return ボーンのワールド行列
		*/
		Matrix GetWorldMatrixFromBoneName(const wchar_t* boneName);

		Skeleton& GetSkeleton() { return m_skeleton; }
		/// <summary>
		/// スキンモデルレンダーをアクティブ化する。
		/// </summary>
		void Activate()
		{
			m_isActive = true;
		}
		/// <summary>
		/// スキンモデルレンダーを非アクティブ化する。
		/// </summary>
		/// <remark>
		/// 非アクティブ化すると、モデルが描画されなくなります。
		/// 一時的に表示をオフにしたい場合などに使用してください。
		/// </remark>
		void Deactivate()
		{
			m_isActive = false;

		}
private:
	//モデルの初期化データ
	ModelInitData m_initData[eModel_Num];
	std::function<void(int boneNo, Bone* bone)> m_onPostCalcLocalMatrix;
	Skeleton m_skeleton; //スケルトン
	Model m_model[eModel_Num]; //モデル
	Vector3 m_position; //座標
	Quaternion m_qRot; //回転
	Vector3 m_scale; //拡大率
	AnimationClip* m_animationClips = nullptr;	//アニメーションクリップ
	int m_animationClipNum = 0;					//アニメーションクリップの数
	Animation m_animation;						//アニメーション。
	float m_animationSpeed = 1.0f;				//アニメーション速度
	bool m_isShadowCaster = true;				//このモデルは影を作るか?
	bool m_isWipeModel[3] = { false };			//ワイプ用のモデルか
	bool m_isMainModel = true;					//メイン画面用のモデルか
	bool m_isActive = true;						//アクティブフラグ
};
}