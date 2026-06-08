
#include <stdio.h>
#include "cpu.h"
extern uint8_t V[0xF]; 
int main()
{
	memory[0x0] = 0x31;
	memory[0x1] = 2;
	memory[0x2] = 4;
	PC = 0x0;
	execute();
}
