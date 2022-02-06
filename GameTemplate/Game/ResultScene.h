#pragma once
namespace nsMuscle
{
	namespace nsResultSceneConstant {
		const Vector4 SPRITE_MULLCOLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
		const Vector3 RESULT_PLAYER0_POS = { -160.0f,0.0f,670.0f };
		const Vector3 RESULT_PLAYER1_POS = { -160.0f,0.0f,760.0f };
		const Vector3 RESULT_PLAYER2_POS = { -100.0f,0.0f,730.0f };
		const Vector3 RESULT_PLAYER3_POS = { -120.0f,0.0f,830.0f };
		const float DISPLACE_QUANTITY = 50.0f;
		const Vector3 RESULT_CAMERA_POS = { -350.0f,100.0f,650.0f };
		const Vector3 RESULT_CAMERA_TARGET = { 0.0f,50.0f,650.0f };
		const Vector3 RESULT_SCORE_INI_POS = { 1000.0f,0.0f,0.0f };
		const Vector2 SPRITE_SCALE = { 1280,720 };
		const int RESULT_SCORE_NUM = 3;
		const Vector2 TITLE_FONT_POS = {200.0f,-50.0f};
		const Vector2 RESTART_FONT_POS = {200.0f,-150.0f};
		const float FONT_SCALE = 1.5f;
		const Vector3 TITLE_POS = { 300.0f,0.0f,0.0f };
	}
	class GameScene;
class ResultScene:public IGameObject
{
public:
	enum EnResultStatus {
		enStart,
		enKilledCount,
		enMuscleCount,
		enTimeCount,
		enEnd,
	};
	~ResultScene();
	bool Start();
	void Update();
	void SetIsClear(bool flag) { m_isClear=flag; }
	void CreateSprite();
	void NextScene();
private:
	//EnResultStatus m_resultStatus = enStart;
	GameScene* m_gameScene = nullptr;
	bool m_isClear = true;

	SpriteRender* m_title = nullptr;//クリアか失敗か
	SpriteRender* m_score[nsResultSceneConstant::RESULT_SCORE_NUM] = { nullptr };//倒した数、鍛えた量、プレイ時間

	FontRender* m_toTitle = nullptr;//タイトルへ
	FontRender* m_toReStart = nullptr;//もう一度
	bool m_isTitle = true;

	Vector2 m_titlePos = Vector2::Zero;
	Vector2 m_scorePos[nsResultSceneConstant::RESULT_SCORE_NUM] = { Vector2::Zero, Vector2::Zero, Vector2::Zero };

};
}


