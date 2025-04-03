#include <Windows.h>
#include <thread>

const uintptr_t loadPakOffset = 0x10CE3EF0;

typedef void(__thiscall* fnLoadPak)(void* __this, std::wstring& path, int order);

DWORD WINAPI mainThread(LPVOID lpReserved, HMODULE hModule) {
    while (!FindWindowA("UnrealWindow", 0)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    uintptr_t base = reinterpret_cast<uintptr_t>(GetModuleHandleA("Client-Win64-ShippingBase.dll"));
    uintptr_t loadPakAddress = base + loadPakOffset;
    fnLoadPak loadPak = reinterpret_cast<fnLoadPak>(loadPakAddress);

    std::wstring path = L"./mod.pak";
    loadPak(nullptr, path, 11);

    while (!GetAsyncKeyState(VK_END)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    DisableThreadLibraryCalls(hModule);

    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)mainThread, hModule, 0, 0);
        break;
    }

    return TRUE;
}