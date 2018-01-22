#include "Chip8.h"
#include <chrono>
#include <thread>

void Chip8::start()
{
	while (true)
	{
		uint16_t instruction = (_romData[cpu.pc] << 8 | _romData[cpu.pc + 1]);

		cpu.executeInstruction(instruction);
		cpu.pc += 2;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}