#include "Mem.h"
#include "Offsets.h"
#include "Roblox.h"

int main()
{
	DWORD Pid = WindowApi::GetProcId(L"RobloxPlayerBeta.exe");
	if (!Pid)
	{
		std::cerr << "Failed to Get Process Id" << std::endl;
		return 1;
	}
	
	std::cout << "Found Process Id: " << Pid << std::endl;

	HANDLE Process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid);
	if (!Process) {
		std::cerr << "Failed to open process handle. Error: " << GetLastError() << "\n";
		return -1;
	}

	uintptr_t baseAddress = WindowApi::GetModuleBaseAddy(Pid, L"RobloxPlayerBeta.exe");

	if (baseAddress == 0) {
		std::cerr << "Failed to get module base address." << std::endl;
		CloseHandle(Process);
		return -1;
	}

	std::cout << "Found Base Address: " << std::hex << baseAddress << std::endl;

	uintptr_t FakeDm = WindowApi::read<uintptr_t>(Process, baseAddress + offsets::FakeDataModelPointer);
	if (!FakeDm)
	{
		std::cerr << "Failed to Use FakeDataModel To Get DataModel." << std::endl;
		return -1;
	}

	uintptr_t Dm = WindowApi::read<uintptr_t>(Process, FakeDm + offsets::FakeDataModelToDataModel);
	if (!Dm)
	{
		std::cerr << "Failed to Get DataModel." << std::endl;
		return -1;
	}

	uintptr_t PlayersService = FindFirstChildByName(Process, Dm, "Players");
	uintptr_t LocalPlayer = WindowApi::read<uintptr_t>(Process, PlayersService + offsets::LocalPlayer);
	uintptr_t character = WindowApi::read<uintptr_t>(Process, LocalPlayer + offsets::ModelInstance);
	uintptr_t Humanoid = FindFirstChildByName(Process, character, "Humanoid");

	uintptr_t walkspeed = WindowApi::read<uintptr_t>(Process, Humanoid + offsets::WalkSpeed);
	uintptr_t walkspeedcheck = WindowApi::read<uintptr_t>(Process, Humanoid + offsets::WalkSpeedCheck);
	uintptr_t JumpPower = WindowApi::read<uintptr_t>(Process, Humanoid + offsets::JumpPower);

	

	std::cout << "Currect WalkSpeed: " << std::hex << walkspeed << std::endl;
	std::cout << "Currect JumpPower: " << std::hex << JumpPower << std::endl;

	float speed = 50;

	WindowApi::write<float>(Process, Humanoid + offsets::WalkSpeed, 200.0f);
	WindowApi::write<float>(Process, Humanoid + offsets::WalkSpeedCheck, 200.0f);
	WindowApi::write<float>(Process, Humanoid + offsets::JumpPower, 200.0f);
	

	Sleep(1200);

	std::cout << "New Walkspeed done. New Current Walkspeed: " << std::hex << WindowApi::read<uintptr_t>(Process, Humanoid + offsets::WalkSpeed) << std::endl;
	std::cout << "New JumpPower done. New Current JumpPower: " << std::hex << WindowApi::read<uintptr_t>(Process, Humanoid + offsets::JumpPower) << std::endl;

	return 1;
}