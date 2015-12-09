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
#include <string.h>
#include <math.h>

#include "objeto.h"
#include "integration.h"
#include "peca.h"
#include "FEN.h"
#include "regra.h"
#include "IA.h"



/**
 * Função de métrica para IA
 *
 * DESCRIÇÃO:
 *		A função irá utilizar a função __riscoRei da biblioteca "regra.h", que
 *		verifica o risco do rei para dada jogada pretendia, para tanto substitui-se
 *		na posição o rei inimigo nela, e dessa forma é possivel receber no retorno se
 *		esse rei situacional sofre risco de ataque. Repete-se para o lado oposto do
 *		turno. Com esta ferramenta é possível construir a metrica:
 *
 *			S(mov) = sum ( alfa * v )1_64 / [sum ( !alfa * !v ) + 1]
 *
 *		Sendo alfa = { 1, para casa atacado pela peça do turno, 0 para não}
 *			 v = {50, para casa vazia; pj, para ataque a peça inimiga; pj/2, para ataque a peça aliada}
 *
 * 	PARAMETROS:
 * 		MOV_PARAM - define descrito no integration.h
 *
 * 	RETORNO:
 * 		double metrica - valor da métrica calculado para dada jogada proposta
 */
double __metricKingRisc (MOV_PARAM)
{
	int i, j;
	int x, y;
	double sumAlly = 0;
	double sumFoe = 0;
	int turn = (getType(obj) < 'a')?1:0;

	//*****************************************						Percorrer a matriz

	for(i = 0; i < TABLE_ROWS; i++)
	{
		for(j = 0; j < TABLE_COLS; j++)
		{
			OBJETO *target = table[i][j];
			OBJETO *king = getKingTable(table, turn);
			OBJETO *kingFoe = getKingTable(table, !turn);



			//*********************************** 					verificar ataque pelo Jogador

			table[i][j] = kingFoe;
			sumAlly += __riscoRei(table, kingFoe, i, j, !turn, i, j) * ((target == NULL)? 50 :
					(getType(target) < 'a' != turn)?(point(getType(target))):(point(getType(target))/2)
					);

			table[i][j] = target;



			//**********************************					verificar ataque pelo adversário
			table[i][j] = king;

			sumFoe += __riscoRei(table, king, i, j, turn, i, j) * ((target == NULL)? 50 :
					(getType(target) < 'a' != !turn)?(point(getType(target))):(point(getType(target))/2)
					);

			table[i][j] = target;
		}
	}
	return sumAlly / ++sumFoe;
}






//estrutura para ordenação das métricas
typedef struct{
	OBJETO *obj;
	char* play;
	double worth;
}PLAY_WORTH;






//função geral para chamada de função auxiliar ou core
double metric (MOV_PARAM)
{
	int size;
	return __metricKingRisc(MOV_VALUE);
}




//função para ordenação em qsort com ponteiro para estrutura específica

int sortPlayWorth (const void *a, const void *b)
{
	PLAY_WORTH *objA = *(PLAY_WORTH **)a, *objB = *(PLAY_WORTH**)b;
	float result = objA->worth - objB->worth;

	//variação de sinal só ocorre em -1 < 0 < 1, fora isso tanto ceil quanto floor retorno mesmo sinal
	//ordenação inversa, primeira posição tem o maior valor
	return (-1)*((int)ceil(result) + (int)floor(result));
}





/**
 * Função irá pesquisar por métrica a melhor jogada para a rodada
 *
 *	DESCRIÇÃO:
 *		Função irá alterar as posições do tabuleiro segundo a jogada proposta, marcar as supostas capturas,
 *		e alterar a promoção de peão. Com a situação criada realiza-se a métrica das jogas possíveis
 *		nestas condições, guardando em um vetor todos os valores obtidos. Após cada jogada testada
 *		desfaz-se as modificaçẽos para a condição anterior a mudança, tanto o tabuleiro, marca de captura,
 *		e uma promoção atingida.
 *
 *		Ao fim das métricas é ordenada a lista de métricas para obter a maior delas, assim preenchendo
 *		a estrutura de jogada PLAY que é retornada para a função principal main.
 *
 *	PARAMETROS:
 *		OBJETO **const collectionAlly - coleção de peças aliadas da rodada
 *		OBJETO **const collectionFoe - coleção de peças inimigas da rodada
 *		int ally - número da coleção aliada
 *		int foe - número da coleção inimiga
 *		MOV_PARAM - define descrita na biblioteca integration.h
 *
 *	RETORNO:
 *		PLAY play - estrutura de dados de uma jogada
 */
