#include "../include/sdl_handlers.h"

#define WINDOW_WIDTH 64
#define WINDOW_HEIGTH 32
#define SCALE 10

void handle_input(chip8* vm){
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT){
            vm->running = false;
        }

        if(event.type == SDL_KEYDOWN) {

            uint8_t key = 0xFF;

            switch(event.key.keysym.sym) {

                case SDLK_1: key = 0x1; break;
                case SDLK_2: key = 0x2; break;
                case SDLK_3: key = 0x3; break;
                case SDLK_4: key = 0xC; break;

                case SDLK_q: key = 0x4; break;
                case SDLK_w: key = 0x5; break;
                case SDLK_e: key = 0x6; break;
                case SDLK_r: key = 0xD; break;

                case SDLK_a: key = 0x7; break;
                case SDLK_s: key = 0x8; break;
                case SDLK_d: key = 0x9; break;
                case SDLK_f: key = 0xE; break;

                case SDLK_z: key = 0xA; break;
                case SDLK_x: key = 0x0; break;
                case SDLK_c: key = 0xB; break;
                case SDLK_v: key = 0xF; break;
            }

            if(key != 0xFF){

                vm->keypad[key] = 1;

                // FX0A support
                if(vm->waiting_for_key){

                    vm->V[vm->waiting_register] = key;

                    vm->waiting_for_key = false;

                    vm->PC += 2;
                }
            }
        }

        if(event.type == SDL_KEYUP) {

            switch(event.key.keysym.sym) {

                case SDLK_1: vm->keypad[0x1] = 0; break;
                case SDLK_2: vm->keypad[0x2] = 0; break;
                case SDLK_3: vm->keypad[0x3] = 0; break;
                case SDLK_4: vm->keypad[0xC] = 0; break;

                case SDLK_q: vm->keypad[0x4] = 0; break;
                case SDLK_w: vm->keypad[0x5] = 0; break;
                case SDLK_e: vm->keypad[0x6] = 0; break;
                case SDLK_r: vm->keypad[0xD] = 0; break;

                case SDLK_a: vm->keypad[0x7] = 0; break;
                case SDLK_s: vm->keypad[0x8] = 0; break;
                case SDLK_d: vm->keypad[0x9] = 0; break;
                case SDLK_f: vm->keypad[0xE] = 0; break;

                case SDLK_z: vm->keypad[0xA] = 0; break;
                case SDLK_x: vm->keypad[0x0] = 0; break;
                case SDLK_c: vm->keypad[0xB] = 0; break;
                case SDLK_v: vm->keypad[0xF] = 0; break;
            }
        }
    }
}
void update_timers(chip8* vm){
    static uint32_t last_timer_update = 0;
    uint32_t now = SDL_GetTicks();

    if(now - last_timer_update >= 16) {

        if(vm->delay_timer > 0)
            vm->delay_timer--;

        if(vm->sound_timer > 0) {

            // BEEP
            vm->sound_timer--;
        }

        last_timer_update = now;
    }
}
void render(chip8* vm, SDL_Renderer* renderer){

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for(uint32_t y = 0; y < WINDOW_HEIGTH; y++){
        for(uint32_t x = 0; x < WINDOW_WIDTH; x++){
            uint32_t index = x + (y * WINDOW_WIDTH);

            if(vm->display[index]){

                SDL_Rect rect;

                rect.x = x * SCALE;
                rect.y = y * SCALE;

                rect.w = SCALE;
                rect.h = SCALE;

                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    SDL_RenderPresent(renderer);
}