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
		uint8_t second : 4;
		uint8_t third : 4;
		uint8_t opcode : 4;
	} nibble;

	struct {
		const uint8_t operator[](size_t index) const {
			return (instr & (0x000F << index) >> index * 4);
		}
	private:
		uint16_t instr;
	} nibbles;

	uint16_t address : 12;
};

void Cpu::executeInstruction(uint16_t instruction)
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
					this->pc = instr.address;
					printf("Call %03X", instr.address);
					break;
				}
			}
			break;
		}
		case 0x3:
		{
			if (vx[instr.nibble.third] == instr.byte.first)
			{
				this->pc += 2;
			}
			break;
		}
		case 0x4:
		{
			if (vx[instr.nibble.third] != instr.byte.first)
			{
				this->pc += 2;
			}
			break;
		}
		case 0x6:
		{
			vx[instr.nibble.third] = instr.byte.first;
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
