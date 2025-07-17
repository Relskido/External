#pragma once
#include <Windows.h>
#include <string>
#include <chrono>
#include <thread>
#include <iostream> 
#include <vector>
#include <cstdint>
#include <TlHelp32.h>

namespace WindowApi
{
    template <typename T>
    T read(HANDLE handle, uintptr_t Addy)
    {
        T buffer{};
        ReadProcessMemory(handle, (LPCVOID)Addy, &buffer, sizeof(T), nullptr);
        return buffer;
    }

    template <typename T>
    bool write(HANDLE handle, uintptr_t addy, T value)
    {
        return WriteProcessMemory(handle, (LPVOID)addy, &value, sizeof(T), nullptr);
    }

    DWORD GetProcId(const std::wstring& ProcName) {
        DWORD PId = 0;
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32W entry = { sizeof(entry) };
        if (Process32FirstW(hSnap, &entry)) {
            do {
                if (!_wcsicmp(entry.szExeFile, ProcName.c_str())) {
                    PId = entry.th32ProcessID;
                    break;
                }
            } while (Process32NextW(hSnap, &entry));
        }
        CloseHandle(hSnap);
        return PId;
    }

    uintptr_t GetModuleBaseAddy(DWORD ProcId, const std::wstring& ModName) {
        uintptr_t Base = 0;
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcId);
        MODULEENTRY32W modEntry = { sizeof(modEntry) };
        if (Module32FirstW(hSnap, &modEntry)) {
            do {
                if (!_wcsicmp(modEntry.szModule, ModName.c_str())) {
                    Base = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32NextW(hSnap, &modEntry));
        }
        CloseHandle(hSnap);
        return Base;
    }
}