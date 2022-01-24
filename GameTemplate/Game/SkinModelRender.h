#pragma once
namespace nsMuscle {
	class SkinModelRender : public IGameObject
	{
	public: 
		//���f���̏��
		enum EModel {
			eModel,			//��ʂɕ`�悳��郂�f��		
			eModel_Wipe0,	//���C�v��ʂɕ`�悳��郂�f��
			eModel_Wipe1,
			eModel_Wipe2,			
			eModel_Shadow,	//�e��`�悷��p���f��
			eModel_Num,		//���f���̏�Ԃ̐�
		};
		SkinModelRender() :m_position(Vector3::Zero), m_qRot(g_quatIdentity), m_scale(Vector3::One) {}
		~SkinModelRender();
		void Update();
		/**
		 * @brief ���f���̏����X�V����
		*/
		void UpdateModel();
		void Render(RenderContext& rc,Camera*camera)override;
		void InitSkeleton(const char* skeletonPath, AnimationClip* animClips, int animClipNum);
		void Init(const char* modelPath, const char* maxModelPath,StructuredBuffer* muscleRateArraySB);
		
		/**
		 * @brief ���f���̏������֐��@���f���p�X������
		 * @param modelPath ���f���t�@�C���̃p�X(.tkm)
		*/
		void Init(const char* modelPath);
		/**
		 * @brief ���f���̍��W��ݒ肷��
		 * @param pos ���W
		*/
		void SetPosition(Vector3 pos);
		/**
		 * @brief ���f���̉�]��ݒ肷��
		 * @param qRot ��]
		*/
		void SetRotation(Quaternion qRot);
		/**
		 * @brief ���f���̊g�嗦��ݒ肷��
		 * @param scale �g�嗦
		*/
		void SetScale(Vector3 scale);

		/**
		 * @brief ���f���̃��[���h�s��𒼐ڃZ�b�g����
		 * @param world ���[���h�s��
		*/
		void SetMatrix(Matrix world)
		{
			for (auto& model : m_model) {
				model.UpdateWorldMatrix(world);
				m_skeleton.Update(world);
			}
		}
		/**
		 * @brief ���f���̍��W���擾
		 * @return ���W
		*/
		Vector3 GetPosition()
		{
			return m_position;
		}
		/**
		 * @brief ���f���̉�]���擾
		 * @return ��]
		*/
		Quaternion GetRotation()
		{
			return m_qRot;
		}

		/**
		 * @brief ���f���̊g�嗦���擾
		 * @return �g�嗦
		*/
		Vector3 GetScale()
		{
			return m_scale;
		}
		/**
		* @brief ���f�����擾
		* @param model �r���[�|�[�g�ԍ�(�J�����ԍ�)
		* @return �ԍ�����擾�������f���ւ̎Q��
		*/
		Model& GetModel(EModel model = eModel)
		{
			return m_model[model];
		}
		/**
		 * @brief �{�[���̖��O���炻�̃{�[���̃��[���h�s����擾����
		 * @param boneName �{�[���̖��O
		 * @return �{�[���̃��[���h�s��
		*/
		//Matrix GetWorldMatrixFromBoneName(const wchar_t* boneName);

		/**
		 * @brief �A�j���[�V�����̍Đ�
		 * @param animationNo �A�j���[�V�����ԍ�
		 * @param interpolateTime �A�j���[�V�����̕⊮����
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
		 * @brief �A�j���[�V�����̍Đ����x��ݒ肷��
		 * @param animationSpeed �Đ����x
		*/
		void SetAnimationSpeed(float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

		/**
		 * @brief �A�j���[�V�������Đ����Ă��邩?
		 * @return �Đ����Ă���:true �Đ����Ă��Ȃ�:false
		*/
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}
		/**
		 * @brief ���f�����e����邩�ǂ�����ݒ肷��
		 * @param isShadowCaster ���:true ���Ȃ�:false
		*/
		void SetShadowCasterFlag(bool isShadowCaster)
		{
			m_isShadowCaster = isShadowCaster;
		}
		/// <summary>
		/// ���C�v��ʕ\���悤���ǂ���
		/// </summary>
		/// <param name="flag"></param>
		void SetIsWipeModel(bool flag,int num)
		{
			m_isWipeModel[num] = flag;
		}
		/// <summary>
		/// ���C����ʕ\�����ǂ���
		/// </summary>
		/// <param name="flag"></param>
		void SetIsMainModel(bool flag)
		{
			m_isMainModel = flag;
		}
		/**
		 * @brief �{�[���̖��O���炻�̃{�[���̃��[���h�s����擾����
		 * @param boneName �{�[���̖��O
		 * @return �{�[���̃��[���h�s��
		*/
		Matrix GetWorldMatrixFromBoneName(const wchar_t* boneName);

		Skeleton& GetSkeleton() { return m_skeleton; }
		/// <summary>
		/// �X�L�����f�������_�[���A�N�e�B�u������B
		/// </summary>
		void Activate()
		{
			m_isActive = true;
		}
		/// <summary>
		/// �X�L�����f�������_�[���A�N�e�B�u������B
		/// </summary>
		/// <remark>
		/// ��A�N�e�B�u������ƁA���f�����`�悳��Ȃ��Ȃ�܂��B
		/// �ꎞ�I�ɕ\�����I�t�ɂ������ꍇ�ȂǂɎg�p���Ă��������B
		/// </remark>
		void Deactivate()
		{
			m_isActive = false;

		}
private:
	//���f���̏������f�[�^
	ModelInitData m_initData[eModel_Num];
	std::function<void(int boneNo, Bone* bone)> m_onPostCalcLocalMatrix;
	Skeleton m_skeleton; //�X�P���g��
	Model m_model[eModel_Num]; //���f��
	Vector3 m_position; //���W
	Quaternion m_qRot; //��]
	Vector3 m_scale; //�g�嗦
	AnimationClip* m_animationClips = nullptr;	//�A�j���[�V�����N���b�v
	int m_animationClipNum = 0;					//�A�j���[�V�����N���b�v�̐�
	Animation m_animation;						//�A�j���[�V�����B
	float m_animationSpeed = 1.0f;				//�A�j���[�V�������x
	bool m_isShadowCaster = true;				//���̃��f���͉e����邩?
	bool m_isWipeModel[3] = { false };			//���C�v�p�̃��f����
	bool m_isMainModel = true;					//���C����ʗp�̃��f����
	bool m_isActive = true;						//�A�N�e�B�u�t���O
};
}