#pragma once

namespace Offset {
	static constexpr int GameManager = 0x2B93128;

	static constexpr int WorldToScreenPoint = 0x1F7E850;
	static constexpr int MainCamera = 0x1F7F120;
	static constexpr int TransformGetPosition = 0x1FC7180;


	namespace GM {
		static constexpr int playerController = 0xB0;
		static constexpr int currentPlayers = 0x110;
	}

	namespace PlayerController {
		static constexpr int Position = 0x1E0;
		static constexpr int JumpHeight = 0x1CC;
		static constexpr int RunSpeed = 0x1C8;
		static constexpr int MoveSpeed = 0x1C4;
		static constexpr int PlayerType = 0x88;
		static constexpr int IsDead = 0x170;
		static constexpr int HeadTransform = 0xC0;
		static constexpr int StandingCharacterTransform = 0xB0;

		namespace Function {
			static constexpr int FixedUpdate = 0x42D2F0;
			static constexpr int SetWeaponTypeClientRpc = 0x430290;
			static constexpr int TeleportPlayerToSpawnPoint = 0x430EC0;
		}

	}
}

namespace Function {
	inline void* WorldToScreenPoint = nullptr;
	inline void* TransformGetPosition = nullptr;
	inline void* MainCamera = nullptr;
	inline void* SetWeaponTypeClientRpc = nullptr;
	inline void* TeleportPlayerToSpawnPoint = nullptr;

}