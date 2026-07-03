
#ifndef CPU
#define CPU
#include <stdint.h>
    extern uint8_t memory[0xFFFF];
    extern uint16_t PC;
    extern uint8_t V[0xF]; //Registers
    extern uint16_t r16[0x4]; //16 bit registers
    void execute();
    extern void p_registers();
	extern void p_line();
#endif
