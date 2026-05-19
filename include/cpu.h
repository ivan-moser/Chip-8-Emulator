#ifndef CPU_H
#define CPU_H

#define _POSIX_C_SOURCE 199309L

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>


typedef uint8_t operator_t;
typedef uint16_t value_t;

typedef struct {
    uint8_t memory[4095];
    uint8_t V[16];
    uint16_t stack[16];
    uint8_t keypad[16];  // 1-F  values of 0/1 

    uint8_t display[64 * 32];

    uint8_t sp;
    uint16_t I;
    uint16_t PC;

    uint8_t delay_timer;
    uint8_t sound_timer;

    bool running;
} chip8;

typedef struct {
    operator_t operation;
    value_t value;
}instruction_t;

// Get the clock time in ms
uint64_t get_time_ms();

// fetch the program to find the raw instructions (2 bytes skip)
// Use the program counter (PC)
// returns: opcode;
uint16_t fetch(chip8* vm);

// Takes each opcode to read it, check 
// the operation / value and registers
// return: instruction
instruction_t decode(uint16_t opcode);

// Takes each instruction, execute the 
// it and update the PC / check if
// the program have to end 
void execute(instruction_t instruction, chip8* vm);

uint16_t cycle(chip8* vm);

#endif