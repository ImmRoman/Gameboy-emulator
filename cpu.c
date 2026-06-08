#include <stdio.h>
#include "cpu.h"
uint8_t memory[0xFFFF];
uint16_t PC;
uint16_t SP;
uint8_t V[0xF];
uint16_t HL;

void execute(){

	//null
	
	uint8_t command = memory[PC];
	uint8_t imm8 = memory[PC + 1];
	
	uint16_t imm16 = memory[PC + 1];
	imm16 = (imm16 << 8) + memory[PC + 2];
	HL = V[0x5];
	HL = (HL << 8) + V[0x6];
	
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
					HL--;
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

					HL--;
					V[0x5] = HL >> 8;
					V[0x6] = HL & 0xF;
					break;
				}
				
				addr = (addr<<8) + V[2*CN+2];
				V[0] = memory[addr];
				//hl+
				if(CN == 2){
					HL++;	
					V[0x5] = HL >> 8;
					V[0x6] = HL & 0xF;

				}
			break;
			case 0x8:
				// ld [imm16], sp. EZ one
				SP = imm16;
			break;
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
	

}
