
#include <stdio.h>
#include "cpu.h"
extern uint8_t V[0xF];
extern uint8_t memory[0xFFFF];
extern enum r8_t;
extern enum r16_t;
void test_b0_N11();
void test_b0_N3();
void test_b0_N2();
void test_b0_NA();
void test_b0_N4();
void test_b0_N5();
int main()
{
	test_b0_N5();
}

//Test Block 0 Nibble 2
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
	ASSERT(memory[0x9050] == 0x87);
}
//Test Block 0 Nibble A
void test_b0_NA(){
	memory[0x0] = 0xA;
	V[B] = 0x90;	
	V[C] = 0x50;
	memory[0x9050] = 0x87;
	PC = 0x0;
	execute();
	p_registers();
	ASSERT(V[A] == memory[0x9050]);
}
//Insert imm16 into SP
void test_b0_N8(){
	memory[0x0] = 0x8;
	memory[0x1] = 0x10;
	memory[0x2] = 0x9F;
	PC = 0x0;
	execute();
	p_registers();
	ASSERT(SP == 0x109F);
}

void test_b0_N3(){
	memory[0x0] = 0x3;
	V[B] = 0x0F;	
	V[C] = 0xFF;
	PC = 0x0;
	execute();
	p_registers();
	ASSERT(V[B] == 0x10);
	ASSERT(V[C] == 0x00);
}

void test_b0_N11(){
	memory[0x0] = 0xB;
	V[B] = 0x0F;	
	V[C] = 0xFE;
	PC = 0x0;
	execute();
	p_registers();
	ASSERT(V[B] == 0x0F);
	ASSERT(V[C] == 0xFD);
}

void test_b0_N4(){
	memory[0x0] = 0x4;
	V[B] = 0x0A;	
	PC = 0x0;
	execute();
	p_registers();
	ASSERT(V[B] == 0x0B);
}



void test_b0_N5(){
	memory[0x0] = 0x5;
	V[B] = 0x0A;	
	PC = 0x0;
	execute();
	p_registers();
	ASSERT(V[B] == 0x09);
}
