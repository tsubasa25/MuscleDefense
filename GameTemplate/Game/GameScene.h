#pragma once
#include "stdafx.h"
#include <Camera/SpringCamera.h>
#include "Player.h"

#include "Path.h"
#include "PathFinding.h"

namespace nsMuscle {
	namespace nsGameSceneConstant {
		//const Vector3 BASE_POSITION = { 0.0f,0.0f,200.0f };
		const int MAX_PLAYER_NUM = 4;
		const int WAVE_POP_NUM[4] = { 2,3,4,50 };
		const int WAVEENEMY_POP_NUM[4] = { 3,3,4,50 };
		const int ZOMBI_BGM_NUM = 4;
		const float INGAME_BGM_VOL = 0.15f;
		const float TITLE_BGM_VOL = 0.3f;
		const Vector2 RANGE_APPEAR_WAVEENEMY_GYM0 = { 800.0f,800.0f};
		const Vector2 RANGE_APPEAR_WAVEENEMY_GYM1 = { 800.0f,500.0f};
		const Vector2 RANGE_APPEAR_WAVEENEMY_GYM2 = { 1000.0f,1000.0f };
		const Vector2 POSITION_APPEAR_WAVEENEMY_GYM0 = { 400.0f,800.0f };
		const Vector2 POSITION_APPEAR_WAVEENEMY_GYM1 = { 300.0f,0.0f };
		const Vector2 POSITION_APPEAR_WAVEENEMY_GYM2 = { 500.0f,500.0f };
		const Vector3 CAMERA_ARM = { 0.0f, 150.0f, 400.0f };
		const float MAX_CAMERA_SPEED = 1000.0f;
		const float CAMERA_RADIUS = 5.0f;
		const float CAMERA_DAMPINGRATE = 1.0f;
	}
	//class Player;
	class Enemy;
	class TestEnemy;
	class HUD;
	class BackGround;
	class Map;
	class GameScene :public IGameObject
	{
	public:
		enum EnGameStatus {
			enGameStatus_Waiting,//待機、何もしない
			enGameStatus_Opening,//ゲーム開始前の処理
			enGameStatus_Preparation,//準備
			enGameStatus_Confirmation,//トレーニングメニュー確認
			enGameStatus_Wave1,//ウェーブ１、敵が来る
			enGameStatus_Wave2,
			enGameStatus_Wave3,
			//enGameStatus_Wave4,
			enGameStatus_WaveEnemyPop,//一定数倒したらジムの近くで敵が湧く
			enGameStatus_WaveWin,
			enGameStatus_Clear,//ゲームクリア
			enGameStatus_Lose
		};
		
