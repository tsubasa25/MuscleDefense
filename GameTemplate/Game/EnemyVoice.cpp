#include "stdafx.h"
#include "EnemyVoice.h"
#include "EnemyAnimation.h"
namespace nsMuscle {
	EnemyVoice::~EnemyVoice()
	{
		//一応全部消す
		NotRunVoice();
		NotAttackVoice();
		NotDamageVoice();
		NotKnockBackVoice();
		NotDieVoice();
	}
	bool EnemyVoice::Start()
	{		
		return true;
	}
	void EnemyVoice::Update()
	{
		if (m_isVoice)//敵が近くにいるとき
		{
			switch (m_enemyAnimation->GetAnimationStatus())
			{
			case nsMuscle::EnemyAnimation::enStatus_Idle:
				//そもそもEnemy.cppで声を出さないようにしているからいらないかも
				/*NotRunVoice();
				NotAttackVoice();
				NotDamageVoice();
				NotKnockBackVoice();*/
				break;
			case EnemyAnimation::enStatus_Walk:
				//RunVoice();
				NotAttackVoice();
				NotDamageVoice();
				NotKnockBackVoice();
				break;
			case EnemyAnimation::enStatus_Attack:
				PunchVoice();
				NotRunVoice();
				NotDamageVoice();
				NotKnockBackVoice();
				break;
			case EnemyAnimation::enStatus_Damage:
				DamageVoice();
				NotRunVoice();
				NotAttackVoice();
				NotKnockBackVoice();
				break;
			case EnemyAnimation::enStatus_KnockBack:
				KnockBackVoice();
				NotRunVoice();
				NotAttackVoice();
				NotDamageVoice();
				break;
			default:
				break;
			}
		}
		else if (m_enemyAnimation->GetAnimationStatus() == EnemyAnimation::enStatus_Die)
		{
			DieVoice();
			NotRunVoice();
			NotAttackVoice();
			NotDamageVoice();
			NotKnockBackVoice();
		}
		else
		{
			NotRunVoice();
			NotAttackVoice();
			NotDamageVoice();
			NotKnockBackVoice();
			NotDieVoice();
		}
	}
	void EnemyVoice::RunVoice()
	{
		//ゾンビはうめき声を上げる
		if (EnemySEManager::GetInstance()->GetWalkSENum() < m_runSENum) {//n個まで同時に再生
			if (m_isRunPlay == false)
			{
				EnemySEManager::GetInstance()->PlusWalkSENum();
				m_isRunPlay = true;
				m_isRunCreate = true;
				m_ssRun = NewGO<SoundSource>(0);
				m_ssRun->SetIsDeleteGOSelf(false);//自動で消えないようにする
				m_ssRun->Init(L"Assets/sound/Zombie_Moaning.wav", enSE);
				m_ssRun->SetVolume(0.5f);
			}
			if(m_isRunCreate == true&&m_ssRun!=nullptr)
			m_ssRun->Play(false);
		}
		if (m_isRunCreate == true && m_ssRun->IsPlaying() == false)
		{
			EnemySEManager::GetInstance()->MinusWalkSENum();
			m_isRunCreate = false;
			m_isRunPlay = false;
			m_ssRun->Release();//消す
		}
	}
	void EnemyVoice::PunchVoice()
	{
		m_attackSETimer++;
		if (EnemySEManager::GetInstance()->GetAttackSENum() < m_attackSENum && m_attackSETimer==18) {//n個まで同時に再生			
			if (m_isAttackPlay == false)
			{
				EnemySEManager::GetInstance()->PlusAttackSENum();
				m_isAttackPlay = true;
				m_isAttackCreate = true;
				m_ssAttack = NewGO<SoundSource>(0);
				m_ssAttack->SetIsDeleteGOSelf(false);//
				m_ssAttack->Init(L"Assets/sound/Zombie_Attack.wav", enSE);
				m_ssAttack->SetVolume(nsInGameSoundConstant::ENEMY_ATTACK_SE_VOL);
			}
			if (m_isAttackCreate == true) {
				m_ssAttack->Play(false);				
			}
		}
		if (m_isAttackCreate == true && m_ssAttack->IsPlaying() == false)
		{
			EnemySEManager::GetInstance()->MinusAttackSENum();
			m_isAttackCreate = false;
			m_isAttackPlay = false;
			m_attackSETimer = 0;
			m_ssAttack->Release();//消す
		}
	}
	void EnemyVoice::DamageVoice()
	{
		if (EnemySEManager::GetInstance()->GetDamageSENum() < m_damageSENum) {//n個まで同時に再生
			if (m_isDamagePlay == false)
			{
				EnemySEManager::GetInstance()->PlusDamageSENum();
				m_isDamagePlay = true;
				m_isDamageCreate = true;
				m_ssDamage = NewGO<SoundSource>(0);
				m_ssDamage->SetIsDeleteGOSelf(false);//自動で消えないようにする
				m_ssDamage->Init(L"Assets/sound/Zombie_Damage.wav", enSE);
				m_ssDamage->SetVolume(nsInGameSoundConstant::ENEMY_DAMAGE_SE_VOL);
			}
			if(m_isDamageCreate==true)
			m_ssDamage->Play(false);
			if (m_isDamageCreate == true && m_ssDamage->IsPlaying() == false)
			{
				EnemySEManager::GetInstance()->MinusAttackSENum();
				m_isDamageCreate = false;
				m_isDamagePlay = false;
				m_ssDamage->Release();//消す
			}
		}
	}
	void EnemyVoice::KnockBackVoice()
	{
		if (EnemySEManager::GetInstance()->GetKnockBackSENum() < m_knockBackSENum&&
			m_enemyAnimation->GetKnockBackTimer()==2) {//n個まで同時に再生
			if (m_isKnockBackPlay == false)
			{
				EnemySEManager::GetInstance()->PlusKnockBackSENum();
				m_isKnockBackPlay = true;
				m_isKnockBackCreate = true;
				m_ssKnockBack = NewGO<SoundSource>(0);
				m_ssKnockBack->SetIsDeleteGOSelf(false);//自動で消えないようにする
				m_ssKnockBack->Init(L"Assets/sound/Zombie_KnockBack.wav", enSE);
				m_ssKnockBack->SetVolume(nsInGameSoundConstant::ENEMY_DIE_SE_VOL);
			}
			if (m_isKnockBackCreate == true)
			m_ssKnockBack->Play(false);
			if (m_isKnockBackCreate == true && m_ssKnockBack->IsPlaying() == false)
			{
				EnemySEManager::GetInstance()->MinusKnockBackSENum();
				m_isKnockBackCreate = false;
				//m_isKnockBackPlay = false;//一撃で死ぬから作り直さないでいいかも
				m_ssKnockBack->Release();//消す
			}
		}
	}
	void EnemyVoice::DieVoice()
	{
		if (EnemySEManager::GetInstance()->GetDieSENum() < m_dieSENum) {//n個まで同時に再生
			if (m_isDiePlay == false)
			{
				EnemySEManager::GetInstance()->PlusDieSENum();
				m_isDiePlay = true;
				m_isDieCreate = true;
				m_ssDie = NewGO<SoundSource>(0);
				m_ssDie->SetIsDeleteGOSelf(false);
				m_ssDie->Init(L"Assets/sound/Zombie_Die.wav", enSE);
				m_ssDie->SetVolume(nsInGameSoundConstant::ENEMY_DIE_SE_VOL);
				m_ssDie->Play(false);
			}
			if (m_isDieCreate == true && m_ssDie->IsPlaying() == false)
			{
				EnemySEManager::GetInstance()->MinusDieSENum();
				m_isDieCreate = false;				
				m_ssDie->Release();//消す
			}
		}
	}
	///以降Not ////////////////////////////////////////////////////////////////

