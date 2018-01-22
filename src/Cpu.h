#include <stdint.h>
#include <array>

struct Cpu
{
	uint16_t pc = 0x200;

	std::array<uint8_t, 16> vx;


	void executeInstruction(uint16_t instruction);
};