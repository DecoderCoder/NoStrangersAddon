#include "EzHook.h"
#include "makesyscall.h"
#include <thread>
#include "../Utils.h"
#include "psapi.h"
#include "winternl.h"
#include "Zydis/Zydis.h"
#include "../Utils.h"

#if _WIN32 || _WIN64
#if _WIN64
#define ZYDIS_MODE ZYDIS_MACHINE_MODE_LONG_64
#else
#define ZYDIS_MODE ZYDIS_MACHINE_MODE_LONG_COMPAT_32
#endif
#endif



//относительная кодировка E9 jmp используется следующим образом :
//
//CURRENT_RVA: jmp(DESTINATION_RVA - CURRENT_RVA - 5[sizeof(E9 xx xx xx xx)])

/* MinSize is 6 */

bool inRange(unsigned low, unsigned high, unsigned x)
{
	return  ((x - low) <= (high - low));
}

bool EzHook::SysVirtualProtect(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect) {
	LPVOID addr = lpAddress;
	SIZE_T size = dwSize;
	return makesyscall<NTSTATUS>(0x50, 0x00, 0x00, 0x00)(GetCurrentProcess(), &addr, &size, flNewProtect, &lpflOldProtect);
}

uintptr_t EzHook::Restore::NtProtectVirtualMemory() {
	HMODULE ntdll = (HMODULE)GetModuleHandle(L"ntdll.dll");

	uintptr_t NtProtectVirtualMemoryAddr = reinterpret_cast<uintptr_t>(
		GetProcAddress(ntdll, "NtProtectVirtualMemory"));

	if (!NtProtectVirtualMemoryAddr)
		return 0;

#if _WIN64
	BYTE ZwQIP[] = { 0x4C, 0x8B, 0xD1, 0xB8, 0x50, 0x00, 0x00, 0x00, 0xF6, 0x04, 0x25, 0x08, 0x03, 0xFE, 0x7F, 0x01, 0x75, 0x03, 0x0F, 0x05, 0xC3 };
#else

#endif	
	DWORD old = 0;
	auto ret = SysVirtualProtect((char*)NtProtectVirtualMemoryAddr, sizeof(ZwQIP), PAGE_EXECUTE_READWRITE, &old);
	if (ret)
		return 0;
	for (int i = 0; i < sizeof(ZwQIP); i++) {
		*(BYTE*)(NtProtectVirtualMemoryAddr + i) = ZwQIP[i];
	}
	SysVirtualProtect((LPVOID)NtProtectVirtualMemoryAddr, sizeof(ZwQIP), old, &old);
	return NtProtectVirtualMemoryAddr;
}

uintptr_t EzHook::Restore::NtQueryVirtualMemory()
{
	HMODULE ntdll = (HMODULE)GetModuleHandle(L"ntdll.dll");

	uintptr_t NtQueryVirtualMemoryAddr = reinterpret_cast<uintptr_t>(
		GetProcAddress(ntdll, "NtQueryVirtualMemory"));

	if (!NtQueryVirtualMemoryAddr)
		return 0;

#if _WIN64
	BYTE ZwQIP[] = { 0x4C, 0x8B, 0xD1, 0xB8, 0x23, 0x00, 0x00, 0x00, 0xF6, 0x04, 0x25, 0x08, 0x03, 0xFE, 0x7F, 0x01, 0x75, 0x03, 0x0F, 0x05, 0xC3 };
#else

#endif
	DWORD old = 0;
	auto ret = SysVirtualProtect((char*)NtQueryVirtualMemoryAddr, sizeof(ZwQIP), PAGE_EXECUTE_READWRITE, &old);
	if (ret)
		return 0;
	for (int i = 0; i < sizeof(ZwQIP); i++) {
		*(BYTE*)(NtQueryVirtualMemoryAddr + i) = ZwQIP[i];
	}
	SysVirtualProtect((LPVOID)NtQueryVirtualMemoryAddr, sizeof(ZwQIP), old, &old);
	return NtQueryVirtualMemoryAddr;
}

