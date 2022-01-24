#include "stdafx.h"
#include "Player.h"
#include "PlayerAnimation.h"
#include "PlayerVoice.h"
#include "Enemy.h"
#include "GameScene.h"
#include "HUD.h"
namespace nsMuscle {
	Player::~Player()
	{
		delete[] m_muscleRateArray;
		DeleteGO(m_playerAnimation);
		DeleteGO(m_skinModelRender);
		DeleteGO(m_playerVoice);		
	}
	bool Player::Start()
	{
		m_playerAnimation = NewGO<PlayerAnimation>(0);
		m_playerAnimation->SetParent(*this);
		m_playerVoice = NewGO<PlayerVoice>(0);
		m_playerVoice->SetParent(*m_playerAnimation);
		m_skinModelRender = NewGO<SkinModelRender>(0);
		m_skinModelRender->SetIsMainModel(true);
		for (int i = 0; i < 3; i++)
		{
			m_skinModelRender->SetIsWipeModel(true, i);
		}
		//骨、アニメーション情報を初期化
		m_skinModelRender->InitSkeleton("Assets/modelData/Player/NakedPlayer.tks", m_playerAnimation->GetAnimationClips(), m_playerAnimation->GetAnimationClipNum());//

		// 筋肉量の配列を初期化。
		const auto& skeleton = m_skinModelRender->GetSkeleton();
		m_muscleRateArray = new float[skeleton.GetNumBones()];
		// 筋肉量を0.0で初期化する。
		memset(m_muscleRateArray, 0, sizeof(float) * skeleton.GetNumBones());

		// ストラクチャードバッファを構築。
		m_muscleRateArraySB.Init(
			sizeof(float),
			skeleton.GetNumBones(),
			m_muscleRateArray
		);
		for (int i = 0; i < skeleton.GetNumBones(); i++) {
			m_muscleRateArray[i] = nsPlayerConstant::MUSCLE_DEFAULT_VALUE;//補間率を代入
		}
		m_muscleRateArraySB.Update(m_muscleRateArray);
		//スキン情報を初期化
		switch (m_playerNum)
		{
		case 0:
			m_skinModelRender->Init("Assets/modelData/Player/NakedPlayerMin0.tkm", "Assets/modelData/Player/NakedPlayerMax0.tkm", &m_muscleRateArraySB);//
			break;
		case 1:
			m_skinModelRender->Init("Assets/modelData/Player/NakedPlayerMin1.tkm", "Assets/modelData/Player/NakedPlayerMax1.tkm", &m_muscleRateArraySB);
			break;
		case 2:
			m_skinModelRender->Init("Assets/modelData/Player/NakedPlayerMin2.tkm", "Assets/modelData/Player/NakedPlayerMax2.tkm", &m_muscleRateArraySB);
			break;
		case 3:
			m_skinModelRender->Init("Assets/modelData/Player/NakedPlayerMin3.tkm", "Assets/modelData/Player/NakedPlayerMax3.tkm", &m_muscleRateArraySB);
			break;
		default:
			break;
		}
		//位置を初期化
		m_skinModelRender->SetPosition(m_position);
		//回転を初期化
		
		m_playerRot.SetRotationDegY(nsPlayerConstant::PLAYER_DIR_DEG_Y);
		
		m_skinModelRender->SetRotation(m_playerRot);
		//キャラコンの初期化
		m_charaCon.Init(nsPlayerConstant::PLAYER_RADIUS, m_position);

		m_skinModelRender->SetShadowCasterFlag(true);//影の生成

		m_skinModelRender->SetScale({ m_scale });	

		m_gameScene = FindGO<GameScene>("gameScene");
		m_hud = FindGO<HUD>("hUD");
		return true;
	}
	void Player::Update()
	{
		m_muscleRateArraySB.Update(m_muscleRateArray);//頂点モーフ用
		m_loopCount++;
		int a = m_gameScene->GetEnGameStatus();
		if (a == GameScene::EnGameStatus::enGameStatus_Wave1
			|| a == GameScene::EnGameStatus::enGameStatus_Wave2
			|| a == GameScene::EnGameStatus::enGameStatus_Wave3)//戦闘状態の時
		{			
			if (m_playerStatus == EnPlayerStatus::enPlayerStatus_War)//戦闘状態の時
			{
				m_playerVoice->SetIsVoice(true);
				PlayerMove();
				Turn();
				Attack();
			}
			else if (m_playerStatus == EnPlayerStatus::enPlayerStatus_Training)//トレーニング状態の時
			{
				m_playerVoice->SetIsVoice(false);				
				switch (m_trainingStatus)
				{
				case nsMuscle::Player::enTrainingStatus_Idle:
					break;
				case nsMuscle::Player::enTrainingStatus_PushUp:
					PushUp();
					break;
				case nsMuscle::Player::enTrainingStatus_SitUp:
					SitUp();
					break;
				case nsMuscle::Player::enTrainingStatus_Squat:
					Squat();
					break;
				default:
					break;
				}
			}
		}
		else
		{
			m_playerVoice->SetIsVoice(false);
		}
		PlayerDeath();
	}
void Player::PushUp()
{
	std::vector<int>boneId;
	boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:RightArm"));
	boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:LeftArm"));
	boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:RightShoulder"));
	boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:LeftShoulder"));
	boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:Spine2"));
	boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:LeftForeArm"));
	boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:RightForeArm"));

	for (int i = 0; i < boneId.size(); i++) {
		m_muscleRateArray[boneId[i]] = min( 1.0f, m_armScale);//補間率を代入
		//m_skinModelRender->GetSkeleton().GetBone(boneId[i])->SetScale({ scale+1,scale + 1, scale + 1 });
	}
	//補間率を上げていく
	//プレイヤーごとに影響率を変えたいが、今は一律であげている
	if (m_playerAnimation->GetAnimationStatus() == m_playerAnimation->GetEnStatusPushUp()
		&&(m_gameScene->GetEnGameStatus()== GameScene::enGameStatus_Wave1
			|| m_gameScene->GetEnGameStatus() == GameScene::enGameStatus_Wave2
			|| m_gameScene->GetEnGameStatus() == GameScene::enGameStatus_Wave3))
	{
		m_armScale += nsPlayerConstant::AMOUNT_INCREASE_MUSCLE;
		m_armScale = min(1.0f, m_armScale);
		m_hud->SetMuscleParameter(m_gymLiveNum,m_armScale);		
		LevelManagement(m_armScale, m_armLevel, m_oldArmLevel);
	}
}
void Player::SitUp()
{
	std::vector<int>boneId;
	boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:Hips"));
	boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:Spine"));
	boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:Spine1"));	

	for (int i = 0; i < boneId.size(); i++) {
		m_muscleRateArray[boneId[i]] = min(1.0f, m_abdominalScale);//補間率を代入
		//m_skinModelRender->GetSkeleton().GetBone(boneId[i])->SetScale({ scale+1,scale + 1, scale + 1 });
	}
	//補間率を上げていく
	//プレイヤーごとに影響率を変えたいが、今は一律であげている
	if (m_playerAnimation->GetAnimationStatus() == PlayerAnimation::EnPlayerAnimStatus::enStatus_SitUp
		&& (m_gameScene->GetEnGameStatus() == GameScene::enGameStatus_Wave1
			|| m_gameScene->GetEnGameStatus() == GameScene::enGameStatus_Wave2
			|| m_gameScene->GetEnGameStatus() == GameScene::enGameStatus_Wave3))
	{
		m_abdominalScale += nsPlayerConstant::AMOUNT_INCREASE_MUSCLE;
		m_abdominalScale = min(1.0f, m_abdominalScale);
		m_hud->SetMuscleParameter(m_gymLiveNum, m_abdominalScale);
		LevelManagement(m_abdominalScale, m_abdominalLevel, m_oldAbdominalLevel);
	}
}
void Player::Squat()
{
	std::vector<int>boneId;
	boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:RightUpLeg"));
	boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:RightLeg"));
	boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:RightFoot"));
	boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:LeftUpLeg"));
	boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:LeftLeg"));
	boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:LeftFoot"));
	
	for (int i = 0; i < boneId.size(); i++) {
		m_muscleRateArray[boneId[i]] = min(1.0f, m_footScale);//補間率を代入
		//m_skinModelRender->GetSkeleton().GetBone(boneId[i])->SetScale({ scale+1,scale + 1, scale + 1 });
	}
	//補間率を上げていく
	//プレイヤーごとに影響率を変えたいが、今は一律であげている
	if (m_playerAnimation->GetAnimationStatus() == PlayerAnimation::EnPlayerAnimStatus::enStatus_Squat
		&& (m_gameScene->GetEnGameStatus() == GameScene::enGameStatus_Wave1
			|| m_gameScene->GetEnGameStatus() == GameScene::enGameStatus_Wave2
			|| m_gameScene->GetEnGameStatus() == GameScene::enGameStatus_Wave3))
	{
		m_footScale += nsPlayerConstant::AMOUNT_INCREASE_MUSCLE;
		m_footScale = min(1.0f, m_footScale);
		m_hud->SetMuscleParameter(m_gymLiveNum, m_footScale);
		LevelManagement(m_footScale, m_footLevel, m_oldFootLevel);
	}
}
void Player::PlayerMove()
{
	if (m_playerAnimation->GetAnimationStatus() == m_playerAnimation->GetEnStatusRun())
	{
		float levelVelocity = nsPlayerConstant::BASE_SPEED + (m_footLevel * nsPlayerConstant::VALUE_APPLY_IN_LEVEL_SPEED);
		m_velocity = nsPlayerConstant::RUN_SPEED * levelVelocity;//足の筋肉料に依存*(m_footScale+1)		
	}
	else
	{
		m_velocity = nsPlayerConstant::WALK_SPEED;
	}

	//このフレームの移動量を求める。
	//左スティックの入力量を受け取る。
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//カメラの前方方向と右方向を取得。
	Vector3 cameraForward = g_camera3D[0]->GetForward();
	Vector3 cameraRight = g_camera3D[0]->GetRight();
	//XZ平面での前方方向、右方向に変換する。
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ成分の移動速度をクリア。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	m_moveSpeed += cameraForward * lStick_y *m_velocity+(m_playerDir*m_punchSpeed);	//奥方向への移動速度を加算。
	m_moveSpeed += cameraRight * lStick_x  * m_velocity;		//右方向への移動速度を加算。
	
	m_moveSpeed.y -= nsPlayerConstant::GRAVITY * g_gameTime->GetFrameDeltaTime();
	//キャラクターコントローラーを使用して、座標を更新。
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//if (m_charaCon.IsOnGround()) {
	//	//地面についた。
	//	
	//}
	m_moveSpeed.y = 0.0f;
	//座標を設定。
	m_skinModelRender->SetPosition(m_position);	
}
void Player::Turn()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}
	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
	//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
	//atan2を使用して、角度を求めている。
	//これが回転角度になる。
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_playerRot.SetRotationY(-angle);
	
	//回転を設定する。
	m_skinModelRender->SetRotation(m_playerRot);
	
	m_playerDir = m_moveSpeed;
	m_playerDir.Normalize();
}

