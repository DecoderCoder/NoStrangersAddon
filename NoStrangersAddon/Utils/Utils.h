#pragma once
#include <string>
#include <algorithm>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <functional>
#include <list>
#include <iostream>
#include <ranges>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <fstream>
#include <mutex>
#include <codecvt>
#include <locale>
#include <string>
#include <cassert>


#define baseAddr ((uintptr_t)GetModuleHandleA("GameAssembly.dll"))

#define ISVALIDPTR(x) (x != 0 && x != 0xFFFFFFFFFFFFFFFF)
#define STRCMP(str1, str2) (strcmp(str1, str2) == 0)

#define GETINDEXVEC(vec, val) (std::find(vec.begin(), vec.end(), val) - vec.begin())
#define FINDVECTOR(vec, val) (vec.size() && std::find(vec.begin(), vec.end(), val) != vec.end())
#define ERASEVECTOR(vec, index) vec.erase(vec.begin() + index)
#define ERASEVECTORVAL(vec, val) if(FINDVECTOR(vec, val)) vec.erase(vec.begin() + GETINDEXVEC(vec, val)); else {}

#define DEFINE_RVA(address) (baseAddr + (uintptr_t)address)
#define STR_MERGE_IMPL(x, y)                x##y
#define STR_MERGE(x,y)                        STR_MERGE_IMPL(x,y)
#define MAKE_PAD(size)                        BYTE STR_MERGE(pad_, __COUNTER__) [ size ]
#define DEFINE_MEMBER_0(x)                    x;
#define DEFINE_MEMBER_N(x,offset)            struct { MAKE_PAD((DWORD)offset); x; };

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

#define START_SCAN() std::list<uintptr_t*> variablesToCheck;
#define WAIT_SCAN() while (true){bool shouldBreak = true;for (auto obj : variablesToCheck) {if (*obj == 0){shouldBreak = false;break;}}if (shouldBreak)break;}
#define CHECK_SCAN(ifTrue, ifFalse)	{bool hasBad = false; for (auto obj : variablesToCheck) { if (!ISVALIDPTR(*obj)) { hasBad = true; break; } }if (hasBad) { ifFalse } else { ifTrue }}
#define SIGSCAN(variable, scan, fut) if(std::find(variablesToCheck.begin(), variablesToCheck.end(), &variable) == variablesToCheck.end()) variablesToCheck.push_back(&variable);  variable = scan
#define SIGSCAN_A(variable, scan, fut) if(std::find(variablesToCheck.begin(), variablesToCheck.end(), &variable) == variablesToCheck.end()) variablesToCheck.push_back(&variable); std::future<int> TOKENPASTE2(asd, __LINE__) = std::async(std::launch::async, [] { variable = scan; return 0; })
#define UNLOAD_VARS() for(auto obj : variablesToCheck) *obj = 0;

#define GET_LOADED_VARS(total, loaded) { total = variablesToCheck.size(); loaded = total; for (auto obj : variablesToCheck) {if (*obj == 0){ loaded--; }} }


template<typename FuncType>
__forceinline static FuncType CallVFunction(void* ppClass, int index)
{
	DWORD64* pVTable = *(DWORD64**)ppClass;
	DWORD64 dwAddress = pVTable[index];
	return (FuncType)(dwAddress);
}

template< typename T >
std::string to_hex(T i)
{
	std::stringstream stream;
	stream
		<< std::setfill('0') << std::setw(sizeof(T) * 2)
		<< std::hex << i;
	return stream.str();
}

template< typename T >
std::string to_binary(T i)
{
	std::stringstream stream;
	stream << std::bitset<64>(i).to_string();
	return stream.str();
}

static std::vector<std::string> explode(std::string const& s, char delim)
{
	std::vector<std::string> result;

	if (s.find(delim) == std::string::npos) {
		return result;
	}

	std::istringstream iss(s);

	for (std::string token; std::getline(iss, token, delim); )
	{
		result.push_back(std::move(token));
	}

	return result;
}

static void* HookVTableFunction(void* pVTable, void* fnHookFunc, int nOffset) // https://guidedhacking.com/threads/how-to-hook-directinput-emulate-key-presses.14011/
{
	intptr_t ptrVtable = *((intptr_t*)pVTable);
	intptr_t ptrFunction = ptrVtable + sizeof(intptr_t) * nOffset;
	intptr_t ptrOriginal = *((intptr_t*)ptrFunction);
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery((LPCVOID)ptrFunction, &mbi, sizeof(mbi));
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect);
	*((intptr_t*)ptrFunction) = (intptr_t)fnHookFunc;
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect);
	return (void*)ptrOriginal;
}

void WriteToFile(std::string FileName, std::string text);

void* SignatureScan(std::string signature, uintptr_t handle = NULL);

template<typename T>
static uintptr_t GetOffset(std::string signature, int offset, uintptr_t handle = NULL) {
	void* result = SignatureScan(signature, handle);
	if (result) {
		uintptr_t off = (uintptr_t) * (T*)((uintptr_t)result + offset);
		uintptr_t address = (uintptr_t)result + offset + off + sizeof(T);
		return address;
	}
	else
		return -1;
}

static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;

inline std::string GetGlyph(const unsigned int code)
{
	return convert.to_bytes(code);
}

static void CopyToClipboard(const char* str) {
	const size_t len = strlen(str) + 1;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(hMem), str, len);
	GlobalUnlock(hMem);
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
}