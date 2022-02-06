#pragma once
namespace nsMuscle {
	namespace nsTitleSceneConstant
	{
		const float TITLE_BGM_VOL = 0.3f;
		const float DETERMINATION_SE_VOL = 0.3f;
	}
	class BackGround;
	//背景はステージをぼかして表示
	//タイトルロゴを表示
	class TitleScene :public IGameObject
	{
	public:
		enum EnTitleStatus
		{
			enStart,
			enOnButton,
		};

		~TitleScene();
		bool Start();
		void Update();

		void MoveCamera();
		void GameStart();
		void SetIsReStart(bool flag) { m_isReStart = flag; }
	private:
		EnTitleStatus m_titleStatus = enStart;
		Vector3 m_cameraPos = Vector3::Zero;
		Quaternion m_qRot = Quaternion::Identity;
		Vector3 m_cameraDir = { 2.0f,1.2f,0.0f };
		float m_rotation = 0.0f;

		SpriteRender* m_titleLogo = nullptr;
		SpriteRender* m_startLogo = nullptr;
		BackGround* m_backGround = nullptr;

		bool m_isReStart = false;
	
	};
}
