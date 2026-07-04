
#include <stdio.h>
#include "cpu.h"
extern uint8_t V[0xF];
extern uint8_t memory[0xFFFF];
extern enum r8_t;
extern enum r16_t;
int main()
{
	memory[0x0] = 50;
	V[H] = 0x90;	
	V[L] = 0x50;
	V[A] = 0x87;
	memory[0x9050] = 0x10;
	PC = 0x0;
	execute();
	printf("memory[r16] = %x\n", memory[0x9050]);
	p_registers();
}

//Test Block 0 Nibble 2
void test_b0_N2(){
	memory[0x0] = 50;
	V[H] = 0x90;	
	V[L] = 0x50;
	V[A] = 0x87;
	memory[0x9050] = 0x10;
	PC = 0x0;
	execute();
	printf("memory[r16] = %x\n", memory[0x9050]);
	p_registers();
}