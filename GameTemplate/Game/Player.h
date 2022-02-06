#pragma once
#include "PhysicsGhostObject.h"

namespace nsMuscle {
	namespace nsPlayerConstant {
		const Vector3 INI_POSITION = { 0.f,0.f,0.f };
		const Vector3 GYM_POSITION = { -5000.0f,0.0f,-50.0f };
		const Vector3 PLAYER_DIR_Y= { 0.0f,0.0f, -1.0f };
		const Vector3 UPPER_BOX_SIZE = { 100.0f,500.0f,100.0f };		
		const Vector3 PLAYER_COLOR[4] = { {10.0f,10.0f,10.0f},{1.0f,10.0f,1.0f},{1.0,1.0f,10.0f},{1.0f,10.0f,10.0f} };
		const float CAMERA_SPEED = 3.0f;
		const float LEVEL_1_VALUE = 0.25f;
		const float LEVEL_2_VALUE = 0.5f;
		const float LEVEL_3_VALUE = 0.75f;
		const float LEVEL_4_VALUE = 1.f;
		const float PUNCHI_DAMAGE = 20.0f;
		const float ARM_POSITION_Y = 60.0f;
		const float ARM_POSITION_Z = 60.0f;
		const float UPPER_BOX_SPEED = 20.0f;
		const float AMOUNT_TAKE_DAMAGE = 1.5f;
		const float GRAVITY = 100.0f;
		const float RUN_SPEED = 400.0f;
		const float WALK_SPEED = 50.0f;
		const float BASE_SPEED = 0.9f;
		const float VALUE_APPLY_IN_LEVEL_SPEED = 0.1f;
		const float AMOUNT_INCREASE_MUSCLE = 0.0003f;
		const float PLAYER_RADIUS = 30.0f;
		const float PLAYER_DIR_DEG_Y = -90.0f;
		const float MUSCLE_DEFAULT_VALUE = 0.5f;
	}									 //10.0f,1.0f,1.0f
	class PlayerAnimation;
	class PlayerVoice;
	class GameScene;
	class HUD;
	class Player :public IGameObject
	{
	public:
		enum EnPlayerStatus//プレイヤーの状態を表す
		{
			enPlayerStatus_War,
			enPlayerStatus_Training
		};
		enum EnTrainingStatus//トレーニング状態
		{
			enTrainingStatus_Idle,//待機
			enTrainingStatus_PushUp,//腕立て伏せ
			enTrainingStatus_SitUp,//上体起こし
			enTrainingStatus_Squat,//スクワット
			enTrainingStatus_LevelUp//レベルアップ
		};
	private:
		EnPlayerStatus m_playerStatus = enPlayerStatus_War;
		EnTrainingStatus m_trainingStatus = enTrainingStatus_Idle;
	public:
		~Player();
		bool Start();
		void Update();
		/// <summary>
		/// 腕立て伏せ
		/// </summary>
		void PushUp();
		/// <summary>
		/// 上体起こし
		/// </summary>
		void SitUp();
		/// <summary>
		/// スクワット
		/// </summary>
		void Squat();
		/// <summary>
		/// 移動処理
		/// </summary>
		void PlayerMove();
		/// <summary>
		/// 回転処理
		/// </summary>
		void Turn();
		/// <summary>
		/// 死んだ時
		/// </summary>
		void PlayerDeath();
		/// <summary>
		/// 攻撃処理
		/// </summary>
		void Attack();
		void JumpAttack();//ジャンプアタック処理
		/// <summary>
		/// 的にダメージを与える
		/// </summary>
		/// <param name="damage">ダメージ量</param>
		void EnemyAnyDamage(float damage);
		/// <summary>
		/// ダメージを受ける
		/// </summary>
		/// <param name="num">ダメージ量</param>
		void AnyDamage(float num);
		/// <summary>
		/// HPを取得
		/// </summary>
		/// <returns>HP</returns>
		float GetHP() { return m_hp; }
		/// <summary>
		/// プレイヤー番号を設定
		/// </summary>
		/// <param name="num">プレイヤー番号</param>
		void SetPlayerNum(int num) { m_playerNum = num; }
		/// <summary>
		/// プレイヤー番号を取得
		/// </summary>
		/// <returns>プレイヤー番号</returns>
		int GetPlayerNum() { return m_playerNum; }

