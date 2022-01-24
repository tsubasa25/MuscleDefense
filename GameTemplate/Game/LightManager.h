#pragma once
#include <array>
#include "stdafx.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"
namespace nsMuscle {
	namespace nsLMConstant
	{
		//�e���C�g�̍ő吔�@���̐���model.fx�̃��C�g�z��̐��ƈꏏ�ɂ��Ȃ���΂Ȃ�Ȃ�	
		//const int DIRLIGHT_NUMBER_MAX = 5;
		const int POINTLIGHT_NUMBER_MAX = 20;
		const int SPOTLIGHT_NUMBER_MAX = 20;
	}

	class LightManager
	{
	public:
		LightManager() = default;
		~LightManager() = default;
		private:
		//���C�g�̏��ƃJ�����̈ʒu
		struct LigDatas
		{
			DirLigData directionLight;
			PointLigData pointLight[nsLMConstant::POINTLIGHT_NUMBER_MAX];
			SpotLigData spotLight[nsLMConstant::SPOTLIGHT_NUMBER_MAX];
			int pointLightNum;
			int spotLightNum;
			Vector2 pad8;
			Vector3 eyePos;         // ���_�̈ʒu
			float pad3;
			Vector3 ambientLight;   // �A���r�G���g���C�g
			float pad4;
		};
		LigDatas m_ligData;				//���C�g�̃f�[�^
		
		int m_size = sizeof(m_ligData);	//���C�g�̃f�[�^�̃T�C�Y
		int m_pointLigCreateNum = 0;
		
		struct LigCameraDatas
		{
			Matrix lightCameraProjectionMatrix; //���C�g�r���[�v���W�F�N�V�����s��
			Vector3 lightCameraPosition;//���C�g�J�����̈ʒu
			float pad;
			Vector3 lightCameraDirection;//���C�g�J�����̌���
			float pad1;
			float interporateRate = 0.5f;//��ԗ�
		};
		LigCameraDatas m_ligCameraData;
		int m_ligCameraDataSize = sizeof(m_ligCameraData);

		Camera m_lightCamera;//�V���h�E�}�b�v�p�̃��C�g�̈ʒu�̃J�����B�Ƃ肠�����e�X�g�B

	public:

		static LightManager* m_instance;

		/**
		 * @brief ���C�g�}�l�[�W���[�̃C���X�^���X���쐬����
		*/
		static void CreateInstance()
		{
			if (!m_instance)
			{
				m_instance = new LightManager;
			}
		}

		/**
		 * @brief ���C�g�}�l�[�W���[�̃C���X�^���X���폜����
		*/
		static void DeleteInstance()
		{
			delete m_instance;
			m_instance = nullptr;
		}

		/**
		 * @brief ���C�g�}�l�[�W���[�̃C���X�^���X���擾����
		*/
		static LightManager* GetInstance() { return m_instance; }
		/**
		 * @brief ���C�g�̃f�[�^�̉���擾����(�萔�o�b�t�@�ɓn���p)
		 * @return �S���C�g�̃f�[�^
		*/

		LigDatas* GetLigDatas() { return &m_ligData; }

		/**
		 * @brief ���C�g�̃f�[�^�̉�̃T�C�Y���擾����(�萔�o�b�t�@�ɓn���p)
		 * @return �S���C�g�̃f�[�^�̃T�C�Y
		*/
		int GetLigDataSize() { return m_size; }

		/**
		 * @brief �J�����̃|�W�V�������X�V����
		 * @param �J�����̔ԍ�
		*/
		void UpdateEyePos(int camNo) { m_ligData.eyePos = g_camera3D[camNo]->GetPosition(); }

		//���C�g�̐�����������
		//int GetDirectionLigNum() { return m_ligData.dirctionLightNum; }
		int GetPointLigNum() { return m_ligData.pointLightNum; }
		void SetPointLigNum(int num) { m_ligData.pointLightNum = num; }
		int GetSpotLigNum() { return m_ligData.spotLightNum; }
		void SetSpotLigNum(int num) { m_ligData.spotLightNum = num; }
		//���C�g�̐���ݒ肷��
		//void AddDirectionLigNum(int num) { m_ligData.dirctionLightNum = num + 1; }

