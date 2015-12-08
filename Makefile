all: objeto.o peca.o node.o lista.o hash.o regra.o FEN.o IA.o xadrez.o
	gcc -o chess objeto.o peca.o node.o lista.o hash.o regra.o FEN.o IA.o xadrez.o -g -pg -lm

all_test: objeto.o peca.o node.o lista.o hash.o regra.o FEN.o IA.o xadrez.o
	gcc -o chess_test objeto.o peca.o node.o lista.o hash.o regra.o FEN.o IA.o xadrez.o -g -pg -lm

xadrez.o: xadrez.c
	gcc -c xadrez.c -g -pg

objeto.o: objeto.c
	gcc -c objeto.c -g -pg
	
peca.o: peca.c
	gcc -c peca.c -g -pg
	
regra.o: regra.c
	gcc -c regra.c -g -pg -lm
	
FEN.o: FEN.c
	gcc -c FEN.c -g -pg
	
IA.o: IA.c
	gcc -c IA.c -g -pg	
	
node.o: node.c
	gcc -c node.c -g -pg
	
lista.o: lista.c
	gcc -c lista.c -g -pg
	
hash.o: hash.c
	gcc -c hash.c -g -pg

	
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all -v ./chess_test
	
run:
	./chess

clear:
	rm *.o chess chess_test Casos_FILES/chess_test