		SkinModelRender* GetSkinModelRender() { return m_skinModelRender; }
		/// <summary>
		/// スピードを取得
		/// </summary>
		/// <returns></returns>
		Vector3 GetMoveSpeed() { return m_moveSpeed; }
		/// <summary>
		/// 位置を取得
		/// </summary>
		/// <returns></returns>
		Vector3 GetPosition() { return m_position; }
		/// <summary>
		/// 前向きのベクトルを取得
		/// </summary>
		/// <returns></returns>
		Vector3 GetPlayerDir() { return m_playerDir; }
		/// <summary>
		/// プレイヤーの状態を取得
		/// </summary>
		/// <returns></returns>
		EnPlayerStatus GetPlayerStatus() { return m_playerStatus; }
		/// <summary>
		/// プレイヤーのトレーニング状態を取得
		/// </summary>
		/// <returns></returns>
		EnTrainingStatus GetTrainingStatus() { return m_trainingStatus; }
		/// <summary>
		/// プレイヤーのトレーニング状態を設定
		/// </summary>
		/// <param name="status"></param>
		void SetTrainingStatus(EnTrainingStatus status) { m_trainingStatus = status; }
		/// <summary>
		/// 位置を設定
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(Vector3 pos)
		{
			m_position = pos;
			m_charaCon.SetPosition(pos);			
		}
		/// <summary>
		/// プレイヤーの状態を取得
		/// </summary>
		void SetPlayerStatus(EnPlayerStatus playerStatus) { m_playerStatus = playerStatus; }
		/// <summary>
		/// プレイヤーのスピードを設定
		/// </summary>
		/// <param name="speed"></param>
		void SetMoveSpeed(Vector3 speed) { m_moveSpeed = speed; }
		/// <summary>
		/// 現在のコンボ数を取得
		/// </summary>
		/// <returns></returns>
		const int& GetComboNum() const { return m_comboNum; }
		/// <summary>
		/// コンボ数を０にする
		/// </summary>
		void ReSetComboNum() { m_comboNum = 0; }
		/// <summary>
		/// 的にダメージを与えれるようにする
		/// </summary>
		void ReSetIsAttackOn() { m_isAttackOn = true; }

		/// <summary>
		/// トレーニング状態と戦闘状態を切り替えるときに必要
		/// </summary>
		/// <param name="flag"></param>
		void SetIsOnce(bool flag) {	m_isOnce = flag;}
		bool GetIsOnce() { return m_isOnce; }
		/// <summary>
		/// アニメーションを最初から再生できる
		/// </summary>
		/// <param name="flag"></param>
		void SetIsRePlay(bool flag) { m_isRePlay = flag; }
		bool GetIsRePlay() { return m_isRePlay; }
		/// <summary>
		/// ダメージを食らっている
		/// </summary>
		void SetIsDamage(bool flag) { m_isDamage = flag; }
		bool GetIsDamage() { return m_isDamage; }
		/// <summary>
		/// 死んだか
		/// </summary>
		void SetIsDeath(bool flag) { m_isDeath = flag; }
		bool GetIsDeath() { return m_isDeath; }
		/// <summary>
		/// ボーンごとの頂点モーフの影響率を設定
		/// </summary>
		/// <param name="bone">ボーンID</param>
		/// <param name="num">影響率</param>
		void SetMuscleRateArray(int bone, float num) { m_muscleRateArray[bone] = num; }
		/// <summary>
		/// 自分がいる拠点の番号
		/// </summary>
		/// <param name="num"></param>
		void SetGymLiveNum(int num) { m_gymLiveNum = num; }
		/// <summary>
		/// レベルアップの時TRUE
		/// </summary>
		/// <param name="flag"></param>
		void SetLevelUpOnce(bool flag) { m_levelUpOnce = flag; }
		bool GetLevelUpOnce() { return m_levelUpOnce; }
		/// <summary>
		/// 筋肉量に応じてレベルを変更する
		/// </summary>
		/// <param name="muscleScale">筋肉量</param>
		/// <param name="muscleLevel">筋肉レベル</param>
		/// <param name="oldLevel">前フレームの筋肉レベル</param>
		void LevelManagement(float muscleScale,int &muscleLevel,int &oldLevel);

