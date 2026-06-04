
#include <stdio.h>
#include "cpu.h"

int main(){
	memory[0x5] = 12;
	printf("OK %d", memory[0x5]);
}

