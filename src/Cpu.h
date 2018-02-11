#include <stdint.h>
#include <array>
#include <stack>

struct Cpu
{
	std::array<uint8_t, 0x1000> memory;
	std::stack<uint16_t> stack;
	struct {
		uint16_t pc = 0x200;
		std::array<uint8_t, 16> v{};
		uint16_t l = 0;
	} reg;
	
	



	void executeInstruction(uint16_t instruction);
};