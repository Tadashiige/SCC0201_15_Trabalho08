
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
