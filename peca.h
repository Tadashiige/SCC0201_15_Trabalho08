/**
 * Saulo Tadashi Iguei NºUsp 7573548
 *
 * DATA entrega limite: 08/12/15
 *
 * SCC0201_01 - ICC2 _ Prof. Moacir
 *
 * Trabalho 6: Xadrez - Parte 1 (Geração de movimentos)
 * >>>>> Trabalho 7: Xadrez -Parte 2 (Implementação de jogabilidade)
 */

/*
 * Biblioteca para manipulação de peças
 */

#ifndef __PECA_H_
#define __PECA_H_

#include <stdlib.h>
#include <stdio.h>
#include "integration.h"
#include "objeto.h"

//funções buscam todos os possíveis movimentos de uma dada peça
char** movPeasant (MOV_PARAM);
char** movKnight (MOV_PARAM);
char** movBishop (MOV_PARAM);
char** movRook (MOV_PARAM);
char** movQueen (MOV_PARAM);
char** movKing (MOV_PARAM);

//funções buscam e imprimem todos os possíveis movimentos de uma dada peça
void printListMovPeasant (MOV_PARAM);
void printListMovKnight (MOV_PARAM);
void printListMovBishop (MOV_PARAM);
void printListMovRook (MOV_PARAM);
void printListMovQueen (MOV_PARAM);
void printListMovKing (MOV_PARAM);

//*********************** trabalho 07

//Função irá atualizar a coleção de peças segundo a jogada anterior
OBJETO ** updateCollection (OBJETO ** collection, int *white_pieces, int *pieces_num, int turn);

//efetivar a jogada na matriz de tabuleiros, modificando outras peças quando necessário, e desativando peças capturadas
OBJETO ** doPlay(OBJETO *** table, PLAY play, OBJETO ** collection, int white_pieces, int pieces_num, int fullTurn);

// ********************** trabalho 08

char** movPeasantAttack (OBJETO ***const table, OBJETO *const obj, int *length);

#endif
