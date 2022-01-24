#pragma once
namespace nsMuscle {
	struct PointLigData
	{
		Vector3 position;//�ʒu
		float pointLigNum;//���Ԗڂɍ������
		Vector3 color;//�F
		float range;//���a
	};
	class PointLight :public IGameObject
	{

		bool Start();
		void Update();

		PointLigData* m_pointLigData;	//�|�C���g���C�g�̃f�[�^

	public:
		PointLight();
		~PointLight();
		int GetPointLigNum() { return int(m_pointLigData->pointLigNum); }
		void SetPointLigNum(int num) { m_pointLigData->pointLigNum = num; }
		/**
			 * @brief �|�C���g���C�g�̃f�[�^���擾
			 * @return �|�C���g���C�g�̃f�[�^
			*/
		PointLigData* GetLigData() { return m_pointLigData; }

		/**
		 * @brief �|�C���g���C�g�̃f�[�^�̃T�C�Y���擾
		 * @return �|�C���g���C�g�̃f�[�^�̃T�C�Y
		*/
		int GetLigDataSize() { return sizeof(*m_pointLigData); }

		/**
		 * @brief �|�C���g���C�g�̍��W��ݒ�
		 * @param pos �|�C���g���C�g�̍��W
		*/
		void SetPosition(Vector3 pos) { m_pointLigData->position = pos; }

		/**
		 * @brief �|�C���g���C�g�̍��W���擾
		 * @return �|�C���g���C�g�̍��W
		*/
		Vector3 GetPosition() { return m_pointLigData->position; }

		/**
		 * @brief �|�C���g���C�g�̐F��ݒ�
		 * @param color �|�C���g���C�g�̐F
		*/
		void SetColor(Vector3 color) { m_pointLigData->color = color; }

		/**
		 * @brief �|�C���g���C�g�̐F���擾
		 * @return �|�C���g���C�g�̐F
		*/
		Vector3 GetColor() { return m_pointLigData->color; }

		/**
		 * @brief �|�C���g���C�g�̉e���͈͂�ݒ�
		 * @param range �|�C���g���C�g�̉e���͈�
		*/
		void SetRange(float range) { m_pointLigData->range = range; }

		/**
		 * @brief �|�C���g���C�g�̉e���͈͂��擾
		 * @return �|�C���g���C�g�̉e���͈�
		*/
		float GetRange() { return m_pointLigData->range; }

	};
}