void Player::Attack()
{
	if (g_pad[0]->IsTrigger(enButtonA))//パンチ
	{		
		Effect windEff;
		Vector3 pos = m_position;
		Quaternion rot = Quaternion::Identity;
		Vector3 frontY = nsPlayerConstant::PLAYER_DIR_Y;//前を向いている方向
		float angle = 0.0f;
		float n = frontY.Dot(m_playerDir);
		m_punchDamage = nsPlayerConstant::PUNCHI_DAMAGE *m_armLevel;//腕の筋肉料に依存
		switch (m_playerAnimation->GetAnimationStatus())
		{
		case PlayerAnimation::EnPlayerAnimStatus::enStatus_Punchi0:
			m_comboNum = 1;
			EnemyAnyDamage(m_punchDamage);
			break;
		case PlayerAnimation::EnPlayerAnimStatus::enStatus_Punchi1:
			m_comboNum = 2;
			EnemyAnyDamage(m_punchDamage);
			break;
		case PlayerAnimation::EnPlayerAnimStatus::enStatus_Punchi2:
			m_comboNum = 3;
			EnemyAnyDamage(m_punchDamage*3.0f);
			windEff.Init(u"Assets/effect/wind.efk");
			windEff.Play();
			pos.y += nsPlayerConstant::ARM_POSITION_Y;
			pos += m_playerDir * nsPlayerConstant::ARM_POSITION_Z;
			windEff.SetPosition(pos);
			angle = acosf(n);
			if (m_playerDir.x > 0) {
				angle *= -1;
			}
			/*if (angle <= 1.0f && angle >= -1.0f) {
				angle *= -1;
			}*/
			rot.SetRotationY(angle);
			windEff.SetRotation(rot);
			windEff.Update();
			break;
		case PlayerAnimation::EnPlayerAnimStatus::enStatus_JumpAttack:
			
			break;
		default:
			break;
		}
		
	}
	else
	{
		m_punchSpeed = 0.0f;
	}
	if (g_pad[0]->IsTrigger(enButtonY))//キック
	{
		switch (m_playerAnimation->GetAnimationStatus())
		{		
		case PlayerAnimation::EnPlayerAnimStatus::enStatus_Kick0:
			m_comboNum = 1;
			EnemyAnyDamage(m_punchDamage);
			break;
		case PlayerAnimation::EnPlayerAnimStatus::enStatus_Kick1:
			m_comboNum = 2;
			EnemyAnyDamage(m_punchDamage);
			break;
		case PlayerAnimation::EnPlayerAnimStatus::enStatus_Kick2:
			m_comboNum = 3;
			EnemyAnyDamage(m_punchDamage*3);
			break;
		case PlayerAnimation::EnPlayerAnimStatus::enStatus_JumpAttack:
			
			break;
		default:
			break;
		}
		
	}
	
}
void Player::JumpAttack()
{
	m_jumpAttackLoop++;
	if (m_jumpAttackLoop > 10&&m_comboNum==3)
	{
		//m_moveSpeed.y = 400.0f;	//上方向に速度を設定
		m_comboNum = 0;
	}
}
void Player::EnemyAnyDamage(float damage)
{	
	if (m_comboNum == 3) {
		m_upperWave += nsPlayerConstant::UPPER_BOX_SPEED * m_armLevel;
		m_boxSize.x += nsPlayerConstant::UPPER_BOX_SPEED * m_armLevel;
		m_boxSize.z += nsPlayerConstant::UPPER_BOX_SPEED * m_armLevel;
		m_ghostObject.CreateBox(m_position + (m_playerDir * (nsPlayerConstant::ARM_POSITION_Z + m_upperWave)), Quaternion::Identity, m_boxSize);

		QueryGOs<Enemy>("enemy", [this, damage](Enemy* enemy)->bool {

			PhysicsWorld::GetInstance()->ContactTest(enemy->GetCharaCon(), [&](const btCollisionObject& contactObject) {
				if (m_ghostObject.IsSelf(contactObject) == true) {
					//m_ghostObjectとぶつかった
					enemy->AnyDamage(damage);//ダメージを与える					
					enemy->SetIsRePlay(true);//リアクションを最初から再生させる					
					enemy->SetIsKnockBack(true);//敵は後退する
					enemy->SetPlayerPos(m_position);
				}
				});
			return true;
			});		
		m_ghostObject.Release();
	}
	else if (m_isAttackOn) {
	/*m_boxCollider.Create(m_boxSize);*/
		//キャラクターとゴーストのあたり判定を行う。
		m_boxSize = nsPlayerConstant::UPPER_BOX_SIZE;
		m_ghostObject.CreateBox(m_position+(m_playerDir*50.0f), Quaternion::Identity, m_boxSize);

		QueryGOs<Enemy>("enemy", [this, damage](Enemy* enemy)->bool {
			
			PhysicsWorld::GetInstance()->ContactTest(enemy->GetCharaCon(), [&](const btCollisionObject& contactObject) {
				if (m_ghostObject.IsSelf(contactObject) == true) {
					//m_ghostObjectとぶつかった
					enemy->AnyDamage(damage);//ダメージを与える					
					enemy->SetIsRePlay(true);//リアクションを最初から再生させる
					if (m_comboNum == 3) {
						enemy->SetIsKnockBack(true);//敵は後退する
					}
				}
				});
			return true;
			});
		m_upperWave = 1.0f;
		m_isAttackOn = false;//敵は一定時間無敵(trueのときだけダメージを与える)
		m_ghostObject.Release();		
	}	
}

