#include <stdio.h>
#include "cpu.h"
uint8_t memory[0xFFFF];
uint16_t PC;
uint16_t SP;
uint8_t V[0xF];



/* 16 bit registers
	initialize r16 copies from V[0xF] into the u16 r16 registers
	commit copies r16 changes into the correspective V[0xF]
*/
uint16_t r16[0x4];
extern enum r8_t;
extern enum r16_t;
void initialize_r16();
void commit_r16();

int line_bool = 1; //global variable for line printer
	
void execute(){
		
	uint8_t command = memory[PC];
	uint8_t imm8 = memory[PC + 1];
	uint16_t imm16 = memory[PC + 1];
	
	imm16 = (imm16 << 8) + memory[PC + 2];
	
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
					SP = imm16;
					break;
				}
				V[2*CN] = memory[PC+1];
				V[2*CN+1] = memory[PC+2];
				//we've read the imm16 so we go 2 steps ahead to the next instruction with the PC
				PC += 2;
			break;

			case 0x2:
			// ld [mem16], a
				if(CN > 1){
					//hl-
					initialize_r16();
					uint16_t addr = V[H];
					addr = (addr<<8) + V[L];
					memory[addr] = V[A];
					printf("BRO %x BRO",r16[HL]);
					if (CN == 2)
						r16[HL]--;
					else r16[HL]++;
					V[H] = r16[HL] >> 8;
					V[L] = r16[HL] & 0xFF;
					break;
				}

				uint16_t addr = V[2*CN];
				addr = (addr<<8) + V[2*CN+1];
				memory[addr] = V[A];
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
				SP = imm16;
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
	}

void initialize_r16(){
	for(int i = 0; i < 4 ; i++){
		r16[i] = V[2*i];
		r16[i] = (r16[i] << 8) + V[2*i + 1];
	}
}

void commit_r16(){
	for(int i = 0; i < 3 ; i++){
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
	printf("\nPC = %x | SP = %x", PC, r16[0x3]);
}

void p_line(int lenght){
	if(!line_bool)printf("\n");
	for(int i=0;i<lenght;i++)
		printf("-");
	if(line_bool)printf("\n");
	line_bool = (line_bool + 1)%2;
}