uintptr_t EzHook::Restore::RtlAddVectoredExceptionHandler64() {
	HMODULE ntdll = (HMODULE)GetModuleHandle(L"ntdll.dll");

	uintptr_t RtlAddVectoredExceptionHandlerAddr = reinterpret_cast<uintptr_t>(
		GetProcAddress(ntdll, "RtlAddVectoredExceptionHandler"));

	DWORD old;
	unsigned char rtlaveh[] = { 0x45, 0x33, 0xC0, 0xE9, 0x08, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x89, 0x5C, 0x24, 0x08, 0x48, 0x89, 0x6C, 0x24, 0x10, 0x48, 0x89, 0x74, 0x24, 0x18, 0x57, 0x41, 0x54, 0x41, 0x56, 0x48, 0x83, 0xEC, 0x40, 0x41, 0x8B, 0xE8, 0x48, 0x8B, 0xFA, 0x44, 0x8B, 0xF1 };
	SysVirtualProtect((char*)RtlAddVectoredExceptionHandlerAddr, sizeof(rtlaveh), PAGE_EXECUTE_READWRITE, &old);

	for (int i = 0; i < sizeof(rtlaveh); i++) {
		*(BYTE*)(RtlAddVectoredExceptionHandlerAddr + i) = rtlaveh[i];
	}
	SysVirtualProtect((char*)RtlAddVectoredExceptionHandlerAddr, sizeof(rtlaveh), old, &old);
	return RtlAddVectoredExceptionHandlerAddr;
}

std::vector<EzHook::Struct> EzHooks;

uintptr_t EzHook::Add(uintptr_t target, void* hook, uintptr_t handle) {
	return Add(target, (uintptr_t)hook, handle);
}

uintptr_t EzHook::GetModuleHandleByAddr(uintptr_t addr, std::string* name) {
	HMODULE hMods[1024];
	DWORD cbNeeded;
	unsigned int i;

	if (EnumProcessModules(GetCurrentProcess(), hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			CHAR szModName[MAX_PATH];
			if (GetModuleFileNameExA(GetCurrentProcess(), hMods[i], szModName,
				sizeof(szModName) / sizeof(TCHAR)))
			{
				MODULEINFO modinfo = { 0 };
				HMODULE hModule = hMods[i];
				if (!hModule)
					return (uintptr_t)GetModuleHandle(NULL);

				GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
				if ((uintptr_t)modinfo.EntryPoint <= addr && (uintptr_t)modinfo.EntryPoint + modinfo.SizeOfImage >= addr)
				{
					if (name)
						*name = string(szModName);
					return (uintptr_t)hModule;
				}

			}
		}
	}
	return 0;
}

uintptr_t EzHook::ResloveJMP(uintptr_t targetJMP) {
	uintptr_t currentPtr = targetJMP;
	for (int i = 0; i < 1000; i++) {
		if (*(unsigned char*)(currentPtr) == 0xE9) {
			currentPtr += *(int*)(currentPtr + 1) + 5;
			continue;
		}
		else if (*(short*)currentPtr == 0x25FF) {
			currentPtr = *(uintptr_t*)(currentPtr + *(int*)(currentPtr + 2) + 6);
			continue;
		}
		else {
			return currentPtr;
		}
	}
	return 0;
}

