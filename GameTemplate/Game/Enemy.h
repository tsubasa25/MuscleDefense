#pragma once

#include "Path.h"
#include "PathFinding.h"
namespace nsMuscle {
	namespace nsEnemyConstant {
		const Vector3 INI_POSITION = { 0.0f,0.0f,-500.0f };
		const int ATTACK_COOLTIME = 60;
		const int PLATER_ATTACK_LENGE = 80;
		const float ENEMY_ATTACK_DAMAGE = 3.0f;
	}
	class EnemyAnimation;
	class BackGround;
	class GameScene;
	class EnemyVoice;
	class Map;

	class Enemy :public IGameObject
	{
	public:
		enum EnEnemyStatus {
			enEnemyStatus_Idle,
			enEnemyStatus_War,
			enEnemyStatus_Move,
			enEnemyStatus_Damage
		};
	private:
		EnEnemyStatus m_enemyStatus = enEnemyStatus_Move;
	public:
		~Enemy();
		bool Start();
		void Update();
		void EnemyMove();//�ړ�����
		void EnemyAttack();//�U������
		void ProteinTreatment();//�G���疡���ɂȂ�
		void KnockBack();//�m�b�N�o�b�N����
		void EnemyMoveToPlayer();
		void EnemyMoveToGym();
		void notDisplay();//��ʊO�̓G�͕\�����Ȃ�

		void SetEnemyNum(int num) { m_enemyNum = num; }//�v���C���[�ԍ���ݒ肷��
		SkinModelRender* GetSkinModelRender() { return m_skinModelRender; }
		Vector3 GetMoveSpeed() { return m_moveSpeed; }
		Vector3 GetPosition() { return m_position; }
		void SetPosition(Vector3 pos) { m_position = pos; }
		Vector3 GetPlayerDir() { return m_playerDir; }
		void SetQrot(Quaternion rot) { m_qrot = rot; }
		float GetHP() { return m_hp; }
		void AnyDamage(float num) { m_hp -= num; m_isDamage = true; }
		void SetIsDamage(bool flag) { m_isDamage = flag; }
		bool GetIsDamage() { return m_isDamage; }
		//bool GetIsAttack() { return m_coolTime < 0; }
		void ReSetCoolTime() { m_coolTime = nsEnemyConstant::ATTACK_COOLTIME; }
		bool GetIsAttack() { return m_isAttack; }
		void SetIsAttack(bool flag) { m_isAttack=flag; }

		void AddProteinMeter(int num) { m_proteinMeter += num; }
		int GetStatus() { return m_enemyStatus; }//��Ԃ��擾
		void SetStatus(EnEnemyStatus status) { m_enemyStatus = status; }//��Ԃ�ݒ�
		void SetIsKnockBack(bool flag) { m_isKnockBack = flag; }
		bool GetIsKnockBack() { return m_isKnockBack; }
		void SetPlayerPos(Vector3 pos) { m_playerPos = pos; }
		CharacterController& GetCharaCon() {return m_charaCon; }
		//RigidBody& GetRigidBody() { return m_rigidBody; }
		void SetIsRePlay(bool flag) { m_isRePlay = flag; }//�A�j���[�V�������ŏ�����Đ�
		bool GetIsRePlay() { return m_isRePlay; }
		void PositionUpdate();
		void PlayerAnyDamage(float damage);
		void GymAnyDamage(float damage);

		void SetIsPlayerDamage(bool flag) { m_isPlayerDamage = flag; }
		bool GetIsPlayerDamage() { return m_isPlayerDamage; }

		void SetIsWaveEnemy(bool flag) { m_isWaveEnemy = flag; }

	private:
		int m_enemyNum = 0;
		CharacterController m_charaCon;//�v���C���[�̕����J�v�Z��

		//CCapsuleCollider m_CapsuleCollider;
		//RigidBody m_rigidBody;
		float m_radius = 20.0f;				//!<�J�v�Z���R���C�_�[�̔��a�B
		float m_height = 100.0f;				//!<�J�v�Z���R���C�_�[�̍����B

		EnemyAnimation* m_enemyAnimation = nullptr;
		EnemyVoice* m_enemyVoice = nullptr;
		SkinModelRender* m_skinModelRender = nullptr;
		Map* m_map = nullptr;
		
		Vector3 m_position = nsEnemyConstant::INI_POSITION;
		Vector3 m_screenPos = Vector3::Zero;
		Matrix		m_viewProjectionMatrix;					//�r���[�v���W�F�N�V�����s��
		Matrix		m_viewMatrix;							//�r���[�s��B
		Vector3 m_scale = { 1.5f,1.5f, 1.5f };
		Quaternion m_qrot = Quaternion::Identity;
		float m_hp = 100.0f;
		Vector3 m_moveSpeed = Vector3::Zero;
		//Vector3 m_enemyDir = Vector3::AxisZ;
		Vector3 m_playerDir = { 0.0f,0.0f,1.0f };//Vector3::AxisZ;
		float m_velocity = 150.0f;//�ړ��X�s�[�h�{��
		bool m_isDamage = false;//�_���[�W���󂯂��true
		bool m_isKnockBack = false;
		Vector3 m_playerPos = Vector3::Zero;
		int m_coolTime = nsEnemyConstant::ATTACK_COOLTIME;//�U���\�ɂȂ�܂ł̎���
		bool m_isAttack = false;
		bool m_isPlayerDamage = false;//�v���C���[�Ƀ_���[�W��^����Ƃ�true
		
		int m_proteinMeter = 0;//�v���e�C���ێ旿
		int m_knockBackLoopCount = 0;
		bool m_isRePlay = false;//�A�j���[�V�������ŏ�����Đ�������

		Vector3 m_targetPointPosition;
		
		Path m_path;
		PathFinding m_pathFiding;

		GameScene* m_gameScene = nullptr;

		bool m_isGymAttack = false;
		BackGround* m_backGround = nullptr;
		Vector3 m_diffGoldGym = Vector3::Zero;
		int m_nearGymNumber = 0;//�߂��ɂ���W���̔ԍ�
		Vector3 m_diffPlayerPos = Vector3::Zero;
		bool m_isKillNumUpdate = true;//���񂾂Ƃ��Ɉ�񂾂����s
		int m_dieTimer = 0;//Delete�����܂ł̎���
	
		//��
		SoundSource* m_ssRun = nullptr;
		bool m_isRunPlay = false;

		bool m_isWaveEnemy = false;
		bool m_isDisplay = true;
		int m_displayTimer = 0;

		bool m_isMapDisplay = true;

		bool m_isToPlayerMove = false;//�v���C���[��ǂ��Ă��邩	
};
}

