#include "stdafx.h"
#include "PlayerVoice.h"
#include "PlayerAnimation.h"
namespace nsMuscle {
	namespace {
		const int UPPER_TIMNING = 15;
	}
	PlayerVoice::~PlayerVoice()
	{
		//一応全部消す
		NotRunVoice();		
		NotDamageVoice();
		NotDieVoice();
	}
	bool PlayerVoice::Start()
	{
		return true;
	}
	void PlayerVoice::Update()
	{
		if (m_isVoice)//
		{
			switch (m_playerAnimation->GetAnimationStatus())
			{
			case nsMuscle::PlayerAnimation::enStatus_Idle:
				//そもそもEnemy.cppで声を出さないようにしているからいらないかも
				NotRunVoice();
				NotDamageVoice();		
				NotPunch0Voice();
				NotPunch1Voice();
				NotUpperVoice();
				break;
			case PlayerAnimation::enStatus_Run:
				RunVoice();				
				NotDamageVoice();
				NotPunch0Voice();
				NotPunch1Voice();
				NotUpperVoice();
				break;
			case PlayerAnimation::enStatus_Punchi0:
				Punch0Voice();
				NotPunch1Voice();
				NotRunVoice();				
				NotUpperVoice();
				NotDamageVoice();
				break;
			case PlayerAnimation::enStatus_Punchi1:
				Punch1Voice();
				NotPunch0Voice();
				NotRunVoice();				
				NotUpperVoice();
				NotDamageVoice();
				break;
			case PlayerAnimation::enStatus_Punchi2:
				UpperVoice();
				NotRunVoice();
				NotPunch0Voice();	
				NotPunch1Voice();
				NotDamageVoice();
				break;
			case PlayerAnimation::enStatus_Damage:
				DamageVoice();
				NotRunVoice();
				NotPunch0Voice();
				NotPunch1Voice();
				NotUpperVoice();
				break;			
			default:

				break;
			}
		}
		else if (m_playerAnimation->GetAnimationStatus() == PlayerAnimation::enStatus_Die)
		{
			DieVoice();
			//NotRunVoice();			
			//NotDamageVoice();
		}
		else
		{
			NotRunVoice();		
			NotPunch0Voice();
			NotPunch1Voice();
			NotDamageVoice();
			NotDieVoice();
		}
	}
	void PlayerVoice::RunVoice()
	{		
		if (m_isRunCreate == false)
		{
			m_isRunCreate = true;
			m_ssRun = NewGO<SoundSource>(0);
			m_ssRun->Init(L"Assets/sound/run.wav", enSE);
			m_ssRun->SetVolume(nsInGameSoundConstant::PLAYER_FOOT_STEP_SE_VOL);
		}
		if (m_isRunCreate == true && m_ssRun != nullptr)
			m_ssRun->Play(true);		
	}
	void PlayerVoice::Punch0Voice()
	{
		if (m_isPunch0Create == false)
		{			
			m_isPunch0Create = true;
			m_ssPunch0 = NewGO<SoundSource>(0);
			m_ssPunch0->SetIsDeleteGOSelf(false);
			m_ssPunch0->Init(L"Assets/sound/punchkara.wav", enSE);
			m_ssPunch0->SetVolume(nsInGameSoundConstant::PLAYER_ATTACK_01_SE_VOL);
		}		
		if (m_isPunch0Create == true) {
			m_ssPunch0->Play(false);
		}
		if (m_isPunch0Create == true && m_ssPunch0->IsPlaying() == false)
		{			
			m_isPunch0Create = false;
		}
	}
	void PlayerVoice::Punch1Voice()
	{
		if (m_isPunch1Create == false)
		{
			m_isPunch1Create = true;
			m_ssPunch1 = NewGO<SoundSource>(0);
			m_ssPunch1->SetIsDeleteGOSelf(false);
			m_ssPunch1->Init(L"Assets/sound/punchkara.wav", enSE);
			m_ssPunch1->SetVolume(nsInGameSoundConstant::PLAYER_ATTACK_02_SE_VOL);
		}
		if (m_isPunch1Create == true) {
			m_ssPunch1->Play(false);
		}
		if (m_isPunch1Create == true && m_ssPunch1->IsPlaying() == false)
		{
			m_isPunch1Create = false;
		}
	}
	void PlayerVoice::UpperVoice()
	{
		if (m_isUpperCreate == false)
		{
			m_upperTimer++;
			if (m_upperTimer == UPPER_TIMNING) {
				m_isUpperCreate = true;
				m_ssUpper = NewGO<SoundSource>(0);
				m_ssUpper->SetIsDeleteGOSelf(false);
				m_ssUpper->Init(L"Assets/sound/punchWind.wav", enSE);
				m_ssUpper->SetVolume(nsInGameSoundConstant::PLAYER_ATTACK_03_SE_VOL);
			}
		}
		if (m_isUpperCreate == true) {
			m_ssUpper->Play(false);
		}
		if (m_isUpperCreate == true && m_ssUpper->IsPlaying() == false)
		{			
			m_isUpperCreate = false;
		}
	}
	void PlayerVoice::DamageVoice()
	{
		if (m_isDamageCreate == false)
		{			
			m_isDamageCreate = true;
			m_ssDamage = NewGO<SoundSource>(0);
			m_ssDamage->SetIsDeleteGOSelf(false);			
			int randam = rand() % 3;
			switch (randam)
			{
			case 0:
				m_ssDamage->Init(L"Assets/sound/damage0.wav", enSE);
				break;
			case 1:
				m_ssDamage->Init(L"Assets/sound/damage1.wav", enSE);
				break; 
			case 2:
				m_ssDamage->Init(L"Assets/sound/damage2.wav", enSE);
					break;
			default:
				break;
			}			
			m_ssDamage->SetVolume(nsInGameSoundConstant::PLAYER_DAMAGE_SE_VOL);
		}
		if (m_isDamageCreate == true) {
			m_ssDamage->Play(false);
		}
		if (m_isDamageCreate == true && m_ssDamage->IsPlaying() == false)
		{
			m_isDamageCreate = false;
		}
	}
	void PlayerVoice::DieVoice()
	{
		if (m_isDieCreate == false)
		{
			m_isDieCreate = true;
			m_ssDie = NewGO<SoundSource>(0);
			m_ssDie->Init(L"Assets/sound/die.wav", enSE);
		}
		if (m_isDieCreate == true) {
			m_ssDie->Play(false);
		}
	}
	/// <summary>
	/// ここからNot///////////////////////////////////////
	/// </summary>

