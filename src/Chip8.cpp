#include "Chip8.h"
#include <chrono>
#include <thread>

Chip8::Chip8(std::array<uint8_t, 0x1000> romData)
	: _cpu{ romData }, _romData(romData) {};

void Chip8::start()
{
	while (true)
	{
		uint16_t instruction = (_romData[_cpu.reg.pc] << 8 | _romData[_cpu.reg.pc + 1]);

		_cpu.executeInstruction(instruction);
		_cpu.reg.pc += 2;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}