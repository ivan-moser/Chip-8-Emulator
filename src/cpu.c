#include "../include/cpu.h"
#include "../include/cpu.h"

#define PC_NEXT vm->PC+=2
#define PC_NEXT_X2 vm->PC+=4


uint16_t fetch(chip8* vm){
    uint8_t raw_code = (vm->memory[vm->PC] << 8) | vm->memory[vm->PC + 1];
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

    switch(opcode){

        // Return / Clean screen
        case 0:
            if((value & 0x00f) == 1110){
                PC_NEXT;
            } else if ((value & 0x00f) == 0000) {
                // TODO: Clearing the screen
                PC_NEXT;
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
            if(!(value & 0x0ff) == vm->V[value & 0xf00]){
                PC_NEXT_X2;
            } else {
                PC_NEXT;
            }
            break;

        // If NN in vx
        case 4:
            if((value & 0x0ff) == vm->V[value & 0xf00]){
                PC_NEXT;
            } else {
                PC_NEXT_X2;
            }
            break;

        // If vx and vy content are different    
        case 5:
            if(vm->V[value & 0xf00] == vm->V[value & 0x0f0]){
                PC_NEXT;
            } else {
                PC_NEXT_X2;
            }
            break;

        // Put NN in vx
        case 6:
            vm->V[value & 0xf00] = value & 0x0ff;
            PC_NEXT;
            break;
        
        // Add NN to vx
        case 7:
            vm->V[0xf00] += value & 0x0ff;
            PC_NEXT;
            break;



        // TWO REGISTERS OPERATIONS
        case 8:
            switch (value & 0x00f)
            {
            // vx = vy
            case 0:
                vm->V[value & 0xf00] = vm->V[value & 0x0f0];
                PC_NEXT;
                break;
            // BITWISE OR
            case 1:
                vm->V[value & 0xf00] | vm->V[value & 0x0f0];
                PC_NEXT;
                break;
            // BITWISE AND
            case 2:
                vm->V[value & 0xf00] & vm->V[value & 0x0f0];
                PC_NEXT;
                break;
            // BITWISE XOR
            case 3:
                vm->V[value & 0xf00] ^ vm->V[value & 0x0f0];
                PC_NEXT;
                break;
            // SUM vx + vy  with vf = 1 on carry
            case 4:
                uint16_t sum = vm->V[value & 0xf00] + vm->V[value & 0x0f0];
                if(!(sum > 255)){
                    vm->V[value & 0xf00] += vm->V[value & 0x0f0];
                } else {
                    vm->V[0xf] = 1;
                    vm->V[value & 0xf00] = sum & 0xff;
                }
                PC_NEXT;
                break;
            // SUB vx - vy  with vf = 0 on borrow
            case 5:
                vm->V[0xf] = (vm->V[value & 0xf00] >= vm->V[value & 0x0f0]) ? 1 : 0;
                vm->V[value & 0xf00] -= vm->V[value & 0x0f0];
                PC_NEXT;
                break;;
            }


        case 9:
            break;
        case 10:
            break;
        case 11:
            break;
        case 12:
            break;
        case 13:
            break;
        case 14:
            break;
        case 15:
            break;
    }
}

void cycle(chip8* vm){
    uint16_t operation = fetch(vm->memory);
    instruction_t instruction = decode(operation);
    execute(instruction, &vm);
}