clean:
	rm -f cpu.o errors.o load.o main.o main.exe

compile:
	gcc ./src/cpu.c ./src/errors.c ./src/load.c ./src/main.c -o main.exe

test:
	rm -f Testing/main.exe; gcc Testing/test.c -o test.exe; ./test.exe