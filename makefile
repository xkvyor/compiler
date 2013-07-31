CC=g++

all: compiler

compiler: main.o parser.o lexer.o
	$(CC) main.o parser.o lexer.o -o compiler.exe

lexer.o: lexer.cpp
	$(CC) -c lexer.cpp

parser.o: parser.cpp
	$(CC) -c parser.cpp

main.o: main.cpp
	$(CC) -c main.cpp

clean:
	rm *.o -f
	rm *.out -f
	rm *.exe -f