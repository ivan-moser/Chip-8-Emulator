#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "../include/cpu.h"
#include "../include/load.h"
#include "../include/font.h"

int main(void){
    chip8 vm = {0};
    srand(time(NULL));

    vm.running = true;
    vm.PC = 0x200;

    load_fonts(&vm);
    loading(&vm);

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
        printf("PC: 0x%04x OPCODE: %04X\n", vm.PC, opcode);
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
    for(int i = 0x50; i < 100; i++){
        if(vm.memory[i] != 0){
            printf("[&0x%04x> \033[0;32m%04x\033[0m]\n", i, vm.memory[i]);
        }else {
            printf("[&0x%04x> [%x]\n", i , vm.memory[i]);
        }
    }
//==============================================================================
    
    return 0;
}