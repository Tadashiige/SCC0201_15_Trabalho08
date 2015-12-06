/**
 * Saulo Tadashi Iguei NºUsp 7573548
 *
 * DATA entrega limite: 08/12/15
 *
 * SCC0201_01 - ICC2 _ Prof. Moacir
 *
 * Trabalho 6: Xadrez - Parte 1 (Geração de movimentos)
 *
 * >>>>> Trabalho 7: Xadrez -Parte 2 (Implementação de jogabilidade)
 */

/*
 * Biblioteca para tratamento da notação FEN
 */


#ifndef __FEN_H_
#define __FEN_H_

#include <stdlib.h>
#include <stdio.h>
#include "integration.h"
#include "objeto.h"
#include "peca.h"

funcPtr getFunctionType (char type);

//função irá extrair todas as peças da notação FEN
OBJETO ** extractTableFEN (char* pieces, int *tam, int *white);

//funções para extrair coluna e linha da peça segundo notação armazenada
int getObjectColumn (OBJETO *obj);
int getObjectRow (OBJETO *obj);
int getNotationColumn (char *not);
int getNotationRow (char *not);

//função irá adicionar notação para colisão de peças caso ocorra
char* collision (OBJETO *** const table, char *notation, OBJETO *const obj, int row, int col, int turn);

//função irá adicionar notação para colisão de peças após todas as análises de peças
void conflict (OBJETO **const collection, int size);

//****************** trabalho 7
//função irá criar a estrutura que comporta os dados da notação FEN
FEN* createFEN (char* string);

//função irá eliminar a estrutura fen da memória
void deleteFEN (FEN** fen);

//função irá atualizar a situação atual do tabuleiro e suas condições
FEN* updateFEN (FEN* fen, OBJETO *** const table, PLAY play);

//função irá imprimir a notação FEN
void printFEN (FEN *fen);

#endif
