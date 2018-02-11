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
					reg.pc = stack.top();
					stack.pop();
					//printf("not implemented: return");
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
		case 0x1:
		{
			reg.pc = instr.address - 2;
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
		case 0x7:
		{
			reg.v[instr.nibble.x] += instr.byte.first;
			break;
		}
		case 0x8:
		{
			switch (instr.nibble.first)
			{
				case 0x0:
				{
					reg.v[instr.nibble.x] = reg.v[instr.nibble.y];
					break;
				}
				case 0x1:
				{
					reg.v[instr.nibble.x] |= reg.v[instr.nibble.y];
					break;
				}
				case 0x2:
				{
					reg.v[instr.nibble.x] &= reg.v[instr.nibble.y];
					break;
				}
				case 0x3:
				{
					reg.v[instr.nibble.x] ^= reg.v[instr.nibble.y];
					break;
				}
				case 0x4:
				{
					int res = reg.v[instr.nibble.x] + reg.v[instr.nibble.y];
					reg.v[0xF] = res > std::numeric_limits<uint8_t>::max();
					reg.v[instr.nibble.x] = uint8_t(res);
					break;
				}
				case 0x5:
				{
					int res = reg.v[instr.nibble.x] - reg.v[instr.nibble.y];
					reg.v[0xF] = res >= 0;
					reg.v[instr.nibble.x] = uint8_t(res);
					
					break;
				}
				case 0x6:
				{
					reg.v[0xF] = reg.v[instr.nibble.y] & 0b1;
					reg.v[instr.nibble.y] >>= 1;
					reg.v[instr.nibble.x] = reg.v[instr.nibble.y];
					break;
				}
				case 0x7:
				{
					int res = reg.v[instr.nibble.y] - reg.v[instr.nibble.x];
					reg.v[0xF] = res >= 0;
					reg.v[instr.nibble.x] = uint8_t(res);
					break;
				}
				case 0xE:
				{
					reg.v[0xF] = (reg.v[instr.nibble.y] & 0b1000'0000) >> 7;
					reg.v[instr.nibble.y] <<= 1;
					reg.v[instr.nibble.x] = reg.v[instr.nibble.y];
					break;
				}
				default:
				{
					printf("Not implemented 0x8");
					break;
				}
			}
			break;
		}
		case 0xA:
		{
			reg.l = instr.address;
			break;
		}
		case 0xC:
		{
			reg.v[instr.nibble.x] = (rand() % 256) & instr.byte.first;
			break;
		}
		case 0xD:
		{
			const auto& vx = reg.v[instr.nibble.x];
			const auto& vy = reg.v[instr.nibble.y];
			printf("Draw x: %d y: %d h: %d", vx, vy, instr.nibble.first);
			//size_t spriteIndex = (&memMap.memory[reg.l] - &(memMap.namedMemory.sprites[0][0])) / sizeof(std::array<uint8_t, 5>);

			//const std::array<uint8_t, 5>& sprite = memMap.namedMemory.sprites[spriteIndex];

			const auto memPtr = &memMap.memory[reg.l];


			bool pixelUnset = false;
			for (int y = 0; y < instr.nibble.first; y++)
			{
				for (int x = 0; x < 8; x++)
				{
					std::bitset<64*32>::reference pixel = pixels[x + vx + (y + vy) * 64];
					//bool newPixel = (sprite[y] & (1 << x)) >> x;

					const uint8_t spriteRow = *(memPtr + y);

					bool newPixel = (spriteRow & (0b1000'0000 >> x));
					if (!newPixel && pixel)
					{
						pixelUnset = true;
					}

					pixel = newPixel;
				}
			}
			reg.v[0xF] = pixelUnset;
			redraw(pixels);
			break;
		}
		case 0xF:
		{
			switch(instr.byte.first)
			{
				case 0x07:
				{
					reg.v[instr.nibble.x] = delayTimer;
					break;
				}
				case 0x15:
				{
					delayTimer = reg.v[instr.nibble.x];
					break;
				}
				case 0x1E:
				{
					reg.l += reg.v[instr.nibble.x];
					break;
				}
				case 0x29:
				{
					// Address of the sprite in the actual memory
					reg.l = uint16_t(&(memMap.namedMemory.sprites[instr.nibble.x][0]) - &(memMap.namedMemory.sprites[0][0]));
					break;
				}
				case 0x33:
				{
					const auto& vx = reg.v[instr.nibble.x];
					memMap.memory[reg.l + 0] = vx / 100;
					memMap.memory[reg.l + 1] = vx / 10;
					memMap.memory[reg.l + 2] = vx / 1;
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
