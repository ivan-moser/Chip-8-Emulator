#include "../include/load.h"
#include "../include/cpu.h"
#include <stdio.h>
#include <stdlib.h>

#define ROM_PATH "ROM/rom.ch8"

void loading(chip8* vm){
    FILE* rom = fopen(ROM_PATH, "rb");

    if(rom == NULL){
        vm->running = false;
        printf("\nFATAL ERROR - CAN NOT LOAD THE ROM\n");
        exit(-1);
    }

    fseek(rom, 0, SEEK_END);
    uint64_t size = ftell(rom);
    rewind(rom);

    if(size > sizeof(vm->memory) - 0x200){
        vm->running = false;
        printf("\nFATAL ERROR - ROM FILE IS BIGGER THAN 4096 bytes!\n");
        exit(-1);
    }

    size_t bytes_read = fread(vm->memory + 0x200, 1, size, rom);

    fclose(rom);

    printf("ROM SIZE: %lu\n", size);
    printf("BYTES READ: %lu\n", bytes_read);
}