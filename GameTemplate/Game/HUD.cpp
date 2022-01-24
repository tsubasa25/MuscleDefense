#include "stdafx.h"
#include "HUD.h"
#include "GameScene.h"
#include "Player.h"
#include "BackGround.h"
#include "GoldGym.h"
#include "Map.h"
#include "ResultScene.h"
#include <cmath>
namespace nsMuscle {
	HUD::~HUD()
	{
		Map* map = FindGO<Map>("map");
		DeleteGO(map);
		DeleteGO(m_killFont);
		for (int i = 0; i < WIPE_MAX_NUM; i++) {
			DeleteGO(m_gymHPBar[i]);
			DeleteGO(m_muscleBar[i]);
			DeleteGO(m_muscleIcon[i]);
			DeleteGO(m_edgebar[i]);
			DeleteGO(m_edgebar[i+WIPE_MAX_NUM]);
			DeleteGO(m_builIcon[i]);
		}
	}
	bool HUD::Start()
	{
		m_gameScene = FindGO<GameScene>("gameScene");
		/// <summary>
		/// 事前に作っておく
		/// </summary>
		m_muscleFont = NewGO<FontRender>(2);
		m_muscleFont->SetPosition({ -155.0f,-260.0f });
		m_muscleFont->SetShadowFlag(true);		
		m_muscleFont->SetColor(Vector4::White);		
		m_muscleFont->SetText(m_preTextPectoralOne);
		m_muscleFont->Deactivate();
		return true;
	}
	void HUD::Update()
	{
		switch (m_gameScene->GetEnGameStatus())
		{
		case GameScene::EnGameStatus::enGameStatus_Opening:
			Opening();			
			break;
		case GameScene::EnGameStatus::enGameStatus_Preparation:
			Preparation();//準備
			break;
		case GameScene::EnGameStatus::enGameStatus_Confirmation:
			MenuConfirmation();//準備完了確認
			WipeMove(m_isWipeToFull[m_gameScene->GetBaseNum()], m_gameScene->GetBaseNum());
			break;
		case GameScene::EnGameStatus::enGameStatus_Wave1:
			
			WipeMove(m_isWipeToFull[m_gameScene->GetBaseNum()], m_gameScene->GetBaseNum());
			KilledNumberDisplay();
			break;
		case GameScene::EnGameStatus::enGameStatus_Wave2:			
			
			WipeMove(m_isWipeToFull[m_gameScene->GetBaseNum()], m_gameScene->GetBaseNum());
			KilledNumberDisplay();
			break;
		case GameScene::EnGameStatus::enGameStatus_Wave3:
			
			WipeMove(m_isWipeToFull[m_gameScene->GetBaseNum()], m_gameScene->GetBaseNum());
			KilledNumberDisplay();
			break;
		case GameScene::EnGameStatus::enGameStatus_WaveEnemyPop:
			
			break;
		case GameScene::EnGameStatus::enGameStatus_WaveWin:
			WaveWinning();
			//KilledNumberDisplay();
			break;
		case nsMuscle::GameScene::enGameStatus_Lose:
			Lose();
			break;
		case nsMuscle::GameScene::enGameStatus_Clear:
			Clear();
			break;
		default:
			break;
		}
		
	}
	void HUD::Opening()
	{
		SentenceDisplay(m_opeText0);
	}
	void HUD::Preparation()
	{
		/*if (m_isFirstPreparation == false)
		{			
			m_isFirstPreparation = true;
		}*/
		switch (HUD::m_preparationStatus)
		{
		case nsMuscle::HUD::enPreparationStatus_MoveCamera:
			for (int i = 0; i < 4; i++)
			{
				WipeMove(m_isWipeToFull[i], i);
			}
			break;		
		case nsMuscle::HUD::enPreparationStatus_MenuSelect:
			if(m_gameScene->GetBaseNum()==0)
			{
				MuscleSentence(0, Vector4::Blue);
			}
			if (m_gameScene->GetBaseNum() == 1)
			{
				MuscleSentence(1, Vector4::Red);//m_preTextAbdominal
			}
			if (m_gameScene->GetBaseNum() == 2)
			{
				MuscleSentence(2, Vector4::Green);
			}
			SentenceDisplay(m_preBaseText);
			TrainingMenuDisplay();
			break;
		default:
			break;
		}
	}
	void HUD::WaveDisplay(int gymNum)
	{
		if (m_loopCount == 0) {			
			switch (gymNum)
			{
			case 0:
				swprintf_s(m_waveEnemyText, L"胸筋ジムにゾンビが現れた!");
				break;
			case 1:
				swprintf_s(m_waveEnemyText, L"腹筋ジムにゾンビが現れた!");
				break;
			case 2:
				swprintf_s(m_waveEnemyText, L"足筋ジムにゾンビが現れた!");
				break;
			default:
				break;
			}
			SentenceDisplay(m_waveEnemyText);			
		}
		else{
			SentenceDisplay(m_waveEnemyText);
		}
		//
		m_loopCount++;
	}

