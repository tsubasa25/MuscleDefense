#pragma once
namespace nsMuscle {
	struct DirLigData {
		Vector3 direction;//���C�g�̕���
		float pad0;//���Ԗڂɍ������
		Vector3 color;	 //���C�g�̃J���[
		float pad1;
	};
	class DirectionLight :public IGameObject
	{
		bool Start();
		void Update();
		//�f�[�^���쐬����
		DirLigData* m_dirLigData;

	public:
		~DirectionLight();
		DirectionLight();
		//�f�B���N�V�������C�g�̃f�[�^���擾����
		DirLigData* GetLigData() { return m_dirLigData; }
		//�f�B���N�V�������C�g�̃T�C�Y���擾����
		int GetLigDataSize() { return sizeof(*m_dirLigData); }
		//������ݒ肷��
		void SetDirection(Vector3 dir) {
			m_dirLigData->direction = dir;
			m_dirLigData->direction.Normalize();
		}

		//���������擾����
		Vector3 GetDirection() { return m_dirLigData->direction; }

		//�F��ݒ肷��
		void SetColor(Vector3 Color) { m_dirLigData->color = Color; }

		//�F�����擾����
		Vector3 GetColor() { return m_dirLigData->color; }

		Quaternion qRotY;
	};
}