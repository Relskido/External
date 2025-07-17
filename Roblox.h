#pragma once
#include <Windows.h>
#include <string>
#include <chrono>
#include <thread>
#include <iostream> 
#include <vector>

#include "Mem.h"
#include "Offsets.h"

std::string GetName(HANDLE process, uintptr_t addy)
{
    uintptr_t namepointer = WindowApi::read<uintptr_t>(process, addy + offsets::Name);

    std::string name; // Variable To store the name i think

    if (WindowApi::read<int>(process, namepointer + 0x10) >= 16) // '0x10' returns the size of the string if '0x10' is bigger than 16 it means that the string is too big to read directly 
    {
        uintptr_t namePointer2 = WindowApi::read<uintptr_t>(process, namepointer); // Call the Pointer of the Name again because we did a if something idk what im talking about
        while (WindowApi::read<char>(process, namePointer2) != '\0')
        {
            name += WindowApi::read<char>(process, namePointer2); // we store the name
            namePointer2++; // we increase the index of the name pointer so that the next char can be read
        }
    }
    else
    {
        name = WindowApi::read<std::string>(process, namepointer); // if the pointer of the name is not big then we can read it directly
    }

    return name; // then we return the name
}

std::string GetClassName(HANDLE process, uintptr_t addy)
{
    uintptr_t ClassDescriptor = WindowApi::read<uintptr_t>(process, addy + offsets::ClassDescriptor); // Reads the Offsets or idk
    uintptr_t ClassDescriptorToName = WindowApi::read<uintptr_t>(process, ClassDescriptor + offsets::ClassDescriptorToName); // Reads the Offsets or idk
    std::string name;

    if (!ClassDescriptorToName) // eh idk
    {
        return "Unknown";
    }

    if (WindowApi::read<int>(process, ClassDescriptorToName + 0x10) >= 16) // '0x10' returns the size of the string if '0x10' is bigger than 16 it means that the string is too big to read directly 
    {
        uintptr_t namePointer2 = WindowApi::read<uintptr_t>(process, ClassDescriptorToName); // Call the Pointer of the Name again because we did a if something idk what im talking about
        while (WindowApi::read<char>(process, namePointer2) != '\0')
        {
            name += WindowApi::read<char>(process, namePointer2); // we store the name
            namePointer2++; // we increase the index of the name pointer so that the next char can be read
        }
    }
    else
    {
        name = WindowApi::read<std::string>(process, ClassDescriptorToName); // if the pointer of the name is not big then we can read it directly
    }

    return name; // the arguments is the same for class name
}

std::vector<uintptr_t> GetChildren(HANDLE process, uintptr_t addy)
{
    std::vector<uintptr_t> childrens; // store all the functions

    uintptr_t Children = WindowApi::read<uintptr_t>(process, addy + offsets::Children);
    uintptr_t ChildrenEnd = WindowApi::read<uintptr_t>(process, Children + offsets::ChildrenEnd);

    for (auto child = WindowApi::read<uintptr_t>(process, Children); child < ChildrenEnd; child += 0x10)
    {
        childrens.push_back(WindowApi::read<uintptr_t>(process, child));
    }

    return childrens; // returns the list of the children
}

uintptr_t FindFirstChildByName(HANDLE process, uintptr_t addy, std::string name)
{
    for (auto child : GetChildren(process, addy)) { // for each item "child" in the list of children of addy (we will reuse our getchildren function here)
        if (GetName(process, child) == name) { // we check if the name of the child is the same as the name we provided
            return child; // if the check passes it returns the child
        }
    }

    return 0; // else it returns 0x0 means no child with that name found
}