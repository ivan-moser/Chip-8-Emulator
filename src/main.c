#include <stdint.h>
#include "..\include\cpu.h"

int main(void){
    chip8 vm = {0};

    loading();

    while(vm.running){
        cycle(&vm);
    }
    
    return 0;
}