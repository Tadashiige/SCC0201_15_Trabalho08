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

#ifndef __NODE_H_
#define __NODE_H_

#include <stdlib.h>
#include <stdio.h>
#include "integration.h"



typedef struct{
	unsigned long long int cod;
	int num;
}ITEM;



typedef struct no{
	ITEM item;
	struct no *next;
}NODE;



NODE *createNode ();

void deleteNode (NODE **node);

//adicionar um novo codigo hashing no nó e reseta a ocorrencia na lista
void setHASH (NODE *node, unsigned long long int cod);

unsigned long long int getHASH (NODE* node);

void setNext (NODE* node, NODE* next);

NODE *getNext (NODE* node);

//incrementar a ocorrencia do código hashing
int sumHash (NODE *node, unsigned long long int cod);

//compara se o código hashing é igual a do nó
int compareHash (NODE *node , unsigned long long int cod);

#endif
