#pragma once
// インゲームのサウンド関係の定数ヘッダー。

namespace nsMuscle {
	namespace nsInGameSoundConstant {
		// プレイヤーの足音SEのボリューム。
		const float PLAYER_FOOT_STEP_SE_VOL = 1.0f;
		// 一発目の攻撃SEのボリューム。
		const float PLAYER_ATTACK_01_SE_VOL = 1.0f;
		// ２発目の攻撃SEのボリューム。
		const float PLAYER_ATTACK_02_SE_VOL = 1.0f;
		// ３発目の攻撃SEのボリューム。
		const float PLAYER_ATTACK_03_SE_VOL = 1.0f;
		// プレイヤーのやられSEのボリューム。
		const float PLAYER_DAMAGE_SE_VOL = 1.0f;
		// ゾンビの攻撃SEのボリューム。
		const float ENEMY_ATTACK_SE_VOL = 0.5f;
		// ゾンビのやられSEのボリューム。
		const float ENEMY_DAMAGE_SE_VOL = 0.5f;
		// ゾンビの死亡SEのボリューム。
		const float ENEMY_DIE_SE_VOL = 0.4f;
		// 環境音SEのボリューム。
		const float AMBIENT_SE_VOL = 0.15f;
	}
}
