#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "../include/cpu.h"

int main(void){
    chip8 vm = {0};
    srand(time(NULL));

    loading();

    uint64_t last_timer_update = get_time_ms();

    while(vm.running){
        cycle(&vm);

        uint64_t now = clock_gettime();

        if (now - last_timer_update >= 16) {
            if (vm.delay_timer > 0){
                vm.delay_timer --;
            }
            if (vm.sound_timer > 0){
                vm.sound_timer --;
            }
        }
    }
    
    return 0;
}