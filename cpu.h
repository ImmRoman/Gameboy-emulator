
#ifndef CPU
#define CPU
#include <stdint.h>
    extern uint8_t memory[0xFFFF];
    extern uint16_t PC;
    extern uint16_t SP;
    extern uint8_t V[0xF]; //Registers
    extern uint16_t HL;
    void execute();
#endif
