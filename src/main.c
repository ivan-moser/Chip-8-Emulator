#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../include/cpu.h"

int main(void){
    chip8 vm = {0};
    srand(time(NULL));

    //loading();

//===================== TESTING ============================
    uint8_t rom[] = {

        // V0 = 5
        0x60, 0x05,

        // V1 = 10
        0x61, 0x0A,

        // V0 += 3
        0x70, 0x03,

        // delay_timer = V0
        0xF0, 0x15,

        // V2 = delay_timer
        0xF2, 0x07,

        // jump to self (infinite loop)
        0x0f, 0xff
    };

    for (uint16_t i = 0; i < sizeof(rom); i++){
        vm.memory[0x200 + i] = rom[i];
    }

    vm.PC = 0x200;
    vm.running = true;
//============================================================


    uint64_t last_timer_update = get_time_ms();

    while(vm.running){
        uint16_t opcode = cycle(&vm);

        uint64_t now = get_time_ms();

        if (now - last_timer_update >= 16) {
            if (vm.delay_timer > 0){
                vm.delay_timer --;
            }
            if (vm.sound_timer > 0){
                // BEEP
                vm.sound_timer --;
            }
            last_timer_update = now;
        }
        printf("PC: %08b OPCODE: %04X\n", vm.PC, opcode);
    }

//==================== DEBUG ===================================================
    printf("\nREGISTERS: ");
    for(int i = 0; i < 16; i++){
        if(vm.V[i] != 0){
            printf("[\033[0;32m%04x\033[0m]", vm.V[i]);
        } else {
            printf("[%d]", vm.V[i]);
        }

    }
    printf("\nMEMORY: \n");
    for(int i = 0x200; i < 550; i++){
        if(vm.memory[i] != 0){
            printf("[&0x%04x> \033[0;32m%04x\033[0m]\n", i, vm.memory[i]);
        }else {
            printf("[&0x%04x> [%04x]\n", i , vm.memory[i]);
        }
    }
//==============================================================================
    
    return 0;
}