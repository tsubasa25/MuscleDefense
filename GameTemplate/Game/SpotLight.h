#pragma once
namespace nsMuscle {
	struct SpotLigData
	{
		Vector3 position = Vector3::Zero;
		float spotLigNum = 0.0f;
		Vector3 color = Vector3::Zero;
		float range = 0.0f;
		Vector3 direction = Vector3::Zero;
		float angle = 0.0f;
	};
	class SpotLight :public IGameObject
	{

		bool Start();
		void Update();
		SpotLigData* m_spotLigData;	//�X�|�b�g���C�g�̃f�[�^

	public:
		SpotLight();
		~SpotLight();
		/**
			 * @brief �X�|�b�g���C�g�̃f�[�^���擾
			 * @return �X�|�b�g���C�g�̃f�[�^
			*/
		SpotLigData* GetLigData() { return m_spotLigData; }

		/**
		 * @brief �X�|�b�g���C�g�̃f�[�^�̃T�C�Y���擾
		 * @return �X�|�b�g���C�g�̃f�[�^�̃T�C�Y
		*/
		int GetLigDataSize() { return sizeof(SpotLigData); }

		/**
		 * @brief �X�|�b�g���C�g�̍��W��ݒ�
		 * @param pos �X�|�b�g���C�g�̍��W
		*/
		void SetPosition(Vector3 pos) { m_spotLigData->position= pos; }

		/**
		 * @brief �X�|�b�g���C�g�̍��W���擾
		 * @return �X�|�b�g���C�g�̍��W
		*/
		Vector3 GetPosition() { return m_spotLigData->position; }

		/**
		 * @brief �X�|�b�g���C�g�̐F��ݒ�
		 * @param color �X�|�b�g���C�g�̐F
		*/
		void SetColor(Vector3 color) { m_spotLigData->color = color; }

		/**
		 * @brief �X�|�b�g���C�g�̐F���擾
		 * @return �X�|�b�g���C�g�̐F
		*/
		Vector3 GetColor() { return m_spotLigData->color; }

		/**
		 * @brief �X�|�b�g���C�g�̉e���͈͂�ݒ�
		 * @param range �X�|�b�g���C�g�̉e���͈�
		*/
		void SetRange(float range) { m_spotLigData->range = range; }

		/**
		 * @brief �X�|�b�g���C�g�̉e���͈͂��擾
		 * @return �X�|�b�g���C�g�̉e���͈�
		*/
		float GetRange() { return m_spotLigData->range; }

		/**
		 * @brief �X�|�b�g���C�g�̌�����ݒ�
		 * @param dir �X�|�b�g���C�g�̌���
		*/
		void SetDirection(Vector3 dir) { m_spotLigData->direction = dir; }

		/**
		 * @brief �X�|�b�g���C�g�̌������擾
		 * @return �X�|�b�g���C�g�̌���
		*/
		Vector3 GetDirection() { return m_spotLigData->direction; }

		/**
		 * @brief �X�|�b�g���C�g�̉e���p�x��ݒ�(���W�A���P��)
		 * @param angle_rad �X�|�b�g���C�g�̉e���p�x
		*/
		void SetAngle(float angle_rad) { m_spotLigData->angle = angle_rad; }

		/**
		 * @brief �X�|�b�g���C�g�̉e���p�x���擾(���W�A���P��)
		 * @return �X�|�b�g���C�g�̉e���p�x
		*/
		float GetAngle() { return m_spotLigData->range; }

		/**
		 * @brief �X�|�b�g���C�g�̉e���p�x��ݒ�(�f�O���[�P��)
		 * @param angle_deg �X�|�b�g���C�g�̉e���p�x
		*/
		void SetAngleDeg(float angle_deg) { m_spotLigData->angle = Math::DegToRad(angle_deg); }

		/**
		 * @brief �X�|�b�g���C�g�̉e���p�x���擾(�f�O���[�P��)
		 * @return �X�|�b�g���C�g�̉e���p�x
		*/
		float GetAngleDeg() { return Math::RadToDeg(m_spotLigData->angle); }

	};

}