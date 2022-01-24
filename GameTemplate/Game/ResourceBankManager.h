#pragma once
#include "TResourceBank.h"

//class TkmFile;
namespace nsMuscle {
	class ResourceBankManager
	{
	private:
		ResourceBankManager() = default;
		~ResourceBankManager() = default;

		static ResourceBankManager* m_instance;

		TResourceBank<TkmFile> m_tkmFileBank;			//tkm�t�@�C���o���N�B
		TResourceBank<Shader> m_shaderBank;				//�V�F�[�_�[�o���N
		TResourceBank<Texture>	m_textureBank;			// �e�N�X�`���o���N�B
		TResourceBank<LowTexture> m_lowTextureBank;		// �e�N�X�`���̐��f�[�^�o���N�B
		TResourceBank<TksFile> m_tksFileBank;			//tks�t�@�C���o���N
		TResourceBank<TkaFile> m_tkaFileBank;			//tka�t�@�C���o���N

	public:
		static void CreateInstance()
		{
			if (!m_instance)
			{
				m_instance = new ResourceBankManager;
			}
		}

		static void DeleteInstance()
		{
			delete m_instance;
			m_instance = nullptr;
		}

		static ResourceBankManager* GetInstance()
		{
			return m_instance;
		}

		/// <summary>
		/// tkm�t�@�C�����o���N����擾�B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <returns>tkm�t�@�C���Bnullptr���Ԃ��Ă�����o���N�ɓo�^����Ă��Ȃ��B</returns>
		TkmFile* GetTkmFileFromBank(const char* filePath)
		{
			return m_tkmFileBank.Get(filePath);
		}
		/// <summary>
		/// tkm�t�@�C�����o���N�ɓo�^�B
		/// </summary>
		/// <param name="filePath">tkm�t�@�C���p�X</param>
		/// <param name="tkmFile"></param>
		void RegistTkmFileToBank(const char* filePath, TkmFile* tkmFile)
		{
			m_tkmFileBank.Regist(filePath, tkmFile);
		}
		/// <summary>
		/// tks�t�@�C�����o���N����擾�B
		/// </summary>
		/// <param name="tksFilePath">�t�@�C���p�X</param>
		/// <returns>tks�t�@�C���Bnullptr���Ԃ��Ă�����o���N�ɓo�^����Ă��Ȃ��B</returns>
		TksFile*GetTksFromBank(const char* tksFilePath)
		{
			return m_tksFileBank.Get(tksFilePath);
		}
		/// <summary>
		/// tkm�t�@�C�����o���N�ɓo�^�B
		/// </summary>
		/// <param name="tksFilePath">tks�t�@�C���p�X</param>
		/// <param name="tksFile"></param>
		void RegistTksFileToBank(const char* tksFilePath, TksFile* tksFile)
		{
			m_tksFileBank.Regist(tksFilePath, tksFile);
		}
		/// <summary>
		/// tka�t�@�C�����o���N����擾�B
		/// </summary>
		/// <param name="tkaFilePath">�t�@�C���p�X</param>
		/// <returns>tka�t�@�C���Bnullptr���Ԃ��Ă�����o���N�ɓo�^����Ă��Ȃ��B</returns>
		TkaFile* GetTkaFromBank(const char* tkaFilePath)
		{
			return m_tkaFileBank.Get(tkaFilePath);
		}
		/// <summary>
		/// tka�t�@�C�����o���N�ɓo�^�B
		/// </summary>
		/// <param name="tkaFilePath">tka�t�@�C���p�X</param>
		/// <param name="tkaFile"></param>
		void RegistTkaFileToBank(const char* tkaFilePath, TkaFile* tkaFile)
		{
			m_tkaFileBank.Regist(tkaFilePath, tkaFile);
		}
		/// <summary>
		/// �V�F�[�_�[�t�@�C���o���N����V�F�[�_�[���擾�B
		/// </summary>
		/// <param name="filePath"></param>
		/// <returns></returns>
		Shader* GetShaderFromBank(const char* filePath, const char* entryPointFuncName)
		{
			std::string programName = filePath;
			programName += entryPointFuncName;
			return m_shaderBank.Get(programName.c_str());
		}
		/// <summary>
		/// �V�F�[�_�[�t�@�C�����o���N�ɓo�^�B
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="shader"></param>
		void RegistShaderToBank(const char* filePath, const char* entryPointFuncName, Shader* shader)
		{
			std::string programName = filePath;
			programName += entryPointFuncName;
			m_shaderBank.Regist(programName.c_str(), shader);
		}
		Texture* GetTextureFromBank(const char* filePath)
		{
			return m_textureBank.Get(filePath);
		}		
		/// <summary>
		/// �e�N�X�`�����o���N�ɓo�^�B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <param name="texture">�e�N�X�`���B</param>
		void RegistTextureToBank(const char* filePath, Texture* texture)
		{
			m_textureBank.Regist(filePath, texture);
		}
		/// <summary>
		/// �o���N����e�N�X�`���̐��f�[�^���擾
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <returns></returns>
		LowTexture* GetLowTextureFromBank(const char* filePath)
		{
			return m_lowTextureBank.Get(filePath);
		}
		/// <summary>
		/// �e�N�X�`���̐��f�[�^���o���N�ɓo�^�B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <param name="texture">�e�N�X�`���B</param>
		void RegistLowTextureToBank(const char* filePath, LowTexture* textureLow)
		{
			m_lowTextureBank.Regist(filePath, textureLow);
		}
		
	};

}