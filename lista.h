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

#ifndef __LISTA_H_
#define __LISTA_H_

#include "integration.h"
#include "node.h"


typedef struct LIST LIST;

LIST *createList ();

void deleteList (LIST **list);

void addNode (LIST *list, NODE *new);

NODE* getLast (LIST* list);

NODE *remLast (LIST* list);

NODE *searchHash (LIST *list, unsigned long long int cod);

int addHash (LIST *list, unsigned long long int cod);

#endif
