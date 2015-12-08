
#include <stdlib.h>
#include <stdio.h>
#include "integration.h"
#include "node.h"
#include "lista.h"


struct LIST{
	NODE* head;
	NODE* tail;
	int size;
};

LIST *createList ()
{
		LIST* list = (LIST*)malloc(sizeof(LIST));

		list->head = NULL;

		list->tail = NULL;

		list->size = 0;

		return list;
}

void deleteList (LIST **list)
{
	if(list != NULL && *list != NULL)
	{
		NODE *paux = (*list)->head;
		NODE *prem = getNext(paux);
		while(prem != NULL)
		{
			//após deletar um nó seu valor de next é NULL, assim o delete node não da crash, e o proximo ponteiro é null
			paux = getNext(prem);
			deleteNode(&prem);
		}
		(*list)->size = 0;
		(*list)->head = NULL;
		(*list)->tail = NULL;
		free(*list);
		*list = NULL;
	}
}

void addNode (LIST *list, NODE *new)
{
	if(list != NULL && new != NULL)
	{
		if(list->size == 0)
		{
			list->head = new;
			list->tail = new;
			setNext(new, new);
		}
		else
		{
			setNext(list->tail, new);
			setNext(new, list->head);
			list->tail = new;
		}

		list->size++;
	}
}

NODE* getLast (LIST* list)
{
	if( list != NULL)
	{
		return list->tail;
	}
	return NULL;
}

NODE *remLast (LIST* list)
{
	if(list!= NULL && list->size > 0)
	{
		NODE* rem = list->tail;
		NODE *paux = list->head;
		while(paux->next != rem)
		{
			paux = getNext(paux);
		}

		setNext(paux, paux->next->next);
		list->tail = paux;

		list->size--;

		if(list->size == 0)
		{
			list->head = NULL;
			list->tail = NULL;
		}

		return rem;
	}
	return NULL;
}

NODE *searchHash (LIST *list, unsigned long long int cod)
{
	if(list != NULL && list->size > 0)
	{
		NODE* paux = list->head;
		do
		{
			if(compareHash(paux, cod))
				return paux;

			paux = getNext(paux);
		}while(paux != list->head);
	}
	return NULL;
}

int addHash (LIST *list, unsigned long long int cod)
{
	if(list != NULL)
	{
//printf("inserir hash %llu\n", cod);
		if(list->size == 0)
		{
			NODE *new = createNode();
			setHASH(new, cod);
			addNode(list, new);
//printf("inserido\n");
		}
		else
		{
			NODE* paux = list->head;
			int sum;
			do
			{
				if((sum = sumHash(paux, cod)))
					break;

				paux = getNext(paux);
			}while(paux != list->head);

			if(!sum)
			{
				NODE *new = createNode();
				setHASH(new, cod);
				addNode(list, new);
//printf("inserido\n");
			}
//printf("soma: %d\n", sum);
			return sum;
		}
	}
	return 0;
}