	private:
		EnGameStatus m_enGameStatus = EnGameStatus::enGameStatus_Waiting;
	public:
		~GameScene();
		bool Start();
		void Update();
		/// <summary>
		/// プレイヤーについていくカメラの移動処理
		/// </summary>
		void PlayerCamera();
		/// <summary>
		/// ワイプ画面を大きくする
		/// </summary>
		/// <returns></returns>
		bool GetIsWipeOn() { return m_isFullWipe; }
		/// <summary>
		/// ゲームの状態を取得
		/// </summary>
		/// <returns></returns>
		int GetEnGameStatus() { return m_enGameStatus; }
		/// <summary>
		/// ゲームの状態を設定
		/// </summary>
		/// <param name="status"></param>
		void SetEnGameStatus(EnGameStatus status) { m_enGameStatus = status; }
		/// <summary>
		/// ワイプ画面の大きさを設定
		/// </summary>
		void wipeScreenScale();
		/// <summary>
		/// トレーニング中のプレイヤーと入れ替わる
		/// </summary>
		void TakeOver();
		/// <summary>
		/// ウェーブエネミーが湧く
		/// </summary>
		void WaveEnemyPopScene();
		/// <summary>
		/// ゾンビの声のBGMを再生
		/// </summary>
		void ZombiBGM();
		/// <summary>
		/// BGMを再生
		/// </summary>
		void InGameBGM();
		/// <summary>
		/// トレーニングメニューをセットする
		/// </summary>
		/// <param name="menuNum">メニュー番号[0]胸[1]腹[2]足</param>
		/// <param name="playerNum">プレイヤー番号</param>
		void TrainingMenuSet(int menuNum, int playerNum);
		/// <summary>
		/// トレーニング中の人数を取得
		/// </summary>
		/// <returns></returns>
		int GetTrainingMemberNum() { return m_trainingMemberNum; }
		/// <summary>
		/// トレーニング状態から戦闘状態にする
		/// </summary>
		void Sally();
		/// <summary>
		///	プレイヤーが死んだ時
		/// </summary>
		/// <param name="num"></param>
		void DeathPlayer(int num);
		/// <summary>
		/// 一定時間で敵が出現
		/// </summary>
		/// <param name="enemyNum">一度に湧く数</param>
		void EnemyPop(int enemyNum);
		/// <summary>
		///	最初に敵を複数湧かせる
		/// </summary>
		/// <param name="enemyNum">敵の数</param>
		void EnemyPopStart(int enemyNum);
		/// <summary>
		/// 敵の総数を加算
		/// </summary>
		/// <param name="num"></param>
		void SetEnemyTotalNumber(int num) { m_enemyTotalNumber += num; }
		/// <summary>
		/// 敵の総数を取得
		/// </summary>
		/// <returns></returns>
		int GetEnemyTotalNumber() { return m_enemyTotalNumber; }
		/// <summary>
		/// プレイヤー情報を取得
		/// </summary>
		/// <param name="num">プレイヤー番号</param>
		/// <returns>プレイヤー情報</returns>
		Player& GetPlayer(int num) { return *m_player[num]; }
		/// <summary>
		/// 戦闘状態のプレイヤー情報を取得
		/// </summary>
		/// <returns>戦闘中のプレイヤー</returns>
		Player& GetNowWarPlayer() { return *m_player[m_nowWarNum]; }
		/// <summary>
		/// 戦闘中のプレイヤーの位置を取得
		/// </summary>
		/// <returns>プレイヤーの位置</returns>
		Vector3 GetTargetPosition() { return GetPlayer(m_nowWarNum).GetPosition(); }
		/// <summary>
		/// 戦闘中のプレイヤーの番号を取得
		/// </summary>
		/// <returns></returns>
		int GetNowWarNum() { return m_nowWarNum; }
		/// <summary>
		/// オープニング処理
		/// </summary>
		void Opening();
		/// <summary>
		/// スプリングカメラの位置を設定
		/// </summary>
		/// <param name="pos"></param>
		void SetCameraPos(Vector3 pos) { m_cameraPos = pos; }
		/// <summary>
		/// 経路探査で拠点に向かって動くカメラの処理
		/// </summary>
		/// <param name="num">拠点の番号</param>
		void BaseMoveCamera();

		void WaveEnemyPop(int enemyNum,int gymNum);//拠点の近くにランダムに大量ポップする
		void KillCount();
		void WaveManagement();

		void AddEnemyDethNum() { m_enemyDeathNum++; }
		int GetEnemyDethNum() { return m_enemyDeathNum; }

		void AddWaveEnemyDethNum() { m_waveEnemyDeathNum++; }
		int GetWaveEnemyDethNum() { return m_waveEnemyDeathNum; }
		void SetWaveEnemyDethNum(int num) { m_waveEnemyDeathNum = num; }
		void AddRedWaveEnemyDethNum() { m_redWaveEnemyDeathNum++; }
		int GetRedWaveEnemyDethNum() { return m_redWaveEnemyDeathNum; }
		void SetRedWaveEnemyDethNum(int num) { m_redWaveEnemyDeathNum = num; }
		int GetCurrentWaveNum() { return m_currentWaveNum; }
		void MinusPlayerTotalNumber();
		int GetPlayerTotalNumber() { return m_playerTotalNumber; }
		/// <summary>
		/// ジムが壊れたらtrue
		/// </summary>
		/// <param name="num"></param>
		void SetIsBreakGym(int num) { m_isBreakGym[num] = true; }
		bool GetIsBreakGym(int num) { return m_isBreakGym[num]; }

		void SetIsDeathPlayer(int num) { m_isDeathPlayer[num] = true; }
		bool GetIsDeathPlayer(int num) { return m_isDeathPlayer[num]; }

