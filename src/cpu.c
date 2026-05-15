#include "..\include\cpu.h"
#include "..\include\cpu.h"

#define PC_NEXT vm->PC+=2


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
        case 0:
            
            // broo
            // yooo
            if((value & 0x00f) == 1110){ //          RETURN
                PC_NEXT;
            } else if ((value & 0x00f) == 0000) { // CLEAN
                // TODO: Clearing the screen  <=============================================
                PC_NEXT;
            } else {
                // INVALID ARGUMENT ERROR!
                vm->running=false;
                return;
            }                   
            break;

        case 1:                 //                    JUMP 
            vm->PC = value;
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
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