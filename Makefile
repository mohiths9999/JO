CC=gcc

all: program1

program1: program1.o
	$(CC)  -o program1 program1.o -lm

program1.o: program1.c
	$(CC)  -c program1.c

execute:
	./program1 -m market_price.txt -p price_list.txt
clean:
	rm -f program1 program1.o
	rm output.txt




