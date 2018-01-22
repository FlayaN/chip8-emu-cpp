#include "Cpu.h"

#include <array>

class Chip8
{
public:
	Chip8(std::array<uint8_t, 0x1000> romData)
		: _romData(romData) {}

	void start();
private:
	Cpu cpu = {};
	std::array<uint8_t, 0x1000> _romData;
};