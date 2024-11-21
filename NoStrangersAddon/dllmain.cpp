#include <thread>
#include <Windows.h>
#include "DirectX/DirectX.h"
#include "Globals.h"
#include "Main/Main.h"

uintptr_t initThreadHandle;

void OnExit() noexcept {
	Global::Working = false;
	DirectXHook::unHook();
	Main::Deinit();
	//CloseHandle((HANDLE)initThreadHandle);
}

DWORD WINAPI MainThread(LPVOID param) {
    Global::Working = true;
    DirectXHook::HookDX11();
    Main::Init();
    while (!GetAsyncKeyState(VK_END)) {
        Sleep(1);
    }
    Global::Working = false;
    return 1;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	Global::g_module = hModule;
	DisableThreadLibraryCalls(hModule);
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		std::atexit(OnExit);
		initThreadHandle = _beginthreadex(nullptr, 0, (_beginthreadex_proc_type)MainThread, Global::g_module, 0, nullptr);
		FreeLibrary(hModule);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		OnExit();
		break;
	}
	return TRUE;
}

