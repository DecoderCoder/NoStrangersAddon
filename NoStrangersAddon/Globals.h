#pragma once
#include "Vector/Vector.h"
#include <Windows.h>
#include "Utils/Utils.h"
#include <string>
#include "Patchables/Offsets.h"
#include "Patchables/Structs.h"
#include <map>

class CachedPlayer {
public:
	PlayerTypeEnum Type;
	Vector3 HeadPos;
	Vector3 HeadPosW2S;
	bool IsDead;
};

namespace Global {
	inline HMODULE g_module;
	inline bool Inited = false;
	inline bool Working = false;

	inline ImColor PlayerTypeColors[] = { ImColor(255, 255, 255), ImColor(255, 255, 255), ImColor(0, 0, 255), ImColor(255, 0, 0), };

	inline std::map<PlayerController*, CachedPlayer*> CachedPlayers;

	static GameManager* GetGameManager() {
		auto a = *(uintptr_t*)(baseAddr + Offset::GameManager);
		if (!a)
			return nullptr;
		auto b = *(uintptr_t**)(a + 0xB8);
		if (!b)
			return nullptr;
		if (!*b)
			return nullptr;
		return (GameManager*)*b;
	}

	static Vector3 WorldToScreen(Vector3 in) {
		Vector3 out = Vector3(0, 0, 0);
		auto currentCamera = ((void* (*)())Function::MainCamera)();
		if (!currentCamera)
			return out;
		auto resp = ((Vector3 * (__fastcall*)(void* out, void* camera, void* in))Function::WorldToScreenPoint)(&out, currentCamera, &in);
		if (!resp || out.z < 0)
			return Vector3(0, 0, 0);
		out.y = GetSystemMetrics(SM_CYSCREEN) - out.y;
		return out;
	}
}