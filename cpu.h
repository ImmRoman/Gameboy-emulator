
#ifndef CPU
#define CPU
#include <stdint.h>
    extern uint8_t memory[0xFFFF];
    extern uint16_t PC;
    extern uint16_t SP;
    /*
    registers
    V[0x0] -> B, V[0x1] -> C, V[0x2] -> D, V[0x3] -> E, V[0x4] -> H, V[0x5] -> L V[0x6] -> A, V[0x7] -> F

    r16 are just BC, DE, HL and AF(or SP, i'll see in implementation)
    */
    extern uint8_t V[0xF];
    extern uint16_t r16[0x4];
    typedef enum
    {BC,DE,HL,SP_} r16_t;

    typedef enum
    {B,C,D,E,H,L,A,F
    } r8_t;
    void execute();
    extern void p_registers();
	extern void p_line();
#endif
