#include "stdafx.h"
#include "GameScene.h"
#include "Enemy.h"
#include "HUD.h"
#include "GoldGym.h"
#include "NaviMesh.h"
#include "Cell.h"
#include "BackGround.h"
#include "Enemy.h"
#include "ResultScene.h"
#include "time/stopwatch.h"
#include "Map.h"
#include <random>
namespace nsMuscle {
	/// <summary>
	/// �E�F�[�u�G�l�~�[���|�b�v�����鏈���B
	/// </summary>
	class EnemyWavePopper : public IGameObject {
	private:
		int m_requestNumPop = 0;						// �|�b�v�����鐔�B
		std::function<void(Enemy*)> m_onEnemyPopedFunc;	// �G���|�b�v�����Ƃ��ɌĂяo���֐��B
		int m_gymNum = 0;								// ���_�̈ʒu�B
		BackGround* m_backGround = nullptr;				// �w�i�B		
	public:
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="numPop">�|�b�v������G�̐�</param>
		/// <param name="onEnemyPopedFunc">�G���|�b�v�����Ƃ��ɌĂяo�����֐��B</param>
		void Init(
			int numPop,
			int gymNum,
			BackGround* background,
			std::function<void(Enemy*)> onEnemyPopedFunc)
		{
			m_requestNumPop = numPop;
			m_onEnemyPopedFunc = onEnemyPopedFunc;				
			m_backGround = background;
			m_gymNum = gymNum;
		}
		/// <summary>
		/// 
		/// </summary>
		void Update() override
		{
			if (m_requestNumPop) {
				// ��̂Â|�b�v���Ă����B
				Vector3 randPos = Vector3::Zero;
				switch (m_gymNum)
				{
				case 0:
					randPos.x = rand() % int(nsGameSceneConstant::RANGE_APPEAR_WAVEENEMY_GYM0.x);
					randPos.z = rand() % int(nsGameSceneConstant::RANGE_APPEAR_WAVEENEMY_GYM0.y);
					randPos.x -= nsGameSceneConstant::POSITION_APPEAR_WAVEENEMY_GYM0.x;
					randPos.z -= nsGameSceneConstant::POSITION_APPEAR_WAVEENEMY_GYM0.y;
					break;
				case 1:
					randPos.x = rand() % int(nsGameSceneConstant::RANGE_APPEAR_WAVEENEMY_GYM1.x);
					randPos.z = rand() % int(nsGameSceneConstant::RANGE_APPEAR_WAVEENEMY_GYM1.y);
					randPos.x -= nsGameSceneConstant::POSITION_APPEAR_WAVEENEMY_GYM1.x;
					randPos.z -= nsGameSceneConstant::POSITION_APPEAR_WAVEENEMY_GYM1.y;
					break;
				case 2:
					randPos.x = rand() % int(nsGameSceneConstant::RANGE_APPEAR_WAVEENEMY_GYM2.x);
					randPos.z = rand() % int(nsGameSceneConstant::RANGE_APPEAR_WAVEENEMY_GYM2.y);
					randPos.x -= nsGameSceneConstant::POSITION_APPEAR_WAVEENEMY_GYM2.x;
					randPos.z -= nsGameSceneConstant::POSITION_APPEAR_WAVEENEMY_GYM2.y;
					break;
				default:
					break;
				}
				
				randPos += m_backGround->GetGoldGym(m_gymNum).GetPosition();//�W���̈ʒu�ɒn���Ƃ���œG�͂킭
				const Cell& nearestCell = m_backGround->GetNaviMesh().FindNearestCell(randPos);
				Vector3  enemyPos = nearestCell.GetCenterPosition();
				// �G�̃C���X�^���X�𐶐��B
				Enemy* popEnemy = NewGO<Enemy>(0, "enemy");
				popEnemy->SetPosition(enemyPos);
				popEnemy->SetIsWaveEnemy(true);
				// �G���|�b�v�����Ƃ��ɌĂяo���֐������s����B
				m_onEnemyPopedFunc(popEnemy);
				m_requestNumPop--;
			}
			if (m_requestNumPop == 0) {
				DeleteGO(this);
			}
		}
	};
	GameScene::~GameScene()
	{
		QueryGOs<EnemyWavePopper>("EnemyWavePopper", [&](IGameObject* go) {
			DeleteGO(go);
			return true;
			});
		for (int i = 0; i < PLAYER_MAX_NUM; i++)
		{
			DeleteGO(m_player[i]);
		}
		m_player.clear();
		m_enemy.clear();
		DeleteGO(m_hud);
	}
	bool GameScene::Start()
	{
		m_hud = NewGO<HUD>(0, "hUD");
		m_hud->SetGameScene(*this);
		for (int i = 0; i < PLAYER_MAX_NUM; i++)
		{
			m_player.push_back(NewGO<Player>(0, "player"));
			m_player[i]->SetPlayerNum(i);
			m_player[i]->SetPlayerStatus(Player::EnPlayerStatus::enPlayerStatus_Training);//�g���[�j���O��Ԃɂ���
			//m_player[i]->SetPosition(nsGameSceneConstant::TRAINING_POSITION[i]);
			m_player[i]->SetMoveSpeed(Vector3::Zero);
			m_player[i]->SetIsOnce(true);
			m_player[i]->SetGymLiveNum(i);
		}
		//���x�����\�z����B
		Level3D level3D;
		int playerNum = 0;
		level3D.Init("Assets/levelData/level00.tkl", [&](LevelObjectData& objData) {
			if (objData.EqualObjectName(L"GoldGym") == true) {
				m_player[playerNum]->SetPosition(objData.position);
				playerNum++;
				return true;
			}
			return false;
			});

		//�o�l�J������������
		m_toCameraPos.Set(nsGameSceneConstant::CAMERA_ARM);//�����_���王�_�܂ł̃x�N�g����ݒ�B
		m_springCamera.Init(
			*g_camera3D[0],
			nsGameSceneConstant::MAX_CAMERA_SPEED,
			true,
			nsGameSceneConstant::CAMERA_RADIUS
		);
		m_springCamera.SetDampingRate(nsGameSceneConstant::CAMERA_DAMPINGRATE);
		m_backGround = FindGO<BackGround>("backGround");
		
		m_ssZombieVoice = NewGO<SoundSource>(0);
		m_ssZombieVoice->Init(L"Assets/sound/Zombie_roop.wav", enBGM);
		m_ssZombieVoice->SetIsDeleteGOSelf(false);
		m_ssZombieVoice->SetVolume(nsInGameSoundConstant::AMBIENT_SE_VOL);
		if (IS_BGM) {
			m_ssTitleBGM = NewGO<SoundSource>(0, "titleBGM");
			m_ssTitleBGM->Init(L"Assets/sound/BGM/TitleBGM.wav", enBGM);
			m_ssTitleBGM->SetVolume(nsGameSceneConstant::TITLE_BGM_VOL);
			m_ssTitleBGM->Play(true);
			m_isTitleBGM = true;
		}
		return true;
	}
	void GameScene::Update()
	{
		switch (m_enGameStatus)
		{
		case nsMuscle::GameScene::enGameStatus_Waiting:
			//�^�C�g���V�[��
			for (int i = 0; i < PLAYER_MAX_NUM; i++)
			{
				m_player[i]->GetSkinModelRender()->SetShadowCasterFlag(false);
			}
			break;
		case nsMuscle::GameScene::enGameStatus_Opening:
			Opening();
			break;
		case nsMuscle::GameScene::enGameStatus_Preparation:
			if (m_isTitleBGM == false&&IS_BGM)
			{
				m_ssTitleBGM = NewGO<SoundSource>(0, "titleBGM");
				m_ssTitleBGM->Init(L"Assets/sound/BGM/TitleBGM.wav", enBGM);
				m_ssTitleBGM->SetVolume(nsGameSceneConstant::TITLE_BGM_VOL);
				m_ssTitleBGM->Play(true);
				m_isTitleBGM = true;
			}
			BaseMoveCamera();
			break;
		case GameScene::EnGameStatus::enGameStatus_Confirmation:
			m_springCamera.Refresh();
			m_springCamera.SetPosition(m_advanceCameraPos);
			m_springCamera.SetTarget(m_waveCameraTarget);
			m_hud->ParameterDeactivate(false);		
			m_waveEnemyDeathNum = 0;
			m_redWaveEnemyDeathNum = 0;
			m_isTitleBGM = false;

			m_enemy.clear();//�G�̃f�[�^�����Z�b�g����
			m_map->ClearEnemySprite();//�}�b�v�̓G�̃f�[�^�����Z�b�g����
			m_enemyNum = 0;//
			break;			
		case nsMuscle::GameScene::enGameStatus_Wave1:
			m_ssZombieVoice->Play(true);
			//ZombiBGM();
			InGameBGM();
			PlayerCamera();
			wipeScreenScale();//�E���o�[�������݂Ń��C�v��ʂ̑傫�����ς��
			TakeOver();//�ҋ@���Ă���l�Ɠ���ւ��
			EnemyPop(1);//��莞�ԂœG���N��			
			WaveManagement();
			break;
		case nsMuscle::GameScene::enGameStatus_Wave2:
			m_ssZombieVoice->Play(true);
			//ZombiBGM();
			InGameBGM();
			PlayerCamera();
			wipeScreenScale();//�E���o�[�������݂Ń��C�v��ʂ̑傫�����ς��
			TakeOver();//�ҋ@���Ă���l�Ɠ���ւ��
			EnemyPop(1);//��莞�ԂœG���N��
			WaveManagement();
			break;
		case nsMuscle::GameScene::enGameStatus_Wave3:
			m_ssZombieVoice->Play(true);
			//ZombiBGM();
			InGameBGM();
			PlayerCamera();
			wipeScreenScale();//�E���o�[�������݂Ń��C�v��ʂ̑傫�����ς��
			TakeOver();//�ҋ@���Ă���l�Ɠ���ւ��
			EnemyPop(1);//��莞�ԂœG���N��			
			WaveManagement();
			break;
		case nsMuscle::GameScene::enGameStatus_WaveEnemyPop://��萔�|������W���̋߂��œG���N��
			WaveEnemyPopScene();
			break;
		case nsMuscle::GameScene::enGameStatus_WaveWin:
			m_ssZombieVoice->Stop();
			if (IS_BGM) {
				m_ssInGameBGM->Stop();
			}
			m_player[m_nowWarNum]->SetMoveSpeed(Vector3::Zero);
			break;
		case nsMuscle::GameScene::enGameStatus_Clear:		
			GameEnd(true);
			break;
		case nsMuscle::GameScene::enGameStatus_Lose:
			GameEnd(false);
			break;
		default:
			break;
		}
	}
	void GameScene::PlayerCamera()
	{
		nsMuscle::LightManager::GetInstance()->SetLightCameraPosition(LIGHTCAMERA_POSITION + m_player[m_nowWarNum]->GetPosition());
		nsMuscle::LightManager::GetInstance()->SetLightCameraTarget(LIGHTCAMERA_TARGET + m_player[m_nowWarNum]->GetPosition());
		
		//�J�������X�V�B
		//�����_���v�Z����B
		Vector3 target = m_player[m_nowWarNum]->GetPosition();
		//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
		target.y += 80.0f;
		target += g_camera3D[0]->GetForward() * 20.0f;

		Vector3 toCameraPosOld = m_toCameraPos;
		//�p�b�h�̓��͂��g���ăJ�������񂷁B
		float x = g_pad[0]->GetRStickXF();
		float y = g_pad[0]->GetRStickYF();
		//Y������̉�]
		Quaternion qRot;
		qRot.SetRotationDeg(Vector3::AxisY, nsPlayerConstant::CAMERA_SPEED * x);
		qRot.Apply(m_toCameraPos);
		//X������̉�]�B
		Vector3 axisX;
		axisX.Cross(Vector3::AxisY, m_toCameraPos);
		axisX.Normalize();
		qRot.SetRotationDeg(axisX, nsPlayerConstant::CAMERA_SPEED * y);
		qRot.Apply(m_toCameraPos);
		//�J�����̉�]�̏�����`�F�b�N����B
		//�����_���王�_�܂ł̃x�N�g���𐳋K������B
		//���K������ƁA�x�N�g���̑傫�����P�ɂȂ�B
		//�傫�����P�ɂȂ�Ƃ������Ƃ́A�x�N�g�����狭�����Ȃ��Ȃ�A�����݂̂̏��ƂȂ�Ƃ������ƁB
		Vector3 toPosDir = m_toCameraPos;
		toPosDir.Normalize();
		if (toPosDir.y < -0.5f) {
			//�J����������������B
			m_toCameraPos = toCameraPosOld;
		}
		else if (toPosDir.y > 0.8f) {
			//�J�����������������B
			m_toCameraPos = toCameraPosOld;
		}

		//���_���v�Z����B
		Vector3 pos = target + m_toCameraPos;

		//�o�l�J�����ɒ����_�Ǝ��_��ݒ肷��B
		m_springCamera.SetPosition(pos);
		m_springCamera.SetTarget(target);

		//�J�����̍X�V�B
		m_springCamera.Update();
		m_springCamera.SetDampingRate(1.0f);
	}
	void GameScene::wipeScreenScale()
	{
		if (g_pad[0]->IsTrigger(enButtonRB3)) {
			if (m_isFullWipe) {
				m_isFullWipe = false;
			}
			else
				m_isFullWipe = true;
		}
		if (m_isFullWipe) {//�^�Ȃ�
			if (m_timer <= 10) {
				m_timer++;
				//PostEffectManager::GetInstance()->WipeToFull(m_timer / 10.0f);
			}
		}
		else//�U�Ȃ�
		{
			if (m_timer > 0) {
				m_timer--;
				//PostEffectManager::GetInstance()->WipeToFull(m_timer / 10.0f);
			}
		}
	}
	void GameScene::TakeOver()
	{
		if (g_pad[0]->IsTrigger(enButtonX))//
		{
			//�ҋ@���Ă���l�̈ʒu�����炷
			QueryGOs<GoldGym>("goldGym", [this](GoldGym* goldGym)->bool {
				Vector3 diff = goldGym->GetPosition() - m_player[m_nowWarNum]->GetPosition();
				if (diff.Length() < 500.0f &&
					goldGym->GetIsLivePlayer())//�������߂����A�W���Ƀv���C���[�������
				{
					Effect takeOverEff;
					takeOverEff.Init(u"Assets/effect/TakeOver.efk");
					takeOverEff.Play();
					takeOverEff.SetPosition(m_player[m_nowWarNum]->GetPosition());
					takeOverEff.SetScale({ 10.0f,10.0f,10.0f });
					takeOverEff.Update();

					Vector3 livePlayerpos = m_player[goldGym->GetLivePlayerNum()]->GetPosition();

					//�g���[�j���O��Ԃ���퓬��Ԃɂ���
					m_player[goldGym->GetLivePlayerNum()]->SetPlayerStatus(Player::EnPlayerStatus::enPlayerStatus_War);//�ҋ@���Ă���l��퓬��Ԃɂ���
					m_player[goldGym->GetLivePlayerNum()]->SetPosition(m_player[m_nowWarNum]->GetPosition());
					m_player[goldGym->GetLivePlayerNum()]->GetSkinModelRender()->SetPosition(m_player[m_nowWarNum]->GetPosition());
					m_player[goldGym->GetLivePlayerNum()]->GetSkinModelRender()->SetShadowCasterFlag(true);
					m_player[goldGym->GetLivePlayerNum()]->GetSkinModelRender()->SetIsWipeModel(false, goldGym->GetGoldGymSpawnNum());
					m_player[goldGym->GetLivePlayerNum()]->GetSkinModelRender()->SetIsMainModel(true);
					m_player[goldGym->GetLivePlayerNum()]->SetIsOnce(true);
					
					//�퓬��Ԃ���g���[�j���O��Ԃɂ���
					m_player[m_nowWarNum]->SetPlayerStatus(Player::EnPlayerStatus::enPlayerStatus_Training);
					m_player[m_nowWarNum]->SetPosition(livePlayerpos);
					m_player[m_nowWarNum]->GetSkinModelRender()->SetPosition(livePlayerpos);
					m_player[m_nowWarNum]->GetSkinModelRender()->SetShadowCasterFlag(false);
					m_player[m_nowWarNum]->GetSkinModelRender()->SetIsWipeModel(true, goldGym->GetGoldGymSpawnNum());
					m_player[m_nowWarNum]->GetSkinModelRender()->SetIsMainModel(false);
					m_player[m_nowWarNum]->SetIsOnce(true);
					//��]��������
					Quaternion playerRot;
					playerRot.SetRotationDegY(-90.0f);
					m_player[m_nowWarNum]->GetSkinModelRender()->SetRotation(playerRot);

					int tmp = goldGym->GetLivePlayerNum();
					goldGym->SetLivePlayerNum(m_nowWarNum);//�W���ɓ���v���C���[�̔ԍ�
					m_player[m_nowWarNum]->SetGymLiveNum(goldGym->GetGoldGymSpawnNum());//�v���C���[���ǂ̃W���ɓ��邩�L�^
					m_nowWarNum = tmp;
					TrainingMenuSet(goldGym->GetGoldGymSpawnNum(), goldGym->GetLivePlayerNum());
				}
				return true;
				});
		}
	}
	void GameScene::TrainingMenuSet(int menuNum, int playerNum)
	{
		switch (menuNum)
		{
		case 0:
			m_player[playerNum]->SetTrainingStatus(Player::EnTrainingStatus::enTrainingStatus_PushUp);
			break;
		case 1:
			m_player[playerNum]->SetTrainingStatus(Player::EnTrainingStatus::enTrainingStatus_SitUp);
			break;
		case 2:
			m_player[playerNum]->SetTrainingStatus(Player::EnTrainingStatus::enTrainingStatus_Squat);
			break;
		default:
			m_player[playerNum]->SetTrainingStatus(Player::EnTrainingStatus::enTrainingStatus_Idle);
			break;
		}
	}
	void GameScene::Sally()//����������Ǝ��s
	{
		m_player[m_nowWarNum]->SetPlayerStatus(Player::EnPlayerStatus::enPlayerStatus_War);//�ҋ@���Ă���l��퓬��Ԃɂ���
		m_player[m_nowWarNum]->SetPosition({ 500.0f,0.0f,500.0f });
		m_player[m_nowWarNum]->GetSkinModelRender()->SetPosition({ 500.0f,0.0f,500.0f });
		m_player[m_nowWarNum]->GetSkinModelRender()->SetShadowCasterFlag(true);
		m_player[m_nowWarNum]->GetSkinModelRender()->SetIsMainModel(true);
		for (int i = 0; i < WIPE_MAX_NUM; i++) {
			m_player[m_nowWarNum]->GetSkinModelRender()->SetIsWipeModel(false, i);
		}
		m_player[m_nowWarNum]->SetIsOnce(true);
	}
	void GameScene::EnemyPop(int Num)
	{
		m_enemyPopTimer++;
		if (m_enemyPopTimer > 5&&m_enemyTotalNumber<200)
		{
			m_enemyPopTimer = 0;

			Vector3 randPos = Vector3::Zero;

			for (int i = 0; i < Num; i++) {
				randPos.x = rand() % 5000;
				randPos.z = rand() % 4000;
				randPos.x -= 2500;
				randPos.z -= 2000;
				const Cell& nearestCell = m_backGround->GetNaviMesh().FindNearestCell(randPos);
				Vector3  enemyPos = nearestCell.GetCenterPosition();
				m_enemy.push_back(NewGO<Enemy>(0, "enemy"));
				m_enemy[m_enemyNum]->SetPosition(enemyPos);
				m_enemy[m_enemyNum]->SetEnemyNum(m_enemyNum);
				m_enemyNum++;
			}
			m_enemyTotalNumber += Num;
		}
	}
	void GameScene::EnemyPopStart(int Num)
	{
		Vector3 randPos = Vector3::Zero;

		for (int i = 0; i < Num; i++) {
			randPos.x = rand() % 5000;
			randPos.z = rand() % 4000;
			randPos.x -= 2500;
			randPos.z -= 2000;
			const Cell& nearestCell = m_backGround->GetNaviMesh().FindNearestCell(randPos);
			Vector3  enemyPos = nearestCell.GetCenterPosition();
			m_enemy.push_back(NewGO<Enemy>(0, "enemy"));
			m_enemy[m_enemyNum]->SetPosition(enemyPos);
			m_enemyNum++;
		}
	}
	
