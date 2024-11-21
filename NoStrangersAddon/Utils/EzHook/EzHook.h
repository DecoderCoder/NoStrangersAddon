#pragma once
#include <Windows.h>
#include <vector>
#include <VersionHelpers.h>
#include <string>
#include <stddef.h>

class EzHook {
private:
	static bool IsDoneInit;
	static PVOID VEH_Handle;

	static bool SysVirtualProtect(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect);
	static uintptr_t ResloveJMP(uintptr_t targetJMP);
public:
	static class Restore {
	public:
		static uintptr_t RtlAddVectoredExceptionHandler64();
		static uintptr_t NtProtectVirtualMemory();
		static uintptr_t NtQueryVirtualMemory();
		static bool SysDll(std::string name);
	};

	struct Struct {
		bool hooked;
		bool JMP;
		uintptr_t address;
		size_t hookSize;

		uintptr_t CC;
		uintptr_t hookFunction;
		uintptr_t origFunc;
	};

	struct VEHStruct {
		uintptr_t address;
		uintptr_t hookAddress;

		uintptr_t origFunc;


		uintptr_t MdiBaseHooked;
		uintptr_t MdiBaseHookedSize;

		uintptr_t MdiBase;
		uintptr_t MdiBaseSize;

		DWORD oldProtect;
	};

	static uintptr_t GetModuleHandleByAddr(uintptr_t addr, std::string* name = nullptr);

	static uintptr_t Add(uintptr_t target, void* hook, uintptr_t handle = NULL);
	static uintptr_t Add(uintptr_t target, uintptr_t hook, uintptr_t handle = NULL);

	static uintptr_t AddVEHHook(uintptr_t target, uintptr_t hook, uintptr_t handle);
	static void FixRellocation(uintptr_t newRegion, size_t newRegionSize, uintptr_t oldRegion, size_t oldRegionSize);

	static bool Remove(uintptr_t target);
	static bool Remove(void* origFunction);
};