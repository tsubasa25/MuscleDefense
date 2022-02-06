#pragma once
#include "stdafx.h"
namespace nsMuscle {
	namespace nsHUDConstant{
		const Vector2 MENUFONT_POS[4] = { { -550.0f,200.0f },{-250.0f,200.0f},{80.0f,200.f},{350.0f,200.f} };//真ん中の人30.0f,50.0f
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
		enum EnPreparationStatus {//準備フェーズでの状態
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
		void WaveDisplay(int gymNum);//Wave数が画面に出る
		/// <summary>
		/// 文字を表示させる
		/// </summary>
		/// <param name="sourceText">テキスト</param>
		/// <param name="isTraining">メニュー選択時</param>
		void SentenceDisplay(wchar_t sourceText[256]);//文字が流れる
		void TrainingMenuDisplay();
		void SetMenuColor();
		void SetMenuPosition();
		void MenuConfirmation();
		void WaveWinning();//ウェーブ突破時に出される画面
		bool GetIsMenuSelect() { return m_isMenuSelect; }
		void SetIsMenuSelect(bool flag) { m_isMenuSelect = flag; }
		void SetPreparationStatus(EnPreparationStatus state) { m_preparationStatus = state; }
		int GetPreparationStatus(){ return m_preparationStatus; }
		/// <summary>
		/// trueなら全画面
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
		float m_sentenceTimer = 0.0f;//文章表示用
		
		wchar_t m_preText[256] = { L"トレーニングメニューを選ぼう!" };
		wchar_t m_preTextAbdominal[20] = { L"腹筋" };
		wchar_t m_preTextPectoral[20] = { L"胸筋" };
		wchar_t m_preTextLeg[20] = { L"足筋" };
		wchar_t m_preTextAbdominalOne[20] = { L"腹" };
		wchar_t m_preTextPectoralOne[20] = { L"胸" };
		wchar_t m_preTextLegOne[20] = { L"足" };
		wchar_t m_muscleDrawText[256];
		wchar_t m_preBaseText[256] = { L"ここは　　を鍛えれそうだ!" };
		bool m_isPreTextMuscleFontOne = true;

		wchar_t m_opeText0[256] = { L"ゾンビが現れた!拠点を防衛せよ!" };
		wchar_t m_opeText1[256] = { L"拠点を防衛せよ!" };
		wchar_t m_waveEnemyText[256];

		float m_readSpeed = 0.01f;//テキストが進むスピード

		wchar_t m_drawText[256];
		//bool m_gameFlag = true;
		int m_textNum = 0;

		Vector2 m_menuFontPos = { Vector2::Zero };
		int m_menuSelectNum = 0;//メニュー番号
		int m_selectPlayerNum = 0;//メニュー設定するプレイヤーの番号
		bool m_lastConfirmation = false;//メニュー最終確認
		bool m_isFirstConfirmation = false;//確認状態の最初のループ
		bool m_isFirstPreparation = false;//準備状態の最初のループ
		bool m_isFirstSentece = false;//文章を出すときの最初のループ
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