#pragma once
#include <Windows.h>
#include <string>
#include <chrono>
#include <thread>
#include <iostream> 
#include <vector>
#include <cstdint>

namespace offsets {
    inline constexpr uintptr_t FakeDataModelPointer = 0x6833728;
    inline constexpr uintptr_t FakeDataModelToDataModel = 0x1C0;
    inline constexpr uintptr_t LocalPlayer = 0x128;
    inline constexpr uintptr_t ModelInstance = 0x340;
    inline constexpr uintptr_t WalkSpeed = 0x1DC;
    inline constexpr uintptr_t JumpPower = 0x1B8;
    inline constexpr uintptr_t WalkSpeedCheck = 0x3B8;
    inline constexpr uintptr_t Children = 0x80;
    inline constexpr uintptr_t ChildrenEnd = 0x8;
    inline constexpr uintptr_t Name = 0x78;
    inline constexpr uintptr_t ClassDescriptor = 0x18;
    inline constexpr uintptr_t ClassDescriptorToName = 0x8;
}