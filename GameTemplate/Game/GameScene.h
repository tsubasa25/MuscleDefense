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
			enGameStatus_Waiting,//�ҋ@�A�������Ȃ�
			enGameStatus_Opening,//�Q�[���J�n�O�̏���
			enGameStatus_Preparation,//����
			enGameStatus_Confirmation,//�g���[�j���O���j���[�m�F
			enGameStatus_Wave1,//�E�F�[�u�P�A�G������
			enGameStatus_Wave2,
			enGameStatus_Wave3,
			//enGameStatus_Wave4,
			enGameStatus_WaveEnemyPop,//��萔�|������W���̋߂��œG���N��
			enGameStatus_WaveWin,
			enGameStatus_Clear,//�Q�[���N���A
			enGameStatus_Lose
		};
		
	private:
		EnGameStatus m_enGameStatus = EnGameStatus::enGameStatus_Waiting;
	public:
		~GameScene();
		bool Start();
		void Update();
		/// <summary>
		/// �v���C���[�ɂ��Ă����J�����̈ړ�����
		/// </summary>
		void PlayerCamera();
		/// <summary>
		/// ���C�v��ʂ�傫������
		/// </summary>
		/// <returns></returns>
		bool GetIsWipeOn() { return m_isFullWipe; }
		/// <summary>
		/// �Q�[���̏�Ԃ��擾
		/// </summary>
		/// <returns></returns>
		int GetEnGameStatus() { return m_enGameStatus; }
		/// <summary>
		/// �Q�[���̏�Ԃ�ݒ�
		/// </summary>
		/// <param name="status"></param>
		void SetEnGameStatus(EnGameStatus status) { m_enGameStatus = status; }
		/// <summary>
		/// ���C�v��ʂ̑傫����ݒ�
		/// </summary>
		void wipeScreenScale();
		/// <summary>
		/// �g���[�j���O���̃v���C���[�Ɠ���ւ��
		/// </summary>
		void TakeOver();
		/// <summary>
		/// �E�F�[�u�G�l�~�[���N��
		/// </summary>
		void WaveEnemyPopScene();
		/// <summary>
		/// �]���r�̐���BGM���Đ�
		/// </summary>
		void ZombiBGM();
		/// <summary>
		/// BGM���Đ�
		/// </summary>
		void InGameBGM();
		/// <summary>
		/// �g���[�j���O���j���[���Z�b�g����
		/// </summary>
		/// <param name="menuNum">���j���[�ԍ�[0]��[1]��[2]��</param>
		/// <param name="playerNum">�v���C���[�ԍ�</param>
		void TrainingMenuSet(int menuNum, int playerNum);
		/// <summary>
		/// �g���[�j���O���̐l�����擾
		/// </summary>
		/// <returns></returns>
		int GetTrainingMemberNum() { return m_trainingMemberNum; }
		/// <summary>
		/// �g���[�j���O��Ԃ���퓬��Ԃɂ���
		/// </summary>
		void Sally();
		/// <summary>
		///	�v���C���[�����񂾎�
		/// </summary>
		/// <param name="num"></param>
		void DeathPlayer(int num);
		/// <summary>
		/// ��莞�ԂœG���o��
		/// </summary>
		/// <param name="enemyNum">��x�ɗN����</param>
		void EnemyPop(int enemyNum);
		/// <summary>
		///	�ŏ��ɓG�𕡐��N������
		/// </summary>
		/// <param name="enemyNum">�G�̐�</param>
		void EnemyPopStart(int enemyNum);
		/// <summary>
		/// �G�̑��������Z
		/// </summary>
		/// <param name="num"></param>
		void SetEnemyTotalNumber(int num) { m_enemyTotalNumber += num; }
		/// <summary>
		/// �G�̑������擾
		/// </summary>
		/// <returns></returns>
		int GetEnemyTotalNumber() { return m_enemyTotalNumber; }
		/// <summary>
		/// �v���C���[�����擾
		/// </summary>
		/// <param name="num">�v���C���[�ԍ�</param>
		/// <returns>�v���C���[���</returns>
		Player& GetPlayer(int num) { return *m_player[num]; }
		/// <summary>
		/// �퓬��Ԃ̃v���C���[�����擾
		/// </summary>
		/// <returns>�퓬���̃v���C���[</returns>
		Player& GetNowWarPlayer() { return *m_player[m_nowWarNum]; }
		/// <summary>
		/// �퓬���̃v���C���[�̈ʒu���擾
		/// </summary>
		/// <returns>�v���C���[�̈ʒu</returns>
		Vector3 GetTargetPosition() { return GetPlayer(m_nowWarNum).GetPosition(); }
		/// <summary>
		/// �퓬���̃v���C���[�̔ԍ����擾
		/// </summary>
		/// <returns></returns>
		int GetNowWarNum() { return m_nowWarNum; }
		/// <summary>
		/// �I�[�v�j���O����
		/// </summary>
		void Opening();
		/// <summary>
		/// �X�v�����O�J�����̈ʒu��ݒ�
		/// </summary>
		/// <param name="pos"></param>
		void SetCameraPos(Vector3 pos) { m_cameraPos = pos; }
		/// <summary>
		/// �o�H�T���ŋ��_�Ɍ������ē����J�����̏���
		/// </summary>
		/// <param name="num">���_�̔ԍ�</param>
		void BaseMoveCamera();

		void WaveEnemyPop(int enemyNum,int gymNum);//���_�̋߂��Ƀ����_���ɑ�ʃ|�b�v����
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
		/// �W������ꂽ��true
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

		bool m_isFullWipe = false;//���C�v���S��ʂ��ۂ�
		float m_timer = 0.0f;//���C�v�̑傫�������X�ɕς�邽�߂̕ϐ�
		
		Vector3 m_gymPlayerPos;//�g���[�j���O���̃v���C���[�̃|�W�V����

		int m_playerTotalNumber = nsGameSceneConstant::MAX_PLAYER_NUM;
		int m_nowWarNum = 3;//���ݐ���Ă���v���C���[�̔ԍ�
		int m_nextChangeNum = 0;//����シ��v���C���[�̔ԍ�
		int m_trainingMemberNum = 3;//�g���[�j���O���̃v���C���[�̐�

		SpringCamera m_springCamera;//�v���C���[����J�����ɐL�т�A�[��
		Vector3 m_toCameraPos = Vector3::One;//
		Vector3 m_cameraPos = Vector3::Zero;//�J�����̈ʒu
		Vector3 m_cameraToEnemy=Vector3::Zero;//�J��������I�Ɍ������x�N�g��
		int m_enemyTotalNumber = 0;//�G�̑���

		int m_enemyNum = 0;//�G�̐�
		int m_enemyNumMap = 0;//�}�b�v�Ŏg��
		int m_enemyPopTimer = 0;//�G���N���Ԋu

		bool m_isOpening = true;//

		float m_cameraGain = 0.0f;//�J�����ړ��X�s�[�h�̌W��
		//�o�H�T��
		Path m_path;
		PathFinding m_pathFiding;

		int m_baseNum = 0;//���_�̔ԍ�
		
		bool m_isEnd = false;//�o�H�T���t���O
		bool m_isWaveFlag = true;
		float m_pathMoveSpeed = 10.0f;//�o�H�T���̈ړ��X�s�[�h

		int m_enemyDeathNum = 0;//�G�����񂾒l
		int m_waveEnemyDeathNum = 0;//���ꂼ��̃E�F�[�u�œG�����񂾐�
		int m_redWaveEnemyDeathNum = 0;//�E�F�[�u�G�l�~�[�����񂾐�

		int m_currentWaveNum = 0;//���݂̃E�F�[�u�ԍ�

		bool m_isBreakGym[4] = { false, false, false, false };
		bool m_isDeathPlayer[4] = { false,  false,  false,  false};

		float m_setupTimer = 0.0f;

		bool m_isWavePop = false;
		int m_randGymNum = 0;//�����_���ȃW���ԍ�
		int m_waveCameraMoveTimer = 0;
		Vector3 m_waveCameraPos = Vector3::Zero;
		Vector3 m_waveCameraTarget = Vector3::Zero;
		bool m_isSecondaryStage = false;//�G��K�v�l���|��������true
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