		int GetBaseNum() { return m_baseNum; }

		//void SetIsSecondaryStage() { m_isSecondaryStage = true; }
		bool GetIsSecondaryStage() { return m_isSecondaryStage; }
		SpringCamera& GetSpringCamera() { return m_springCamera; }
		Vector3& GetAdvanceCameraPos() { return m_advanceCameraPos; }
		Vector3& GetAdvanceCameraTarget() { return m_advanceCameraTarget; }

		void SetIsOpening(bool flag) { m_isOpening = flag; }

		void Delete() { DeleteGO(this); }

		void EnemySpriteDataReset();
	private:
		
		HUD* m_hud = nullptr;//
		BackGround* m_backGround = nullptr;
		Map* m_map = nullptr; 
		std::vector<Player*> m_player;//
		std::vector<Enemy*>m_enemy;
		//std::vector<TestEnemy*>m_testEnemy;

		int m_loopCount = 0;//
		Quaternion m_rot = Quaternion::Identity;
		
		float rotY = 0.f;
		Vector3 m_ligColor =  Vector3::One; 

		bool m_isFullWipe = false;//ワイプが全画面か否か
		float m_timer = 0.0f;//ワイプの大きさが徐々に変わるための変数
		
		Vector3 m_gymPlayerPos;//トレーニング中のプレイヤーのポジション

		int m_playerTotalNumber = nsGameSceneConstant::MAX_PLAYER_NUM;
		int m_nowWarNum = 3;//現在戦っているプレイヤーの番号
		int m_nextChangeNum = 0;//次交代するプレイヤーの番号
		int m_trainingMemberNum = 3;//トレーニング中のプレイヤーの数

		SpringCamera m_springCamera;//プレイヤーからカメラに伸びるアーム
		Vector3 m_toCameraPos = Vector3::One;//
		Vector3 m_cameraPos = Vector3::Zero;//カメラの位置
		Vector3 m_cameraToEnemy=Vector3::Zero;//カメラから的に向かうベクトル
		int m_enemyTotalNumber = 0;//敵の総数

		int m_enemyNum = 0;//敵の数
		int m_enemyNumMap = 0;//マップで使う
		int m_enemyPopTimer = 0;//敵が湧く間隔

		bool m_isOpening = true;//

		float m_cameraGain = 0.0f;//カメラ移動スピードの係数
		//経路探査
		Path m_path;
		PathFinding m_pathFiding;

		int m_baseNum = 0;//拠点の番号
		
		bool m_isEnd = false;//経路探査フラグ
		bool m_isWaveFlag = true;
		float m_pathMoveSpeed = 10.0f;//経路探査の移動スピード

		int m_enemyDeathNum = 0;//敵が死んだ値
		int m_waveEnemyDeathNum = 0;//それぞれのウェーブで敵が死んだ数
		int m_redWaveEnemyDeathNum = 0;//ウェーブエネミーが死んだ数

		int m_currentWaveNum = 0;//現在のウェーブ番号

		bool m_isBreakGym[4] = { false, false, false, false };
		bool m_isDeathPlayer[4] = { false,  false,  false,  false};

		float m_setupTimer = 0.0f;

		bool m_isWavePop = false;
		int m_randGymNum = 0;//ランダムなジム番号
		int m_waveCameraMoveTimer = 0;
		Vector3 m_waveCameraPos = Vector3::Zero;
		Vector3 m_waveCameraTarget = Vector3::Zero;
		bool m_isSecondaryStage = false;//敵を必要人数倒したあとtrue
		int m_delayTimer = 0;

		Vector3 m_advanceCameraPos = Vector3::Zero;
		Vector3 m_advanceCameraTarget = Vector3::Zero;

		SoundSource* m_ssZombieVoice = nullptr;
		SoundSource* m_ssZombieBGM =  nullptr ;
		bool m_isZombieBGMCreate = false;

		SoundSource* m_ssInGameBGM = nullptr;
		bool m_isInGameBGMCreate = false;

		SoundSource* m_ssTitleBGM = nullptr;
		bool m_isTitleBGM = false;
	};
}