	void GameScene::Opening()
	{
		if (m_isOpening) {
			m_isOpening = false;
			for (int i = 0; i < WIPE_MAX_NUM; i++)
			{
				m_backGround->GetGoldGym(i).SetLivePlayerNum(i);
			}
			m_enemy.push_back(NewGO<Enemy>(0, "enemy"));
			m_enemy[m_enemyNum]->SetPosition({ 0.0f,0.0f,-300.0f });
			Quaternion rot = { 0.0f,2.0f,0.0f,-0.2f };
			rot.Normalize();
			m_enemy[m_enemyNum]->SetQrot(rot);			
			Vector3 enemyHeadPos = { 0.0f,100.0f,-300.0f };
			//enemyHeadPos.y += 100.0f;
			m_cameraToEnemy = enemyHeadPos - m_cameraPos + (m_enemy[m_enemyNum]->GetPlayerDir() * 100.0f);				
			m_enemyNum++;
			for (int i = 0; i < WIPE_MAX_NUM; i++)
			{
				for (int j = 0; j < m_playerTotalNumber; j++)
				{
					if (i != j)
					{
						m_player[j]->GetSkinModelRender()->SetIsWipeModel(false, i);
					}
					else
					{
						m_player[j]->GetSkinModelRender()->SetIsWipeModel(true, i);
					}
				}
			}
			m_hud->ParameterBarCreate();
		}
		if (m_cameraGain < 1.0f)//�x�N�g���Ɋ|����l
			m_cameraGain += 0.01f;		
		//if (m_cameraGain > 0.5f)//�J�����������̋����܂ōs���ƃA�j���[�V�������Đ�����
		//{
		//	//m_isWinAnimOn = true;
		//}
		Vector3 uuu = m_cameraToEnemy;
		uuu.y += 150.0f;
		uuu.z -= 500.0f;
		m_cameraPos = (uuu * (pow(m_cameraGain, 1.5f))) - m_cameraToEnemy;//�w���֐��I�ɃJ�������߂Â�

		g_camera3D[0]->SetPosition(m_cameraPos);
		Vector3 targetPos;
		targetPos.Lerp(m_cameraGain, { 0.0f,500.0f,0.0f }, m_enemy[0]->GetPosition());
		g_camera3D[0]->SetTarget(targetPos);

		m_map = FindGO<Map>("map");//�}�b�v�N���X��������
	}