uintptr_t EzHook::Add(uintptr_t target, uintptr_t hook, uintptr_t handle) {
	DWORD oldProt;

	int hookSize = 0;

	VirtualProtect((void*)((uintptr_t)target - 7), 7, 0x40, &oldProt);
	bool has6bytes = false;
	if (hookSize < 6) {
		has6bytes = true;
		for (char i = 1; i < 7; i++) {
			if (*(char*)(target - i) != (char)0xCC)
			{
				has6bytes = false;
				break;
			}
		}
	}
	if (!has6bytes)
		VirtualProtect((void*)(target - 7), 7, oldProt, &oldProt);

	{
		ZyanUSize offset = 0;
		ZydisDisassembledInstruction instruction;
		while (ZYAN_SUCCESS(ZydisDisassembleIntel(
			/* machine_mode:    */ ZYDIS_MODE,
			/* runtime_address: */ target,
			/* buffer:          */ (void*)(target + offset),
			/* length:          */ 1024,
			/* instruction:     */ &instruction
		))) {
			offset += instruction.info.length;
			hookSize += instruction.info.length;
			if (has6bytes && hookSize == 2) {
				break;
			}
			else if (hookSize >= 6) {
				break;
			}
		}
	}

	if (hookSize < 2)
		return -1;


	DWORD oldProt2;

	EzHook::Struct ezHook;
	ezHook.hooked = true;
	ezHook.hookSize = hookSize;
	ezHook.hookFunction = hook;
	ezHook.address = target;

	uintptr_t allocation = (uintptr_t)VirtualAlloc(NULL, 1024, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (allocation == 0)
		return -1;
	ezHook.origFunc = allocation;

	//////////////////////
	uintptr_t ptr = (uintptr_t)SignatureScan("CC CC CC CC CC CC CC CC", handle);
	if (!ptr)
		return -1;
	ezHook.CC = ptr;
	VirtualProtect((void*)ptr, hookSize, 0x40, &oldProt2);
	*(uintptr_t*)ptr = hook;
	//////////////////////

	VirtualProtect((void*)target, hookSize, 0x40, &oldProt);
	memcpy((void*)allocation, (void*)target, hookSize);
	int offset = ptr - target - 6;
	char jmpFROM[] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00 }; // jmp [rip+offset]
	char jmpBACK[] = {
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,	// jmp [rip]
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // PTR
	};
	*((uintptr_t*)&jmpBACK[6]) = (uintptr_t)(target + hookSize);
	memset((void*)target, 0x90, hookSize);
	if (has6bytes) {
		VirtualProtect((void*)(target - 6), hookSize, 0x40, &oldProt2);
		offset += 6;
		*(char*)(target) = 0xEB;
		*(char*)(target + 1) = 0xF8;
	}


	*((int*)&jmpFROM[2]) = (int)(offset);

	memcpy((void*)(uintptr_t)(allocation + hookSize), &jmpBACK, sizeof(jmpBACK)); // write Trampoline BACK

	if (has6bytes)
		memcpy((void*)(target - 6), jmpFROM, sizeof(jmpFROM));
	else
		memcpy((void*)target, jmpFROM, sizeof(jmpFROM));

	VirtualProtect((void*)target, hookSize, oldProt, &oldProt);

	EzHooks.push_back(ezHook);

	return allocation;

}

std::vector<EzHook::VEHStruct> EzHooksVEH;

