/**
 * Saulo Tadashi Iguei NºUsp 7573548
 *
 * DATA entrega limite: 08/12/15
 *
 * SCC0201_01 - ICC2 _ Prof. Moacir
 *
 * Trabalho 6: Xadrez - Parte 1 (Geração de movimentos)
 * >>>>> Trabalho 7: Xadrez -Parte 2 (Implementação de jogabilidade)
 */

/*
 * Arquivo tratador de peças - particularidades e movimento
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "FEN.h"
#include "peca.h"
#include "regra.h"

/**
 * Função irá ordenar a lista de jogadas
 *
 * DESCRIÇÃO:
 * 		Função irá chamar o qsort para ordenar a lista de jogadas segundo
 * 		o critério de desempate descrito em sortPlay, que prescreve a ordem
 * 		de prioridade: coluna, linha e promoção (quando para peão).
 *
 * 	PARAMETRO:
 * 		char** list - ponteiro para lista de jogadas a ordenar
 * 		int size - tamanho da lista
 *
 * 	RETORNO:
 * 		char** list - ponteiro para lista de jogadas já ordenada
 */
char** sortList (char **list, int size)
{
	qsort(list, size, sizeof(char*), &sortPlay);
	return list;
}

/**
 * Função irá executar a pilhagem de jogada de promoção na lista
 *
 * DESCRIÇÃO:
 * 		Função irá fazer a adicionaão de novas jogadas dadas por notation
 * 		acrescendo os tipos de peças promovíveis, que são 4 (rainha,
 * 		bispo, torre e cavalo).
 *
 * 	PARAMETROS:
 * 		char** list - ponteiro para lista de jogadas
 * 		char* notation - ponteiro para string de jogada
 * 		int *size - ponteiro para o tamanho da lista
 * 		int const turn - valor representativo de turno
 *
 * 	RETORNO:
 * 		char* list - ponteiro para lista de jogadas atualizada
 */
char** promotion (char** list, int size, char* notation, int const turn)
{
	//promoção
	char promo[3] = "X";
	int i;
	char white, black;
	for(i = 0; i < 4; i++)
	{
		switch(i)
		{
		case 0:
			white = 'N'; black = 'n';
			break;
		case 1:
			white = 'B'; black = 'b';
			break;
		case 2:
			white = 'R'; black = 'r';
			break;
		case 3:
			white = 'Q'; black = 'q';
			break;
		default:
			white = '?';black = '?';
			break;
		}
		size++;
		list = (char**)realloc(list, sizeof(char*)*size);
		char* newPlay = (char*)malloc(sizeof(char)*(strlen(notation)+1+1));
		strcpy(newPlay, notation);
		//mudar o final de string para adicionar a promoção
		newPlay[strlen(newPlay) + 1] = '\0';
		//novo final estabelecido, adicionar a promoção imediatamente antes ao finalizador de strings
		newPlay[strlen(newPlay)] = (turn == 1)?white:black;
		list[size - 1] = newPlay;
	}

	return list;
}

/**
 * Função irá criar lista de movimentos possível de um peão
 * DESCRIÇÃO:
 * 		Função irá analisar os movimentos de um peão, descrito por avanço de uma casa a frente
 * 		se la estiver vazia e captira em diagonal de uma peça inimiga, assim como salto de 2 casas
 * 		caso o peão não tenha se movido desde o começo do jogo. O rei não deve ser exposto a cheque
 * 		ou permanecer em cheque por um movimento do peão ou na sua ausência. Para cada movimento válido
 * 		será adicionado na lista de jogadas.
 *
 * 	PARAMENTROS:
 * 		MOV_PARAM - define em integration.h
 *
 * 	RETURNO:
 * 		char** list - lista de jogadas em string
 */
