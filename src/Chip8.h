#include "Cpu.h"

#include <array>

class Chip8
{
public:
	Chip8(std::array<uint8_t, 0x1000> romData);

	void start();
private:
	Cpu _cpu = {};
	const std::array<uint8_t, 0x1000> _romData;
};