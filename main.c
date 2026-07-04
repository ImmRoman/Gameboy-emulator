
#include <stdio.h>
#include "cpu.h"
extern uint8_t V[0xF];
extern uint8_t memory[0xFFFF];
extern enum r8_t;
extern enum r16_t;
void test_b0_N2();
int main()
{
	test_b0_N2();
}

//Test Block 0 Nibble 2
//TODO SP BUG
// ld [r16mem], a
void test_b0_N2(){
	memory[0x0] = 0x22;
	V[H] = 0x90;	
	V[L] = 0x50;
	V[A] = 0x87;
	memory[0x9050] = 0x10;
	PC = 0x0;
	execute();
	printf("memory[r16] = %x\n", memory[0x9050]);
	p_registers();
}
//Test Block 0 Nibble 3
void test_b0_N3(){
	memory[0x0] = 0xA;
	V[B] = 0x90;	
	V[C] = 0x50;
	memory[0x9050] = 0x87;
	PC = 0x0;
	execute();
	printf("memory[r16] = %x\n", memory[0x9050]);
	p_registers();
}
