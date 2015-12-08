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
 *
 * >>>>> >>>>> Trabalho 8: Xadrez - Parte 3 (Implementação de Inteligência Articial)
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

double __metricKingRisc (MOV_PARAM)
{
	int i, j;
	int x, y;
	double sumAlly = 0;
	double sumFoe = 0;
	int turn = (getType(obj) < 'a')?1:0;
	for(i = 0; i < TABLE_ROWS; i++)
	{
		for(j = 0; j < TABLE_COLS; j++)
		{
			OBJETO *target = table[i][j];
			OBJETO *king = getKingTable(table, turn);
			OBJETO *kingFoe = getKingTable(table, !turn);

//			int kingRow = 7 - getObjectRow(king), kingCol = getObjectColumn(king);
//			int kingFoeRow = 7 - getObjectRow(kingFoe), kingFoeCol = getObjectColumn(kingFoe);

			//verificar ataque pelo Jogador
			table[i][j] = kingFoe;
			sumAlly += __riscoRei(table, kingFoe, i, j, !turn, i, j) * ((target == NULL)? 50 :
					(getType(target) < 'a' != turn)?(point(getType(target))):(point(getType(target))/2)
					);

			table[i][j] = target;

			//verificar ataque pelo adversário
			table[i][j] = king;

			sumFoe += __riscoRei(table, king, i, j, turn, i, j) * ((target == NULL)? 50 :
					(getType(target) < 'a' != !turn)?(point(getType(target))):(point(getType(target))/2)
					);

			table[i][j] = target;
		}
	}
//printTable(table);
//getchar();
	return sumAlly / ++sumFoe;
}

double __metricMatrix (MOV_PARAM)
{
	int i, j;
	int x, y;
	double sum = 0;
	int turn = (getType(obj) < 'a')?1:0;
	for(i = 0; i < TABLE_ROWS; i++)
	{
		for(j = 0; j < TABLE_COLS; j++)
		{
			OBJETO *target = table[i][j];

			//ortogonais
			for(x = i + 1; x < TABLE_ROWS; x++)
			{

			}
			for(x = i - 1; x >= 0; x++)
			{

			}
			for(y = j + 1; y < TABLE_COLS; y++)
			{

			}
			for(y = j - 1; y >=0 ; y--)
			{

			}

			//diagonais
			for(x = i + 1, y = j + 1; x < TABLE_ROWS && y < TABLE_COLS; x++, y++)
			{

			}
			for(x = i - 1, y = j - 1; x >= 0 && y >= 0; x--, y--)
			{

			}
			for(x = i - 1, y = j + 1; x >= 0 && y < TABLE_COLS; x--, y++)
			{

			}
			for(x = i + 1, y = j - 1; x < TABLE_ROWS && y >= 0; x++, y--)
			{

			}

			//cavalo

		}
	}
	return sum;
}

