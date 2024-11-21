#pragma once
#include <vector>
#include "../Vector/Vector.h"
#include "../Patchables/Offsets.h"

#define STR_MERGE_IMPL(x, y)				x##y
#define STR_MERGE(x,y)						STR_MERGE_IMPL(x,y)
#define MAKE_PAD(size)						char STR_MERGE(pad_, __COUNTER__) [ size ]
#define DEFINE_MEMBER_0(x, y)				x
#define DEFINE_MEMBER_N(x,offset)			struct { MAKE_PAD(offset); x; }

// sizeof(PlayerController) = 492

enum class PlayerTypeEnum {
	None,
	Innocent,
	Guard,
	Cursed
};

enum class WeaponTypeEnum
{
	None,
	Knife,
	Pistol,
	Dynamite
};

class Transform {
public:
	Vector3 GetPosition() {
		Vector3 out = Vector3(0, 0, 0);
		try {
			((void* (__fastcall*)(void* out, void* transform))Function::TransformGetPosition)(out, this);
		}
		catch (char* e)
		{
		}

		return out;
	}
};

template<typename T>
class NetworkVariable {
public:
	T GetValue() {
		return *(T*)((uintptr_t)this + 0x58);
	}
};

class PlayerController {
public:
	union {
		DEFINE_MEMBER_N(Transform* HeadTransform, Offset::PlayerController::HeadTransform);
		DEFINE_MEMBER_N(Transform* StandingCharacterTransform, Offset::PlayerController::StandingCharacterTransform);
		DEFINE_MEMBER_N(NetworkVariable<int>* PlayerType, Offset::PlayerController::PlayerType);
		DEFINE_MEMBER_N(NetworkVariable<bool>* IsDeadVariable, Offset::PlayerController::IsDead);
		DEFINE_MEMBER_N(Vector3 Position, Offset::PlayerController::Position);
		DEFINE_MEMBER_N(float RunSpeed, Offset::PlayerController::RunSpeed);
		DEFINE_MEMBER_N(float MoveSpeed, Offset::PlayerController::MoveSpeed);
		DEFINE_MEMBER_N(float JumpHeight, Offset::PlayerController::JumpHeight);
	};

	Transform* GetHeadTransform() {
		return this->HeadTransform;
	}

	//Transform* GetStandingCharacterTransform() {
	//	return this->StandingCharacterTransform;
	//}

	PlayerTypeEnum GetPlayerType() {
		return (PlayerTypeEnum)this->PlayerType->GetValue();
	}

	Vector3 GetPosition() {
		return this->Position;
	}

	bool IsDead() {
		return this->IsDeadVariable->GetValue();
	}

	void* SetWeaponTypeClientRpc(WeaponTypeEnum wishWeaponType) {
		return ((void* (__fastcall*)(void*, WeaponTypeEnum))Function::SetWeaponTypeClientRpc)(this, wishWeaponType);
	}

	void* TeleportPlayerToSpawnPoint() {
		return ((void* (__fastcall*)(void*))Function::TeleportPlayerToSpawnPoint)(this);
	}
};

class GameManager {
public:
	union {
		DEFINE_MEMBER_N(PlayerController* LocalPlayer, Offset::GM::playerController);
	};

	std::vector<PlayerController*> GetPlayers() {
		uintptr_t playersArray = *(uintptr_t*)((uintptr_t)this + Offset::GM::currentPlayers);

		size_t playersCount = *(size_t*)(playersArray + 0x18);
		std::vector<PlayerController*> players;
		for (int i = 0; i < playersCount; i++) {
			players.push_back(*(PlayerController**)(playersArray + 0x20 + 8 * i));
		}
		return players;
	}
};