char** movPeasant (MOV_PARAM)
{
	char** list = NULL;
	if(obj != NULL)
	{
		//coordenadas matriciais
		int col = getObjectColumn(obj);
		int row = 7 - getObjectRow(obj);

		//variável identificadora de aliada
		int turn = (getType(obj) < 'a')? 1 : 0;

		//variável para identificar borda lateral
		int col_border = (col - 1 < 0)? 0 : 1;

		//variável para decidir direção de avanço (cima - baixo)
		int row_adv = (turn == 1)? -1 : 1;

		list = (char**)malloc(sizeof(char*));
		int size = 0;
		int i;

		for(i = col - col_border; i <= col + 1 && i < TABLE_COLS; i++)
		{
			//avançar
			if(i == col)
			{
				//avançar 1 casa
				if(table[row + row_adv][i] == NULL &&
						!riscoRei (table, obj, row + row_adv, i, turn))
				{
					//construção de notação
					char notationFrom[6+2] = {"Paa8"};
					notationFrom[1] += i;
					notationFrom[2] += i;
					notationFrom[3] -= (row + row_adv);

					char *notation = collision (table, notationFrom, obj, row, col, turn);

					//verificar se há promoção de peça
					if(row + row_adv == (TABLE_ROWS - 1)*(!turn))
						list = promotion (list, size, notation, turn);
					//acrescer a jogada nova para a lista
					else
					{
						list = (char**)realloc(list, sizeof(char*)*(++size));
						char *newPlay = (char*)malloc(sizeof(char)*(strlen(notation)+1));
						strcpy(newPlay, notation);
						list[size - 1] = newPlay;
					}
				}

				//avançar 2 casas - as casas por onde passa e o alvo deve estar vazia, assim como não arriscar o rei
				if(((turn == 1 && row == 6) || (turn == 0 && row == 1)) &&
						table[row + row_adv][i] == NULL && table[row + row_adv*2][i] == NULL &&
						!riscoRei (table, obj, row + row_adv*2, i, turn))
				{
					//criar notação para jogada
					char notationFrom[6+2] = {"Paa8"};
					notationFrom[1] += i;
					notationFrom[2] += i;
					notationFrom[3] -= (row + row_adv*2);

					char *notation = collision (table, notationFrom, obj, row, col, turn);

					//verificar se há promoção de peça
					if(row + row_adv == (TABLE_ROWS - 1)*(!turn))
						list = promotion (list, size, notation, turn);
					//acrescer jogada a lista
					else
					{
						list = (char**)realloc(list, sizeof(char*)*(++size));
						char *newPlay = (char*)malloc(sizeof(char)*(strlen(notation)+1));
						strcpy(newPlay, notation);
						list[size - 1] = newPlay;
					}
				}//if 2 casas
			}//if i == col

			//capturar
			else
			{
				//existe peça na diagonal E ele é inimiga
				if (((table[row + row_adv][i] != NULL && (getType(table[row + row_adv][i]) < 96) != turn)
						||
						//capiturar por en passant
						//existe peça na lateral E é inimiga, assim como existe por FEN a autorização para tal
						(table[row][i] != NULL && (getType(table[row][i]) < 'a') != turn &&
								(strcmp(fen->pass, "-") &&
										getType(table[row][i]) == 'P' + (getType(table[row][i]) >= 'a')*32) &&
										//E a casa corresponde à coordenada de en passant
										row + row_adv == 7 - fen->pass[1] + '1' &&
										i == fen->pass[0] - 'a')) &&
									!riscoRei (table, obj, row + row_adv, i, turn))
				{
					//criação da notação
					char notationFrom[7+2] = {"Paxa8"};
					notationFrom[1] += col;
					notationFrom[3] += i;
					notationFrom[4] -= (row + row_adv);

					char *notation = collision (table, notationFrom, obj, row, col, turn);

					//verificar se existe promoção
					if(row + row_adv == (TABLE_ROWS - 1)*(!turn))
						list = promotion (list, size, notation, turn);
					//adicionar nova jogada a lista
					else
					{
						list = (char**)realloc(list, sizeof(char*)*(++size));
						char *newPlay = (char*)malloc(sizeof(char)*(strlen(notation)+1));
						strcpy(newPlay, notation);
						list[size - 1] = newPlay;
					}
					//en passant - apenas modifica a jogada atual com adição de "e.p."
					if(row + row_adv == 7 - fen->pass[1] + '1' && i == fen->pass[0] - 'a'  &&
							getType(table[row][i]) == 'P' + (getType(table[row][i]) >= 'a')*32)
					{
						list[size - 1] = (char*)realloc(list[size - 1],
								sizeof(char)*(strlen(list[size- 1])+strlen("e.p.")+1));
						strcat(list[size- 1], "e.p.");
					}
				}//if
			}//else
		}//for
		*length = size;
		list = sortList(list, size);
	}//if obj != NULL
	return list;
}

/**
 * Função padrão para adicionar a jogada pretendida na lista
 *
 * 	DESCRIÇÃO:
 * 		Função irá avaliar se a jogada proposta oferece risco ao rei. Caso
 * 		o rei fique salvo, então a jogada será adicionada na lista da peça.
 *
 * 	PARAMETROS:
 * 		OBJETO *** const table -  ponteiro para o tabuleiro
 * 		OBJETO* const obj  - ponteiro para o objeto/peça
 * 		char** list  - endereço do ponteiro da lista de jogadas
 * 		int i  - linha do tabuleiro invertido verticalmente
 * 		int j - coluna do tabuleiro invertido verticalmente
 * 		int turn - valor representativo de turno (1 para branco, 0 para preto)
 * 		char white  - caracter da peça no time branco
 * 		char black - caracter da peça no time preto
 *
 * 	RETORNO:
 *		int break - valor de encontra de uma pessa, caso deva interromper um looping ao encontrar
 *				uma peça, então break retorna 0, caso contrário 1.
 */