//função calcula métrica de decisão da jogada
double __metricPieces (OBJETO **const collectionAlly, OBJETO **const collectionFoe, int ally, int foe, MOV_PARAM)
{
	double best = 0.0;
//printf("metrica\n");
	if(table != NULL && collectionAlly != NULL && collectionFoe != NULL)
	{
		int i, j;

		//matriz de verificação, inicializado com Zeros
		int fields[TABLE_ROWS][TABLE_COLS];
		for(i = 0; i < TABLE_ROWS; i++)
			for(j = 0; j < TABLE_COLS; j++)
				fields[i][j] = 0;

		double denom = 0;
		double numer = 0;

		int turn = (getType(collectionAlly[0]) < 'a')? 1: 0;

//printf("aliado\n");
		//*********************************** calcular somatória das casas atacadas por aliados
		for(i = 0; i < ally; i++)
		{
			OBJETO *obj = collectionAlly [i];

			//pular peça desativada
			if(!getActive(obj))
				continue;
//printf("i(%d) peça %c[%d][%d]\n", i, getType(obj), 7-getObjectRow(obj), getObjectColumn(obj));
			white = getType(obj) - (getType(obj) >= 'a')*32;
			black = getType(obj) + (getType(obj) < 'a')*32;

			//pegar a lista de movimentos para a cada peça
			int size;
			char** list = (*getFunctionMov(obj))(MOV_VALUE);

			//todo para o peão tem que analisar a casa na diagonal como casa em ataque
			if(getType(obj) == 'p' || getType(obj) == 'P')
			{
				int sizePeasant = 0;
				char** peasantList = movPeasantAttack(table, obj, &sizePeasant);
				for(j = 0; j < sizePeasant; j++)
				{
					//estrair a casa de destino
					int *to = getPlayTo(peasantList[j]);

					//verificar se a casa já não foi computada
					if(!fields[to[0]][to[1]])
					{
						numer += (table[to[0]][to[1]] == NULL)? 50.0 :
								((getType(table[to[0]][to[1]]) < 'a' == turn)?
										point(getType(table[to[0]][to[1]]))/2:
										point(getType(table[to[0]][to[1]]))
										);
						fields[to[0]][to[1]] = 1;
					}
					free(to);
				}

				free(peasantList);
			}
			else
			{
				for(j = 0; j < size; j++)
				{
					//estrair a casa de destino
					int *to = getPlayTo(list[j]);

					//verificar se não é roque, o roque não configura ameaça de casas com o Rei, para a torre cobre o alcance
					if((getType(obj) == 'k' || getType(obj) == 'K') && abs(getObjectColumn(obj) - to[1]) > 1)
						continue;

					//verificar se a casa já não foi computada
					if(!fields[to[0]][to[1]])
					{
						numer += (table[to[0]][to[1]] == NULL)? 50.0 :
								((getType(table[to[0]][to[1]]) < 'a' == turn)?
										point(getType(table[to[0]][to[1]]))/2:
										point(getType(table[to[0]][to[1]]))
										);
						fields[to[0]][to[1]] = 1;
					}
					free(to);
				}//for
			}//else

			free(list);
		}//for pieces_num
//printf("inimigo\n");
		// ******************************** calcular somatória das casas atacadas por inimigos
		for(i = 0; i < foe; i++)
		{
			OBJETO *obj = collectionFoe [i];

			//pular peça desativada
			if(!getActive(obj))
				continue;

			white = getType(obj) - (getType(obj) >= 'a')*32;
			black = getType(obj) + (getType(obj) < 'a')*32;

			//pegar a lista de movimentos para a cada peça
			int size;
			char** list = (*getFunctionMov(obj))(MOV_VALUE);

			//todo para o peão tem que analisar a casa na diagonal como casa em ataque
			if(getType(obj) == 'p' || getType(obj) == 'P')
			{
				int sizePeasant = 0;
				char** peasantList = movPeasantAttack(table, obj, &sizePeasant);

				for(j = 0; j < sizePeasant; j++)
				{
					//estrair a casa de destino
					int *to = getPlayTo(peasantList[j]);

					//verificar se a casa já não foi computada
					if(fields[to[0]][to[1]] != 2)
					{
						denom += (table[to[0]][to[1]] == NULL)? 50.0 :
								((getType(table[to[0]][to[1]]) < 'a' == turn)?
										point(getType(table[to[0]][to[1]]))/2:
										point(getType(table[to[0]][to[1]]))
										);
						fields[to[0]][to[1]] = 2;
					}
					free(to);
				}

				free(peasantList);
			}
			else
			{
				for(j = 0; j < size; j++)
				{
					//estrair a casa de destino
					int *to = getPlayTo(list[j]);

					//verificar se não é roque, o roque não configura ameaça de casas com o Rei, para a torre cobre o alcance
					if((getType(obj) == 'k' || getType(obj) == 'K') && abs(getObjectColumn(obj) - to[1]) > 1)
						continue;

					//verificar se a casa já não foi computada
					if(fields[to[0]][to[1]] != 2)
					{
						denom += (table[to[0]][to[1]] == NULL)? 50.0 :
								((getType(table[to[0]][to[1]]) < 'a' == turn)?
										point(getType(table[to[0]][to[1]]))/2:
										point(getType(table[to[0]][to[1]]))
										);
						fields[to[0]][to[1]] = 2;
					}
					free(to);
				}//for
			}//else

			free(list);
		}//for pieces_num

		best = (double)numer / ++denom;
	}

	return best;
}

typedef struct{
	OBJETO *obj;
	char* play;
	double worth;
}PLAY_WORTH;

double metric (MOV_PARAM)
{
	int size;
	return __metricKingRisc(MOV_VALUE);
}

int sortPlayWorth (const void *a, const void *b)
{
	PLAY_WORTH *objA = *(PLAY_WORTH **)a, *objB = *(PLAY_WORTH**)b;
	float result = objA->worth - objB->worth;
	//variação de sinal só ocorre em -1 < 0 < 1, fora isso tanto ceil quanto floor retorno mesmo sinal
	//ordenação inversa, primeira posição tem o maior valor
	return (-1)*((int)ceil(result) + (int)floor(result));
}

