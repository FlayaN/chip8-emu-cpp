
#include "Chip8.h"

#include <fstream>
#include <stdint.h>
#include <array>

#ifdef _MSC_VER
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

#if ! (_MSC_VER >= 1912)
namespace std
{
	namespace filesystem = experimental::filesystem;
}
#endif

int main(int argc, char **argv)
{
	srand(time(NULL));
	std::filesystem::path romPath = "";
	if(argc > 1)
	{
		romPath = argv[1];
	}

	std::filesystem::path fullRomPath = std::filesystem::canonical(std::filesystem::current_path() / romPath);

	std::fstream romFile(fullRomPath, std::fstream::in | std::fstream::binary);

	if(!romFile.is_open()) {
		printf("Failed to open file:%s\n", fullRomPath.string().c_str());
		return -1;
	}

	std::array<uint8_t, 0x800> romData = {};
	romFile.read((char*)romData.data(), 0x800);
	Chip8 chip8(romData);
	chip8.start();
}