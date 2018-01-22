#include "Cpu.h"
#include <stdio.h>

void Cpu::executeInstruction(uint16_t instruction)
{
	printf("%04X: ", instruction);
	uint8_t opcode = (instruction & 0xF000) >> 12;

	switch (opcode)
	{
		case 0x0:
		{
			switch (instruction)
			{
				case 0x00E0:
				{
					printf("not implemented: screen cleared");
					break;
				}
				case 0x00EE:
				{
					printf("not implemented: return");
					break;
				}
				default:
				{
					uint16_t address = instruction & 0x0FFF;
					this->pc = address;
					printf("Call %03X", address);
					break;
				}
			}
			break;
		}
		case 0x3:
		{
			if (vx[(instruction & 0x0F00) >> 8] == instruction & 0x00FF)
			{
				this->pc += 2;
			}
			break;
		}
		case 0x4:
		{
			if(vx[(instruction & 0x0F00) >> 8] != instruction & 0x00FF)
			{
				this->pc += 2;
			}
			break;
		}
		case 0x6:
		{
			vx[(instruction & 0x0F00) >> 8] = instruction & 0x00FF;
			break;
		}
		default:
		{
			printf("Not implemented", instruction);
			break;
		}
	}
	printf("\n");
}
