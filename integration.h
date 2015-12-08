/**
 * Saulo Tadashi Iguei NºUsp 7573548
 *
 * DATA entrega limite: 08/11/15
 *
 * SCC0201_01 - ICC2 _ Prof. Moacir
 *
 * Trabalho 6: Xadrez - Parte 1 (Geração de movimentos)
 */

/*
 * Biblioteca para integração de definições e tipagens
 */
#ifndef __INTEGRATION_H_
#define __INTEGRATION_H_

//OBJETO
typedef struct objeto OBJETO;

//HASH
#define HASH_MAX 1024

//TABELA
#define TABLE_BLOCKS (8 * 8)
#define TABLE_ROWS 8
#define TABLE_COLS 8

//STRUCT FEN
//definições das dimensões de vetores estáticos
#define MAX_piece (64 + 7 + 1)
#define MAX_ROCK (4 + 1)
#define MAX_PASS (5 + 1)

#include "node.h"
#include "lista.h"

//estrutura para comportar os dados da notação FEN
typedef struct{
	char pieces[MAX_piece];
	char turn;
	char rock[MAX_ROCK];
	char pass[MAX_PASS];
	int halfTurn;
	int fullTurn;
	LIST *hash[HASH_MAX];

}FEN;

//estrutura para manipular ordenações e mudanças dados de endereços reais
typedef struct CONFLICT {
	OBJETO *obj;
	char **play; //endereço do ponteiro que guarda a jogada
	int order;
}CONFLICT;

//estrutura para armazenar comando de uma jogada
typedef struct{
	OBJETO *obj;
	int fromRow;
	int fromCol;
	char promotion;
}PLAY;

//PARAMETRO FUNÇÕES MOV
#define MOV_PARAM OBJETO *** const table, OBJETO * const obj, int *length, FEN* fen, char white, char black
#define MOV_VALUE table, obj, &size, fen, white, black

//FUNÇÕES MOVIMENTAÇÃO
typedef char** (*funcPtr) (MOV_PARAM);

#endif
