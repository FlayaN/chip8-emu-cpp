#include <stdint.h>
#include <array>
#include <stack>
#include <bitset>

struct Cpu
{
	union
	{
		struct {
			std::array<std::array<uint8_t, 5>, 16> sprites;
			std::array<uint8_t, 0x200 - (5 * 16)> dummyInterpArea;
			std::array<uint8_t, 0x800> romData;
		} namedMemory;
		
		std::array<uint8_t, 0x1000> memory;
	} memMap;

	std::stack<uint16_t> stack{};
	struct {
		uint16_t pc = 0x200;
		std::array<uint8_t, 16> v{};
		uint16_t l = 0;
	} reg;

	uint8_t delayTimer = 0;
	
	void redraw(const std::bitset<64 * 32>& pixels) const;
	void executeInstruction(uint16_t instruction, std::bitset<64 * 32>& pixels);
};