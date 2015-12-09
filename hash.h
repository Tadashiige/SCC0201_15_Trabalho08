/**
 * 								Saulo Tadashi Iguei NºUsp 7573548
 * 							_______________________________________
 *
 *																**************************************
 *																*									 *
 * 																*	DATA entrega limite: 08/12/15	 *
 *																*									 *
 * 																*	SCC0201_01 - ICC2 _ Prof. Moacir *
 * 																*									 *
 * 																**************************************
 *
 *	 					Trabalho 6: Xadrez - Parte 1 (Geração de movimentos)
 *
 * 				>>>>> Trabalho 7: Xadrez -Parte 2 (Implementação de jogabilidade)
 *
 * 			>>>>> >>>>> Trabalho 8: Xadrez - Parte 3 (Implementação de Inteligência Articial)
 */

/*
 * Biblioteca para funcionalidade de Hashing
 */

#ifndef __HASH_H_
#define __HASH_H_

#include "integration.h"



//gerar código hash
unsigned long long int genCode (OBJETO *** const table, FEN *fen);

//inserir o código hash ou retornar a quantidade de ocorrências
int insertHash (LIST** hash, unsigned long long int cod);




#endif
