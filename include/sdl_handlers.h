#ifndef SDL_HANDLERS_H
#define SDL_HANDLERS_H
#include <SDL2/SDL.h>
#include "cpu.h"
#include <stdint.h>

void handle_input(chip8* vm);
void update_timers(chip8* vm);
void render(chip8* vm, SDL_Renderer* renderer);

#endif