PLAY inputIA (OBJETO **const collectionAlly, OBJETO **const collectionFoe, int ally, int foe, MOV_PARAM)
{
	PLAY play;
	play.obj = NULL;
	play.fromCol = 8;
	play.fromRow = 8;
	play.promotion = '-';
//printf("função inputIA\n");
	if(collectionAlly != NULL && collectionFoe != NULL)
	{
		PLAY_WORTH** playsValue = (PLAY_WORTH**)malloc(sizeof(PLAY_WORTH*));
		int playsSize = 0;

		int i, j;
		int turn = (getType(collectionAlly[0]) < 'a')? 1: 0;
		for(i = 0; i < ally; i++)
		{
//printf("analisando peça %d\n", i+1);
			OBJETO *obj = collectionAlly [i];

			//pegar a lista de movimentos para a cada peça
			int size = getNList(obj);
			char** list = getList(obj);

			//fazer calculo para cada jogada possível
			for(j = 0; j < size; j++)
			{
				int * to = getPlayTo(list[j]);
				int row = 7 - getObjectRow(obj), col = getObjectColumn(obj);
//printf("analisando peça %d_ jogada %d -> %c[%d][%d]->[%d][%d]\tjogada: %s\n", i+1, j+1, getType(obj), row, col, to[0], to[1], list[j]);
				OBJETO *rem = table[to[0]][to[1]];

			// >>>>>>>>>>>>> Ajustar mudança
				table[to[0]][to[1]] = table[row][col];
				table[row][col] = NULL;

				//jogada é uma captura
				if(rem != NULL)
					captured(rem);

				//movimento de roque
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

				//movimento de promoção
				int wordSize = strlen(list[j]);
				if(list[j][wordSize - 1] >= 'B' + !turn * 32 && list[j][wordSize - 1] <= 'R' + !turn * 32)
					changeType(obj,
							(list[j][wordSize - 1]),
							getFunctionType(list[j][wordSize - 1]));

			// ************* realizar a métrica
				playsValue = (PLAY_WORTH**)realloc(playsValue, sizeof(PLAY_WORTH*)*++playsSize);
				PLAY_WORTH* newPlay = (PLAY_WORTH*)malloc(sizeof(PLAY_WORTH));
//printf("tamanho do vetor comparação %d\t%c[%d][%d] calcular ", playsSize, getType(obj), 7 - getObjectRow(obj), getObjectColumn(obj));
				newPlay->worth = metric(MOV_VALUE);
//printf("Metrica: %.8f %s\n", newPlay->worth, list[j]);
				newPlay->obj = obj;
				newPlay->play = list[j];
				playsValue[playsSize - 1] = newPlay;

			// <<<<<<<<<<<<< voltar com as configurações originais do tabuleiro
				table[row][col] = table[to[0]][to[1]];
				table[to[0]][to[1]] = rem;

				//jogada foi uma captura
				if(rem != NULL)
					uncaptured(rem);

				//movimento foi roque
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

				//movimento de promoção
				if(list[j][strlen(list[j]) - 1] >= 'B' && list[j][strlen(list[j]) - 1] <= 'R')
					changeType(obj, 'P' + !turn * 32, getFunctionType('p'));

				free(to);
//printf("fim de análise da jogada %s\n", list[j]);
			}//for

		}//for ally
//printf("fim das análises das peças\n");
		//extrair dados para estrutura PLAY
		qsort(playsValue, playsSize, sizeof(PLAY_WORTH*), &sortPlayWorth);

		play.obj = playsValue[0]->obj;

		char *paux = playsValue[0]->play;

		play.promotion = (paux[strlen(paux) - 1] >= 'B' + !turn * 32 && paux[strlen(paux) - 1] <= 'R' + !turn * 32)?
				paux[strlen(paux) - 1] : '-';
		play.fromRow = 7 - getObjectRow(play.obj);
		play.fromCol = getObjectColumn(play.obj);

		char* newPosition = (char*)malloc(sizeof(char)*3);
		int *TO = getPlayTo(paux);

		newPosition[0] = 'a' + TO[1];
		newPosition[1] = '8' - TO[0];
		newPosition[2] = '\0';

		changePosition(play.obj, newPosition);
//printf("movimentos: %d\tpior metrica: %.8f\tmelhor metrica: %.8f\n", playsSize, playsValue[playsSize - 1]->worth, playsValue[0]->worth);
		free(TO);
		for(i = 0; i < playsSize; i++)
			free(playsValue[i]);
		free(playsValue);
	}
//printf("fim da busca IA\n");
	return play;
}
