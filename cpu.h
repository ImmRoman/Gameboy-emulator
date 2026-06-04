
#ifndef CPU
#define CPU
#include <stdint.h>
extern uint8_t memory[0xFFFF];
extern uint16_t PC;
extern uint16_t SP;
void execute(uint8_t command); //to revisit
extern uint8_t V[0xF]; //Registers
extern uint16_t HL;
#endif
