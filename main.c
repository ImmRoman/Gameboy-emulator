
#include <stdio.h>
#include "cpu.h"
extern uint8_t V[0xF];
extern uint8_t memory[0xFFFF];
int main()
{
	memory[0x0] = 10;
	memory[0x1501] = 0x60;
	V[0x0] = 0x15;
	V[0x1] = 1;
	V[0x2] = 7;
	PC = 0x0;
	execute();
	printf("memory[0x7] = %x\n", memory[0x17]);
	p_registers();
	printf("\nPC = %x | SP = %x",PC,r16[0x3]);
}


void test_0(){

}