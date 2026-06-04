#include <stdio.h>
#include "cpu.h"

void execute(uint8_t command){
		PC++;
		if(command == 0) return;
		uint8_t imm8 = memory[PC +1];
		uint16_t imm16 = memory[PC +1]<<8 + memory[PC];
		switch(command << 6){

		//BLOCK 1 pandocs
		case 0x0:

		// LOAD
		switch(command & 0xF)
			case 0x1:
			
		
			//we've read the imm16 so we go 2 steps ahead to the next instruction with the PC
			PC += 2
			break;	

		// BLOCK 2 pandocs
		case 0x1:

		break;
		
		// BLOCK 3
		case 0x2 
		
		break;
		
		// BLOCK 4
		case 0x3 
		
		break;

		}


}
