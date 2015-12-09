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
 *	Biblioteca do TAD de lista ligada - circular
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "integration.h"
#include "node.h"
#include "lista.h"


// *************************************************************************************

//OBS.: séries de funções padrões de listas que DESPENÇAM documentação por ser genéricas

// As duas últimas funções são específico com hashing e possuem documentação

// *************************************************************************************



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





// *************************************** 	ESPECÍFICO DE HASHING ********************************************* //


/**
 * Função verifica se o hashing dos nós é igual a da comparação
 */
NODE *searchHash (LIST *list, unsigned long long int cod)
{
	if(list != NULL && list->size > 0)
	{
		//percorrer a lista em busca do nó com o hashing igual a da comparação

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






/**
 * Função verifica se existe o código em algum nó da lista. Caso sim
 * incrementa a ocorrencia dela no nó, caso contrário adicionar um
 * novo nó com o hashing dado.
 */
int addHash (LIST *list, unsigned long long int cod)
{
	if(list != NULL)
	{

		//a lista vazia. Pede-se um novo nó
		if(list->size == 0)
		{
			NODE *new = createNode();
			setHASH(new, cod);
			addNode(list, new);
		}

		else
		{
			//percorrer a lista para verificar cada nó
			NODE* paux = list->head;
			int sum;
			do
			{
				//tentar incrementar a ocorrencia do hashing. Retorna Zero se o hashing divergir.
				if((sum = sumHash(paux, cod)))
					break;

				paux = getNext(paux);
			}while(paux != list->head);

			//se não houve hashing na lista adicionar como novo nó
			if(!sum)
			{
				NODE *new = createNode();
				setHASH(new, cod);
				addNode(list, new);
			}
			return sum;
		}
	}
	return 0;
}
