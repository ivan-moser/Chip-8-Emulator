#include "../include/sdl_handlers.h"

#define WINDOW_WIDTH 64
#define WINDOW_HEIGTH 32
#define SCALE 10

int map_key(SDL_Keycode key){
    switch (key)
    {
        case SDLK_1: return 0x1;
        case SDLK_2: return 0x2;
        case SDLK_3: return 0x3;
        case SDLK_4: return 0xC;

        case SDLK_q: return 0x4;
        case SDLK_w: return 0x5;
        case SDLK_e: return 0x6;
        case SDLK_r: return 0xD;

        case SDLK_a: return 0x7;
        case SDLK_s: return 0x8;
        case SDLK_d: return 0x9;
        case SDLK_f: return 0xE;

        case SDLK_z: return 0xA;
        case SDLK_x: return 0x0;
        case SDLK_c: return 0xB;
        case SDLK_v: return 0xF;

        default:
            return -1;
    }
}

void handle_input(chip8* vm){
    SDL_Event event;

while (SDL_PollEvent(&event))
{
    if (event.type == SDL_QUIT){
        vm->running = false;
    }

    if (event.type == SDL_KEYDOWN){
        int chip8_key = map_key(event.key.keysym.sym);

        if(chip8_key != -1){
            vm->keypad[chip8_key] = 1;

            printf("CHIP-8 KEY: 0x%X\n", chip8_key);

            if (vm->waiting_for_key)
            {
                vm->V[vm->waiting_register] = chip8_key;
                vm->waiting_for_key = false;
            }
        }
    }

            if (event.type == SDL_KEYUP){
            int chip8_key = map_key(event.key.keysym.sym);

            if(chip8_key != -1)
            {
                vm->keypad[chip8_key] = 0;
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