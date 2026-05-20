#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "../include/cpu.h"
#include "../include/load.h"
#include "../include/font.h"
#include "../include/sdl_handlers.h"

int main(void){
    chip8 vm = {0};
    srand(time(NULL));

    vm.running = true;
    vm.PC = 0x200;
    vm.waiting_for_key = true;

    load_fonts(&vm);
    loading(&vm);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    while(vm.running){

        handle_input(&vm);
        uint16_t opcode = cycle(&vm);
        update_timers(&vm);
        render(&vm, renderer);
        
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