	void EnemyVoice::NotRunVoice()
	{
		if (m_ssRun != nullptr && m_isRunPlay == true) {//声を出さなくなる
			m_ssRun->Release();
			m_isRunPlay = false;
			EnemySEManager::GetInstance()->MinusWalkSENum();
			m_isRunCreate = false;
		}
	}
	void EnemyVoice::NotAttackVoice()
	{
		if (m_ssAttack != nullptr && m_isAttackPlay == true) {//声を出さなくなる
			m_ssAttack->Release();
			m_isAttackPlay = false;
			EnemySEManager::GetInstance()->MinusAttackSENum();
			m_isAttackCreate = false;
			m_attackSETimer = 0;
		}
	}
	void EnemyVoice::NotDamageVoice()
	{
		if (m_ssDamage != nullptr && m_isDamagePlay == true) {//声を出さなくなる
			m_ssDamage->Release();
			m_isDamagePlay = false;
			EnemySEManager::GetInstance()->MinusDamageSENum();
			m_isDamageCreate = false;
		}
	}
	void EnemyVoice::NotKnockBackVoice()
	{
		if (m_ssKnockBack != nullptr && m_isKnockBackPlay == true) {//声を出さなくなる
			m_ssKnockBack->Release();
			m_isKnockBackPlay = false;
			EnemySEManager::GetInstance()->MinusKnockBackSENum();
			m_isKnockBackCreate = false;
		}
	}
	void EnemyVoice::NotDieVoice()
	{
		if (m_ssDie != nullptr && m_isDiePlay == true) {//声を出さなくなる
			m_ssDie->Release();
			m_isDiePlay = false;
			EnemySEManager::GetInstance()->MinusDieSENum();			
		}
	}
}