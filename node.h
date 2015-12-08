
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

void setHASH (NODE *node, unsigned long long int cod);

unsigned long long int getHASH (NODE* node);

void setNext (NODE* node, NODE* next);

NODE *getNext (NODE* node);

int sumHash (NODE *node, unsigned long long int cod);

int compareHash (NODE *node , unsigned long long int cod);

#endif