		void AddPointLig(PointLigData* pointLigData) {
			//���C�g�̐����ŏ��Ɍ��߂����ȏ�Ȃ�throw����(�����ς����Ƃӂ��ɋN����)
			if (m_ligData.pointLightNum >= nsLMConstant::POINTLIGHT_NUMBER_MAX)
			{
				MessageBoxA(nullptr, "�|�C���g���C�g�̐����ő吔�𒴂��Ă��܂��B\n", "�G���[", MB_OK);
			}
			m_ligData.pointLight[GetPointLigNum()] = *pointLigData;//�f�[�^���i�[����
			pointLigData->pointLigNum = float(GetPointLigNum());//���Ԗڂɍ��ꂽ��
			m_ligData.pointLightNum = float(GetPointLigNum()) + 1.0f;//���C�g�̑���������₷
		}

		void AddSpotLigNum(SpotLigData*spotLigData) { 
		//���C�g�̐����ŏ��Ɍ��߂����ȏ�Ȃ�throw����(�����ς����Ƃӂ��ɋN����)
		if (m_ligData.spotLightNum >= nsLMConstant::POINTLIGHT_NUMBER_MAX)
		{
			MessageBoxA(nullptr, "�|�C���g���C�g�̐����ő吔�𒴂��Ă��܂��B\n", "�G���[", MB_OK);
		}
		m_ligData.spotLight[GetSpotLigNum()] = *spotLigData;//�f�[�^���i�[����
		spotLigData->spotLigNum = float(GetSpotLigNum());//���Ԗڂɍ��ꂽ��
		m_ligData.spotLightNum = float(GetSpotLigNum()) + 1.0f;//���C�g�̑���������₷
		}

		void UpdateDirctionLight(int directionnum, DirLigData* dirctionLigData)
		{
			m_ligData.directionLight = *dirctionLigData;
		}

		void UpdatePointLight(int pointLigNum, PointLigData* pointLigData)
		{
			m_ligData.pointLight[pointLigNum] = *pointLigData;//
		}

		void UpdateSpotLight(int spotLignum,SpotLigData* spotLigData)
		{ 
			m_ligData.spotLight[spotLignum] = *spotLigData; 
		}

		//void RemoveDirectionLight(int num)
		//{
		//	DirectionLightMinus();//���C�g�̐������炷
		//	for (int i = num; i < GetDirectionLigNum() - 1; i++) {
		//		m_ligData.directionLight[i] = m_ligData.directionLight[i + 1];//�f�[�^�����炷
		//		m_ligData.directionLight[i].directionLigNum--;//���������ꂽ�ԍ������炷
		//	}
		//}
		//void DirectionLightMinus()
		//{
		//	m_ligData.dirctionLightNum--;
		//	//���C�g�̐���0�ȉ��ɂȂ鎞�͂��������Ȃ��Ă���̂�throw����(�N���蓾�Ȃ��ƐM������)
		//	if (m_ligData.dirctionLightNum < 0)
		//	{
		//		MessageBoxA(nullptr, "�|�C���g���C�g�̐����}�C�i�X�ɂȂ��Ă��܂��B\n", "�G���[", MB_OK);
		//	}
		//}
		void RemovePointLight(int num)
		{
			PointLightMinus();//���C�g�̐������炷
			for (int i = num; i < GetPointLigNum() - 1; i++) {
				m_ligData.pointLight[i] = m_ligData.pointLight[i + 1];//�f�[�^�����炷
				m_ligData.pointLight[i].pointLigNum--;//���������ꂽ�ԍ������炷
			}

		}
		/**
		 * @brief �|�C���g���C�g�̐��̃J�E���g���}�C�i�X����
		*/
		void PointLightMinus()
		{
			m_ligData.pointLightNum--;

			//���C�g�̐���0�ȉ��ɂȂ鎞�͂��������Ȃ��Ă���̂�throw����(�N���蓾�Ȃ��ƐM������)
			if (m_ligData.pointLightNum < 0)
			{
				MessageBoxA(nullptr, "�|�C���g���C�g�̐����}�C�i�X�ɂȂ��Ă��܂��B\n", "�G���[", MB_OK);
			}
		}
		void RemoveSpotLight(int num)
		{
			SpotLightMinus();//���C�g�̐������炷
			for (int i = num; i < GetSpotLigNum() - 1; i++) {
				m_ligData.spotLight[i] = m_ligData.spotLight[i + 1];//�f�[�^�����炷
				m_ligData.spotLight[i].spotLigNum--;//���������ꂽ�ԍ������炷
			}
		}
		/**
		 * @brief �|�C���g���C�g�̐��̃J�E���g���}�C�i�X����
		*/
		void SpotLightMinus()
		{
			m_ligData.spotLightNum--;

			//���C�g�̐���0�ȉ��ɂȂ鎞�͂��������Ȃ��Ă���̂�throw����(�N���蓾�Ȃ��ƐM������)
			if (m_ligData.spotLightNum < 0)
			{
				MessageBoxA(nullptr, "�|�C���g���C�g�̐����}�C�i�X�ɂȂ��Ă��܂��B\n", "�G���[", MB_OK);
			}
		}
		DirLigData* GetDirLigData() { return &m_ligData.directionLight; }
		PointLigData* GetPointLigData() { return &m_ligData.pointLight[GetPointLigNum()]; }
		SpotLigData* GetSpotLigData() { return &m_ligData.spotLight[GetSpotLigNum()]; }

