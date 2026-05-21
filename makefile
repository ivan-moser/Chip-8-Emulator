clean:
	rm -f sdl_handlers.o font.o cpu.o errors.o load.o main.o main.exe

compile:
	gcc ./src/*.c -o main.exe -lSDL2