	void GameScene::BaseMoveCamera()
	{				
		if (m_isOpening == false) {
			// �p�X����
			m_cameraPos.y = 0.0f;
			m_pathFiding.Execute(
				m_path,							// �\�z���ꂽ�p�X�̊i�[��
				m_backGround->GetNaviMesh(),		// �i�r���b�V��
				m_cameraPos,						// �J�n���W
				m_backGround->GetGoldGym(m_baseNum).GetPosition(),			// �ړ��ڕW���W
				PhysicsWorld::GetInstance(),	// �����G���W��	
				50.0f,							// AI�G�[�W�F���g�̔��a
				200.0f							// AI�G�[�W�F���g�̍����B
			);
			m_path.SectionArrayPopBack(3);//�Ō�܂ōs���ƃr���ɃJ���������܂�̂Ŕz��̖��[�������B
			m_isOpening = true;
		}
		// �p�X����ړ�����B
		m_cameraPos = m_path.Move(
			m_cameraPos,
			m_pathMoveSpeed,
			m_isEnd
		);
		Vector3 Road = m_cameraPos;
		Road.y = 300.0f;
		Vector3 target = m_backGround->GetGoldGym(m_baseNum).GetPosition();
		target.y += 300.0f;
		g_camera3D[0]->SetPosition(Road);
		g_camera3D[0]->SetTarget(target);//�p�X����ړ��������擾

		if (m_isEnd == true)//�o�H�T�����I���A�؃g�����j���[��I�����I����
		{
			m_hud->ParameterDeactivate(true);
			if (m_hud->GetIsMenuSelect()) {//�g���[�j���O���j���[���I����������
				if (m_baseNum < m_backGround->GetGoldGymTotalNum()) {//�Ō�̃r���łȂ����
					m_baseNum++;
					while (m_isBreakGym[m_baseNum])
					{
						m_baseNum++;
					}
					m_isOpening = false;
					m_isEnd = false;//�o�H�T���J�n����
				}
					
				m_hud->SetIsMenuSelect(false);//���j���[�t���O�����낷
			}
			if (m_isOpening == true) {//�I�[�v�j���O��(���j���[�I�����͍s���Ȃ�)
				m_hud->SetPreparationStatus(HUD::EnPreparationStatus::enPreparationStatus_MenuSelect);
			}
		}
		else //�o�H�T����
		{
			m_hud->ParameterDeactivate(false);
			if (g_pad[0]->IsPress(enButtonA))//
			{
				m_pathMoveSpeed = 200.0f;
			}
			else
			{
				m_pathMoveSpeed = 100.0f;
			}
		}
	}
	void GameScene::WaveEnemyPop(int enemyNum,int gymNum)
	{		
		//��񂾂����s
		if (m_isWaveFlag)
		{			
			m_isWaveFlag = false;
			m_setupTimer = 0.0f;
			// �E�F�[�u�G�l�~�[���|�b�v�����鏈���̃C���X�^���X���쐬�B
			EnemyWavePopper* enemyWavePopper = NewGO< EnemyWavePopper >(0, "EnemyWavePopper");
			enemyWavePopper->Init(
				enemyNum,		// �|�b�v�����鐔�B
				gymNum,			//�W���̔ԍ�
				m_backGround,	// �w�i�N���X�̃C���X�^���X�B�����Ŏg���̂ŁB
				[&](Enemy* popedEmey) {	// �|�b�v�����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
					//m_enemy.push_back(popedEmey);
					//m_enemyNum++;
				}
			);
			m_enemyTotalNumber += enemyNum;
		}		
	}
	void GameScene::KillCount()
	{
	
	}
	void GameScene::WaveManagement()
	{
		if (m_isSecondaryStage == true)//�E�F�[�u�G�l�~�[���N��������
		{
			if (m_redWaveEnemyDeathNum >= nsGameSceneConstant::WAVEENEMY_POP_NUM[m_currentWaveNum])//���̃E�F�[�u�ł̖ڕW�������ɒB������
			{
				m_player[m_nowWarNum]->SetMoveSpeed(Vector3::Zero);
				m_delayTimer++;
				if (m_delayTimer > 30)
				{					
					m_isSecondaryStage = false;
					m_delayTimer = 0;
					m_enGameStatus = enGameStatus_WaveWin;
					m_hud->SetPreparationStatus(HUD::EnPreparationStatus::enPreparationStatus_MoveCamera);
					m_enemyDeathNum += m_waveEnemyDeathNum;
					m_waveEnemyDeathNum = 0;
					m_redWaveEnemyDeathNum = 0;
					m_isOpening = false;
					m_currentWaveNum++;//�E�F�[�u���𑫂�
					m_baseNum = 0;//�J�����𓮂������߂̋��_�ԍ�
					while (m_isBreakGym[m_baseNum])
					{
						m_baseNum++;
					}
					m_isEnd = false;//�o�H�T���̃t���O��������
					m_hud->SetIsMenuSelect(false);//���j���[�I��p�̃t���O��������
					for (int i = 0; i < 3; i++)
					{
						//���Ă��炸�A���v���C���[�����Ȃ��W��
						if (!m_backGround->GetGoldGym(i).GetIsBreak() && !m_backGround->GetGoldGym(i).GetIsLivePlayer())
						{
							m_player[m_nowWarNum]->SetPosition(m_backGround->GetGoldGym(i).GetPosition());
							m_player[m_nowWarNum]->GetSkinModelRender()->SetPosition(m_backGround->GetGoldGym(i).GetPosition());
							m_player[m_nowWarNum]->SetPlayerStatus(Player::EnPlayerStatus::enPlayerStatus_Training);//�g���[�j���O��Ԃɂ���
							m_player[m_nowWarNum]->SetTrainingStatus(Player::EnTrainingStatus::enTrainingStatus_Idle);
							m_player[m_nowWarNum]->SetIsOnce(true);
							break;
						}
					}
					//��]��������
					Quaternion playerRot;
					playerRot.SetRotationDegY(-90.0f);
					m_player[m_nowWarNum]->GetSkinModelRender()->SetRotation(playerRot);
					m_isWaveFlag = true;
					if (m_currentWaveNum == 3)
					{
						m_enGameStatus = enGameStatus_Clear;//�Ō�̃E�F�[�u�܂ōs������N���A
					}
				}
			}
		}
		else//�E�F�[�u�G�l�~�[�������O
		{
			if (m_waveEnemyDeathNum >= nsGameSceneConstant::WAVE_POP_NUM[m_currentWaveNum])//���̃E�F�[�u�ł̖ڕW�������ɒB������
			{
				m_delayTimer++;
				if (m_delayTimer > 30) {
					m_enGameStatus = enGameStatus_WaveEnemyPop;//�����t�F�[�Y�ɂ���
					if (m_hud != nullptr) {
						m_hud->SetIsKillNumUpdate();
					}								
					m_isSecondaryStage = true;
					//���O�Ƀo�l�J�����̏����R�s�[���Ă���
					m_isWavePop = true;
					m_advanceCameraPos = m_springCamera.GetPosition();
					m_advanceCameraTarget = m_springCamera.GetTarget();
					m_delayTimer = 0;
				}
			}
		}
	}
	void GameScene::DeathPlayer(int num)
	{
		SetIsDeathPlayer(num);//�v���C���[�����񂾂��Ƃ�`����
		DeleteGO(m_player[num]);//�Ō�̃f�[�^���폜		
		m_playerTotalNumber--;//�v���C���[�̐������炷
		//for (int i = 0; i < MAX_PLAYER_NUM ; i++)
		//{
		//	if (m_isDeathPlayer[i]==false)//����ł��Ȃ��v���C���[��T��
		//	{
		//		m_nowWarNum = i;//�܂�����ł��Ȃ��v���C���[������ΐ퓬��Ԃɂ���
		//		Sally();
		//		return;
		//	}
		//}		
		//if (m_playerTotalNumber == 0)//�v���C���[���N�����Ȃ����
		//{
		if (m_nowWarNum == num) {
			m_enGameStatus = GameScene::enGameStatus_Lose;//����
		}
		//}
	}
	void GameScene::WaveEnemyPopScene()
	{
		if (m_isWavePop)
		{
			m_isSecondaryStage = true;
			m_isWavePop = false;
			m_randGymNum = rand() % 3;
			WaveEnemyPop(nsGameSceneConstant::WAVEENEMY_POP_NUM[m_currentWaveNum], m_randGymNum);
			//�o�l�J�����ɒ����_�Ǝ��_��ݒ肷��B
			m_player[m_nowWarNum]->GetPosition();
			m_waveCameraPos = m_player[m_nowWarNum]->GetPosition();
			m_waveCameraPos.y += 3000.0f;
			m_waveCameraTarget = m_backGround->GetGoldGym(m_randGymNum).GetPosition();
			g_camera3D[0]->SetPosition(m_waveCameraPos);
			g_camera3D[0]->SetTarget(m_waveCameraTarget);
			//�J�����̍X�V�B
			m_springCamera.Update();
			m_waveCameraMoveTimer=0;
		}
		else
		{
			m_hud->WaveDisplay(m_randGymNum);
			m_waveCameraMoveTimer++;
			if (m_waveCameraMoveTimer >= 5)
			{
				Vector3 sky = { 0.0f,3000.0f,0.0f };
				Vector3 diff = m_backGround->GetGoldGym(m_randGymNum).GetPosition() - m_waveCameraPos;
				if (diff.Length() > 1500.0f) {
					diff.Normalize();
					m_waveCameraPos += diff * 120.0f;
				}
				g_camera3D[0]->SetPosition(m_waveCameraPos);
				m_springCamera.Refresh();
				if (g_pad[0]->IsPress(enButtonA))
				{
					switch (m_currentWaveNum)
					{
					case 0:
						m_enGameStatus = enGameStatus_Wave1;
						break;
					case 1:
						m_enGameStatus = enGameStatus_Wave2;
						break;
					case 2:
						m_enGameStatus = enGameStatus_Wave3;
						break;
					default:
						break;
					}
					m_waveEnemyDeathNum = 0;
					m_redWaveEnemyDeathNum = 0;
					m_waveCameraPos = m_advanceCameraPos;
					m_waveCameraTarget = m_advanceCameraTarget;
					m_springCamera.SetPosition(m_advanceCameraPos);
					m_springCamera.SetTarget(m_waveCameraTarget);
					//m_springCamera.SetDampingRate(0.0001f);
				}
			}
			g_camera3D[0]->SetTarget(m_waveCameraTarget);
			g_camera3D[0]->SetPosition(m_waveCameraPos);
			g_camera3D[0]->Update();			
		}
	}
	void GameScene::ZombiBGM()
	{
		if (m_isZombieBGMCreate == false)
		{
			m_isZombieBGMCreate = true;
			m_ssZombieBGM = NewGO<SoundSource>(0);
			m_ssZombieBGM->SetIsDeleteGOSelf(true);
			int randam = rand() % 4;
			std::mt19937 mt;            // �����Z���k�E�c�C�X�^��32�r�b�g��
			std::random_device rnd;     // �񌈒�I�ȗ���������
			std::uniform_int_distribution<> rand4(0, 3);        // [0, 99] �͈͂̈�l����

			switch (rand4(mt))
			{
			case 0:
				m_ssZombieBGM->Init(L"Assets/sound/BGM/voice0.wav", enSE);
				break;
			case 1:
				m_ssZombieBGM->Init(L"Assets/sound/BGM/voice1.wav", enSE);
				break;
			case 2:
				m_ssZombieBGM->Init(L"Assets/sound/BGM/voice2.wav", enSE);
				break;
			case 3:
				m_ssZombieBGM->Init(L"Assets/sound/BGM/voice3.wav", enSE);
				break;
			default:
				break;
			}
			m_ssZombieBGM->SetVolume(0.1f);
		}		
		
		if (m_isZombieBGMCreate == true) {
			m_ssZombieBGM->Play(false);
		}
		if (m_isZombieBGMCreate == true&&m_ssZombieBGM->IsPlaying() == false)
		{
			m_isZombieBGMCreate = false;
			m_ssZombieBGM->Release();
		}
	}
	void GameScene::InGameBGM()
	{
		if (m_isInGameBGMCreate == false&&IS_BGM)
		{
			m_isInGameBGMCreate = true;
			m_ssInGameBGM = NewGO<SoundSource>(0);
			m_ssInGameBGM->Init(L"Assets/sound/BGM/InGameBGM.wav", enSE);
			m_ssInGameBGM->SetVolume(nsGameSceneConstant::INGAME_BGM_VOL);
		}
		if (m_isInGameBGMCreate == true) {
			m_ssInGameBGM->Play(false);
		}
		if (m_isInGameBGMCreate == true && m_ssInGameBGM->IsPlaying() == false)
		{
			m_isInGameBGMCreate = false;
			m_ssInGameBGM->Release();
		}
	}
	void GameScene::MinusPlayerTotalNumber()
	{ 
		m_playerTotalNumber--;
	}
	void GameScene::EnemySpriteDataReset()
	{
		m_enemy.clear();//�G�̃f�[�^�����Z�b�g����
		m_map->ClearEnemySprite();//�}�b�v�̓G�̃f�[�^�����Z�b�g����
		m_enemyNum = 0;//
	}
	void GameScene::GameEnd(bool flag)
	{
		if (m_isGameEnd == true&& m_isResultCreate==false)//1�t���[���x��Ă���HUD�N���X������
		{
			DeleteGO(m_hud);
			ResultScene*resultScene=NewGO<ResultScene>(0, "resultScene");
			resultScene->SetIsClear(flag);
			m_isResultCreate = true;//�����NewGO�����Ȃ�
		}
		if (m_isGameEnd==false) {
			PostEffectManager::GetInstance()->SetIsWipeRender(false);//�ǂ�����false�ɂ��Ă�����
			GameObjectManager::GetInstance()->SetWipeScreenMode(false);//�Ȃ��2��ލ�����̂�
			m_isGameEnd = true;
			DeleteGO(m_ssInGameBGM);
			DeleteGO(m_ssZombieBGM);
			DeleteGO(m_ssZombieVoice);
		}
	}
}