int __mov (OBJETO *** const table, OBJETO* const obj, char*** old_list, int *length,
		int i, int j, int turn, char white, char black)
{
	char** list = *old_list;
	int size = *length;

	//casa alvo está vazio
	if(table[i][j] == NULL && !riscoRei (table, obj, i, j, turn))
	{
		char notationFrom[6+2] = {"TTa8"};
		notationFrom[0] = white;
		notationFrom[1] = (turn == 1)?white : black;
		notationFrom[2] += j;
		notationFrom[3] -= i;
		notationFrom[5] = '\0';

		char *notation = collision (table, notationFrom, obj, i, j, turn);

		//procurar por jogada já existente na lista
		int i;
		for(i = 0; i < size; i++)
		{
			if(!strcmp (list[i], notation))
				return 1;
		}

		list = (char**)realloc(list, sizeof(char*)*(++size));
		char* newPlay = (char*)malloc(sizeof(char)*(strlen(notation)+1));
		strcpy(newPlay, notation);
		list[size - 1] = newPlay;
		*old_list = list;
		*length = size;

		return 0;
	}
	else
	{
		//casa alvo tem inimigo
		if((getType(table[i][j]) < 'a') != turn && !riscoRei (table, obj, i, j, turn))
		{
			char notationFrom[7+2] = {"TTxa8"};
			notationFrom[0] = white;
			notationFrom[1] = (turn == 1)?white : black;
			notationFrom[3] += j;
			notationFrom[4] -= i;
			notationFrom[6] = '\0';

			char *notation = collision (table, notationFrom, obj, i, j, turn);

			//procurar por jogada já existente na lista
			int i;
			for(i = 0; i < size; i++)
			{
				if(!strcmp (list[i], notation))
					return 1;
			}

			list = (char**)realloc(list, sizeof(char*)*(++size));
			char* newPlay = (char*)malloc(sizeof(char)*(strlen(notation)+1));
			strcpy(newPlay, notation);
			list[size - 1] = newPlay;
			*old_list = list;
			*length = size;
		}
	}

	return 1;
}

/**
* Função irá criar lista de movimentos possível de um cavalo
* DESCRIÇÃO:
* 		Função irá analisar os movimentos de um cavalo, descrito por um movimento em L ignorando
* 		colisão com outras peças do tabuleiro, seja ela aliada ou inimiga para as 4 direções
* 		diagonais por 2 formas diferentes (L vertical ou L horizontal). O rei não deve ser exposto a cheque
* 		ou permanecer em cheque por um movimento do cavalo ou na sua ausência. Para cada movimento válido
* 		será adicionado na lista de jogadas.
*
* 	PARAMENTROS:
* 		MOV_PARAM - define em integration.h
*
* 	RETURNO:
* 		char** list - lista de jogadas em string
*/
char** movKnight (MOV_PARAM)
{
	char** list = NULL;
	if(obj != NULL)
	{
		//coordenadas da peça em índices matriciais
		int col = getObjectColumn(obj);
		int row = 7 - getObjectRow(obj);

		list = (char**)malloc(sizeof(char*));
		int size = 0;

		int turn = (getType(obj) < 'a')? 1 : 0;

		//o cavalo pode saltar para 8 pontos distintos sem colisão
		if(col - 2 >= 0 && row - 1 >= 0)
			__mov(table, obj, &list, &size, row - 1, col - 2, turn, 'N', 'n');

		if(col - 1 >= 0 && row - 2 >= 0)
			__mov(table, obj, &list, &size, row - 2, col - 1, turn, 'N', 'n');

		if(col + 1 < TABLE_COLS && row - 2 >= 0)
			__mov(table, obj, &list, &size, row - 2, col + 1, turn, 'N', 'n');

		if(col + 2 < TABLE_COLS && row - 1 >= 0)
			__mov(table, obj, &list, &size, row - 1, col + 2, turn, 'N', 'n');

		if(col - 2 >= 0 && row + 1 < TABLE_ROWS)
			__mov(table, obj, &list, &size, row + 1, col - 2, turn, 'N', 'n');

		if(col - 1 >= 0 && row + 2 < TABLE_ROWS)
			__mov(table, obj, &list, &size, row + 2, col - 1, turn, 'N', 'n');

		if(col + 1 < TABLE_COLS && row + 2 < TABLE_ROWS)
			__mov(table, obj, &list, &size, row + 2, col + 1, turn, 'N', 'n');

		if(col + 2 < TABLE_COLS && row + 1 < TABLE_ROWS)
			__mov(table, obj, &list, &size, row + 1, col + 2, turn, 'N', 'n');

		*length = size;
		list = sortList (list, size);
	}
	return list;
}

