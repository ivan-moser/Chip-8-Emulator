clean:
	rm -f sdl_handlers.o errors.o font.o cpu.o errors.o load.o main.o main.exe

compile:
	gcc ./src/*.c -o main.exe -lSDL2

test:
	rm -f Testing/main.exe; gcc Testing/test.c -o test.exe; ./test.exe