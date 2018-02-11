#include "Cpu.h"
#include <bitset>
#include <array>

class Chip8
{
public:
	Chip8(std::array<uint8_t, 0x800> romData);

	void start();
private:
	Cpu _cpu;
	const std::array<uint8_t, 0x800> _romData;
	std::bitset<64 * 32> _screenPixels{};
};