/*
* Função irá criar lista de movimentos possível de um bispo
* DESCRIÇÃO:
* 		Função irá analisar os movimentos de um bispo, descrito por avanço de casas em diagonal por quantas
* 		casas quiser antes de uma colisão com outra peça. Caso a peça colidida seja uma inimiga, então ela
* 		é capaz de capturá-la. O rei não deve ser exposto a cheque
* 		ou permanecer em cheque por um movimento do bispo ou na sua ausência. Para cada movimento válido
* 		será adicionado na lista de jogadas.
*
* 	PARAMENTROS:
* 		MOV_PARAM - define em integration.h
*
* 	RETURNO:
* 		char** list - lista de jogadas em string
*/
char** movBishop (MOV_PARAM)
{
	char** list = NULL;
	if(obj != NULL)
	{
		//coordenadas em índices matriciais da peça
		int col = getObjectColumn(obj);
		int row = 7 - getObjectRow(obj);

		list = (char**)malloc(sizeof(char*));
		int size = 0;

		int turn = (getType(obj) < 'a')? 1 : 0;

		int i, j;

		//fazer o percurso para as 4 diagonais a partir do bispo
		for(i = row - 1, j = col - 1; i >= 0 && j >= 0; i-- , j--)
		{
			if(__mov(table, obj, &list, &size, i, j, turn, white, black))
				break;
		}
		for(i = row + 1, j = col -1; i < TABLE_ROWS && j >= 0; i++, j--)
		{
			if(__mov(table, obj, &list, &size, i, j, turn, white, black))
				break;
		}
		for(i = row - 1, j = col + 1; i >= 0 && j < TABLE_COLS; i--, j++)
		{
			if(__mov(table, obj, &list, &size, i, j, turn, white, black))
				break;
		}
		for(i = row + 1, j = col + 1; i < TABLE_ROWS && j < TABLE_COLS; i++, j++)
		{
			if(__mov(table, obj, &list, &size, i, j, turn, white, black))
				break;
		}

		//ordenar a lista
		*length = size;
		list = sortList (list, size);
	}
	return list;
}

