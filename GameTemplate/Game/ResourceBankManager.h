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

		TResourceBank<TkmFile> m_tkmFileBank;			//tkmファイルバンク。
		TResourceBank<Shader> m_shaderBank;				//シェーダーバンク
		TResourceBank<Texture>	m_textureBank;			// テクスチャバンク。
		TResourceBank<LowTexture> m_lowTextureBank;		// テクスチャの生データバンク。
		TResourceBank<TksFile> m_tksFileBank;			//tksファイルバンク
		TResourceBank<TkaFile> m_tkaFileBank;			//tkaファイルバンク

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
		/// tkmファイルをバンクから取得。
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <returns>tkmファイル。nullptrが返ってきたらバンクに登録されていない。</returns>
		TkmFile* GetTkmFileFromBank(const char* filePath)
		{
			return m_tkmFileBank.Get(filePath);
		}
		/// <summary>
		/// tkmファイルをバンクに登録。
		/// </summary>
		/// <param name="filePath">tkmファイルパス</param>
		/// <param name="tkmFile"></param>
		void RegistTkmFileToBank(const char* filePath, TkmFile* tkmFile)
		{
			m_tkmFileBank.Regist(filePath, tkmFile);
		}
		/// <summary>
		/// tksファイルをバンクから取得。
		/// </summary>
		/// <param name="tksFilePath">ファイルパス</param>
		/// <returns>tksファイル。nullptrが返ってきたらバンクに登録されていない。</returns>
		TksFile*GetTksFromBank(const char* tksFilePath)
		{
			return m_tksFileBank.Get(tksFilePath);
		}
		/// <summary>
		/// tkmファイルをバンクに登録。
		/// </summary>
		/// <param name="tksFilePath">tksファイルパス</param>
		/// <param name="tksFile"></param>
		void RegistTksFileToBank(const char* tksFilePath, TksFile* tksFile)
		{
			m_tksFileBank.Regist(tksFilePath, tksFile);
		}
		/// <summary>
		/// tkaファイルをバンクから取得。
		/// </summary>
		/// <param name="tkaFilePath">ファイルパス</param>
		/// <returns>tkaファイル。nullptrが返ってきたらバンクに登録されていない。</returns>
		TkaFile* GetTkaFromBank(const char* tkaFilePath)
		{
			return m_tkaFileBank.Get(tkaFilePath);
		}
		/// <summary>
		/// tkaファイルをバンクに登録。
		/// </summary>
		/// <param name="tkaFilePath">tkaファイルパス</param>
		/// <param name="tkaFile"></param>
		void RegistTkaFileToBank(const char* tkaFilePath, TkaFile* tkaFile)
		{
			m_tkaFileBank.Regist(tkaFilePath, tkaFile);
		}
		/// <summary>
		/// シェーダーファイルバンクからシェーダーを取得。
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
		/// シェーダーファイルをバンクに登録。
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
		/// テクスチャをバンクに登録。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="texture">テクスチャ。</param>
		void RegistTextureToBank(const char* filePath, Texture* texture)
		{
			m_textureBank.Regist(filePath, texture);
		}
		/// <summary>
		/// バンクからテクスチャの生データを取得
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <returns></returns>
		LowTexture* GetLowTextureFromBank(const char* filePath)
		{
			return m_lowTextureBank.Get(filePath);
		}
		/// <summary>
		/// テクスチャの生データをバンクに登録。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="texture">テクスチャ。</param>
		void RegistLowTextureToBank(const char* filePath, LowTexture* textureLow)
		{
			m_lowTextureBank.Regist(filePath, textureLow);
		}
		
	};

}