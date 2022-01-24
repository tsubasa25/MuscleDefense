#include "stdafx.h"
#include "ResultScene.h"
#include "WipeCamera.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "BackGround.h"
namespace nsMuscle
{
	ResultScene::~ResultScene()
	{

	}
	bool ResultScene::Start()
	{
		BackGround* backGround=FindGO<BackGround>("backGround");
		DeleteGO(backGround);
		return true;
	}
	void ResultScene::Update()
	{
		if (g_pad[0]->IsTrigger(enButtonA)) {
			NewGO<nsMuscle::TitleScene>(0, "titleScene");
			NewGO<nsMuscle::GameScene>(0, "gameScene");
			DeleteGO(this);
		}
	}
}