/**
* Função irá criar lista de movimentos possível de um torre
* DESCRIÇÃO:
* 		Função irá analisar os movimentos de um torre, descrito por avanço de casas em eixos ortonormais do tabuleiro,
* 		a quantas casas quiser até que haja uma colisão com outra peça. Caso esta peça seja uma inimiga ela pode ser
* 		capturada. Também é possível realizar um movimento roque em condições legais. O rei não deve ser exposto a cheque
* 		ou permanecer em cheque por um movimento do torre ou na sua ausência. Para cada movimento válido
* 		será adicionado na lista de jogadas.
*
* 	PARAMENTROS:
* 		MOV_PARAM - define em integration.h
*
* 	RETURNO:
* 		char** list - lista de jogadas em string
*/
char** movRook (MOV_PARAM)
{
	char **list = NULL;
	if(obj != NULL)
	{
		//coordenadas em índices matricias da peça
		int col = getObjectColumn(obj);
		int row = 7 - getObjectRow(obj);

		list = (char**)malloc(sizeof(char*));
		int size = 0;

		int turn = (getType(obj) < 'a')? 1 : 0;

		int i, j;

		//fazer o percurso para as 4 retas a partir do bispo
		for(i = row - 1, j = col; i >= 0; i--)
		{
			if(__mov(table, obj, &list, &size, i, j, turn, white, black))
				break;
		}
		for(i = row + 1, j = col; i < TABLE_ROWS; i++)
		{
			if(__mov(table, obj, &list, &size, i, j, turn, white, black))
				break;
		}
		for(i = row, j = col - 1; j >= 0; j--)
		{
			if(__mov(table, obj, &list, &size, i, j, turn, white, black))
				break;
		}
		for(i = row, j = col + 1; j < TABLE_COLS; j++)
		{
			if(__mov(table, obj, &list, &size, i, j, turn, white, black))
				break;
		}

		//roque, a roque não será possível para caso o rei esteja em cheque
		if(!riscoRei(table, obj, row, col, turn))
		{
			OBJETO *king = getKingTable (table, turn);
			//o roque só será permitido caso dentro da notação FEN haja a legalização
			for(i = 0; i < strlen(fen->rock) && white == 'R'; i++)
			{
				//OBS.:  para cada roque possível será feita uma substituição da peça para avaliar o risco do rei
				//Após a análise é feita o retorno do estado anterior do tabuleiro.
				if((fen->rock[i] < 96) == turn)
				{
					//roque para o lado da rainha para o turno branco
					if(fen->rock[i] == 'Q' && getObjectColumn(king) == 0 && 7 - getObjectRow(king) == TABLE_ROWS - 1 &&
							table[TABLE_ROWS - 1][1] == NULL && table[TABLE_ROWS - 1][2] == NULL  && table[TABLE_ROWS - 1][3] == NULL)
					{
						if(!riscoRei(table, king, TABLE_ROWS - 1, 3, turn) &&
								!riscoRei(table, king, TABLE_ROWS - 1, 2, turn))
						{
							table[7][2] = table[7][4];
							table[7][4] = NULL;
							__mov(table, obj, &list, &size, TABLE_ROWS - 1, 3, turn, white, black);
							table[7][4] = table[7][2];
							table[7][2] = NULL;
						}
					}

					//roque para o lado da rainha para o turno preto
					else if(fen->rock[i] == 'q' && getObjectColumn(obj) == 0 && 7 - getObjectRow(obj) == 0 &&
							table[0][1] == NULL && table[0][2] == NULL && table[0][3] == NULL)
					{
						if(!riscoRei(table, king, 0, 3, turn) &&
								!riscoRei (table, king, 0, 2, turn))
						{
							table[0][2] = table[0][4];
							table[0][4] = NULL;
							__mov(table, obj, &list, &size, 0, 3, turn, white, black);
							table[0][4] = table[0][2];
							table[0][2] = NULL;
						}
					}

					//roque para o lado do reio para o turno branco
					else if(fen->rock[i] == 'K' && getObjectColumn(obj) == TABLE_COLS - 1 && 7 - getObjectRow (obj) == TABLE_ROWS - 1 &&
							table[TABLE_ROWS - 1][TABLE_COLS - 3] == NULL && table[TABLE_ROWS - 1][TABLE_COLS - 2] == NULL)
					{
						if(!riscoRei(table, king, TABLE_ROWS - 1, 5, turn))
						{
							table[7][6] = table[7][4];
							table[7][4] = NULL;
							__mov(table, obj, &list, &size, TABLE_ROWS - 1, TABLE_COLS - 2, turn, white, black);
							table[7][4] = table[7][6];
							table[7][6] = NULL;
						}
					}

					//roque para o lado do rei para turno branco
					else if(fen->rock[i] == 'k' && getObjectColumn(obj) == TABLE_COLS - 1  && 7 - getObjectRow(obj) == 0 &&
							table[0][TABLE_COLS - 3] == NULL && table[0][TABLE_COLS - 2] == NULL)
					{
						if(!riscoRei(table, king, 0, 5, turn))
						{
							table[0][6] = table[0][4];
							table[0][4] = NULL;
							__mov(table, obj, &list, &size, 0, TABLE_COLS - 2, turn, white, black);
							table[0][4] = table[0][6];
							table[0][6] = NULL;
						}
					}
				}
			}
		}
		//readicionar a lista no objeto
		*length = size;
		list = sortList (list, size);
	}

	return list;
}

/**
* Função irá criar lista de movimentos possível de uma rainha
* DESCRIÇÃO:
* 		Função irá analisar os movimentos de uma rainha, descrito por avanço de casas em eixos ortonormais e nos
* 		eixos diagonais do tabuleiro, a quantas casas quiser até que haja uma colisão com outra peça.
* 		Caso esta peça seja uma inimiga ela pode ser capturada. O rei não deve ser exposto a cheque
* 		ou permanecer em cheque por um movimento do rainha ou na sua ausência. Para cada movimento válido
* 		será adicionado na lista de jogadas.
* 		Na prática será executada o movimento de um bispo e de uma torre em nome da rainha.
*
* 	PARAMENTROS:
* 		MOV_PARAM - define em integration.h
*
* 	RETURNO:
* 		char** list - lista de jogadas em string
*/
char** movQueen (MOV_PARAM)
{
	int size;
	char **listBishop = movBishop(MOV_VALUE);
	int bishopSize = size;
	char **listRook = movRook(MOV_VALUE);
	size += bishopSize;

	listBishop = (char**)realloc(listBishop, sizeof(char*)*size);
	int i;
	for(i = bishopSize; i < size; i++)
	{
		listBishop[i] = listRook[i - bishopSize];
	}

	char ** list = sortList (listBishop, size);
	free(listRook);

	*length = size;
	return list;
}

