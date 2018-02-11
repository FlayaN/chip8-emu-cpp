#include "Cpu.h"
#include <stdio.h>

union Instruction
{
	uint16_t instr;

	struct
	{
		uint8_t first;
		uint8_t second;
	} byte;

	uint8_t bytes[2];

	struct
	{
		uint8_t first : 4;
		uint8_t y : 4;
		uint8_t x : 4;
		uint8_t opcode : 4;
	} nibble;

	uint16_t address : 12;
};

void Cpu::redraw(const std::bitset<64 * 32>& pixels) const
{
	printf("\n");
	for (int x = 0; x < 66; x++)
	{
		printf("**");
	}
	printf("\n");
	for (int y = 0; y < 32; y++)
	{
		printf("**");
		for (int x = 0; x < 64; x++)
		{
			auto pixelVal = pixels[x + y * 64] ? 219 : ' ';
			printf("%c%c", pixelVal, pixelVal);
		}
		printf("**\n");
	}
	for (int x = 0; x < 66; x++)
	{
		printf("**");
	}
	printf("\n");
}

void Cpu::executeInstruction(uint16_t instruction, std::bitset<64 * 32>& pixels)
{
	printf("%04X: ", instruction);

	const Instruction instr = { instruction };

	switch (instr.nibble.opcode)
	{
		case 0x0:
		{
			switch (instruction)
			{
				case 0x00E0:
				{
					pixels = {};
					redraw(pixels);
					break;
				}
				case 0x00EE:
				{
					printf("not implemented: return");
					break;
				}
				default:
				{
					reg.pc = instr.address;
					printf("Call %03X", instr.address);
					break;
				}
			}
			break;
		}
		case 0x2:
		{
			stack.push(reg.pc);
			reg.pc = instr.address - 2;
			break;
		}
		case 0x3:
		{
			if (reg.v[instr.nibble.x] == instr.byte.first)
			{
				reg.pc += 2;
			}
			break;
		}
		case 0x4:
		{
			if (reg.v[instr.nibble.x] != instr.byte.first)
			{
				reg.pc += 2;
			}
			break;
		}
		case 0x6:
		{
			reg.v[instr.nibble.x] = instr.byte.first;
			break;
		}
		case 0xA:
		{
			reg.l = instr.address;
			break;
		}
		case 0xD:
		{
			const auto& vx = reg.v[instr.nibble.x];
			const auto& vy = reg.v[instr.nibble.y];

			size_t spriteIndex = (&memMap.memory[reg.l] - &(memMap.namedMemory.sprites[0][0])) / sizeof(std::array<uint8_t, 5>);

			const std::array<uint8_t, 5>& sprite = memMap.namedMemory.sprites[spriteIndex];
			
			for (int y = 0; y < instr.nibble.first; y++)
			{
				for (int x = 0; x < 8; x++)
				{
					pixels[x + vx + (y + vy) * 64] = (sprite[y] & (1 << x)) >> x;
				}
			}
			redraw(pixels);
			break;
		}
		case 0xF:
		{
			switch(instr.byte.first)
			{
				case 0x1E:
				{
					reg.l += reg.v[instr.nibble.x];
					break;
				}
				case 0x29:
				{
					// Address of the sprite in the actual memory
					reg.l = &(memMap.namedMemory.sprites[instr.nibble.x][0]) - &(memMap.namedMemory.sprites[0][0]);
					break;
				}
				case 0x55:
				{
					for(int i = 0; i <= instr.nibble.x; i++)
					{
						memMap.memory[reg.l] = reg.v[i];
						reg.l++;
					}
					break;
				}				
				case 0x65:
				{
					for(int i = 0; i <= instr.nibble.x; i++)
					{
						reg.v[i] = memMap.memory[reg.l];
						reg.l++;
					}
					break;
				}
				default:
				{
					printf("Not implemented 0xF");
					break;
				}
			}
			break;
		}
		default:
		{
			printf("Not implemented");
			break;
		}
	}
	printf("\n");
}
