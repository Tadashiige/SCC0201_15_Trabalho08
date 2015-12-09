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

#include <stdlib.h>
#include <stdio.h>
#include "integration.h"
#include "node.h"

// *************************************************************************************

//OBS.: séries de funções padrões de listas & nós que DESPENÇAM documentação por ser genéricas

// As algumas funções são específico com hashing e possuem documentação

// *************************************************************************************




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




/**
 * Função substitui o código hashing do nó e
 * reseta as ocorrencias do código.
 */
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




/**
 * Função incrementa a ocorrencia do código hashing caso
 * a o código dado seja igual a dele, retornando as ocorrencias
 * totais do fim da operação. Retorna Zero caso divirja.
 */
int sumHash (NODE *node, unsigned long long int cod)
{
	if(node != NULL)
	{
		if(node->item.cod == cod)
			return ++(node->item.num);
	}
	return 0;
}




/**
 * Função irá comparar o código hashing do nó com o dado
 * e retornar a resposta da comparação.
 */
int compareHash (NODE *node , unsigned long long int cod)
{
	if(node != NULL)
	{
		return node->item.cod == cod;
	}
	return 0;
}
