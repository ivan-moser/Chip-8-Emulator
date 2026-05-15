#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "cpu.h"

typedef uint8_t operator_t;
typedef uint16_t value_t; 

typedef struct {
    uint8_t memory[4095];
    uint8_t V[16];
    uint16_t PC;

    bool running;
} chip8;

typedef struct {
    operator_t operation;
    value_t value;
}instruction_t;

// fetch the program to find the raw instructions (2 bytes skip)
// Use the program counter (PC)
// returns: opcode;
uint16_t fetch();

// Takes each opcode to read it, check 
// the operation / value and registers
// return: instruction
instruction_t decode(uint16_t opcode);

// Takes each instruction, execute the 
// it and update the PC / check if
// the program have to end 
void execute(instruction);

void cycle();

#endif