/**
* Função irá criar lista de movimentos possível de um rei
* DESCRIÇÃO:
* 		Função irá analisar os movimentos de um rei, descrito por avanço de casas para todas as direções possíves
* 		com uma distancia limitadas por uma casa. Caso esta peça seja uma inimiga ela pode ser capturada.
* 		Também é possível realizar um movimento roque em condições legais. O rei não deve ser exposto a cheque
* 		ou permanecer em cheque por um movimento do peão ou na sua ausência. Para cada movimento válido
* 		será adicionado na lista de jogadas.
*
* 	PARAMENTROS:
* 		MOV_PARAM - define em integration.h
*
* 	RETURNO:
* 		char** list - lista de jogadas em string
*/
char** movKing (MOV_PARAM)
{
	char** list = NULL;
	if(obj != NULL)
	{
		//coordenadas de índices matricias da peça
		int col = getObjectColumn(obj);
		int row = 7 - getObjectRow(obj);

		//controle de borda para looping
		int col_border = (col - 1 < 0)? 0 : 1;
		int row_border = (row - 1 < 0)? 0 : 1;

		list = (char**)malloc(sizeof(char*));
		int size = 0;

		//variável identificadora de alidos
		int turn = (getType(obj) < 'a')? 1 : 0;

		int j, i;
		for(j = col - col_border; j <= col + 1 && j < TABLE_COLS; j++)
		{
			for(i = row - row_border; i <= row + 1 && i < TABLE_ROWS; i++)
			{
				//o rei não pode se alto capturar porque é do mesmo tipo do alvo(si)
				//if(j != col || i != row)
					//casa vazia
					if(table[i][j] == NULL && !riscoRei (table, obj, i, j, turn))
					{
						char notationFrom[6+2] = {"TTa8"};
						notationFrom[0] = 'K';
						notationFrom[1] = (turn == 1)?'K': 'k';
						notationFrom[2] += j;
						notationFrom[3] -= i;
						notationFrom[5] = '\0';

						char *notation = collision (table, notationFrom, obj, i, j, turn);

						int count;

						//percorrer a lista em busca de jogada repetida
						for(count = 0; count < size; count++)
						{
							if(!strcmp (list[count], notation))
								break;
						}
						//se existe a jogada na lista, então a busca não percorreu a lista toda
						if(count < size)
							continue;

						char* newPlay = (char*)malloc(sizeof(char)*(strlen(notation)+1));
						list = (char**)realloc(list, sizeof(char*)*++size);
						strcpy(newPlay, notation);

						list[size - 1] = newPlay;
					}
					//casa com inimigo
					else if((getType(table[i][j]) < 96) != turn && !riscoRei(table, obj, i, j, turn))
					{
						char notationFrom[7+2] = {"TTxa8"};
						notationFrom[0] = 'K';
						notationFrom[1] = (turn == 1)?'K': 'k';
						notationFrom[3] += j;
						notationFrom[4] -= i;
						notationFrom[6] = '\0';

						char *notation = collision (table, notationFrom, obj, i, j, turn);

						int size = getNList(obj);
						int count;
						for(count = 0; count < size; count++)
						{
							if(!strcmp (list[count], notation))
								break;
						}
						if(count < size)
							continue;

						char* newPlay = (char*)malloc(sizeof(char)*(strlen(notation)+1));
						list = (char**)realloc(list, sizeof(char*)*++size);
						strcpy(newPlay, notation);

						list[size - 1] = newPlay;
					}
			}
		}

		//roque
		//para o roque o rei não deve estar em cheque, entrar em cheque ou passar por casas sob ataque
		if(!riscoRei(table, obj, row, col, turn))
		{
			for(i = 0; i < strlen(fen->rock); i++)
			{
				//OBS.:  para cada roque possível será feita uma substituição da peça para avaliar o risco do rei
				//Após a análise é feita o retorno do estado anterior do tabuleiro.
				if((fen->rock[i] < 96) == turn)
				{
					OBJETO *auxKing;
					//roque para o lado da rainha em turno branco
					if(fen->rock[i] == 'Q' && getObjectColumn(obj) == 4 &&
							table[TABLE_ROWS - 1][1] == NULL && table[TABLE_ROWS - 1][2] == NULL && table[TABLE_ROWS - 1][3] == NULL)
					{
						if(!riscoRei(table, obj, TABLE_ROWS - 1, 3, turn))
							__mov(table, obj, &list, &size, TABLE_ROWS - 1, 2, turn, 'K', 'k');
					}

					//roque para o lado da rainha em turno preto
					else if(fen->rock[i] == 'q' && getObjectColumn(obj) == 4 &&
							table[0][1] == NULL && table[0][2] == NULL  && table[0][3] == NULL)
					{
						if(!riscoRei(table, obj, 0, 3, turn))
							__mov(table, obj, &list, &size, 0, 2, turn, 'K', 'k');
					}

					//roque para o lado do rei em turno branco
					else if(fen->rock[i] == 'K' && getObjectColumn(obj) == 4 &&
							table[TABLE_ROWS - 1][TABLE_COLS - 3] == NULL && table[TABLE_ROWS - 1][TABLE_COLS - 2] == NULL)
					{
						if(!riscoRei(table, obj, TABLE_ROWS - 1, 5, turn))
							__mov(table, obj, &list, &size, TABLE_ROWS - 1, TABLE_COLS - 2, turn, 'K', 'k');
					}

					//roque para o lado do rei em turno preto
					else if(fen->rock[i] == 'k' && getObjectColumn(obj) == 4 &&
							table[0][TABLE_COLS - 3] == NULL && table[0][TABLE_COLS - 2] == NULL)
					{
						if(!riscoRei(table, obj, 0, 5, turn))
							__mov(table, obj, &list, &size, 0,  TABLE_COLS - 2, turn, 'K', 'k');
					}
				}
			}
		}
		//readicionar a lista no objeto
		*length = size;
		list = sortList (list, size);
	}

	return list;
}

