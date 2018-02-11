#include "Chip8.h"
#include <chrono>
#include <thread>

constexpr std::array<std::array<uint8_t, 5>, 16> spriteData = {
	{
		{ // 0
			0xF0, 0x90, 0x90, 0x90, 0xF0
		},
		{ // 1
			0x20, 0x60, 0x20, 0x20, 0x70
		},
		{ // 2
			0xF0, 0x10, 0xF0, 0x80, 0xF0
		},
		{ // 3
			0xF0, 0x10, 0xF0, 0x10, 0xF0
		},
		{ // 4
			0x90, 0x90, 0xF0, 0x10, 0x10
		},
		{ // 5
			0xF0, 0x80, 0xF0, 0x10, 0xF0
		},
		{ // 6
			0xF0, 0x80, 0xF0, 0x90, 0xF0
		},
		{ // 7
			0xF0, 0x10, 0x20, 0x40, 0x40
		},
		{ // 8
			0xF0, 0x90, 0xF0, 0x90, 0xF0
		},
		{ // 9
			0xF0, 0x90, 0xF0, 0x10, 0xF0
		},
		{ // A
			0xF0, 0x90, 0xF0, 0x90, 0x90
		},
		{ // B
			0xE0, 0x90, 0xE0, 0x90, 0xE0
		},
		{ // C
			0xF0, 0x80, 0x80, 0x80, 0xF0
		},
		{ // D
			0xE0, 0x90, 0x90, 0x90, 0xE0
		},
		{ // E
			0xF0, 0x80, 0xF0, 0x80, 0xF0
		},
		{ // F
			0xF0, 0x80, 0xF0, 0x80, 0x80
		}
	}
};

Chip8::Chip8(std::array<uint8_t, 0x800> romData)
	: _cpu{ { spriteData, {}, romData } }, _romData(romData) {};

void Chip8::start()
{
	while (true)
	{
		uint16_t instruction = (_cpu.memMap.memory[_cpu.reg.pc] << 8 | _cpu.memMap.memory[_cpu.reg.pc + 1]);

		_cpu.executeInstruction(instruction, _screenPixels);
		_cpu.reg.pc += 2;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}