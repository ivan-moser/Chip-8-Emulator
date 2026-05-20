#include "../include/cpu.h"
#include "stdio.h"

#define PC_NEXT vm->PC+=2
#define PC_NEXT_X2 vm->PC+=4
#define RAM_SIZE (sizeof(vm->memory) / sizeof(uint8_t))
#define DISPLAY_SIZE 64 * 32


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
    uint8_t NN = value & 0x0ff;
    uint16_t NNN = value & 0xfff;

    switch(opcode){

        case 0:

            switch(value){
                // Clean screen
                case 0x0E0:
                    for(uint16_t i = 0; i < DISPLAY_SIZE; i++){
                        vm->display[i] = 0;
                    }
                    PC_NEXT;
                    break;
                // Return from a subroutine
                case 0x0EE:

                    vm->sp--;
                    vm->PC = vm->stack[vm->sp];
                    break;
            }
            break;

        // Jump 
        case 1:
            vm->PC = value;
            break;

        // Call a subroutine 
        case 2:
            vm->stack[vm->sp] = vm->PC + 0x2;
            vm->sp++;
            vm->PC = NNN; 
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
                PC_NEXT_X2;
            } else {
                PC_NEXT;
            }
            break;

        // If vx and vy content are different    
        case 5:
            if(vm->V[x] == vm->V[y]){
                PC_NEXT_X2;
            } else {
                PC_NEXT;
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
                
                vm->V[0xF] = (sum > 255);
                vm->V[x] = sum & 0xFF;

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
                vm->V[0xf] = (vm->V[y] & 0x80) >> 7;
                vm->V[x] = vm->V[y] << 1;
                break;
            }
            PC_NEXT;
            break;

        // If vx == vy then
        case 9:
            if(vm->V[x] == vm->V[y]){
                PC_NEXT_X2;
            } else {
                PC_NEXT;
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
            vm->V[0xF] = 0;
            uint8_t coord_x;
            uint8_t coord_y;

            for(uint8_t row = 0; row < N; row++){
                uint8_t spriteByte = vm->memory[vm->I + row];
                for(uint8_t col = 0; col < 8; col++){
                    if(spriteByte & (0x80 >> col)){
                        coord_x = vm->V[x] + col;
                        coord_y = vm->V[y] + row;

                        coord_x %= 64;
                        coord_y %= 32;

                        uint16_t index = coord_x + (coord_y * 64);

                        if(vm->display[index]){
                            vm->V[0xF] = 1;
                        }

                        vm->display[index] ^= 1;
                    }
                }
            }
            PC_NEXT;
            break;
        // KEY PRESS HANDLER
        case 0xE:

            switch(NN) {
                // skip next if key in VX is pressed
                case 0x9E:

                    if(vm->keypad[vm->V[x]]) {
                        PC_NEXT_X2;
                    } else {
                        PC_NEXT;
                    }

                    break;

                // skip next if key in VX is NOT pressed
                case 0xA1:

                    if(!vm->keypad[vm->V[x]]) {
                        PC_NEXT_X2;
                    } else {
                        PC_NEXT;
                    }

                    break;
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
                            return;                        
                        }
                    }
                    
                    return;

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

                // I = Sprite adress  corrisponding to vx
                case 0x29:
                    vm->I = 0x50 + (vm->V[x] * 5);
                    PC_NEXT;
                    break;

                // Assign to the adressies I, I+1, I+2, Centinaia, decine e unità contenute in vx(0-255)
                case 0x33:
                    vm->memory[vm->I] = vm->V[x] / 100;
                    vm->memory[vm->I + 1] = (vm->V[x] / 10) % 10;
                    vm->memory[vm->I + 2] = vm->V[x] % 10;
                    PC_NEXT;
                    break;

                // Save from v0 to vx into memory starting from I
                case 0x55:
                    
                    for(uint16_t i = 0; i <= x; i++){
                        vm->memory[vm->I + i] = vm->V[i];
                    }

                    PC_NEXT;
                    break;

                // Load I...X  to   v0...vx
                case 0x65:

                    for(uint16_t i = 0; i <= x; i++){
                        vm->V[i] = vm->memory[vm->I + i];
                    }
                    PC_NEXT;
                    break;
            }
            break;

        default:
            printf("Unknown opcode\n");
            vm->running = false;
        }
}

uint16_t cycle(chip8* vm){
    uint16_t operation = fetch(vm);
    instruction_t instruction = decode(operation);
    execute(instruction, vm);

    return operation;
}