LONG __stdcall LeoHandler2(EXCEPTION_POINTERS* pExceptionInfo)
{
#ifdef _WIN64
#define IP pExceptionInfo->ContextRecord->Rip
#else
#define IP pExceptionInfo->ContextRecord->Eip
#endif

	//MessageBoxA(0, to_string(pExceptionInfo->ExceptionRecord->ExceptionInformation[0]).c_str(), "LeoHandler2", 0);
	if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && pExceptionInfo->ExceptionRecord->ExceptionInformation[0] == 8)//&& 
	{
		//MessageBoxA(0, to_string(pExceptionInfo->ExceptionRecord->ExceptionInformation[0]).c_str(), "LeoHandler2", 0);
		for (auto& h : EzHooksVEH) {
			if (h.address == IP) {
				IP = h.hookAddress;
				return EXCEPTION_CONTINUE_EXECUTION;
			}

			if (inRange(h.MdiBase, h.MdiBase + h.MdiBaseSize, IP)) {
				int offset = IP - h.MdiBase;
				IP = static_cast<uintptr_t>(h.MdiBaseHooked + offset);
				//MessageBoxA(0, (to_hex(offset) + "\r\n" + to_hex(IP)).c_str(), "LeoHandler2: 1", 0);
				return EXCEPTION_CONTINUE_EXECUTION;
			}
		}
	}
	for (auto& h : EzHooksVEH) {
		if (inRange(h.MdiBaseHooked, h.MdiBaseHooked + h.MdiBaseHookedSize, IP)) {
			int offset = IP - h.MdiBaseHooked;

			//MessageBoxA(0, (to_hex(offset) + "\r\n" + to_hex(IP)).c_str(), "LeoHandler2: 2", 0);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
#undef IP
}

bool EzHook::IsDoneInit = false;
PVOID EzHook::VEH_Handle = nullptr;

int inline GET_OFFSET(uintptr_t from, uintptr_t to, size_t instruction_size = 5) {

	return (from < to ? to - from : -(int)(from - to)) - instruction_size;
}

void EzHook::FixRellocation(uintptr_t newRegion, size_t newRegionSize, uintptr_t oldRegion, size_t oldRegionSize)
{
	DWORD old;
#ifdef _WIN64
	// TODO: rellocate last instruction
	uintptr_t rellocOffsetBegin = 0;
	uintptr_t rellocOffsetEnd = 0;
	uintptr_t rellocOffsetFromEnd = 0;
	int counter = 0;
	char* newregion = (char*)newRegion;
	ZyanUSize offset = 0;
	ZydisDisassembledInstruction instruction;
	while (ZYAN_SUCCESS(ZydisDisassembleIntel(
		/* machine_mode:    */ ZYDIS_MODE,
		/* runtime_address: */ newRegion + offset,
		/* buffer:          */ (void*)(newRegion + offset),
		/* length:          */ oldRegionSize - offset,
		/* instruction:     */ &instruction
	))) {

		if (instruction.info.mnemonic == ZYDIS_MNEMONIC_INT3)
			*(newregion + offset) = 0x90;
		else if (instruction.info.mnemonic == ZYDIS_MNEMONIC_JMP || instruction.info.mnemonic == ZYDIS_MNEMONIC_CALL ||
			instruction.info.mnemonic == ZYDIS_MNEMONIC_JZ || instruction.info.mnemonic == ZYDIS_MNEMONIC_JNZ ||
			instruction.info.mnemonic == ZYDIS_MNEMONIC_JS || instruction.info.mnemonic == ZYDIS_MNEMONIC_JNS ||
			instruction.info.mnemonic == ZYDIS_MNEMONIC_JL || instruction.info.mnemonic == ZYDIS_MNEMONIC_JNL)
		{
			if (instruction.operands[0].type == ZYDIS_OPERAND_TYPE_MEMORY || instruction.operands[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE)
			{
				uintptr_t oldPtr = 0;
				switch (instruction.info.length) {
				case 2: // jmp short
				{
					char oldOffset = *(char*)(newRegion + offset + 1) + instruction.info.length;
					oldPtr = oldRegion + offset + oldOffset;
					break;
				}
				case 5:
				{
					int oldOffset = *(int*)(newRegion + offset + 1) + instruction.info.length;
					oldPtr = oldRegion + offset + oldOffset;
					break;
				}
				}

				if (oldPtr && (oldPtr < oldRegion || oldPtr > oldRegion + oldRegionSize))
				{
					unsigned char redirect[] = {
						0xFF, 0x15, 0x00, 0x00, 0x00, 0x00, // 0x15 - call, 0x25 - jmp
						0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ptr
					};

					switch (instruction.info.length) {
					case 2:
					{
						// E9 x x x x

						bool inBegin = oldPtr < oldRegion;
						if (inBegin)
							rellocOffsetBegin += 5;
						auto redirectPtr = newregion + newRegionSize - rellocOffsetFromEnd;
						rellocOffsetFromEnd += instruction.info.length;
						redirect[1] = 0x25;
						*((uintptr_t*)&redirect[6]) = oldPtr;

						memcpy(redirectPtr, redirect, sizeof(redirect));
						//MessageBoxA(0, (to_hex(newRegion - rellocOffsetBegin) + " || " + to_hex(newRegion + offset)).c_str(), "Gotcha", 0);

						if (inBegin) {
							*(char*)(newRegion - rellocOffsetBegin) = 0xE9;
							*(int*)(newRegion - rellocOffsetBegin + 1) = (int)(redirectPtr - (newRegion - rellocOffsetBegin) - 5);
							*(char*)(newregion + offset + 1) = (char)((newRegion - rellocOffsetBegin) - instruction.runtime_address - instruction.info.length);
						}
						else {
							*(char*)(newRegion + oldRegionSize + rellocOffsetEnd) = 0xE9;
							*(int*)(newRegion + oldRegionSize + rellocOffsetEnd + 1) = (int)(redirectPtr - (newRegion + oldRegionSize + rellocOffsetEnd) - 5);
							*(char*)(newRegion + offset + 1) = (char)((newRegion + oldRegionSize + rellocOffsetEnd) - instruction.runtime_address - instruction.info.length);
							rellocOffsetEnd += 5;
						}
						break;
					}
					case 5:
					{
						unsigned char redirect[] = {
							0xFF, 0x15, 0x05, 0x00, 0x00, 0x00, // 0x15 - call, 0x25 - jmp
							0xE9, 0x00, 0x00, 0x00, 0x00, // jmp back
							0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ptr
						};

						rellocOffsetFromEnd += sizeof(redirect);

						if (instruction.info.mnemonic == ZYDIS_MNEMONIC_JMP)
							redirect[1] = 0x25;
						*((uintptr_t*)&redirect[11]) = oldPtr;

						auto redirectPtr = newregion + newRegionSize - rellocOffsetFromEnd;
						auto currentPtr = newRegion + offset;
						*(int*)&redirect[7] = (int)((currentPtr + instruction.info.length) - ((unsigned long long)redirectPtr + 6) - 5);
						memcpy(redirectPtr, redirect, sizeof(redirect));

						//SysVirtualProtect(newregion + offset, instruction.info.length, PAGE_EXECUTE_READWRITE, &old);
						*(char*)(newregion + offset) = 0xE9;
						*(int*)(newregion + offset + 1) = (int)(redirectPtr - instruction.runtime_address - instruction.info.length);
						//SysVirtualProtect(newregion + offset, instruction.info.length, old, &old);
						counter++;
						break;
					}
					}
				}
			}
		}
		else if (instruction.info.length >= 5 && (instruction.info.mnemonic == ZYDIS_MNEMONIC_MOV || instruction.info.mnemonic == ZYDIS_MNEMONIC_LEA) && instruction.info.operand_count == 2 &&
			instruction.operands[0].type == ZYDIS_OPERAND_TYPE_REGISTER && instruction.operands[1].type == ZYDIS_OPERAND_TYPE_MEMORY && instruction.operands[1].mem.base == ZYDIS_REGISTER_RIP) // rellocating mov
		{
			std::vector<unsigned char> asmCode;

			uintptr_t oldPtr = oldRegion + offset + instruction.operands[1].mem.disp.value + instruction.info.length;

			if (instruction.operands[0].reg.value != ZYDIS_REGISTER_RAX)
				asmCode.push_back(0x50); // push rax
			{
				ZydisEncoderRequest req;
				memset(&req, 0, sizeof(req));
				req.mnemonic = ZYDIS_MNEMONIC_MOV;
				req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
				req.operand_count = 2;
				req.operands[0].type = ZYDIS_OPERAND_TYPE_REGISTER;
				req.operands[0].reg.value = ZYDIS_REGISTER_RAX;

				req.operands[1].type = ZYDIS_OPERAND_TYPE_IMMEDIATE;
				req.operands[1].imm.u = oldPtr;

				ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
				ZyanUSize encoded_length = sizeof(encoded_instruction);
				if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
				{
					MessageBoxA(0, ("ZydisEncoderRequest1 Failed: " + to_hex(instruction.operands[0].reg.value)).c_str(), "", 0);
					goto skip;
				}
				for (int i = 0; i < encoded_length; i++)
					asmCode.push_back(encoded_instruction[i]); // mov rax, ptr
			}
			{
				ZydisEncoderRequest req;
				memset(&req, 0, sizeof(req));
				req.mnemonic = instruction.info.mnemonic;
				req.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
				req.operand_count = 2;
				req.operands[0].type = ZYDIS_OPERAND_TYPE_REGISTER;
				req.operands[0].reg.value = instruction.operands[0].reg.value;

				req.operands[1].type = ZYDIS_OPERAND_TYPE_MEMORY;
				req.operands[1].mem.base = ZYDIS_REGISTER_RAX;
				req.operands[1].mem.size = ZydisRegisterGetWidth(ZYDIS_MACHINE_MODE_LONG_64, instruction.operands[0].reg.value) / 8;

				ZyanU8 encoded_instruction[ZYDIS_MAX_INSTRUCTION_LENGTH];
				ZyanUSize encoded_length = sizeof(encoded_instruction);
				if (ZYAN_FAILED(ZydisEncoderEncodeInstruction(&req, encoded_instruction, &encoded_length)))
				{
					MessageBoxA(0, ("ZydisEncoderRequest2 Failed: " + to_hex(instruction.operands[0].reg.value)).c_str(), "", 0);
					goto skip;
				}
				for (int i = 0; i < encoded_length; i++)
					asmCode.push_back(encoded_instruction[i]); // mov reg, [rax]
			}
			if (instruction.operands[0].reg.value != ZYDIS_REGISTER_RAX)
				asmCode.push_back(0x58); // pop rax

			rellocOffsetFromEnd += asmCode.size();
			rellocOffsetFromEnd += 5; // size of jmp	
			auto redirectPtr = newregion + newRegionSize - rellocOffsetFromEnd;

			memset(newregion + offset, 0x90, instruction.info.length);
			*(char*)(newregion + offset) = 0xE9;
			*(int*)(newregion + offset + 1) = GET_OFFSET(newRegion + offset, (uintptr_t)redirectPtr);
			memcpy(redirectPtr, asmCode.data(), asmCode.size());
			*(char*)(redirectPtr + asmCode.size()) = 0xE9;
			*(int*)(redirectPtr + asmCode.size() + 1) = GET_OFFSET((uintptr_t)redirectPtr + asmCode.size() /* from jmp */, newRegion + offset + instruction.info.length);

			//MessageBoxA(0, to_hex(newRegion + offset).c_str(), "2", 0);
		}
	skip:
		offset += instruction.info.length;
	}
#else

#endif
}

uintptr_t EzHook::AddVEHHook(uintptr_t target, uintptr_t hookFunc, uintptr_t handle)
{
	DWORD old;
	if (!IsDoneInit) {
		EzHook::Restore::NtProtectVirtualMemory();
		EzHook::Restore::NtQueryVirtualMemory();
#ifdef _WIN64
		EzHook::Restore::RtlAddVectoredExceptionHandler64();
#else
		UltimateHook::Restore::RtlAddVectoredExceptionHandler();
#endif
		VEH_Handle = (PVOID)AddVectoredExceptionHandler(true, static_cast<PTOP_LEVEL_EXCEPTION_FILTER>(LeoHandler2));
		IsDoneInit = true;
	}

	// Orig page
	auto mbi = MEMORY_BASIC_INFORMATION{ 0 };
	if (!VirtualQuery((char*)target, &mbi, sizeof(mbi))) {
		return false;
	}

	// Copy Region
	size_t newRegionSize = mbi.RegionSize + 0xFFFF; // ~bytes reserved for rellocation code
	uintptr_t newRegion = (uintptr_t)VirtualAlloc(NULL, newRegionSize + 0x100, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE); // 
	if (!newRegion)
		return 0;
	newRegion += 0x100;

	memset((char*)(newRegion), 0, newRegionSize); // for aesthetic purposes

	SysVirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &old);
	memcpy((char*)newRegion, mbi.BaseAddress, mbi.RegionSize);
	//SysVirtualProtect(mbi.BaseAddress, mbi.RegionSize, old, &old);


	EzHook::VEHStruct hook;
	memset(&hook, 0, sizeof(EzHook::VEHStruct));
	hook.MdiBaseHooked = newRegion;
	hook.MdiBaseHookedSize = newRegionSize;
	hook.MdiBase = (uintptr_t)mbi.BaseAddress;
	hook.MdiBaseSize = mbi.RegionSize;
	hook.address = target;
	hook.hookAddress = hookFunc;
	hook.oldProtect = old;
	hook.origFunc = newRegion + (target - (uintptr_t)mbi.BaseAddress);
	FixRellocation((uintptr_t)newRegion, newRegionSize, (uintptr_t)mbi.BaseAddress, mbi.RegionSize);
	EzHooksVEH.push_back(hook);
	SysVirtualProtect((char*)mbi.BaseAddress, mbi.RegionSize, PAGE_READONLY, &old);
	return (uintptr_t)hook.origFunc;
}

bool EzHook::Remove(void* origFunction) {
	return Remove((uintptr_t)origFunction);
}

bool EzHook::Remove(uintptr_t origFunction) {
	for (auto obj : EzHooks) {
		if (obj.origFunc == origFunction) {
			DWORD oldProt;
			VirtualProtect((void*)obj.address, obj.hookSize, PAGE_EXECUTE_READWRITE, &oldProt);
			memcpy((void*)obj.address, (void*)obj.origFunc, obj.hookSize);
			memset((void*)obj.CC, 0xCC, sizeof(uintptr_t));
			VirtualProtect((void*)obj.address, obj.hookSize, oldProt, &oldProt);
			return true;
		}
	}
	for (auto obj : EzHooksVEH) {
		if (obj.address == origFunction) {
			DWORD old;
			SysVirtualProtect((char*)obj.MdiBase, obj.MdiBaseSize, obj.oldProtect, &old);
			VirtualFree((char*)obj.MdiBaseHooked - 0x100, obj.MdiBaseHookedSize + 0x100, MEM_RELEASE);
			return true;
		}
	}
	return false;
}
