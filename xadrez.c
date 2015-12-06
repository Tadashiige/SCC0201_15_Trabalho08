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
 * Arquivo Principal - main
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "FEN.h"
#include "objeto.h"
#include "peca.h"
#include "regra.h"

int main (int argc, char* argv[])
{
	int i;

	//declaração de variáveis da notação FEN
	FEN *fen = (FEN*) malloc(sizeof(FEN));

	//recepção dos valores da notação FEN
	fscanf(stdin, "%s ", fen->pieces);
	fscanf(stdin, "%c ", &(fen->turn));
	fscanf(stdin, "%s ", fen->rock);
	fscanf(stdin, "%s ", fen->pass);
	fscanf(stdin, "%d ", &(fen->halfTurn));
	fscanf(stdin, "%d", &(fen->fullTurn));

	//****************************************** Inicialização de Tabuleiro

	//extrair todos as peças da notação FEN
	int pieces_num = 0;
	int white_pieces = 0;
	OBJETO **collection = extractTableFEN (fen->pieces, &pieces_num, &white_pieces);

	//construir o tabuleiro
	OBJETO ***table = (OBJETO***)malloc(sizeof(OBJETO**)*TABLE_ROWS);
	for(i = 0; i < TABLE_ROWS; i++)
		table[i] = (OBJETO**)malloc(sizeof(OBJETO*)*TABLE_COLS);

	//preencher o tabuleiro
	fillTable (table, collection, pieces_num);

	//ordenar por cor, os brancos vem primeiro
	qsort(collection, pieces_num, sizeof(OBJETO*), &separaCor);


	//as primeiras n primeiras posições são ocupadas por peças brancas, e as posteriores por pretas
	//	 imprimir lista de movimentos para cada peça
	int total;
	OBJETO **collection_list;

	if(fen->turn == 'w')
	{
		total = white_pieces;
		collection_list = collection;
	}
	else
	{
		total = pieces_num - white_pieces;
		collection_list = collection + white_pieces;
	}

	//ordenar por critério de desempate e apenas as peças do turno
	qsort(collection, white_pieces, sizeof(OBJETO*), &desempate);

	qsort(collection + white_pieces, pieces_num - white_pieces, sizeof(OBJETO*), &desempate);

	//executar as listagem de movimentos para cada peça criando a lista inicial
	for(i = 0; i < pieces_num; i++)
	{
		funcPtr funcType;
		funcType = getFunctionMov(collection[i]);
		OBJETO *obj = collection[i];
		char white = getType(obj) - (getType(obj) >= 'a')*32;
		char black = getType(obj) + (getType(obj) < 'a')*32;

		funcType(MOV_VALUE);
	}

	printFEN(fen);

	// ****************************************** Interface de entrada de jogadas do usuário

	int endGame;
	while(!(endGame = verifyGameState(table, collection, collection_list, total, pieces_num, fen)))
	{

		//receber jogada válida do usuário
		PLAY play = inputPlay (fen, table, ((fen->turn == 'w')? 1 : 0), fen->fullTurn);

		//realizar mudanças das peças dadas pela jogada
		//OBS.: Nova ordenação do vetor só é necessária quando há promoção de peão, implementado dentro desta função
		doPlay (table, play, collection, white_pieces, pieces_num, fen->fullTurn);

		//updateCollection elimina peça desativada, mas não tira as peças da orden, dispensando ordenação
		updateCollection(collection, &white_pieces, &pieces_num, (fen->turn == 'w')? 1:0);
		updateFEN(fen, table, play);
		changeTurn(fen);

		//tratar as peças a serem consideradas para o próximo jogador
		printFEN(fen);

		if(fen->turn == 'w')
		{
			total = white_pieces;
			collection_list = collection;
		}
		else
		{
			total = pieces_num - white_pieces;
			collection_list = collection + white_pieces;
		}

		char **list;

		//executar as listagem de movimentos de cada peça para próxima jogada
		for(i = 0; i < total; i++)
		{
			funcPtr funcType;
			funcType = getFunctionMov(collection_list[i]);
			OBJETO *obj = collection_list[i];
			char white = getType(obj) - (getType(obj) >= 'a')*32;
			char black = getType(obj) + (getType(obj) < 'a')*32;

			clearList(obj);

			list = funcType(MOV_VALUE);
		}
		//tratar jogadas coincidentes
		conflict (collection_list, total);

	}//while verifyGameState

	// ********************************************************** tratar impressão de resultado

	if(endGame % 2)
	{
		fprintf(stdout, "Cheque-mate -- Vitoria: ");
		//OBS.: o valor de turno esta invertido pq o teste se faz sempre que o turno é virado, então
			//a vitória se dá para a jogada anterior a inversão
		if(fen->turn == 'b')
			fprintf(stdout, "BRANCO");
		else
			fprintf(stdout, "PRETO");
	}
	else
	{
		fprintf(stdout, "Empate -- ");
		if(endGame % 4)
		{
			fprintf(stdout, "Afogamento");
		}else if (endGame % 8)
		{
			fprintf(stdout, "Regra dos 50 Movimentos");
		}else
			fprintf(stdout, "Falta de Material");

	}
	fprintf(stdout, "\n");

	//*********************************************************** liberação de memórias alocadas

	//eliminar todos os objetos alocados
	for(i = 0; i < pieces_num; i++)
	{
		deleteObject(&(collection[i]));
	}
	if(collection != NULL)
		free(collection);

	//as peças dentro do tabuleiro já foram liberado por coleção, então basta liberar o tabuleiro
	if(table != NULL)
	{
		for(i = 0; i < TABLE_ROWS; i++)
		{
			if(table[i] != NULL)
			{
				free(table[i]);
			}
		}
		free(table);
	}

	deleteFEN(&fen);

	return 0;
}
