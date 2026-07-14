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
void init_r16();
void commit_r16();

int line_bool = 1; //global variable for line printer

void set_flag(uint8_t FLAG){
	V[F] |= FLAG; 
}

void clear_flag(uint8_t FLAG){
	V[F] &= ~FLAG;
}

uint8_t get_flag(uint8_t FLAG){
	return (V[F] & FLAG) != 0;
}
uint8_t cond_jump(uint8_t NN){
	uint8_t Z = get_flag(Z_FLAG);
	uint8_t C = get_flag(C_FLAG);
	if(NN==0) return !Z;
	if(NN==1) return Z;
	if(NN==2) return !C;
	if(NN==3) return C;
	printf("WARNING passed to cond_jump a NN != {0..3}");
	return -1;	
}

void execute(){
	ASSERT(PC < sizeof(memory));
	
	uint8_t command = memory[PC];
	uint8_t imm8 = memory[PC + 1];
	uint16_t imm16 = memory[PC + 2];
	int tmp;
	
	imm16 = (imm16 << 8) + memory[PC + 1];
	
	if(command == 0){PC+=1;return;}

		// Central Nibble 
	uint8_t CN = (command & 0x30) >> 4;
	// Central and right 3 digit Nibble
	uint8_t CN3 = (command & 0x38) >> 3;
	uint8_t RN3 = (command & 0x07);
	uint16_t addr;
	switch(command >> 6){
		//BLOCK 0 pandocs
		case 0x0:
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
			clear_flag(SUB_FLAG);
			if(CN == 3){SP++;break;}
			init_r16();
			if((r16[CN] & 0xFF00) == 0xFF00){set_flag(H_FLAG);}
			r16[CN] ++;
			commit_r16();
			break;

			// dec r16
			case 0xB:
			set_flag(SUB_FLAG);
			if(CN == 3){SP--;break;}
			init_r16();
			if((r16[CN] & 0xFF00) == 0x0000){set_flag(H_FLAG);}
			r16[CN] --;
			commit_r16();
			break;

			// add hl, r16
			case 0x9:
			clear_flag(SUB_FLAG);
			if(CN == 3){
				if((SP & 0xFFF) + (r16[HL] & 0xFFF) > 0xFFF){set_flag(H_FLAG);}
				SP += r16[HL];
				break;
			}
			init_r16();
			if((r16[HL] & 0xFFF) + (r16[CN] & 0xFFF) > 0xFFF){set_flag(H_FLAG);}
			tmp = r16[HL];
			r16[HL] += r16[CN];
			if(r16[HL] < tmp) set_flag(C_FLAG);
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
			clear_flag(SUB_FLAG);				
			if(CN3 == 6){
				init_r16();
				if((r16[HL] & 0xFFF) == 0xFFF){set_flag(H_FLAG);}
				r16[HL]++;
				commit_r16();
				break;
			}
			if(V[CN3] == 0xFF){set_flag(C_FLAG);}
			if((V[CN3] & 0xF) == 0xF){set_flag(H_FLAG);}
			V[CN3] ++;
			if(V[CN3] == 0x00){set_flag(Z_FLAG);}
			break;
			// dec r8
			case 0x5:
			// no flag set for r16, only for r8
			if(CN3 == 6){init_r16();
				if((r16[HL] & 0xFFF) == 0x000){set_flag(H_FLAG);}
				r16[HL]--;
				commit_r16();
				break;
			}
			if(V[CN3]==0){set_flag(Z_FLAG);}
			if((V[CN3] & 0xF) == 0x0){set_flag(H_FLAG);}
			V[CN3] --;
			if(V[CN3] == 0x00){set_flag(Z_FLAG);}
			break;
			case 0x6:
			// ld r8, imm8
			if(CN3 == 6){init_r16();r16[HL] = imm8;commit_r16();break;}
			V[CN3] = imm8;
			PC++;
			break;
			
			//jumps
			case 0x0:
			if(CN3 == 0x2){
				//stop
				return;
			}
			// Bring PC to next instruction, ready if condition is false
			PC+=2;
			
			if(CN3 == 0x3){
				//jr imm8
				PC += (int8_t)imm8;
				return;
			}
			else{
				// minus 0x4 to remove the MSB of CN3 
				if(cond_jump(CN3 - 0x4)){
				//I don't want to increase PC again at the end of the switch
					PC += (int8_t)imm8;
					return;
				}
				//Need this return so we don't PC+=3 skipping an instruction
				return;
			}
			break;
			//rotations
			case 0x7:
			switch (CN3)
			{
				case 0x0:
				//rlca rotate V[A] to the left and update the carry flag
					if(V[A] > 0x7F){
						set_flag(C_FLAG);
					}
					V[A] = V[A] << 1;
					V[A] = V[A] | get_flag(C_FLAG);

					break;

				case 0x1:
				//rrca opposite of previous
				if(V[A] & 0x1){
					set_flag(C_FLAG);
				}
				V[A] = V[A] >> 1;
				V[A] = V[A] | (get_flag(C_FLAG) << 7);
				break;

				case 0x2:
				//rla
				if(V[A] > 0x7F){
					V[A] = V[A] << 1;
					V[A] |= get_flag(C_FLAG);
					
					break;
				}
				V[A] = V[A] << 1;
				V[A] |= get_flag(C_FLAG);
				// set carry flag to 0
				clear_flag(C_FLAG);
				break;
				
				case 0x3:
				//rra
				if(V[A] & 0x01){
					V[A] = V[A] >> 1;
					V[A] |= get_flag(C_FLAG) << 7;
					set_flag(C_FLAG);
					break;
				}
				V[A] = V[A] >> 1;
				V[A] |= get_flag(C_FLAG) << 7;
				clear_flag(C_FLAG);
				break;
				
				case 0x4:
				// daa
				tmp = V[A];
				if(get_flag(SUB_FLAG)){
					if(get_flag(H_FLAG)){
						V[A] -= 0x6;
					}
					if(get_flag(C_FLAG)){
						V[A] -= 0x60;
					}
					//MAY BE WRONG, CHECK IF IT'S CORRECT TO COMMENT
					// if(V[A] > tmp){set_flag(C_FLAG);}
				}
				else{
					if(get_flag(H_FLAG) || (V[A] & 0xF) > 0x9){
						V[A] += 0x6;
					}
					if(get_flag(C_FLAG) || V[A] > 0x9F){
						V[A] += 0x60;
					}
					if(V[A] < tmp){set_flag(C_FLAG);}
				}
				if(V[A] == 0x00){set_flag(Z_FLAG);}
				else{clear_flag(Z_FLAG);}
				clear_flag(H_FLAG);
				break;
			
			case 0x5:
			// cpl
			set_flag(SUB_FLAG);
			set_flag(H_FLAG);
			V[A] = ~V[A];
			break;
			case 0x6:
			//scf
			set_flag(C_FLAG);
			clear_flag(SUB_FLAG);
			clear_flag(H_FLAG);
			break;
			case 0x7:
			//ccf
			if(get_flag(C_FLAG))clear_flag(C_FLAG);else set_flag(C_FLAG);
			clear_flag(SUB_FLAG);
			clear_flag(H_FLAG);	
			default:
				break;
			}
			break;
		}
		break;
		// BLOCK 1 pandocs
		case 0x1:
		// halt
		if(command == 0x76){return;}

		//ld r8,r8 Implementazione un po' da sborone ma non mi interessa ahahah
		init_r16();
		uint8_t* dest = CN3 == 6? &memory[r16[HL]] : &V[CN3];
		uint8_t* src = RN3 == 6? &memory[r16[HL]] : &V[RN3];
		*dest = *src;
		break;
		
		// BLOCK 2
		case 0x2:
		
		break;
		
		// BLOCK 3
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