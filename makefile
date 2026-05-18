clean:
	rm -f cpu.o errors.o load.o main.o main.exe

compile:
	gcc ./src/cpu.c ./src/errors.c ./src/load.c ./src/main.c -o main.exe