	void PlayerVoice::NotRunVoice()
	{
		if (m_ssRun != nullptr && m_isRunCreate == true) {//声を出さなくなる
			m_ssRun->Release();
			m_isRunCreate = false;
		}
	}
	void PlayerVoice::NotPunch0Voice()
	{
		if (m_ssPunch0 != nullptr && m_isPunch0Create == true) {//声を出さなくなる
			m_ssPunch0->Release();
			m_isPunch0Create = false;
		}
	}
	void PlayerVoice::NotPunch1Voice()
	{
		if (m_ssPunch1 != nullptr && m_isPunch1Create == true) {//声を出さなくなる
			m_ssPunch1->Release();
			m_isPunch1Create = false;
		}
	}
	void PlayerVoice::NotUpperVoice()
	{
		if (m_ssUpper != nullptr && m_isUpperCreate == true) {//声を出さなくなる
			m_ssUpper->Release();
			m_isUpperCreate = false;
			m_upperTimer = 0;
		}
	}
	void PlayerVoice::NotDamageVoice()
	{
		if (m_ssDamage != nullptr && m_isDamageCreate == true) {//声を出さなくなる
			m_ssDamage->Release();
			m_isDamageCreate = false;
		}		
	}
	void PlayerVoice::NotDieVoice()
	{
		if (m_ssDie != nullptr && m_isDieCreate == true) {//声を出さなくなる
			m_ssDie->Release();
			m_isDieCreate = false;
		}
	}
}