/**
 * função irá imprimir todas tokens da lista, ou seja, todos os movimentos possíveis.
 * DESCRIÇÂO:
 * 		Função irá percorrer a lista de jogadas/strings e irá imprimir na tela, sendo
 * 		o tipo da peça sempre em maísculo.
 *
 * 	PARAMETROS:
 * 		char** list - lista de jogadas
 * 		int size - tamanho da lista
 *
 * 	RETORNO:
 * 		VOID
 */
void printListMov (char** list, int size)
{
	if(list != NULL)
	{
		int i;
		for(i = 0; i < size; i++)
		{
			//fprintf(stdout, "teste impressao: %s\t", list[i] + 1);
			char *paux = list[i] + 1;
			char type = list[i][1];
			int plus = 0;
			if(type > 'h' || (list[i][0] == 'B' && list[i][1] == 'b'))
				plus = 32;

			paux[0] -= (char)plus;
			fprintf(stdout, "%s\n", paux);

			paux[0] += (char)plus;
		}
	}
}

/*
 *Coleção de funções para impressão de jogadas para cada tipo de peça
 */


void printListMovPeasant (MOV_PARAM)
{
	int size = 0;
	char** list = movPeasant(MOV_VALUE);
	setList(obj, list);
	setNList(obj, size);
	printListMov(list, getNList(obj));
}

void printListMovKnight (MOV_PARAM)
{
	int size = 0;
	char** list = movKnight(MOV_VALUE);
	setList(obj, list);
	setNList(obj, size);
	printListMov(list, getNList(obj));
}

void printListMovBishop (MOV_PARAM)
{
	int size = 0;
	char ** list = movBishop(MOV_VALUE);
	setList(obj, list);
	setNList(obj, size);
	printListMov(list, getNList(obj));
}

void printListMovRook (MOV_PARAM)
{
	int size = 0;
	char** list = movRook(MOV_VALUE);
	setList(obj, list);
	setNList(obj, size);
	printListMov(list, getNList(obj));
}

void printListMovQueen (MOV_PARAM)
{
	int size = 0;
	char** list = movQueen(MOV_VALUE);
	setList(obj, list);
	setNList(obj, size);
	printListMov(list, getNList(obj));
}

void printListMovKing (MOV_PARAM)
{
	int size = 0;
	char ** list = movKing(MOV_VALUE);
	setList(obj, list);
	setNList(obj, size);
	printListMov(list, getNList(obj));
}

//********************************** trabalho 07
/**
 * Função irá efetivar a jogada e aplicar as mudanças dela decorrente
 *
 * DESCRIÇÃO:
 * 		Função irá receber uma jogada já validada, assim com as coordenadas da peça já atualizada,
 * 		efetiva-se as mudanças no vetor de coleções, marcando a peça capturada como inativa e
 * 		armazenando o turno da captura na peça da jogada, movendo a peça na matriz de tabuleiro,
 * 		e fazendo a atualização para torres, em roques, e aplicando a promoção para peões quando
 * 		configurados para tal. Caso haja promoção a coleção é ordenada.
 *
 * 	PARAMETROS:
 * 		OBJETO *** table - ponteiro para matriz de tabuleiro
 * 		OBJEOT ** collection - vetor de peças do jogo
 * 		int white_pieces - numero de peças brancas
 * 		int pieces_num - número total de peças
 * 		int fullTurn - número do turno completo
 *
 * 	RETORNO:
 * 		OBJETO ** collection - vetor de peças do jogo (obs.: ela não é necessária no retorno, a ordenação não perde o endereço)
 */
