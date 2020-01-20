all: game

fonctionTran.o: fonctionTran.c
	gcc -Wall -c fonctionTran.c

clientAPI.o: clientAPI.c
	gcc -Wall -c clientAPI.c

snakeAPI.o: snakeAPI.c
	gcc -Wall -c snakeAPI.c

main.o: main.c
	gcc -Wall -c main.c

game: fonctionTran.o clientAPI.o snakeAPI.o main.o
	gcc -Wall -o game fonctionTran.o clientAPI.o snakeAPI.o main.o
