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
		enum EnPlayerStatus//�v���C���[�̏�Ԃ�\��
		{
			enPlayerStatus_War,
			enPlayerStatus_Training
		};
		enum EnTrainingStatus//�g���[�j���O���
		{
			enTrainingStatus_Idle,//�ҋ@
			enTrainingStatus_PushUp,//�r���ĕ���
			enTrainingStatus_SitUp,//��̋N����
			enTrainingStatus_Squat,//�X�N���b�g
			enTrainingStatus_LevelUp//���x���A�b�v
		};
	private:
		EnPlayerStatus m_playerStatus = enPlayerStatus_War;
		EnTrainingStatus m_trainingStatus = enTrainingStatus_Idle;
	public:
		~Player();
		bool Start();
		void Update();
		/// <summary>
		/// �r���ĕ���
		/// </summary>
		void PushUp();
		/// <summary>
		/// ��̋N����
		/// </summary>
		void SitUp();
		/// <summary>
		/// �X�N���b�g
		/// </summary>
		void Squat();
		/// <summary>
		/// �ړ�����
		/// </summary>
		void PlayerMove();
		/// <summary>
		/// ��]����
		/// </summary>
		void Turn();
		/// <summary>
		/// ���񂾎�
		/// </summary>
		void PlayerDeath();
		/// <summary>
		/// �U������
		/// </summary>
		void Attack();
		void JumpAttack();//�W�����v�A�^�b�N����
		/// <summary>
		/// �I�Ƀ_���[�W��^����
		/// </summary>
		/// <param name="damage">�_���[�W��</param>
		void EnemyAnyDamage(float damage);
		/// <summary>
		/// �_���[�W���󂯂�
		/// </summary>
		/// <param name="num">�_���[�W��</param>
		void AnyDamage(float num);
		/// <summary>
		/// HP���擾
		/// </summary>
		/// <returns>HP</returns>
		float GetHP() { return m_hp; }
		/// <summary>
		/// �v���C���[�ԍ���ݒ�
		/// </summary>
		/// <param name="num">�v���C���[�ԍ�</param>
		void SetPlayerNum(int num) { m_playerNum = num; }
		/// <summary>
		/// �v���C���[�ԍ����擾
		/// </summary>
		/// <returns>�v���C���[�ԍ�</returns>
		int GetPlayerNum() { return m_playerNum; }

		SkinModelRender* GetSkinModelRender() { return m_skinModelRender; }
		/// <summary>
		/// �X�s�[�h���擾
		/// </summary>
		/// <returns></returns>
		Vector3 GetMoveSpeed() { return m_moveSpeed; }
		/// <summary>
		/// �ʒu���擾
		/// </summary>
		/// <returns></returns>
		Vector3 GetPosition() { return m_position; }
		/// <summary>
		/// �O�����̃x�N�g�����擾
		/// </summary>
		/// <returns></returns>
		Vector3 GetPlayerDir() { return m_playerDir; }
		/// <summary>
		/// �v���C���[�̏�Ԃ��擾
		/// </summary>
		/// <returns></returns>
		EnPlayerStatus GetPlayerStatus() { return m_playerStatus; }
		/// <summary>
		/// �v���C���[�̃g���[�j���O��Ԃ��擾
		/// </summary>
		/// <returns></returns>
		EnTrainingStatus GetTrainingStatus() { return m_trainingStatus; }
		/// <summary>
		/// �v���C���[�̃g���[�j���O��Ԃ�ݒ�
		/// </summary>
		/// <param name="status"></param>
		void SetTrainingStatus(EnTrainingStatus status) { m_trainingStatus = status; }
		/// <summary>
		/// �ʒu��ݒ�
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(Vector3 pos)
		{
			m_position = pos;
			m_charaCon.SetPosition(pos);			
		}
		/// <summary>
		/// �v���C���[�̏�Ԃ��擾
		/// </summary>
		void SetPlayerStatus(EnPlayerStatus playerStatus) { m_playerStatus = playerStatus; }
		/// <summary>
		/// �v���C���[�̃X�s�[�h��ݒ�
		/// </summary>
		/// <param name="speed"></param>
		void SetMoveSpeed(Vector3 speed) { m_moveSpeed = speed; }
		/// <summary>
		/// ���݂̃R���{�����擾
		/// </summary>
		/// <returns></returns>
		const int& GetComboNum() const { return m_comboNum; }
		/// <summary>
		/// �R���{�����O�ɂ���
		/// </summary>
		void ReSetComboNum() { m_comboNum = 0; }
		/// <summary>
		/// �I�Ƀ_���[�W��^�����悤�ɂ���
		/// </summary>
		void ReSetIsAttackOn() { m_isAttackOn = true; }

		/// <summary>
		/// �g���[�j���O��ԂƐ퓬��Ԃ�؂�ւ���Ƃ��ɕK�v
		/// </summary>
		/// <param name="flag"></param>
		void SetIsOnce(bool flag) {	m_isOnce = flag;}
		bool GetIsOnce() { return m_isOnce; }
		/// <summary>
		/// �A�j���[�V�������ŏ�����Đ��ł���
		/// </summary>
		/// <param name="flag"></param>
		void SetIsRePlay(bool flag) { m_isRePlay = flag; }
		bool GetIsRePlay() { return m_isRePlay; }
		/// <summary>
		/// �_���[�W��H����Ă���
		/// </summary>
		void SetIsDamage(bool flag) { m_isDamage = flag; }
		bool GetIsDamage() { return m_isDamage; }
		/// <summary>
		/// ���񂾂�
		/// </summary>
		void SetIsDeath(bool flag) { m_isDeath = flag; }
		bool GetIsDeath() { return m_isDeath; }
		/// <summary>
		/// �{�[�����Ƃ̒��_���[�t�̉e������ݒ�
		/// </summary>
		/// <param name="bone">�{�[��ID</param>
		/// <param name="num">�e����</param>
		void SetMuscleRateArray(int bone, float num) { m_muscleRateArray[bone] = num; }
		/// <summary>
		/// ���������鋒�_�̔ԍ�
		/// </summary>
		/// <param name="num"></param>
		void SetGymLiveNum(int num) { m_gymLiveNum = num; }
		/// <summary>
		/// ���x���A�b�v�̎�TRUE
		/// </summary>
		/// <param name="flag"></param>
		void SetLevelUpOnce(bool flag) { m_levelUpOnce = flag; }
		bool GetLevelUpOnce() { return m_levelUpOnce; }
		/// <summary>
		/// �ؓ��ʂɉ����ă��x����ύX����
		/// </summary>
		/// <param name="muscleScale">�ؓ���</param>
		/// <param name="muscleLevel">�ؓ����x��</param>
		/// <param name="oldLevel">�O�t���[���̋ؓ����x��</param>
		void LevelManagement(float muscleScale,int &muscleLevel,int &oldLevel);

		int GetAbdominalLevel() { return m_abdominalLevel; }

		PlayerAnimation& GetPlayerAnimation() { return *m_playerAnimation; }

		void SetPlayerRot(Quaternion rot) { m_playerRot = rot; }
	private:
		GameScene* m_gameScene = nullptr;
		HUD* m_hud = nullptr;
		float m_hp = 0.0f;					//�̗�
		bool m_isDamage = false;			//�_���[�W��H����Ă��邩
		int m_playerNum = 0;				//�v���C���[�ԍ�
		bool m_isRePlay = false;			//�A�j���[�V�������ŏ�����Đ�������

		SkinModelRender* m_skinModelRender = nullptr;//�v���C���[�̃X�L��
		Vector3 m_moveSpeed = Vector3::Zero;//�X�s�[�h
		Vector3 m_scale = { 1.5f,1.5f, 1.5f };//�g�嗦
		Vector3 m_position = Vector3::Zero;	//�v���C���[�̈ʒu
		CharacterController m_charaCon;		//�v���C���[�̕����J�v�Z��

		PlayerAnimation* m_playerAnimation = nullptr;//�A�j���[�V�����N���X
		PlayerVoice* m_playerVoice = nullptr;//�T�E���h�N���X

		Vector3 m_playerDir = { 0.f,0.f,-1.0f };//�v���C���[�������Ă������
		Quaternion m_playerRot = Quaternion::Identity;//�v���C���[�̉�]
		Quaternion m_RotY= Quaternion::Identity;//�v���C���[�̉�]
		Quaternion m_cameraRot = Quaternion::Identity;//�J�����̉�]

		int m_loopCount = 0;				//���[�v��

		float m_armScale = 0.5f;			//�r�̕�ԗ�
		float m_abdominalScale = 0.5f;		//���̕�ԗ�
		float m_footScale = 0.5f;			//���̕�ԗ�

		float m_punchDamage = 0.0f;			//�p���`�́A�ؓ����x���ɂ���ĕς��

		float m_velocity = 0.f;				//L�X�e�B�b�N���͂Ɋ|����{��

		/// <summary>
		/// �U���Ɋւ���ϐ�
		/// </summary>
		int m_comboNum = 0;					//�R���{��
		int m_jumpAttackLoop = 0;			//�W�����v�A�^�b�N���̃��[�v��
		bool m_isAttackOn = true;			//�_���[�W��^����邩�̃t���O
		float* m_muscleRateArray = nullptr;	// �ؓ��ʂ̔z��B
											// 0.0���ł��n��B1.0�ōő�̋ؓ���
											// �z��̗v�f���̓X�P���g���̍��̐��Ɠ����B
											// ���т��X�P���g���Ɠ����B
		StructuredBuffer m_muscleRateArraySB;	// �ؓ��ʂ̔z��̃X�g���N�`���[�h�o�b�t�@�B

		bool m_isOnce = true;				//�g���[�j���O��ԂƐ퓬��Ԃ�؂�ւ���Ƃ���TRUE

		PhysicsGhostObject m_ghostObject;	//�p���`�����Ƃ��ɂł铖���蔻��
		
		Vector3 m_boxSize = { Vector3::Zero };//�����蔻��̑傫��
		bool m_isDeath = false;				//���񂾂�

		float m_punchSpeed = 0.0f;			//�p���`�����Ƃ��ɑO�ɂ����X�s�[�h
		float m_upperWave = 1.0f;			//�Ռ��g�̑��x

		int m_gymLiveNum = 0;				//�����鋒�_�̔ԍ�

		int m_armLevel = 2;					//�r�̋ؓ����x��
		int m_abdominalLevel = 2;			//���̋ؓ����x��
		int m_footLevel = 2;				//���̋ؓ����x��
		int m_oldArmLevel = 2;				//�O�t���[���̘r�̋ؓ����x��
		int m_oldAbdominalLevel = 2;		//�O�t���[���̕��̋ؓ����x��
		int m_oldFootLevel = 2;				//�O�t���[���̑��̋ؓ����x��
		bool m_levelUpOnce = false;			//���x���A�b�v������TRUE
	};
}
