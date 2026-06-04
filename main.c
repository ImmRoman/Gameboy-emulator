
#include <stdio.h>
#include "cpu.h"

int main(){
	V[0x1] = 0;
	V[0x2] = 4;
	V[0x0] = 120;
	PC = 0x0;
	execute(0x1);
	printf("Final number in memory[%d]: %d", V[0x2],memory[0x4]);

}

