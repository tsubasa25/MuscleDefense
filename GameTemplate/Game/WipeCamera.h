#pragma once
namespace nsMuscle{
	namespace nsWCConstant {
	}
	class BackGround;
	class GameScene;
class WipeCamera :public IGameObject
{
public:
	~WipeCamera();
	bool Start();
	void Update();
	void SwingCamera(int num);
	void SetIsSwing(bool flag,int num) { m_isSwing[num] = flag; }
	Vector3& GetTargetPos(int num) { return m_target[num]; }
private:
	BackGround* m_backGround = nullptr;
	GameScene* m_gameScene = nullptr;
	Vector3 m_position[3] = { Vector3::Zero,Vector3::Zero,Vector3::Zero };
	Vector3 m_target[4] = { Vector3::Zero,Vector3::Zero,Vector3::Zero,Vector3::Zero };
	SpriteInitData m_wipeSpriteInitData;

	RenderContext& m_renderContext = g_graphicsEngine->GetRenderContext();
	int m_wipeNum=1;
	bool m_isSwing[3] = { false };
	//Vector3 m_swingPos[3] = { Vector3::Zero,Vector3::Zero,Vector3::Zero };
	//Vector3 m_swingTatget[3] = { Vector3::Zero,Vector3::Zero,Vector3::Zero };
	int m_swingTimer[3] = { 0 };
};
}

