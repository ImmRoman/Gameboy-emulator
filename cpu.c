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
void init_r16();
void commit_r16();

int line_bool = 1; //global variable for line printer
	
void execute(){
	ASSERT(PC < sizeof(memory));
	
	uint8_t command = memory[PC];
	uint8_t imm8 = memory[PC + 1];
	uint16_t imm16 = memory[PC + 2];
	
	imm16 = (imm16 << 8) + memory[PC + 1];
	
	if(command == 0){PC+=1;return;}

	switch(command >> 6){
		//BLOCK 1 pandocs
		case 0x0:
		// Central Nibble 
		uint8_t CN = (command & 0x30) >> 4;
		uint8_t CN3 = (command & 0x38) >> 3;
		uint16_t addr;
		/*
		-----------------
		Last byte switch
		------------------
		*/
			switch (command & 0xF)
		{
		case 0x1:
			// ld r16, imm16
			if (CN == 3)
			{
				SP = imm16;
				break;
			}
			V[2 * CN] = memory[PC + 1];
			V[2 * CN + 1] = memory[PC + 2];
			// we've read the imm16 so we go 2 steps ahead to the next instruction with the PC
			PC += 2;
			break;

			case 0x2:
			// ld [mem16], a
				if(CN > 1){
					//hl-
					init_r16();
					memory[r16[HL]] = V[A];
					if (CN == 2)
						r16[HL]--;
					else r16[HL]++;
					commit_r16();
					break;
				}

				addr = V[2*CN];
				addr = (addr<<8) + V[2*CN+1];
				memory[addr] = V[A];
			break;

			case 0xA:
			// ld  a, mem[r16]
			if(CN > 1){
				init_r16();
				V[A] = memory[r16[HL]];
				if(CN == 2)
					r16[HL]++;
				else r16[HL]--;
				commit_r16();
				break;
			}
			init_r16();
			V[A] = memory[r16[CN]];
			break;

			case 0x8:
				// ld [imm16], sp. EZ one
				SP = imm16;
			break;
			/*--------------------
			--- r16 Operations ---
			---------------------*/	
			case 0x3:
			// inc r16
			if(CN == 3){SP++;break;}
			init_r16();
			r16[CN] ++;
			commit_r16();
			break;

			// dec r16
			case 0xB:
			if(CN == 3){SP--;break;}
			init_r16();
			r16[CN] --;
			commit_r16();
			break;
			// add hl, r16
			case 0x9:
			if(CN == 3){SP += r16[HL];break;}
			init_r16();
			r16[HL] += r16[CN];
			commit_r16();
			break;
			}
		/*
		---------------------
		Last 3 bits switch
		---------------------
		*/
		switch(command & 0x7){
			// inc r8
			case 0x4:
			if(CN3 == 6){init_r16();r16[HL]++;commit_r16();break;}
			V[CN3] ++;
			break;
			// dec r8
			case 0x5:
			if(CN3 == 6){init_r16();r16[HL]--;commit_r16();break;}
			V[CN3] --;
			break;
			case 0x6:
			// ld r8, imm8
			if(CN3 == 6){init_r16();r16[HL] = imm8;commit_r16();break;}
			V[CN3] = imm8;
			PC++;
			break;
			

			//rotation
			case 0x7:
			switch (CN3)
			{
				case 0x0:
				//rlca rotate V[A] to the left and update the carry flag
					if(V[A] > 0x7F){
						V[F] = V[F] | 0x8;
					}
					V[A] = V[A] << 1;
					V[A] = V[A] | ((V[F] & 0x8) >> 3);
					break;

				case 0x1:
				//rrca opposite of previous
				if(V[A] & 0x1){
					V[F] |= 0x8;
				}
				V[A] = V[A] >> 1;
				V[A] = V[A] | ((V[F] & 0x8) << 4);
				break;

				case 0x2:
				//rla
				if(V[A] > 0x7F){
					V[A] = V[A] << 1;
					V[A] |= ((V[F] & 0x8) >> 3);
					V[F] |= 0x8;
					break;
				}
				V[A] = V[A] << 1;
				V[A] |= ((V[F] & 0x8) >> 3);
				V[F] &= 0xF7;
				break;
				
				case 0x3:
				//rra
				if(V[A] & 0x01){
					V[A] = V[A] >> 1;
					V[A] |= ((V[F] & 0x8) << 4);
					V[F] |= 0x8;
					break;
				}
				V[A] = V[A] >> 1;
				V[A] |= ((V[F] & 0x8) << 4);
				V[F] &= 0xF7;
				break;
				
				case 0x4:
				// daa

				break;

			default:
				break;
			}
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
	PC++;
	}

void init_r16(){
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
	int lines = 105;
	p_line(lines);
	printf("| ");
	for(int i=0; i<8;i++){
		printf("V[%d] = 0x%x | ",i,V[i]);
	}
	p_line(lines);
	printf("\nPC = %x | SP = %x", PC, SP);
}

void p_line(int lenght){
	if(!line_bool)printf("\n");
	for(int i=0;i<lenght;i++)
		printf("-");
	if(line_bool)printf("\n");
	line_bool = (line_bool + 1)%2;
}