	void HUD::SentenceDisplay(wchar_t sourceText[256])
	{
		if (m_isFirstSentece == false)
		{
			m_textBox = NewGO<SpriteRender>(2);//画面下に半透明の画像をおく
			m_textBox->Init("Assets/Image/textBox.dds", 850.0f, 720.0f);
			m_textBox->SetMulColor({ 1.0f, 1.0f, 1.0f, 0.3f });
			m_sentence = NewGO<FontRender>(2);
			m_sentence->SetPosition({ wcslen(sourceText)*-22.0f,-260.0f });
			m_textNum = 0;
			m_sentenceTimer = 0;	
			for (int i = 0; i < 255; i++) {
				m_drawText[i] = L' ';
			}
			m_isFirstSentece = true;			
		}		
		m_delayTimer++;
		if (g_pad[0]->IsTrigger(enButtonA)&&m_delayTimer>5) {//5フレームたっていてAボタンを押されたら消す
			DeleteGO(m_sentence);
			DeleteGO(m_textBox);
			m_muscleFont->Deactivate();
			m_isFirstSentece = false;
			m_sentenceTimer = 0;
			SoundOneShotPlay(L"Assets/sound/piron.wav", nsHUDConstant::DETERMINATION_SE_VOL);
			if (m_gameScene->GetEnGameStatus() == GameScene::EnGameStatus::enGameStatus_Preparation
				|| m_gameScene->GetEnGameStatus() == GameScene::EnGameStatus::enGameStatus_Opening) {
				m_preparationStatus = enPreparationStatus_MoveCamera;
				m_gameScene->SetEnGameStatus(GameScene::EnGameStatus::enGameStatus_Preparation);
			}
			return;
		}
		
		if (m_sentenceTimer < m_readSpeed)//文字が進む速さ
		{
			m_sentenceTimer += g_gameTime->GetFrameDeltaTime();
			return;
		}
		if (sourceText[m_textNum] != L'\0') {
			SoundOneShotPlay(L"Assets/sound/pon.wav", nsHUDConstant::SENTENCE_SE_VOL);
			m_sentenceTimer = 0;
			m_textNum++;
			if (m_textNum == 5|| m_textNum == 4)
			{
				m_isPreTextMuscleFontOne = true;
			}
			std::wcsncpy(m_drawText, sourceText, m_textNum);//ソーステキストからどこまで表示するか数えてからドローテキストにコピーする
			m_sentence->SetText(m_drawText);
		}
	}
	void HUD::TrainingMenuDisplay()
	{
		if (m_timer == 0) {
			
			//ワイプが写しているジムがもう消されていたら、ワイプ番号を飛ばす
			while (m_gameScene->GetIsBreakGym(m_selectWipeNum)|| m_selectWipeNum == 4)
			{
				m_selectWipeNum++;
			}
			
			m_isWipeToFull[m_gameScene->GetBaseNum()] = true;//ワイプを全画面にする	
		}
		m_timer+=g_gameTime->GetFrameDeltaTime();
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_isWipeToFull[m_gameScene->GetBaseNum()] = false;
			BackGround* backGround = FindGO<BackGround>("backGround");
			m_gameScene->TrainingMenuSet(m_gameScene->GetBaseNum(), backGround->GetGoldGym(m_gameScene->GetBaseNum()).GetLivePlayerNum());
			m_isMenuSelect = true;
			m_selectPlayerNum++;
			if (m_gameScene->GetIsDeathPlayer(m_selectPlayerNum) == true)
			{
				m_selectPlayerNum++;
			}
			if (m_selectPlayerNum >= 3) {//最後の人まで行ったら
				m_gameScene->SetEnGameStatus(GameScene::EnGameStatus::enGameStatus_Confirmation);
			}
			else {
				m_selectWipeNum++;
				////ワイプが写しているジムがもう消されていたら、ワイプ番号を飛ばす
				if (m_gameScene->GetIsBreakGym(m_selectWipeNum) == true)
				{
					m_selectWipeNum++;
				}
			}			
			m_timer = 0;
			m_preparationStatus = enPreparationStatus_MoveCamera;
		}
		WipeMove(m_isWipeToFull[m_gameScene->GetBaseNum()], m_gameScene->GetBaseNum());
	}
	void HUD::SetMenuColor()
	{
		
	}
	void HUD::SetMenuPosition()
	{
		m_hantoumei->SetPosition(nsHUDConstant::MENUSPEITE_POS[m_selectPlayerNum]);
		m_menuFontPos = nsHUDConstant::MENUFONT_POS[m_selectPlayerNum];
		for (int i = 0; i < 3; i++) {			
			m_trainingMenu[i]->SetPosition(m_menuFontPos);
			m_menuFontPos.y -= 40;			
		}
		m_menuSelectNum = 0;//ついでに選択中のメニューもリセット
		SetMenuColor();
	}
	void HUD::MenuConfirmation()
	{
		if (m_isFirstConfirmation == false)
		{
			m_waveStart = NewGO<SpriteRender>(2);
			switch (m_gameScene->GetCurrentWaveNum())
			{
			case 0:
				m_waveStart->Init("Assets/image/WAVE1.dds",1280,720);
				break;
			case 1:
				m_waveStart->Init("Assets/image/WAVE2.dds", 1280, 720);
				break;
			case 2:
				m_waveStart->Init("Assets/image/WAVE3.dds", 1280, 720);
				break;
			case 3:
				m_waveStart->Init("Assets/image/WAVE4.dds", 1280, 720);
				break;
			default:
				m_waveStart->Init("Assets/image/WAVE4.dds", 1280, 720);
				break;
			}			
			m_isFirstConfirmation = true;
		}
		if(g_pad[0]->IsTrigger(enButtonA)) {
			//ワイプ画面を小さく
			m_isWipeToFull[0] = false;
			m_isWipeToFull[1] = false;
			m_isWipeToFull[2] = false;
			m_isWipeToFull[3] = false;
			SetIsKillNumUpdate();
			DeleteGO(m_waveStart);			
			if (m_gameScene->GetCurrentWaveNum() == 0) {
				m_gameScene->Sally();
				m_gameScene->SetEnGameStatus(GameScene::EnGameStatus::enGameStatus_Wave1);
			}
			if (m_gameScene->GetCurrentWaveNum() == 1)
				m_gameScene->SetEnGameStatus(GameScene::EnGameStatus::enGameStatus_Wave2);
			if (m_gameScene->GetCurrentWaveNum() == 2)
				m_gameScene->SetEnGameStatus(GameScene::EnGameStatus::enGameStatus_Wave3);
			m_gameScene->GetSpringCamera().SetPosition(m_gameScene->GetAdvanceCameraPos());
			m_gameScene->GetSpringCamera().SetTarget(m_gameScene->GetAdvanceCameraTarget());
			if (IS_BGM) {
				SoundSource* ss = FindGO<SoundSource>("titleBGM");
				if(ss!=nullptr)
				ss->Release();
			}
			SoundOneShotPlay(L"Assets/sound/Determination.wav",nsHUDConstant::DETERMINATION_SE_VOL);
		}
		//WipeMove(m_isWipeToFull[m_selectWipeNum], m_selectWipeNum);
		for (int i = 0; i < 4; i++)
		{
			WipeMove(m_isWipeToFull[i], i);
		}
	}
	void HUD::WaveWinning()
	{
		if (m_waveWinTimer==0)
		{
			m_waveWin = NewGO<SpriteRender>(4);
			m_waveWin->Init("Assets/image/WIN.dds", 1280, 720);
			m_oldKillNum = 0;
			m_loopCount = 0;
			SoundOneShotPlay(L"Assets/sound/winning.wav", nsHUDConstant::WINNING_SE_VOL);
		}
		m_waveWinTimer++;
		if (m_waveWinTimer > 100)
		{
			DeleteGO(m_waveWin);
			m_gameScene->SetEnGameStatus(GameScene::enGameStatus_Confirmation);
			m_waveWinTimer = 0;
			m_selectPlayerNum = 0;//リセット
			m_selectWipeNum = 0;
			m_isFirstConfirmation = false;
		}
	}
	void HUD::WipeMove(bool flag,int wipeNum)
	{
		if (flag) {//真なら
			if (m_wipeTimer[wipeNum] <= 10) {
				m_wipeTimer[wipeNum]++;
				PostEffectManager::GetInstance()->WipeToFull(m_wipeTimer[wipeNum] / 10.0f, wipeNum);
				//g_camera3D[wipeNum + 1]->SetHeightAspect(lerp(0.5625f, 1.0f, m_wipeTimer[wipeNum] / 10.0f));
			}
		}
		else//偽なら
		{
			if (m_wipeTimer[wipeNum] > 0) {
				m_wipeTimer[wipeNum]--;
				PostEffectManager::GetInstance()->WipeToFull(m_wipeTimer[wipeNum] / 10.0f, wipeNum);
				//g_camera3D[wipeNum + 1]->SetHeightAspect(lerp(0.5625f, 1.0f, m_wipeTimer[wipeNum] / 10.0f));
			}
		}
		for (int i = 0; i < WIPE_MAX_NUM; i++)
		{
			g_camera3D[i + 1]->SetHeightAspect(lerp(1.333333f, 1.0f, m_wipeTimer[i] / 10.0f));
		}
	}
	void HUD::KilledNumberDisplay()
	{		
		if (m_isOnce==true)
		{
			m_isOnce = false;
			m_killFont = NewGO<FontRender>(0);
			m_killFont->SetPosition({ -80.0f,340.0f });
			m_killFont->SetColor(Vector4::White);
			m_killFont->SetShadowFlag(true);
			m_killFont->SetShadowColor(Vector4::Black);
			wchar_t killNum[256];// = L"WAVE"
			int ten = nsGameSceneConstant::WAVE_POP_NUM[m_gameScene->GetCurrentWaveNum()] / 10;
			int one = nsGameSceneConstant::WAVE_POP_NUM[m_gameScene->GetCurrentWaveNum()] % 10;
			int nowTen = m_gameScene->GetWaveEnemyDethNum() / 10;
			int nowOne = m_gameScene->GetWaveEnemyDethNum() % 10;
			swprintf_s(killNum, L"%d%*s%d%*s/%d%*s%d", nowTen, 1, L"", nowOne, 1, L"", ten, 1, L"", one);
			m_killFont->SetText(killNum);
		}
		if(m_isKillNumUpdate==true)
		{
			m_isKillNumUpdate = false;
			//m_oldKillNum = m_gameScene->GetWaveEnemyDethNum();
			wchar_t killNum[256];//
			if (m_gameScene->GetIsSecondaryStage())
			{
				int ten = nsGameSceneConstant::WAVEENEMY_POP_NUM[m_gameScene->GetCurrentWaveNum()] / 10;
				int one = nsGameSceneConstant::WAVEENEMY_POP_NUM[m_gameScene->GetCurrentWaveNum()] % 10;
				int nowTen = m_gameScene->GetRedWaveEnemyDethNum() / 10;
				int nowOne = m_gameScene->GetRedWaveEnemyDethNum() % 10;
				swprintf_s(killNum, L"%d%*s%d%*s/%d%*s%d",nowTen,1,L"",nowOne,1,L"",ten,1,L"",one);
				m_killFont->SetColor(Vector4::Red);
			}
			else {
				int ten = nsGameSceneConstant::WAVE_POP_NUM[m_gameScene->GetCurrentWaveNum()] / 10;
				int one = nsGameSceneConstant::WAVE_POP_NUM[m_gameScene->GetCurrentWaveNum()] % 10;
				int nowTen = m_gameScene->GetWaveEnemyDethNum() / 10;
				int nowOne = m_gameScene->GetWaveEnemyDethNum() % 10;
				swprintf_s(killNum,L"%d%*s%d%*s/%d%*s%d",nowTen,1,L"",nowOne,1,L"",ten,1,L"",one);
				m_killFont->SetColor(Vector4::White);
			}
			m_killFont->SetText(killNum);			
		}
	}
	void HUD::ParameterBarCreate()
	{
		for (int i = 0; i < WIPE_MAX_NUM; i++)
		{
			m_gymHPBar[i] = NewGO<SpriteRender>(3);
			m_gymHPBar[i]->Init("Assets/Image/Map/HPbar.dds", 8.0f, 80.0f);
			m_gymHPBar[i]->SetPivot({ 0.5f,0.0f });
			m_muscleBar[i] = NewGO<SpriteRender>(3);
			m_muscleBar[i]->Init("Assets/Image/Map/Musclebar.dds", 8.0f, 80.0f);
			m_muscleBar[i]->SetPivot({ 0.5f,0.0f });
			m_muscleIcon[i] = NewGO<SpriteRender>(3);
			m_edgebar[i] = NewGO<SpriteRender>(4);
			m_edgebar[i+WIPE_MAX_NUM] = NewGO<SpriteRender>(4);
			m_edgebar[i]->Init("Assets/Image/Map/Edgebar.dds", 8.0f, 80.0f);
			m_edgebar[i + WIPE_MAX_NUM]->Init("Assets/Image/Map/EdgebarMuscle.dds", 8.0f, 80.0f);
			m_builIcon[i]= NewGO<SpriteRender>(4);
			m_builIcon[i]->Init("Assets/Image/Map/buil.dds", 30.0f, 30.0f);
			SetMuscleParameter(i, 0.5f);
			switch (i)
			{
			case 0:
				m_muscleIcon[i]->Init("Assets/Image/Map/ChestDot.dds", 30.0f, 30.0f);
				m_gymHPBar[i]->SetPosition({ 460.0f,240.0f,0.0f });				
				m_muscleBar[i]->SetPosition({ 485.0f,240.0f,0.0f });
				m_edgebar[i]->SetPosition({ 460.0f,280.0f,0.0f });
				m_edgebar[i + WIPE_MAX_NUM]->SetPosition({ 485.0f,280.0f,0.0f });
				m_muscleIcon[i]->SetPosition({ 485.0f,225.0f,0.0f });
				m_builIcon[i]->SetPosition({ 460.0f,225.0f,0.0f });
				break;
			case 1:
				m_muscleIcon[i]->Init("Assets/Image/Map/AbdominalDot.dds", 30.0f, 30.0f);
				m_gymHPBar[i]->SetPosition({ -490.0f,240.0f,0.0f });
				m_muscleBar[i]->SetPosition({ -465.0f,240.0f,0.0f });
				m_edgebar[i]->SetPosition({ -490.0f,280.0f,0.0f });
				m_edgebar[i + WIPE_MAX_NUM]->SetPosition({ -465.0f,280.0f,0.0f });
				m_muscleIcon[i]->SetPosition({ -465.0f,225.0f,0.0f });
				
				m_builIcon[i]->SetPosition({ -490.0f,225.0f,0.0f });
				break;
			case 2:
				m_muscleIcon[i]->Init("Assets/Image/Map/legDot.dds", 30.0f, 30.0f);
				m_gymHPBar[i]->SetPosition({ -490.0f,-330.0f,0.0f });
				m_muscleBar[i]->SetPosition({ -465.0f,-330.0f,0.0f });
				m_edgebar[i]->SetPosition({ -490.0f,-290.0f,0.0f });
				m_edgebar[i + WIPE_MAX_NUM]->SetPosition({ -465.0f,-290.0f,0.0f });
				m_muscleIcon[i]->SetPosition({ -465.0f,-345.0f,0.0f });				
				m_builIcon[i]->SetPosition({ -490.0f,-345.0f,0.0f });
				break;
			default:
				break;
			}
		}		
	}
	void HUD::ParameterDeactivate(bool flag)
	{
		for (int i = 0; i < WIPE_MAX_NUM; i++)
		{
			if (flag) {//真なら見えなくする
				m_muscleIcon[i]->Deactivate();
				m_gymHPBar[i]->Deactivate();
				m_muscleBar[i]->Deactivate();
				m_edgebar[i]->Deactivate();
				m_edgebar[i + WIPE_MAX_NUM]->Deactivate();
				m_builIcon[i]->Deactivate();
			}
			else//偽なら見えるようにする
			{
				m_muscleIcon[i]->Activate();
				m_gymHPBar[i]->Activate();
				m_muscleBar[i]->Activate();
				m_edgebar[i]->Activate();
				m_edgebar[i + WIPE_MAX_NUM]->Activate();
				m_builIcon[i]->Activate();
			}
		}
	}
	void HUD::SetMuscleParameter(int gymNum,float muscle)
	{
		//筋肉量に応じてバーの拡大率を変える
		m_muscleBar[gymNum]->SetScale({1.0f,muscle,0.0f});
	}
	void HUD::SetGymHPParameter(int gymNum, float hp)
	{
		m_gymHPBar[gymNum]->SetScale({ 1.0f, hp / 1000, 0.0f });
	}
	void HUD::Lose()
	{
		if (m_isLose == false)
		{
			m_isLose = true;
			m_lose = NewGO<SpriteRender>(4);
			m_lose->Init("Assets/image/LOSE.dds", 1280, 720);
		}
		if (g_pad[0]->IsTrigger(enButtonA)) {
			NewGO<ResultScene>(0, "resultScene");
			m_gameScene->Delete();
			DeleteGO(m_lose);
			DeleteGO(this);
		}
	}
	void HUD::Clear()
	{
		if (m_isClear == false)
		{
			m_isClear = true;
			m_waveWin = NewGO<SpriteRender>(4);
			m_waveWin->Init("Assets/image/WIN.dds", 1280, 720);
		}
		if (g_pad[0]->IsTrigger(enButtonA)) {
			NewGO<ResultScene>(0, "resultScene");
			m_gameScene->Delete();
			m_gameScene->EnemySpriteDataReset();
			DeleteGO(m_waveWin);
			DeleteGO(m_lose);
			DeleteGO(this);			
		}
	}
	void HUD::MuscleSentence(int baseNum, Vector4 color)
	{		
		if (m_textNum == 4 && m_isPreTextMuscleFontOne)
		{
			m_muscleFont->Activate();
			m_isPreTextMuscleFontOne = false;	
			m_muscleFont->SetShadowColor(color);
			switch (baseNum)
			{
			case 0:				
				m_muscleFont->SetText(m_preTextPectoralOne);
				break;
			case 1:				
				m_muscleFont->SetText(m_preTextAbdominalOne);
				break;
			case 2:				
				m_muscleFont->SetText(m_preTextPectoralOne);
				break;
			default:
				break;
			}
		}
		if (m_textNum == 5 && m_isPreTextMuscleFontOne)
		{
			m_isPreTextMuscleFontOne = false;			
			switch (baseNum)
			{
			case 0:
				m_muscleFont->SetText(m_preTextPectoral);
				break;
			case 1:
				m_muscleFont->SetText(m_preTextAbdominal);
				break;
			case 2:
				m_muscleFont->SetText(m_preTextLeg);
				break;
			default:
				break;
			}
		}
	}
}