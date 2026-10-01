
#include <stdio.h>
#include <string.h>
#include "cpu.h"
extern uint8_t V[0xF];
extern uint8_t memory[0x10000];
extern uint8_t get_flag(uint8_t FLAG);
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
	ASSERT(V[F] == C_FLAG);
}

void test_b0_NF(){
	memory[0x0]= 0xF;
	V[A] = 0x01;
	PC = 0x0;
	execute();
	ASSERT(V[A] == 0x80);
	ASSERT(V[F] == C_FLAG);
}
void test_b0_17(){
	memory[0x0]= 0x17;
	V[A] = 0x01;
	V[F] = C_FLAG;
	PC = 0x0;
	execute();
	ASSERT(V[A] == 0x03);
	ASSERT(V[F] == 0x00);
}

void test_b0_1F(){
	memory[0x0]= 0x1F;
	V[A] = 0x01;
	V[F] = C_FLAG;
	PC = 0x0;
	execute();
	ASSERT(V[A] == 0x80);
	ASSERT(V[F] == C_FLAG);
}
void test_add_hl_flag(){
	memory[0x0] = 0x9;
	PC = 0x0;
	V[H] = 0xFF;
	V[L] = 0xFA;
	V[B] = 0x0;
	V[C] = 0x08;
	execute();
	ASSERT(get_flag(C_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(V[L] == 0x2);		
}

void test_daa_0x27_1(){
	memory[0x0] = 0x27;
	PC = 0x0;
	V[A] = 0x6B;
	execute();
	ASSERT(V[A] == 0x71);

}


void test_daa_0x27_2(){
	memory[0x0] = 0x27;
	PC = 0x0;
	V[A] = 0xC4;
	execute();
	ASSERT(V[A] == 0x24);
	ASSERT(V[F] & C_FLAG);
}


void test_daa_0x27_3(){
	memory[0x0] = 0x27;
	PC = 0x0;
	V[A] = 0x11;
	V[F] = H_FLAG;
	execute();
	ASSERT(V[A] == 0x17);
}
void test_daa_0x27_4(){
	memory[0x0] = 0x27;
	PC = 0x0;
	V[A] = 0x0D;
	V[F] = H_FLAG;
	V[F] |= SUB_FLAG;
	execute();
	ASSERT(V[A] == 0x07);
}
void test_0x2F(){
	memory[0x0] = 0x2F;
	PC = 0x0;
	V[A] = 0xF8;
	execute();
	ASSERT(V[A] == 0x07);
}

void test_0x28(){
	memory[0x14] = 0x28;
	memory[0x15] = 0x74;
	PC = 0x14;
	V[F] = Z_FLAG;
	execute();
	ASSERT(PC == 0x74+0x14);
}

void test_0x38(){
	memory[0x0] = 0x38;
	memory[0x1] = 0x74;
	PC = 0x0;
	V[F] = C_FLAG;
	execute();
	ASSERT(PC == 0x74);
}


void test_ld_r8_r8_1(){
	//Test normal registers
	memory[0x0] = 0x42;
	PC = 0x0;
	V[B] = 0x40;
	V[D] = 0x70;
	execute();
	ASSERT(V[B] == 0x70);
}

void test_ld_r8_r8_2(){
	// Ld V[C], (HL)
	memory[0x0] = 0x4E;
	PC = 0x0;
	V[C] = 0x10;
	V[H] = 0x50;
	V[L] = 0x70;
	memory[0x5070] = 0x74;
	execute();
	ASSERT(V[C] == 0x74);
}

void test_ld_r8_r8_3(){
	// Ld (HL),D
	memory[0x0] = 0x72;
	PC = 0x0;
	V[D] = 0x33;
	V[H] = 0x50;
	V[L] = 0x70;
	execute();
	ASSERT(memory[0x5070]==0x33);
}
void test_rlca(){
	memory[0x15] = 0x07;
	PC = 0x15;
	V[A] = 0x8F;
	execute();
	ASSERT(V[A] == 0x1F);
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(get_flag(C_FLAG));

}
void test_add_a_r8(){
	// Add A,D
	memory[0x0] = 0x82;
	PC = 0;
	V[A] = 0x24;
	V[D] = 0x4F;
	execute();
	ASSERT(V[A] == 0x73);
	ASSERT(!get_flag(C_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));

}
void test_add_a_r8_z_flag(){
	// add A,D
	memory[0x0] = 0x82;
	PC = 0;
	V[A] = 0xFF;
	V[D] = 0x01;
	execute();
	ASSERT(V[A] == 0x00);
	ASSERT(get_flag(C_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));

}
void test_adc_a_r8(){
	// adc A,B
	memory[0x0] = 0x88;
	PC = 0;
	V[F] |= C_FLAG;
	V[A] = 0xFF;
	V[B] = 0x00;
	execute();
	ASSERT(V[A] == 0x00);
	ASSERT(get_flag(C_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
}
void test_adc_a_hl(){
	memory[0x0] = 0x8e;
	PC = 0;
	V[A] = 0x20;
	V[H] = 0x30;
	V[L] = 0x14;
	memory[0x3014] = 0x70;
	execute();
	ASSERT(V[A] == 0x90);
}
void test_sub_a_r8(){
	// sub A E
	memory[0x0] = 0x93;
	PC = 0;
	V[A] = 0x30;
	V[E] = 0x35;
	execute();
	ASSERT(V[A] == 0xFB);
	ASSERT(get_flag(C_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(get_flag(SUB_FLAG));
}
void test_sub_a_r8_z_flag(){
	// sub A E
	memory[0x0] = 0x93;
	PC = 0;
	V[A] = 0x30;
	V[E] = 0x30;
	execute();
	ASSERT(V[A] == 0x00);
	ASSERT(!get_flag(C_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(get_flag(Z_FLAG));
	ASSERT(get_flag(SUB_FLAG));
}
void test_sub_a_r8_hl(){
	// sub A HL
	memory[0x0] = 0x96;
	PC = 0;
	V[A] = 0x30;
	V[H] = 0x35;
	V[L] = 0x23;
	memory[0x3523] = 0x25;
	execute();
	ASSERT(V[A] == 0x0B);
	ASSERT(!get_flag(C_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(get_flag(SUB_FLAG));
}
void test_sbc_a_r8(){
	// sbc A C
	memory[0x0] = 0x99;
	PC = 0;
	V[A] = 0x30;
	V[C] = 0x35;
	V[F] |= C_FLAG;
	execute();
	ASSERT(V[A] == 0xFA);
	ASSERT(get_flag(C_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(get_flag(SUB_FLAG));
}
void test_sbc_a_r8_z_flag(){
	// sbc A D
	memory[0x0] = 0x9A;
	PC = 0;
	V[A] = 0x30;
	V[D] = 0x30;
	execute();
	ASSERT(V[A] == 0x00);
	ASSERT(!get_flag(C_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(get_flag(Z_FLAG));
	ASSERT(get_flag(SUB_FLAG));
}
void test_sbc_a_r8_hl(){
	// sbc A, HL
	memory[0x0] = 0x96;
	PC = 0;
	V[F] |= C_FLAG;
	V[A] = 0x30;
	V[H] = 0x35;
	V[L] = 0x23;
	memory[0x3523] = 0x24;
	execute();
	ASSERT(V[A] == 0x0C);
	ASSERT(!get_flag(C_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(get_flag(SUB_FLAG));
}
void test_and(){
	// and a,c
	memory[0x50] = 0xA1;
	PC = 0x50;
	V[A] = 0x08;
	V[C] = 0xF9;
	execute();
	ASSERT(V[A] == 0x08);
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));
}

void test_and_z(){
	// and a,c
	memory[0x50] = 0xA1;
	PC = 0x50;
	V[A] = 0x08;
	V[C] = 0x00;
	execute();
	ASSERT(V[A] == 0x00);
	ASSERT(get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));
}

void test_xor(){
	// xor a,b
	memory[0x50] = 0xA8;
	PC = 0x50;
	V[A] = 0x0F;
	V[B] = 0xF0;
	execute();
	ASSERT(V[A] == 0xFF);
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));
}

void test_xor_z(){
	// xor a,b
	memory[0x50] = 0xA8;
	PC = 0x50;
	V[A] = 0xF0;
	V[B] = 0xF0;
	execute();
	ASSERT(V[A] == 0x00);
	ASSERT(get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));
}
void test_or(){
	// or a,d
	memory[0x50] = 0xB2;
	PC = 0x50;
	V[A] = 0xF0;
	V[D] = 0xF0;
	execute();
	ASSERT(V[A] == 0xF0);
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));
}

void test_or_z(){
	// or a,d
	memory[0x50] = 0xB2;
	PC = 0x50;
	V[A] = 0x00;
	V[D] = 0x00;
	execute();
	ASSERT(V[A] == 0x00);
	ASSERT(get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));
}
void test_cp(){
	//cp a,c
	memory[0x32] = 0xB9;
	PC = 0x32;
	V[A] = 0x4A;
	V[C] = 0x5B;
	execute();
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(get_flag(SUB_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(get_flag(C_FLAG));
}
void test_cp_z_flag(){
	//cp a,c
	memory[0x32] = 0xB9;
	PC = 0x32;
	V[A] = 0x4A;
	V[C] = 0x4A;
	execute();
	ASSERT(get_flag(Z_FLAG));
	ASSERT(get_flag(SUB_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));
}
void test_add_a_imm8(){
	memory[0x00] = 0xC6;
	memory[0x01] = 0x23;
	V[A] = 0xFF;
	execute();
	ASSERT(V[A] == 0x22);
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(get_flag(C_FLAG));
}
void test_add_a_imm8_z_flag(){
	memory[0x00] = 0xC6;
	memory[0x01] = 0x01;
	V[A] = 0xFF;
	execute();
	ASSERT(V[A] == 0x00);
	ASSERT(get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(get_flag(C_FLAG));
}
void test_adc_a_imm8(){
	memory[0x00] = 0xCE;
	memory[0x01] = 0x23;
	V[A] = 0xFF;
	execute();
	ASSERT(V[A] == 0x22);
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(get_flag(C_FLAG));

	memory[0x02] = 0xCE;
	memory[0x03] = 0x00;
	V[F] |= C_FLAG;
	V[A] = 0xFF;
	execute();
	ASSERT(V[A] == 0x00);
	ASSERT(get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(get_flag(C_FLAG));
	ASSERT(PC == 0x4);
}
void test_sub_a_imm8(){
	memory[0x00] = 0xD6;
	memory[0x01] = 0x23;
	V[A] = 0xFF;
	execute();
	ASSERT(V[A] == 0xDC);
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(get_flag(SUB_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));

	memory[0x02] = 0xD6;
	memory[0x03] = 0x36;
	V[A] = 0x23;
	execute();
	ASSERT(V[A] == 0xED);
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(get_flag(SUB_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(get_flag(C_FLAG));

	memory[0x04] = 0xD6;
	memory[0x05] = 0x01;
	V[A] = 0x01;
	execute();
	ASSERT(V[A] == 0x00);
	ASSERT(get_flag(Z_FLAG));
	ASSERT(get_flag(SUB_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));
}
void test_sbc_a_imm8(){
	memory[0x00] = 0xDE;
	memory[0x01] = 0x22;
	V[F] |= C_FLAG;
	V[A] = 0xFF;
	execute();
	ASSERT(V[A] == 0xDC);
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(get_flag(SUB_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));

	memory[0x02] = 0xDE;
	memory[0x03] = 0x35;
	V[A] = 0x23;
	V[F] |= C_FLAG;
	execute();
	ASSERT(V[A] == 0xED);
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(get_flag(SUB_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(get_flag(C_FLAG));

	memory[0x04] = 0xDE;
	memory[0x05] = 0x00;
	V[F] |= C_FLAG;
	V[A] = 0x01;
	execute();
	ASSERT(V[A] == 0x00);
	ASSERT(get_flag(Z_FLAG));
	ASSERT(get_flag(SUB_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));
}
void test_and_a_imm8(){
	memory[0x00] = 0xE6;
	memory[0x01] = 0x07;
	V[A] = 0xF9;
	execute();
	ASSERT(V[A] == 0x01);
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));

	memory[0x02] = 0xE6;
	memory[0x03] = 0x00;
	V[A] = 0xF9;
	execute();
	ASSERT(V[A] == 0x00);
	ASSERT(get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));
}
void test_xor_a_imm8(){
	memory[0x00] = 0xEE;
	memory[0x01] = 0x07;
	V[A] = 0xF9;
	execute();
	ASSERT(V[A] == 0xFE);
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));

	memory[0x02] = 0xEE;
	memory[0x03] = 0x11;
	V[A] = 0x11;
	execute();
	ASSERT(V[A] == 0x00);
	ASSERT(get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));
}
void test_or_a_imm8(){
	memory[0x00] = 0xF6;
	memory[0x01] = 0x07;
	V[A] = 0xF9;
	execute();
	ASSERT(V[A] == 0xFF);
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));

	memory[0x02] = 0xF6;
	memory[0x03] = 0x11;
	V[A] = 0x11;
	execute();
	ASSERT(V[A] == 0x11);
	ASSERT(!get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));

	memory[0x04] = 0xF6;
	memory[0x05] = 0x00;
	V[A] = 0x00;
	execute();
	ASSERT(V[A] == 0x00);
	ASSERT(get_flag(Z_FLAG));
	ASSERT(!get_flag(SUB_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));
}
void test_cp_a_imm8(){
	memory[0x00] = 0xFE;
	memory[0x01] = 0xAB;
	V[A] = 0x9A;
	execute();
	ASSERT(V[A] == 0x9A);
	ASSERT(get_flag(SUB_FLAG));
	ASSERT(get_flag(H_FLAG));
	ASSERT(get_flag(C_FLAG));
	ASSERT(!get_flag(Z_FLAG));

	memory[0x02] = 0xFE;
	memory[0x03] = 0xAB;
	V[A] = 0xAB;
	execute();
	ASSERT(V[A] == 0xAB);
	ASSERT(get_flag(SUB_FLAG));
	ASSERT(!get_flag(H_FLAG));
	ASSERT(!get_flag(C_FLAG));
	ASSERT(get_flag(Z_FLAG));
}
void test_ret_cond(){
	//ret Z
	memory[0x00] = 0xC8;
	SP = 0x56;
	memory[0x56] = 0x21;
	memory[0x57] = 0x4A;
	V[F] |= Z_FLAG; 
	execute();
	ASSERT(PC == 0x4A21);
	ASSERT(SP == 0x58);

}
void test_reti(){
	//TODO
}
void test_jp_cond_imm16(){
	// jp nc imm16
	memory[0x00] = 0xD2;
	V[F] |= C_FLAG;
	execute();


	V[F] = 0;
	memory[0x03] = 0xD2;
	memory[0x04] = 0x33;
	memory[0x05] = 0xA5;
	execute();
	ASSERT(PC == 0xA533);
	
}
int test_suite(){
	clean();
	test_jp_cond_imm16();
	clean();
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
	clean();
	test_daa_0x27_1();
	clean();
	test_daa_0x27_2();
	clean();
	test_daa_0x27_3();
	clean();
	test_daa_0x27_4();
	clean();
	test_0x2F();
	clean();
	test_0x28();
	clean();
	test_0x38();
	clean();
	test_ld_r8_r8_1();
	clean();
	test_ld_r8_r8_2();
	clean();
	test_ld_r8_r8_3();
	clean();
	test_rlca();
	clean();
	test_add_a_r8();
	clean();
	test_add_a_r8_z_flag();
	clean();
	test_adc_a_hl();
	clean();
	test_adc_a_r8();
	clean();
	test_sub_a_r8_z_flag();
	clean();
	test_sub_a_r8_hl();
	clean();
	test_sub_a_r8();
	clean();
	test_sbc_a_r8_z_flag();
	clean();
	test_sbc_a_r8_hl();
	clean();
	test_sbc_a_r8();
	clean();
	test_and();
	clean();
	test_and_z();
	clean();
	test_xor();
	clean();
	test_xor_z();
	clean();
	test_or();
	clean();
	test_or_z();
	clean();
	test_cp();
	clean();
	test_cp_z_flag();
	clean();
	test_add_a_imm8();
	clean();
	test_add_a_imm8_z_flag();
	clean();
	test_adc_a_imm8();
	clean();
	test_sub_a_imm8();
	clean();
	test_sbc_a_imm8();
	clean();
	test_and_a_imm8();
	clean();
	test_xor_a_imm8();
	clean();
	test_or_a_imm8();
	clean();
	test_cp_a_imm8();
	clean();
	test_ret_cond();
	clean();
	test_reti();
	clean();
	return 0;
}


int main(){
	test_suite();
}

