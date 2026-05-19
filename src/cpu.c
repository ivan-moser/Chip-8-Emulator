#include "../include/cpu.h"
#include "stdio.h"

#define PC_NEXT vm->PC+=2
#define PC_NEXT_X2 vm->PC+=4
#define RAM_SIZE (sizeof(vm->memory) / sizeof(uint8_t))


uint64_t get_time_ms() {
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC,  &ts);

    return ts.tv_sec * 1000ULL + 
           ts.tv_nsec / 1000000ULL;
}

uint16_t fetch(chip8* vm){
    uint16_t raw_code = (vm->memory[vm->PC] << 8) | vm->memory[vm->PC + 1];
    return raw_code;
}

instruction_t decode(uint16_t raw_code){
    instruction_t instruction = {
        .operation = (raw_code & 0xf000) >> 12,
        .value = (raw_code & 0x0fff)
    };
    return instruction;
}

void execute(instruction_t instruction, chip8* vm){
    operator_t opcode = instruction.operation;
    value_t value = instruction.value;

    uint8_t x = (value & 0xf00) >> 8;
    uint8_t y = (value & 0x0f0) >> 4;

    uint8_t N = value & 0x00f;
    uint16_t NN = value & 0x0ff;
    uint16_t NNN = value & 0xfff;

    switch(opcode){

        // Return / Clean screen
        case 0:
            if((N) == 0xE){
                PC_NEXT;
            } else if ((N) == 0) {
                // TODO: Clearing the screen
                PC_NEXT;
 //=============== TESTING ========================+
            }else if(NNN == 0xfff){              //|
                vm->running = false;             //|
                printf("PROGRAM ENDED BY USER"); //|
                break;                           //|
  //===============================================+
            } else {
                // INVALID ARGUMENT ERROR!
                vm->running=false;
                return;
            }                   
            break;
        
        // Jump 
        case 1:
            vm->PC = value;
            break;
        // TODO: Call a subroutine 
        case 2:
            break;
        // If not NN in vx
        case 3:
            if((NN) == vm->V[x]){
                PC_NEXT_X2;
            } else {
                PC_NEXT;
            }
            break;

        // If NN in vx
        case 4:
            if((NN) == vm->V[x]){
                PC_NEXT;
            } else {
                PC_NEXT_X2;
            }
            break;

        // If vx and vy content are different    
        case 5:
            if(vm->V[x] == vm->V[y]){
                PC_NEXT;
            } else {
                PC_NEXT_X2;
            }
            break;

        // Put NN in vx
        case 6:
            vm->V[x] = NN;
            PC_NEXT;
            break;
        
        // Add NN to vx
        case 7:
            vm->V[x] += NN;
            PC_NEXT;
            break;



        // TWO REGISTERS OPERATIONS
        case 8:
            switch (value & 0x00f)
            {
            // vx = vy
            case 0:
                vm->V[x] = vm->V[y];
                break;
            // BITWISE OR
            case 1:
                vm->V[x] = vm->V[x] | vm->V[y];
                break;
            // BITWISE AND
            case 2:
                vm->V[x] = vm->V[x] & vm->V[y];
                break;
            // BITWISE XOR
            case 3:
                vm->V[x] = vm->V[x] ^ vm->V[y];
                break;
            // SUM vx + vy  with vf = 1 on carry
            case 4:
                uint16_t sum = vm->V[x] + vm->V[y];
                if(!(sum > 255)){
                    vm->V[x] += vm->V[y];
                } else {
                    vm->V[0xf] = 1;
                    vm->V[x] = sum & 0xff;
                }
                break;
            // SUB vx - vy  with vf = 0 on borrow
            case 5:
                vm->V[0xf] = (vm->V[x] >= vm->V[y]) ? 1 : 0;
                vm->V[x] -= vm->V[y];
                break;;
            // Right shift
            case 6:
                vm->V[0xf] = vm->V[y] & 0x1;
                vm->V[x] = vm->V[y] >> 1; 
                break;
            // vy - vx  with borrow on vf
            case 7:
                vm->V[0xf] = (vm->V[y] >= vm->V[x]) ? 1 : 0;
                vm->V[x] = vm->V[y] - vm->V[x];
                break;
            // Left shift
            case 0xE:
                vm->V[0xf] = (vm->V[y] & 0x8) >> 7;
                vm->V[x] = vm->V[y] << 1;
                break;
            }
            PC_NEXT;
            break;

        // If vx == vy then
        case 9:
            if(vm->V[x] == vm->V[y]){
                PC_NEXT;
            } else {
                PC_NEXT_X2;
            }
            break;
        // Set the index register to NNN
        case 0xA:
            vm->I = NNN;
            PC_NEXT;
            break;
        // Jump to adress NNN + v0
        case 0xB:
            uint16_t addr = NNN + vm->V[0];
            if(addr >= RAM_SIZE){
                vm->running = false;
                // TODO: Out of memory ERROR
            } else {
                vm->PC = addr;
            }
            break;
        // vx = random(0, 255) & NN
        case 0xC:
            vm->V[x] = (rand() % 256) & NN;
            PC_NEXT;
            break;
        // TODO: Sprite collision
        case 0xD:
            break;
        // KEY PRESS HANDLER
        case 0xE:
            switch (N) {
                // If the key identified by vx
                case 0xE:
                    if(vm->keypad[vm->V[x]]) {
                        PC_NEXT_X2;
                    } else {
                        PC_NEXT;
                    }
                    break;
                // If NOT the key identified by vx
                case 0x1:
                    if(!vm->keypad[vm->V[x]]) {
                        PC_NEXT_X2;
                    } else {
                        PC_NEXT;
                    }
            }
            break;
        // Ambiental operations
        case 0xF:
            switch (NN){
                // Get the timer in vx
                case 0x07:
                    vm->V[x] = vm->delay_timer;
                    PC_NEXT;
                    break;
                // WAIT till a key is pressed
                case 0x0A:
                    for(uint16_t i = 0; i < 16; i++){
                        if(vm->keypad[i]){

                            vm->V[x] = i;
                            PC_NEXT;
                            break;
                        }
                    }
                // Set timer from vx
                case 0x15:
                    vm->delay_timer = vm->V[x];
                    PC_NEXT;
                    break;
                // Set sound timer from vx
                case 0x18:
                    vm->sound_timer = vm->V[x];
                    PC_NEXT;
                    break;
                // Add vx on i
                case 0x1E:
                    vm->I += vm->V[x];
                    PC_NEXT;
                    break;
                // TODO: I = Sprite adress  corrisponding to vx
                case 0x29:
                    break;
                case 0x33:
                    break;
                case 0x55:
                    break;
                case 0x65:
                    break;
            }
            break;
    }
}

uint16_t cycle(chip8* vm){
    uint16_t operation = fetch(vm);
    instruction_t instruction = decode(operation);
    execute(instruction, vm);

    return operation;
}