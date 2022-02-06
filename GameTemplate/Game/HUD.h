#pragma once
#include "stdafx.h"
namespace nsMuscle {
	namespace nsHUDConstant{
		const Vector2 MENUFONT_POS[4] = { { -550.0f,200.0f },{-250.0f,200.0f},{80.0f,200.f},{350.0f,200.f} };//�^�񒆂̐l30.0f,50.0f
		const Vector3 MENUSPEITE_POS[4] = { { -550.0f,200.0f,0.0f},{-250.0f,200.0f,0.0f},{80.0f,200.f,0.0f} ,{350.0f,200.f,0.0f} };
		const int MAX_WAVE_NUM = 4;
		const float SENTENCE_SE_VOL = 0.3f;
		const float DETERMINATION_SE_VOL = 0.3f;
		const float WINNING_SE_VOL = 0.5f;
	}
	class GameScene;
	class HUD :public IGameObject
	{
	public:
		enum EnPreparationStatus {//�����t�F�[�Y�ł̏��
			enPreparationStatus_MoveCamera,			
			enPreparationStatus_MenuSelect,
		};
	private:
		EnPreparationStatus m_preparationStatus = enPreparationStatus_MoveCamera;
	public:
		~HUD();
		bool Start();
		void Update();
		void Opening();
		void Preparation();
		void WaveDisplay(int gymNum);//Wave������ʂɏo��
		/// <summary>
		/// ������\��������
		/// </summary>
		/// <param name="sourceText">�e�L�X�g</param>
		/// <param name="isTraining">���j���[�I����</param>
		void SentenceDisplay(wchar_t sourceText[256]);//�����������
		void TrainingMenuDisplay();
		void SetMenuColor();
		void SetMenuPosition();
		void MenuConfirmation();
		void WaveWinning();//�E�F�[�u�˔j���ɏo�������
		bool GetIsMenuSelect() { return m_isMenuSelect; }
		void SetIsMenuSelect(bool flag) { m_isMenuSelect = flag; }
		void SetPreparationStatus(EnPreparationStatus state) { m_preparationStatus = state; }
		int GetPreparationStatus(){ return m_preparationStatus; }
		/// <summary>
		/// true�Ȃ�S���
		/// </summary>
		/// <param name="flag"></param>
		/// <param name="wipeNum"></param>
		void WipeMove(bool flag,int wipeNum);

		void KilledNumberDisplay();

		void SetIsKillNumUpdate() { m_isKillNumUpdate = true; }
	
		float lerp(float start, float end, float t) {
			return (1.0f - t) * start + t * end;
		}

		void ParameterBarCreate();
		void ParameterDeactivate(bool flag);
		void SetMuscleParameter(int gymNum,float muscle);
		void SetGymHPParameter(int gymNum, float hp);

		void Lose();
		void Clear();

		void MuscleSentence(int baseNum, Vector4 color);
		void SetGameScene(GameScene& gameScene) { m_gameScene = &gameScene; }
	private:
		FontRender* m_sentence =  nullptr;
		FontRender* m_muscleFont = nullptr;
		FontRender* m_killFont = nullptr;

		FontRender* m_trainingMenu[3] = { nullptr };//
		SpriteRender* m_hantoumei = nullptr;
		SpriteRender* m_textBox = nullptr;
		SpriteRender* m_gymHPBar[WIPE_MAX_NUM] = { nullptr };
		SpriteRender* m_muscleBar[WIPE_MAX_NUM] = { nullptr };
		SpriteRender* m_muscleIcon[WIPE_MAX_NUM] = { nullptr };
		SpriteRender* m_edgebar[WIPE_MAX_NUM * 2] = { nullptr };
		SpriteRender* m_builIcon[WIPE_MAX_NUM] = { nullptr };
		SpriteRender* m_waveStart = nullptr;
		SpriteRender* m_waveWin = nullptr;
		SpriteRender* m_lose = nullptr;
		GameScene* m_gameScene=nullptr;
		int m_loopCount = 0;

		float m_timer = 0.0f;
		float m_sentenceTimer = 0.0f;//���͕\���p
		
		wchar_t m_preText[256] = { L"�g���[�j���O���j���[��I�ڂ�!" };
		wchar_t m_preTextAbdominal[20] = { L"����" };
		wchar_t m_preTextPectoral[20] = { L"����" };
		wchar_t m_preTextLeg[20] = { L"����" };
		wchar_t m_preTextAbdominalOne[20] = { L"��" };
		wchar_t m_preTextPectoralOne[20] = { L"��" };
		wchar_t m_preTextLegOne[20] = { L"��" };
		wchar_t m_muscleDrawText[256];
		wchar_t m_preBaseText[256] = { L"�����́@�@��b���ꂻ����!" };
		bool m_isPreTextMuscleFontOne = true;

		wchar_t m_opeText0[256] = { L"�]���r�����ꂽ!���_��h�q����!" };
		wchar_t m_opeText1[256] = { L"���_��h�q����!" };
		wchar_t m_waveEnemyText[256];

		float m_readSpeed = 0.01f;//�e�L�X�g���i�ރX�s�[�h

		wchar_t m_drawText[256];
		//bool m_gameFlag = true;
		int m_textNum = 0;

		Vector2 m_menuFontPos = { Vector2::Zero };
		int m_menuSelectNum = 0;//���j���[�ԍ�
		int m_selectPlayerNum = 0;//���j���[�ݒ肷��v���C���[�̔ԍ�
		bool m_lastConfirmation = false;//���j���[�ŏI�m�F
		bool m_isFirstConfirmation = false;//�m�F��Ԃ̍ŏ��̃��[�v
		bool m_isFirstPreparation = false;//������Ԃ̍ŏ��̃��[�v
		bool m_isFirstSentece = false;//���͂��o���Ƃ��̍ŏ��̃��[�v
		bool m_isMenuSelect = false;

		float m_wipeTimer[4] = { 0 };
		bool m_isWipeToFull[4] = { false,false,false,false };
		int m_selectWipeNum = 0;
		int m_oldKillNum = 0;
		bool m_isOnce = true;
		int m_waveWinTimer = 0;
		bool m_isKillNumUpdate = false;
		bool m_isLose = false;
		bool m_isClear = false;

		SoundSource* m_ssSentence = nullptr;

		int m_delayTimer = 0;
	};
}