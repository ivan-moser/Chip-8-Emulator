#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include <string.h>
#include "cpu.h"

extern uint8_t fontset[80];

void load_fonts(chip8* vm);

#endif