		int GetAbdominalLevel() { return m_abdominalLevel; }

		PlayerAnimation& GetPlayerAnimation() { return *m_playerAnimation; }

		void SetPlayerRot(Quaternion rot) { m_playerRot = rot; }
	private:
		GameScene* m_gameScene = nullptr;
		HUD* m_hud = nullptr;
		float m_hp = 0.0f;					//体力
		bool m_isDamage = false;			//ダメージを食らっているか
		int m_playerNum = 0;				//プレイヤー番号
		bool m_isRePlay = false;			//アニメーションを最初から再生させる

		SkinModelRender* m_skinModelRender = nullptr;//プレイヤーのスキン
		Vector3 m_moveSpeed = Vector3::Zero;//スピード
		Vector3 m_scale = { 1.5f,1.5f, 1.5f };//拡大率
		Vector3 m_position = Vector3::Zero;	//プレイヤーの位置
		CharacterController m_charaCon;		//プレイヤーの物理カプセル

		PlayerAnimation* m_playerAnimation = nullptr;//アニメーションクラス
		PlayerVoice* m_playerVoice = nullptr;//サウンドクラス

		Vector3 m_playerDir = { 0.f,0.f,-1.0f };//プレイヤーが向いている方向
		Quaternion m_playerRot = Quaternion::Identity;//プレイヤーの回転
		Quaternion m_RotY= Quaternion::Identity;//プレイヤーの回転
		Quaternion m_cameraRot = Quaternion::Identity;//カメラの回転

		int m_loopCount = 0;				//ループ回数

		float m_armScale = 0.5f;			//腕の補間率
		float m_abdominalScale = 0.5f;		//腹の補間率
		float m_footScale = 0.5f;			//足の補間率

		float m_punchDamage = 0.0f;			//パンチ力、筋肉レベルによって変わる

		float m_velocity = 0.f;				//Lスティック入力に掛ける倍率

		/// <summary>
		/// 攻撃に関する変数
		/// </summary>
		int m_comboNum = 0;					//コンボ回数
		int m_jumpAttackLoop = 0;			//ジャンプアタック中のループ回数
		bool m_isAttackOn = true;			//ダメージを与えれるかのフラグ
		float* m_muscleRateArray = nullptr;	// 筋肉量の配列。
											// 0.0が最も貧弱。1.0で最大の筋肉量
											// 配列の要素数はスケルトンの骨の数と同じ。
											// 並びもスケルトンと同じ。
		StructuredBuffer m_muscleRateArraySB;	// 筋肉量の配列のストラクチャードバッファ。

		bool m_isOnce = true;				//トレーニング状態と戦闘状態を切り替えるときにTRUE

		PhysicsGhostObject m_ghostObject;	//パンチしたときにでる当たり判定
		
		Vector3 m_boxSize = { Vector3::Zero };//当たり判定の大きさ
		bool m_isDeath = false;				//死んだか

		float m_punchSpeed = 0.0f;			//パンチしたときに前にずれるスピード
		float m_upperWave = 1.0f;			//衝撃波の速度

		int m_gymLiveNum = 0;				//今いる拠点の番号

		int m_armLevel = 2;					//腕の筋肉レベル
		int m_abdominalLevel = 2;			//腹の筋肉レベル
		int m_footLevel = 2;				//足の筋肉レベル
		int m_oldArmLevel = 2;				//前フレームの腕の筋肉レベル
		int m_oldAbdominalLevel = 2;		//前フレームの腹の筋肉レベル
		int m_oldFootLevel = 2;				//前フレームの足の筋肉レベル
		bool m_levelUpOnce = false;			//レベルアップした時TRUE
	};
}