void Player::PlayerDeath()
{
	m_hp = m_armScale + m_abdominalScale + m_footScale;
	if (m_hp <= 0.0f && !m_skinModelRender->IsPlayingAnimation()//HPが0以下でアニメーションが止まっている
		//かつ、死亡アニメーションかノックバックアニメーションの後であること
		&& (m_playerAnimation->GetAnimationStatus() == m_playerAnimation->GetEnStatusDie()))//
	{		
		m_gameScene->DeathPlayer(m_playerNum);
	}
}
void Player::AnyDamage(float num) {//ダメージを受けると筋肉が減っていく
	m_isDamage = true;
	float damage = num*(nsPlayerConstant::AMOUNT_TAKE_DAMAGE / m_abdominalLevel);
	if (m_armScale > 0)
	{
		m_armScale -= damage;
		std::vector<int>boneId;
		boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:RightArm"));
		boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:LeftArm"));
		boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:RightShoulder"));
		boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:LeftShoulder"));
		boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:Spine2"));
		boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:LeftForeArm"));
		boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:RightForeArm"));
		for (int i = 0; i < boneId.size(); i++) {
			m_muscleRateArray[boneId[i]] = min(1.0f, m_armScale);//補間率を代入			
		}
	}
	if (m_abdominalScale > 0)
	{
		m_abdominalScale -= damage;
		std::vector<int>boneId;
		boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:Hips"));
		boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:Spine"));
		boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:Spine1"));

		for (int i = 0; i < boneId.size(); i++) {
			m_muscleRateArray[boneId[i]] = min(1.0f, m_abdominalScale);//補間率を代入
			//m_skinModelRender->GetSkeleton().GetBone(boneId[i])->SetScale({ scale+1,scale + 1, scale + 1 });
		}
	}
	if (m_footScale > 0)
	{
		m_footScale -= damage;
		std::vector<int>boneId;
		boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:RightUpLeg"));
		boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:RightLeg"));
		boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:RightFoot"));
		boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:LeftUpLeg"));
		boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:LeftLeg"));
		boneId.push_back(m_skinModelRender->GetSkeleton().FindBoneID(L"mixamorig:LeftFoot"));

		for (int i = 0; i < boneId.size(); i++) {
			m_muscleRateArray[boneId[i]] = min(1.0f, m_footScale);//補間率を代入
			//m_skinModelRender->GetSkeleton().GetBone(boneId[i])->SetScale({ scale+1,scale + 1, scale + 1 });
		}
	}		
}
void Player::LevelManagement(float muscleScale, int& muscleLevel, int& oldLevel)
{
	if (muscleScale >= nsPlayerConstant::LEVEL_1_VALUE)
	{
		muscleLevel = 1;
	}
	if (muscleScale >= nsPlayerConstant::LEVEL_2_VALUE)
	{
		muscleLevel = 2;
	}
	if (muscleScale >= nsPlayerConstant::LEVEL_3_VALUE)
	{
		muscleLevel = 3;
	}
	if (muscleScale == nsPlayerConstant::LEVEL_4_VALUE)
	{
		muscleLevel = 4;
	}
	if (oldLevel != muscleLevel)
	{
		m_levelUpOnce = true;
	}
	oldLevel = muscleLevel;
}
}
