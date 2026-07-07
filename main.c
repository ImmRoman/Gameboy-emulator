
#include <stdio.h>
#include <string.h>
#include "cpu.h"
extern uint8_t V[0xF];
extern uint8_t memory[0xFFFF];
void test_b0_N11();
void test_b0_N3();
void test_b0_N2();
void test_b0_NA();
void test_b0_N4();
void test_b0_N5();
void test_b0_N9();
void test_b0_N6();


void clean(){
	memset(V, 0, sizeof(V));
    memset(memory, 0, sizeof(memory));
	PC = 0;
	SP = 0;
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
	ASSERT(V[A] == memory[0x9050]);
}
//Insert imm16 into SP
void test_b0_N8(){
	memory[0x0] = 0x8;
	memory[0x1] = 0x10;
	memory[0x2] = 0x9F;
	PC = 0x0;
	execute();
	ASSERT(SP == 0x109F);
}

void test_b0_N3(){
	memory[0x0] = 0x3;
	V[B] = 0x0F;	
	V[C] = 0xFF;
	PC = 0x0;
	execute();
	ASSERT(V[B] == 0x10);
	ASSERT(V[C] == 0x00);
}

void test_b0_N11(){
	memory[0x0] = 0xB;
	V[B] = 0x0F;	
	V[C] = 0xFE;
	PC = 0x0;
	execute();
	ASSERT(V[B] == 0x0F);
	ASSERT(V[C] == 0xFD);
}

void test_b0_N4(){
	memory[0x0] = 0x4;
	V[B] = 0x0A;	
	PC = 0x0;
	execute();
	ASSERT(V[B] == 0x0B);
}
void test_b0_N4_2(){
	memory[0x0] = 0x34;
	V[H] = 0x0F;
	V[L] = 0xFF;	
	PC = 0x0;
	execute();
	ASSERT(V[H] == 0x10);
	ASSERT(V[L] == 0x00);
}

void test_b0_N5(){
	memory[0x0] = 0x5;
	V[B] = 0x0A;	
	PC = 0x0;
	execute();
	ASSERT(V[B] == 0x09);
}
void test_b0_N5_2(){
	memory[0x0] = 0x35;
	V[H] = 0x10;
	V[L] = 0x00;	
	PC = 0x0;
	execute();
	ASSERT(V[H] == 0x0F);
	ASSERT(V[L] == 0xFF);
}
void test_b0_N9(){
	memory[0x0] = 0x9;
	V[H] = 0x0A;	
	V[L] = 0xFF;
	V[C] = 0x01;
	PC = 0x0;
	execute();
	ASSERT(V[H] == 0x0B);
	ASSERT(V[L] == 0x00);
}

void test_b0_N6(){
	memory[0x0]= 0x36;
	memory[0x1]= 0xF8;
	V[H] = 0x10;
	PC = 0x0;
	execute();
	ASSERT(V[H] == 0x00);
	ASSERT(V[L] == 0xF8);
}

void test_b0_N7(){
	memory[0x0]= 0x7;
	V[A] = 0x90;
	PC = 0x0;
	execute();
	ASSERT(V[A] == 0x21);
	ASSERT(V[F] == 0x08);
}

void test_b0_NF(){
	memory[0x0]= 0xF;
	V[A] = 0x01;
	PC = 0x0;
	execute();
	ASSERT(V[A] == 0x80);
	ASSERT(V[F] == 0x08);
}
void test_b0_17(){
	memory[0x0]= 0x17;
	V[A] = 0x01;
	V[F] = 0x08;
	PC = 0x0;
	execute();
	ASSERT(V[A] == 0x03);
	ASSERT(V[F] == 0x00);
}

void test_b0_1F(){
	memory[0x0]= 0x1F;
	V[A] = 0x01;
	V[F] = 0x08;
	PC = 0x0;
	execute();
	ASSERT(V[A] == 0x80);
	ASSERT(V[F] == 0x08);
}
void test_add_hl_flag(){
	memory[0x0] = 0x9;
	PC = 0x0;
	V[H] = 0xFF;
	V[L] = 0xFA;
	V[B] = 0x0;
	V[C] = 0x08;
	execute();
	ASSERT(V[F] & 0x10);
	ASSERT(V[L] == 0x2);		

}

	int main(){
		
	test_b0_N6();
	clean();
	test_b0_N9();
	clean();
	test_b0_N4_2();	
	clean();
	test_b0_N5_2();
	clean();
	test_b0_N7();
	clean();
	test_b0_NF();
	clean();
	test_b0_17();
	clean();
	test_b0_1F();
	clean();
	test_add_hl_flag();
}
