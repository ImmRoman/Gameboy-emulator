#include <stdio.h>
#include "cpu.h"
uint8_t memory[0xFFFF];
uint16_t PC;
uint8_t V[0xF];
uint16_t r16[0x4]; //16 bit registers
typedef enum {
	BC,DE,HL,SP
} r16_t;
void initialize_r16();
void update_r16();
int line_bool = 1; //global variable for line printer
	
void execute(){
		
	uint8_t command = memory[PC];
	uint8_t imm8 = memory[PC + 1];
	uint16_t imm16 = memory[PC + 1];
	
	imm16 = (imm16 << 8) + memory[PC + 2];
	initialize_r16();
	
	if(command == 0){PC+=1;return;}

	switch(command >> 6){
		//BLOCK 1 pandocs
		case 0x0:
		// Central Nibble 
		uint8_t CN = (command & 0x30) >> 4;
		// LOAD
		switch(command & 0xF) {
			case 0x1:
				//ld r16, imm16
				if(CN == 3){
					r16[SP] = imm16;
					break;
				}
				V[2*CN+1] = memory[PC+1];
				V[2*CN+2] = memory[PC+2];
				//we've read the imm16 so we go 2 steps ahead to the next instruction with the PC
				PC += 2;
			break;

			case 0x2:
			// ld [mem16], a
				if(CN == 3){
					//hl-
					uint16_t addr = V[0x5];
					addr = (addr<<8) + V[0x6];
					memory[addr] = V[0];
					r16[HL]--;
					V[0x5] = HL >> 8;
					V[0x6] = HL & 0xF;
					break;
				}
				
				uint16_t addr = V[2*CN+1];
				addr = (addr<<8) + V[2*CN+2];
				memory[addr] = V[0];
				//hl+
				if(CN == 2) V[0x6] += 1;
			break;

			case 0x10:
			addr = V[2*CN+1];
			// ld  a, mem[r16]
				if(CN == 3){
					//hl-
					addr = V[0x5];
					addr = (addr<<8) + V[0x6];
					V[0] = memory[addr];

					r16[HL]--;
					V[0x5] = r16[CN] >> 8;
					V[0x6] = r16[CN] & 0xF;
					break;
				}
				
				addr = (addr<<8) + V[2*CN+2];
				V[0] = memory[addr];
				//hl+
				if(CN == HL){
					r16[CN]++;	
					V[0x5] = r16[CN] >> 8;
					V[0x6] = r16[CN] & 0xF;

				}
			break;
			case 0x8:
				// ld [imm16], sp. EZ one
				r16[SP] = imm16;
			break;
			
			case 0x3:
			// inc r16
			r16[CN] ++;

		}	
		// BLOCK 2 pandocs
		case 0x1:

		break;
		
		// BLOCK 3
		case 0x2:
		
		break;
		
		// BLOCK 4
		case 0x3:
		
		break;

	}
	PC++;
	update_r16();
	}

void initialize_r16(){
	for(int i = 0; i < 4 ; i++){
		r16[i] = V[2*i];
		r16[i] = (r16[i] << 8) + V[2*i + 1];
	}
}

void update_r16(){
	for(int i = 0; i < 4 ; i++){
		V[2*i] = r16[i] >> 8;
		V[2*i + 1] = r16[i] & 0xFF;
	}
}

void p_registers(){
	int lines = 103;
	p_line(lines);
	for(int i=0; i<8;i++){
		printf("V[0x%d] = %x | ",i,V[i]);
	}
	p_line(lines);
}

void p_line(int lenght){
	if(!line_bool)printf("\n");
	for(int i=0;i<lenght;i++)
		printf("-");
	if(line_bool)printf("\n");
	line_bool = (line_bool + 1)%2;
}