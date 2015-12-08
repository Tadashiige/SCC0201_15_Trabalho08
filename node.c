#include <stdlib.h>
#include <stdio.h>
#include "integration.h"
#include "node.h"

NODE *createNode ()
{
	NODE* node = (NODE*)malloc(sizeof(NODE));

	node->item.cod = 0;
	node->item.num = 0;

	node->next = NULL;

	return node;
}

void deleteNode (NODE **node)
{
	if(node != NULL && *node != NULL)
	{
		(*node)->next = NULL;
		free(*node);
		*node = NULL;
	}
}

void setHASH (NODE *node, unsigned long long int cod)
{
	if(node != NULL)
	{
		node->item.cod = cod;
		node->item.num = 0;
	}
}

unsigned long long int getHASH (NODE* node)
{
	if( node != NULL)
	{
		return node->item.cod;
	}
	return 0;
}

void setNext (NODE* node, NODE* next)
{
	if(node != NULL && next != NULL)
	{
		node->next = next;
	}
}

NODE *getNext (NODE* node)
{
	if(node != NULL)
	{
		return node->next;
	}
	return NULL;
}

int sumHash (NODE *node, unsigned long long int cod)
{
	if(node != NULL)
	{
//printf("comparação entre Hash: %llu <> %llu\n", node->item.cod, cod);
		if(node->item.cod == cod)
			return ++(node->item.num);
	}
	return 0;
}

int compareHash (NODE *node , unsigned long long int cod)
{
	if(node != NULL)
	{
		return node->item.cod == cod;
	}
	return 0;
}