OBJETO ** doPlay(OBJETO *** table, PLAY play, OBJETO ** collection, int white_pieces, int pieces_num, int fullTurn)
{
	if(table != NULL)
	{
		int row = 7 - getObjectRow(play.obj);
		int col = getObjectColumn(play.obj);

		//jogada é uma captura
		if(table[row][col] != NULL)
		{
			captured(table[row][col]);
			setObjectTurn(play.obj, fullTurn);
		}

		//mover no tabuleiro
		table[row][col] = play.obj;
		table[play.fromRow][play.fromCol] = NULL;

		//movimento de roque
		int diff;
		if((getType(play.obj) == 'k' || getType(play.obj) == 'K' ) && abs(diff = play.fromCol - getObjectColumn(play.obj)) > 1)
		{
			//roque para rei
			if(diff < 0)
			{
				table[7 - getObjectRow(play.obj)][getObjectColumn(play.obj) - 1] = table[7 - getObjectRow(play.obj)][TABLE_COLS - 1];
				table[7 - getObjectRow(play.obj)][TABLE_COLS - 1] = NULL;

				char *position = getPosition(play.obj);
				char *newPosition = (char*)malloc(sizeof(char)*(strlen(position)+1));
				strcpy(newPosition, position);
				newPosition[0] -= 1;
				changePosition(table[7 - getObjectRow(play.obj)][getObjectColumn(play.obj) - 1], newPosition);
			}
			//roque para rainha
			else
			{
				table[7 - getObjectRow(play.obj)][getObjectColumn(play.obj) + 1] = table[7 - getObjectRow(play.obj)][0];
				table[7 - getObjectRow(play.obj)][0] = NULL;

				char *position = getPosition(play.obj);
				char *newPosition = (char*)malloc(sizeof(char)*(strlen(position)+1));
				strcpy(newPosition, position);
				newPosition[0] += 1;
				changePosition(table[7 - getObjectRow(play.obj)][getObjectColumn(play.obj) + 1], newPosition);
			}
		}

		//nova ordenação só é necessária quando há promoção de peão, caso contrário a ordem é mantida
		char turn = (getType(play.obj) == 'P') ? 'w' : 'b';
		if(play.promotion != '-')
		{
			changeType(play.obj, play.promotion, getFunctionType(play.promotion));

			//ordenar por critério de desempate e apenas as peças do turno
			if(turn == 'w')
				qsort(collection, white_pieces, sizeof(OBJETO*), &desempate);
			else
				qsort(collection + white_pieces, pieces_num - white_pieces, sizeof(OBJETO*), &desempate);
		}
	}
	return collection;
}

/**
 * Função irá atualizar a coleção de peças segundo a jogada anterior
 * DESCRIÇÃO:
 * 		Função irá buscar por peças desativadas e eliminá-lo da memória. Assim remanejar
 * 		as peças posteriores para a lacuna, realocando o vetor para o tamanho reduzido.
 * 		Atualiza-se os valores de peças brancas e peças totais.
 *
 * PARAMETROS:
 * 		OBJETO ** collection - coleção de peças
 * 		int *white_pieces - endereço do número de peças brancas
 * 		int *pieces_pieces - endereço do número total de peças
 * 		int turn - valor representativo de turno (1- branco, 0- preto)
 *
 * RETORNO:
 * 		OBJETO **collection - coleção de peças atualizadas
 */
OBJETO ** updateCollection (OBJETO ** collection, int *white_pieces, int *pieces_num, int turn)
{
	if(collection != NULL && pieces_num != NULL && white_pieces != NULL)
	{
		int i;
		int newSize = *pieces_num;
		for(i = 0; i < *pieces_num; i++)
		{
			//manter armazenado as peças ainda ativas
			if(!getActive(collection[i]))
			{
				//peças desativadas
				int j;
				//deletar a peça desativada
				deleteObject(&(collection[i]));

				//puxar a lista para diminuir em uma posição, a que foi deletada
				for(j = i; j + 1 < *pieces_num; j++)
					collection[j] = collection[j + 1];

				collection = (OBJETO**)realloc(collection, sizeof(OBJETO*)*(--newSize));
				//para cada update só pode haver uma captura, uma vez identificada não precisa percorrer o resto
				break;
			}

		}

		//armazenar os novos dados
		*white_pieces -= ((*pieces_num - newSize) * !turn); //se o turno é do Preto, então a peça capturada é a Branca
		*pieces_num = newSize;
	}
	return collection;
}