PLAY inputIA (OBJETO **const collectionAlly, OBJETO **const collectionFoe, int ally, int foe, MOV_PARAM)
{
	PLAY play;
	play.obj = NULL;
	play.fromCol = 8;
	play.fromRow = 8;
	play.promotion = '-';

	if(collectionAlly != NULL && collectionFoe != NULL)
	{
		PLAY_WORTH** playsValue = (PLAY_WORTH**)malloc(sizeof(PLAY_WORTH*));
		int playsSize = 0;

		int i, j;
		int turn = (getType(collectionAlly[0]) < 'a')? 1: 0;

// ==================================================						Avaliar cada peça aliada

		for(i = 0; i < ally; i++)
		{

			OBJETO *obj = collectionAlly [i];

			//pegar a lista de movimentos para a cada peça
			int size = getNList(obj);
			char** list = getList(obj);





			// **************************************						Para cada peça testar as jogadas de sua lista

			//fazer calculo para cada jogada possível
			for(j = 0; j < size; j++)
			{
				int * to = getPlayTo(list[j]);
				int row = 7 - getObjectRow(obj), col = getObjectColumn(obj);

				OBJETO *rem = table[to[0]][to[1]];





				// #################################						 Ajustar mudança

				table[to[0]][to[1]] = table[row][col];
				table[row][col] = NULL;

				//  !!!!!!! 												jogada é uma captura

				if(rem != NULL)
					captured(rem);

				// !!!!!!													movimento de roque

				int diff = 0;
				if((getType(obj) == 'k' || getType(obj) == 'K' ) && abs(diff = to[1] - getObjectColumn(obj)) > 1)
				{
					//roque para rei
					if(diff > 0)
					{
						table[row][to[1] - 1] = table[row][TABLE_COLS - 1];
						table[row][TABLE_COLS - 1] = NULL;
					}
					//roque para rainha
					else
					{
						table[row][to[1] + 1] = table[row][0];
						table[row][0] = NULL;
					}
				}

				// !!!!!!													movimento de promoção

				int wordSize = strlen(list[j]);
				if(list[j][wordSize - 1] >= 'B' + !turn * 32 && list[j][wordSize - 1] <= 'R' + !turn * 32)
					changeType(obj,
							(list[j][wordSize - 1]),
							getFunctionType(list[j][wordSize - 1]));






				// ##################################  						realizar a métrica

				//aumentar o vetor de métricas

				playsValue = (PLAY_WORTH**)realloc(playsValue, sizeof(PLAY_WORTH*)*++playsSize);
				PLAY_WORTH* newPlay = (PLAY_WORTH*)malloc(sizeof(PLAY_WORTH));

				//receber a métrica
				newPlay->worth = metric(MOV_VALUE);

				newPlay->obj = obj;
				newPlay->play = list[j];
				playsValue[playsSize - 1] = newPlay;






				// #################################				voltar com as configurações originais do tabuleiro

				table[row][col] = table[to[0]][to[1]];
				table[to[0]][to[1]] = rem;

				// !!!!												jogada foi uma captura

				if(rem != NULL)
					uncaptured(rem);

				//!!!!! 											movimento foi roque

				if(abs(diff) > 1)
				{
					//roque para rei
					if(diff > 0)
					{
						table[row][TABLE_COLS - 1] = table[row][to[1] - 1];
						table[row][to[1] - 1] = NULL;
					}
					//roque para rainha
					else
					{
						table[row][0] = table[row][to[1] + 1];
						table[row][to[1] + 1] = NULL;
					}
				}

				//!!!!! 											movimento de promoção

				if(list[j][strlen(list[j]) - 1] >= 'B' && list[j][strlen(list[j]) - 1] <= 'R')
					changeType(obj, 'P' + !turn * 32, getFunctionType('p'));

				free(to);
			}//for

		}//for ally







		// *************************************************		extrair dados para estrutura PLAY

		//ordenadar as métricas
		qsort(playsValue, playsSize, sizeof(PLAY_WORTH*), &sortPlayWorth);

		//armazenar dados da estrutura de jogada
		play.obj = playsValue[0]->obj;

		char *paux = playsValue[0]->play;

		play.promotion = (paux[strlen(paux) - 1] >= 'B' + !turn * 32 && paux[strlen(paux) - 1] <= 'R' + !turn * 32)?
				paux[strlen(paux) - 1] : '-';
		play.fromRow = 7 - getObjectRow(play.obj);
		play.fromCol = getObjectColumn(play.obj);

		//atribuir a jogada em string
		char* newPosition = (char*)malloc(sizeof(char)*3);
		int *TO = getPlayTo(paux);

		newPosition[0] = 'a' + TO[1];
		newPosition[1] = '8' - TO[0];
		newPosition[2] = '\0';

		changePosition(play.obj, newPosition);

		free(TO);
		for(i = 0; i < playsSize; i++)
			free(playsValue[i]);
		free(playsValue);
	}

	return play;
}
