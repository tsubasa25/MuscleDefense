/*!
 * @brief	�T�E���h�G���W��
 */

#pragma once

#include "WaveFileBank.h"
#define INPUTCHANNELS 2  // number of source channels
#define OUTPUTCHANNELS 8	//�ő�o�̓`�����l�����B
namespace nsMuscle {
	
	class WaveFile;
	class SoundSource;
	/*!
	 * @brief	�T�E���h�G���W���B
	 */
	class SoundEngine {
	private:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		SoundEngine();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~SoundEngine();

	public:

		static void CreateInstance()
		{
			if (m_instance == nullptr)
			{
				m_instance = new SoundEngine;
			}
		}

		static void DeleteInstance()
		{
			delete m_instance;
			m_instance = nullptr;
		}

		static SoundEngine* GetInstance()
		{
			return m_instance;
		}

		/*!
		 * @brief	�������B
		 */
		void Init();
		/*!
		 * @brief	�J���B
		 */
		void Release();
		/*!
		* @brief	�X�V�B
		*/
		void Update();
		/*!
		* @brief	3D�T�E���h�\�[�X��ǉ��B
		*/
		void Add3DSoundSource(SoundSource* s)
		{
			m_3dSoundSource.push_back(s);
		}
		/*!
		* @brief	3D�T�E���h�\�[�X���폜�B
		*/
		void Remove3DSoundSource(SoundSource* s)
		{
			auto it = std::find(m_3dSoundSource.begin(), m_3dSoundSource.end(), s);
			if (it != m_3dSoundSource.end()) {
				m_3dSoundSource.erase(it);
			}
		}

		//�T�E���h�\�[�X�����X�g�ɒǉ�
		void AddSoundSource(SoundSource* s)
		{
			m_SoundSourceList.push_back(s);
		}

		//�T�E���h�\�[�X�����X�g����폜
		void RemoveSoundSource(SoundSource* s)
		{
			auto it = std::find(m_SoundSourceList.begin(), m_SoundSourceList.end(), s);
			if (it != m_SoundSourceList.end()) {
				m_SoundSourceList.erase(it);
			}
		}

		/*!
		* @brief	�T�E���h���X�i�[�̍��W��ݒ�B
		* @details
		*  3D�������g���Ă��鎞�̌v�Z�Ɏg�p����܂��B
		*/
		void SetListenerPosition(const Vector3& pos)
		{
			m_listenerPosition = pos;
		}
		/*!
		* @brief	�T�E���h���X�i�[�̑O������ݒ�B
		* @details
		*  3D�������g���Ă��鎞�̌v�Z�Ɏg�p����܂��B
		*/
		void SetListenerFront(Vector3 front)
		{
			m_listener.OrientFront.x = front.x;
			m_listener.OrientFront.y = front.y;
			m_listener.OrientFront.z = front.z;
		}
		/*!
		* @brief	�T�E���h���X�i�[�̏������ݒ�B
		* @details
		*  3D�������g���Ă��鎞�̌v�Z�Ɏg�p����܂��B
		*/
		void SetListenerUp(Vector3 up)
		{
			m_listener.OrientTop.x = up.x;
			m_listener.OrientTop.y = up.y;
			m_listener.OrientTop.z = up.z;
		}
		/*!
		* @brief	XAudio2�̃\�[�X�{�C�X���쐬�B
		*/
		IXAudio2SourceVoice* CreateXAudio2SourceVoice(WaveFile* waveFile, bool is3DSound);

		IXAudio2MasteringVoice* GetMasteringVoice()
		{
			return m_masteringVoice;
		}
		IXAudio2SubmixVoice* GetSubmixVoice()
		{
			return m_submixVoice;
		}
		/*!
		* @brief	�o�̓`�����l���̐����擾�B
		*/
		int GetNumChannel() const
		{
			return m_nChannels;
		}
		/*!
		* @brief	�g�`�f�[�^�o���N���擾�B
		*/
		WaveFileBank& GetWaveFileBank()
		{
			return m_waveFileBank;
		}
		/// <summary>
		/// �T�E���h�G���W�������p�\���ǂ�������B
		/// </summary>
		/// <returns>true���Ԃ��Ă����痘�p�\�ł��B</returns>
		bool IsAvailable() const
		{
			return m_isInited;
		}

		//BGM�̉��ʂ�ݒ�
		void SetBGMVolume(float volume);

		//SE�̉��ʂ�ݒ�
		void SetSEVolume(float volume);

		void UpdateVolume();
	private:
		IXAudio2* m_xAudio2 = nullptr;
		IXAudio2MasteringVoice* m_masteringVoice = nullptr;
		X3DAUDIO_HANDLE m_hx3DAudio;					//!<3D�I�[�f�B�I�̃n���h���B
		IUnknown* m_reverbEffect = nullptr;				//!<���o�[�v�G�t�F�N�g�B
		IXAudio2SubmixVoice* m_submixVoice = nullptr;	//!<�T�u�~�b�N�X�{�C�X�B
		DWORD m_channelMask = 0;						//!<�I�[�f�B�I�`�����l���}�X�N�B
		DWORD m_nChannels = 0;							//!<�`�����l�����B
		Vector3 m_listenerPosition = Vector3::Zero;	//!<���X�i�[�̍��W�B
		float m_fListenerAngle = 0.0f;
		bool m_UseListenerCone = false;
		bool m_fUseInnerRadius = false;
		bool m_useRedirectToLFE = false;
		bool m_isInited = false;
		X3DAUDIO_LISTENER m_listener;					//!<�T�E���h���X�i�[�B
		std::list<SoundSource*>	m_3dSoundSource;	//!<3D�T�E���h�̃T�E���h�\�[�X�B
		X3DAUDIO_CONE m_emitterCone;
		WaveFileBank m_waveFileBank;					//!<�g�`�f�[�^�̃o���N�B

		std::list<SoundSource*> m_SoundSourceList;//���ݑ��݂��邷�ׂẴT�E���h�\�[�X�̃��X�g
		float m_BGMBaseVolume = 1.0f;//BGM�̊�b�{�����[��
		float m_SEBaseVolume = 1.0f;//SE�̊�b�{�����[��

		static SoundEngine* m_instance;
	};
}