		////////////////////////////////////////////////////////////////////
		//�e�p
		/**
		 * @brief �e�̕`��Ɏg�p���郉�C�g�J�����̃f�[�^�̉���擾����(�萔�o�b�t�@�ɓn���p)
		 * @return ���C�g�J�����̃f�[�^
		*/
		LigCameraDatas* GetLigCameraDatas() { return &m_ligCameraData; }

		/**
		 * @brief �e�̕`��Ɏg�p���郉�C�g�J�����̃f�[�^�̉�̃T�C�Y���擾����(�萔�o�b�t�@�ɓn���p)
		 * @return ���C�g�J�����̃f�[�^�̃T�C�Y
		*/
		int GetLigCameraDataSize() { return m_ligCameraDataSize; }

		/**
		 * @brief �e�̕`��Ɏg�p���郉�C�g�J�������擾
		 * @return ���C�g�J����
		*/
		Camera* GetLightCamera()
		{
			return &m_lightCamera;
		}

		/**
		 * @brief �e�̕`��Ɏg�p���郉�C�g�J�����̍��W���w�肷��
		 * @param pos ���C�g�J�����̍��W
		*/
		void SetLightCameraPosition(const Vector3& pos)
		{
			m_lightCamera.SetPosition(pos);
			m_lightCamera.Update();
			m_ligCameraData.lightCameraPosition = pos;
			m_ligCameraData.lightCameraProjectionMatrix = m_lightCamera.GetViewProjectionMatrix();

			m_ligCameraData.lightCameraDirection = m_lightCamera.GetTarget() - m_lightCamera.GetPosition();
			m_ligCameraData.lightCameraDirection.Normalize();
		}

		/**
		 * @brief �e�̕`��Ɏg�p���郉�C�g�J�����̃^�[�Q�b�g�̍��W���w�肷��
		 * @param targetPos ���C�g�J�����̃^�[�Q�b�g�̍��W
		*/
		void SetLightCameraTarget(const Vector3& targetPos)
		{
			m_lightCamera.SetTarget(targetPos);
			m_lightCamera.Update();
			m_ligCameraData.lightCameraProjectionMatrix = m_lightCamera.GetViewProjectionMatrix();

			m_ligCameraData.lightCameraDirection = m_lightCamera.GetTarget() - m_lightCamera.GetPosition();
			m_ligCameraData.lightCameraDirection.Normalize();
		}
		/**
		 * @brief �e�̕`��Ɏg�p���郉�C�g�J�����̏�������w�肷��
		 * @param up �J�����̏�����̃x�N�g��
		*/
		void SetLightCameraUp(const Vector3& up)
		{
			m_lightCamera.SetUp(up);
			m_lightCamera.Update();
			m_ligCameraData.lightCameraProjectionMatrix = m_lightCamera.GetViewProjectionMatrix();
		}
		/**
		 * @brief �e�̕`��Ɏg�p���郉�C�g�J�����̎ˉe�s��̍X�V���@��ݒ肷��
		 * @param func �ˉe�s��̍X�V���@
		*/
		void SetLightCameraUpdateProjMatrixFunc(Camera::EnUpdateProjMatrixFunc func)
		{
			m_lightCamera.SetUpdateProjMatrixFunc(func);
			m_lightCamera.Update();
			m_ligCameraData.lightCameraProjectionMatrix = m_lightCamera.GetViewProjectionMatrix();
		}		
		void SetLightCameraFar(float fa)
		{
			m_lightCamera.SetFar(fa);
			m_lightCamera.Update();
		}
		void SetAmbientLight(Vector3 color) { m_ligData.ambientLight = color; }
		//void SetGroundColor(Vector3 color) { m_ligData.groundColor = color; }
		//void SetSkyColor(Vector3 color) { m_ligData.skyColor = color; }
		//void SetGroundNormal(Vector3 normal) { m_ligData.groundNormal = normal; }
		void SetInterporateRate(float num) { m_ligCameraData.interporateRate += num; }
		float GetInterporateRate() { return m_ligCameraData.interporateRate; }
	};
}
