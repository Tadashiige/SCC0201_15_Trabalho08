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
 * Arquivo regulador de regra - compor funções reguladores de regras para conferência de jogadas
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "objeto.h"
#include "peca.h"
#include "regra.h"
#include "FEN.h"


//função para retornar a notação de coluna dado coordenada matricial
char tableColumn (int col)
{
	return 'a' + (col - 1);
}

/**
 * Função retorna a função exclusiva para cada tipo de peça
 *
 * DESCRIÇÃO:
 * 		Função irá buscar por switch o tipo da peça a ser criada e retorna o endereço da função
 * 		respectiva a ela, sendo todas padronizadas em parametros e retornos, mas
 * 		tem comportamentos diferentes.
 *
 *	PARAMETROS:
 *		char type - tipo de peça
 *
 *	RETORNO:
 *		char* (*funcType)(MOV_PARAM) - endereço para função que
 *								retorna string e recebe parametros OBJETO *** const table e OBJETO * const obj.
 */
funcPtr getFunctionType (char type){
	if(type > 96)
		type -= 32;

	funcPtr funcType;

	switch(type)
	{
	case 'P':
		funcType = &movPeasant;
		break;
	case 'N':
		funcType = &movKnight;
		break;
	case 'B':
		funcType = &movBishop;
		break;
	case 'R':
		funcType = &movRook;
		break;
	case 'Q':
		funcType = &movQueen;
		break;
	case 'K':
		funcType = &movKing;
		break;
	default:
		funcType = NULL;
		break;
	}

	return funcType;
}

/**
 * Função extrai os dados das peças da notação FEN
 *
 * DESCRIÇÃO:
 * 		Função irá a partir da notação FEN extrair dados de tipo de peça, sua posição
 * 		dentro do tabuleiro, e a quantidade de peças existente na partida. Assim,
 * 		a posição dos valores dentro da string pieces é diretamente representativo
 * 		para a peça. Para cada peça identificada e os dados extraídos cria-se a
 * 		peça em memória e a armazena no vetor de objetos, para retorná-lo no fim.
 *
 * 	PARAMETROS:
 * 		FEN* fen - ponteiro para estrutura de dados de entrada em notação FEN
 * 		int* tam - endereço da variável externo do tamanho do vetor de objetos
 * 		int* white - endereço da variável externa do número de peças brancas
 *
 * 	RETORNO:
 * 		OBJETO **objetos - vetor de ponteiros para objetos criados da extração
 */
OBJETO ** extractTableFEN (char* pieces, int* tam, int* white)
{
	OBJETO **objetos = (OBJETO **)malloc(sizeof(OBJETO*));
	(*tam) = 0;
	(*white) = 0;

	if(pieces != NULL && tam != NULL && white != NULL && objetos != NULL)
	{
		int i;
		int row = 1;
		int aux = 0;
		//a posição na notação FEN indica a posição no tabuleiro
		for(i = 0; i < strlen (pieces); i++)
		{
			if(pieces[i] == '/')
			{
				row++;
				aux = 0;
			}
			else
			{
				//os números representam casas vazias
				if(48 <= pieces[i] && pieces[i] <= 57)
				{
					aux += atoi(&(pieces[i]));
				}
				else
				{
					char* pos = (char*) malloc(sizeof(char)*3);
					//pela tabela ascii 97 é 'a', como a coluna varia de 'a' (97+0) a 'h'(97+7), então apenas soma-se ao 97
					pos[0] = 97 + aux;
					//seguindo o mesmo principio para a linha anterior, '9' - 1 = '8', '9' - 5 = '4'...
					pos[1] = '9' - row;
					pos[2] = '\0';
					//só criar o objeto se existe um objeto
					if(isgraph((int)pieces[i]) && pieces[i] != '/')
					{
						funcPtr funcType;
						funcType = getFunctionType(pieces[i]);
						if(funcType != NULL)
						{
							OBJETO *tmp = createObject(pieces[i], pos, funcType);
							if(tmp != NULL)
							{
								(*tam)++;
								objetos = (OBJETO **)realloc (objetos, sizeof(OBJETO*) * (*tam));
								objetos[(*tam)-1] = tmp;
								tmp = NULL;
								if(pieces[i] < 96)
									(*white)++;
							}
							else
								free(pos);
						}
						else
							free(pos);
					}
					else
						free(pos);

					//a peça ocupa uma casa
					aux++;
				}
			}
		}
	}

	return objetos;
}

/**
 * Função retorna coluna da peça em coordenada matricial
 * a posição armazenado é da forma 'char-letra''char-numero', onde letra começa em 'a'(97) e número em '1'(49)
 */
int getObjectColumn (OBJETO *obj)
{
	if(obj != NULL)
	{
		return (int)(getPosition(obj)[0] - 97);
	}
	return -1;
}
/**
 * Função retorna linha da peça em coordenada matricial
 * a posição armazenado é da forma 'char-letra''char-numero', onde letra começa em 'a'(97) e número em '1'(49)
 */
int getObjectRow (OBJETO *obj)
{
	if(obj != NULL)
	{
		return (int)(getPosition(obj)[1] - 49);
	}
	return -1;
}

/**
 * Função retorna a coluna da notação em coordenada matricial
 * a posição armazenado é da forma 'tipo''char-letra''char-numero', onde letra começa em 'a'(97) e número em '1'(49)
 */
int getNotationColumn (char *not)
{
	if(not != NULL)
	{
		not++;
		return (int)(not[(not[1] != 'x')?1:2] - 'a');
	}
	return -1;
}
/**
 * Função retorna a linha da notação em coordenada matricial
 * a posição armazenado é da forma 'tipo''char-letra''char-numero', onde letra começa em 'a'(97) e número em '1'(49)
 */
int getNotationRow (char *not)
{
	if(not != NULL)
	{
		not++;
		return (int)(not[(not[1] != 'x')?2:3] - '1');
	}
	return -1;
}

/**
 * função irá adicionar notação para colisão de peças caso ocorra
 */
char* collision (OBJETO *** const table, char *notation, OBJETO *const obj, int row, int col, int turn)
{
	return notation;
}

/**
 * Função ira mudar a notação da jogada adicionando a notação para diferenciar jogadas com notações iguais.
 * DESCRIÇÃO:
 * 		Função irá copiar a jogada atual e acrescentar identificador de coluna e/ou linha na notação.
 * 		A cópia irá substituir a string atual e a antiga será liberada da memória.
 *
 * 	PARAMETROS:
 * 		CONFLICT item - estrutura que contem ponteiro para objeto e endereço para string da jogada
 * 		int value - linha ou coluna identificador
 * 		int plus - linha identificadora para quando houver necessidade de dois identificadores, -1 quando não
 *
 * 	RETORNO:
 * 		VOID
 */
void changePlay (CONFLICT item, int value, int plus)
{
	int aditional = 1;
	if(plus >= 0)
		aditional = 2;

	//copiar a jogada abrindo uma lacuna para adição de identificadores
	char strAux[strlen(*(item.play))+aditional+1];
	strcpy(strAux+aditional, *(item.play));
	strncpy(strAux, *(item.play), 2);

	//adição de identificador
	strAux[2] = value;
	if(plus >= 0)
		strAux[3] = plus;

	//substituição
	char *newPlay = (char*)malloc(sizeof(char)*(strlen(strAux)+1));
	strcpy(newPlay, strAux);
	char *rem = *(item.play);

	free(rem);

	*(item.play) = newPlay;
}

/**
 *	Função recebe vetor de string com comando repetidos, então adiciona-se aos string a notação de identificação
 *	DESCRIÇÂO:
 *		Função ira interpretar o vetor e analisar quantas repetições existem. Assim irá fazer a adição de
 *		identificadores segundo a possibilidade (coluna e/ou linha).
 *
 *	PARAMETROS:
 *		CONFLICT *cmd - ponteiro para estrutura contenedora ponteiro de objeto e endereço da string
 *		int ini - índice inicial do vetor
 *		int end - índice final do vetor
 *
 *	RETORNO:
 *		VOID
 */
void addConflictNotation (CONFLICT* cmd, int ini, int end)
{
	int num = end - ini;

	//não existe jogadas iguais
	if(num == 0)
		return;

	//existem duas peças com jogadas iguais
	if(num == 1)
	{
		if(getObjectColumn(cmd[ini].obj) != getObjectColumn(cmd[end].obj))
		{
			//50 é a diferença pela table ascii entre o '1' (47) e 'a' (97)
			changePlay(cmd[ini], getObjectColumn(cmd[ini].obj) + 97, -1);
			changePlay(cmd[end], getObjectColumn(cmd[end].obj) + 97, -1);
		}
		else
		{
			//a linha é armazenada com '1' + value, portanto não necessita de soma
			changePlay(cmd[ini], getObjectRow(cmd[ini].obj) + 47, -1);
			changePlay(cmd[end], getObjectRow(cmd[end].obj) + 47, -1);
		}
	}
	//existem mais de 2 peças com jogadas iguais
	else
	{
		int i, j;
		//para mais de 2 peças é possível que identificador única não seja suficiente, então analisar linha E coluna
		int testVR[num];
		int testVC[num];

		for(i = 0; i < num; i++)
		{
			testVR[i] = 0;
			testVC[i] = 0;
		}

		//fazer busca por colunas e/ou linhas iguais
		for(i = 0; i < num; i++)
		{
			for(j = i + 1; j < num; j++)
			{
				//linhas iguais
				if(getObjectRow(cmd[ini + i].obj) == getObjectRow(cmd[ini + j].obj))
				{
					testVR[i]++;
					testVR[j]++;
				}
				//colunas iguais
				if(getObjectColumn(cmd[ini + i].obj) == getObjectColumn(cmd[ini + j].obj))
				{
					testVC[i]++;
					testVC[j]++;
				}
			}
		}

		//fazer o acréscimo de notação de diferenciação
		for(i = ini; i <= end; i++)
		{
			//apenas identificador de coluna
			if(testVC[i] == 0)
				changePlay(cmd[i], getObjectColumn(cmd[i].obj) + 97, -1);
			//apenas identificador e linha
			else if(testVR[i] == 0)
				changePlay(cmd[i], getObjectRow(cmd[i].obj) + 47, -1);
			//identificador duplo
			else
				changePlay(cmd[i], getObjectColumn(cmd[i].obj) + 97, getObjectRow(cmd[i].obj) + 47);
		}
	}//else
}

/**
 * Funçao irá adicionar notação para colisão de pecas entre peças do vetor
 * DESCRIÇÃO:
 * 		Função irá criar vetores auxiliares correspondentes a coleção. Através do looping será feito
 * 		busca por jogadas iguais e com o uso de vetor de índices serão avaliadas sempre as jogadas mais
 * 		baixas de cada lista, para que não haja perda de análise, ou seja, o índice irá avançar sempre
 * 		que ela foi menor ou igual em relação as jogadas de outras peças.
 *
 * 	PARAMETROS:
 * 		OBJETO ** const collection - vetor de objetos
 * 		int size - tamanho do vetor
 *
 *
 * 	RETORNO:
 * 		VOID
 */
void _conflict (OBJETO ** const collection, int size)
{
	if(collection != NULL && size > 0)
	{
		char** piecesList[size];
		int indexList [size];
		int count[size];

		//pegas as listas de movimentos possíveis de cada peça
		int i;
		for(i = 0; i < size; i++)
		{
			piecesList[i] = getList (collection[i]);
			indexList[i] = 0;
			count[i] = getNList (collection[i]);
		}


		int cmd_size = INT_MAX;
		int ini = 0, end = ini;
		CONFLICT cmd[size];

		//fazer busca-comparação até que todos os índices de vetores cheguem ao fim
		while(ini < cmd_size)
		{
			cmd_size = 0;
			//compor vetor com jogadas do mesmo tipo de peça com os menores valores
			for(i = 0; i < size; i++)
			{
				if(indexList[i] < count[i])
				{
					cmd[cmd_size].play = &(piecesList[i][indexList[i]]);
					cmd[cmd_size].obj = collection[i];
					cmd[cmd_size].order = i;
					cmd_size++;
				}
			}

			//ordenar segundo crierio de desempate proposto (coluna e linha)
			qsort(cmd, cmd_size, sizeof(CONFLICT), &sortPlayConflict);

			//achar indice da lista a ser avançada, sendo sempre a jogada de menor valor (segundo critério de desempate)
			for(i = 0; i < cmd_size && indexList[cmd[i].order] >= count[cmd[i].order]; i++);

			//verificar se todas as jogadas ja' não foram verificadas
			if(i >= cmd_size)
				break;


			ini = i;
			end = ini;
			char *typeCmd = *(cmd[ini].play);
			//caso jogadas anteriormente iguais cair na comparação já terão sido distinguidos por notação e serão diferentes
			while(end < cmd_size && !strcmp(typeCmd, *(cmd[end].play)))
			{
				//avançar com os índices com os menores valores de desempate
				indexList[cmd[end++].order]++;
			}

			addConflictNotation (cmd, ini, end - 1);
		}//while least < size
	}//if collection != NULL size > 0
}

/**
 * função irá adicionar notação para colisão de peças após todas as análises de peças
 * DESCRIÇÂO:
 * 		Função irá criar uma lista de peças de mesmo tipo para que possa se buscar jogadas para a mesma casa
 * 		sobre mesmas condições.
 *
 * 	PARAMETROS:
 * 		OBJETO **const collecton - vetor de peças
 * 		int size - tamanho do vetor
 *
 * 	RETORNO:
 * 		VOID
 */
void conflict (OBJETO **const collection, int size)
{
	if(collection != NULL && size > 0)
	{
		int i;
		OBJETO ** pecas = (OBJETO**) malloc(sizeof(OBJETO*));
		int length = 0;
		char tipo = getType(collection [0]);

		for(i = 0; i < size; i++)
		{
			//colocar na lista as peças de mesmo tipo
			if(tipo == getType(collection[i]))
			{
				length++;
				pecas = (OBJETO**)realloc(pecas, sizeof(OBJETO*)*length);
				pecas[length - 1] = collection[i];
			}
			//reiniciar a lista de peças e realizar a identificação de conflito
			else
			{
				tipo = getType(collection[i]);
				_conflict(pecas, length);
				free(pecas);
				pecas = (OBJETO**)malloc(sizeof(OBJETO*));
				pecas[0] = collection[i];
				length = 1;
			}
		}
		//quando sae do looping a condição i >= size é atingida, portanto ainda existe a última jogada para ser analizada
		_conflict (pecas, length);
		//liberar apenas o vetor de peças, as peças são mantidas na memória
		free(pecas);
	}
}

//******************************** Trabalho 7

//função irá criar a estrutura que comporta os dados da notação FEN
FEN* createFEN (char* string)
{
	FEN* fen = (FEN*) malloc(sizeof(FEN));
	char* token;
	token = strtok (string, " ");
	int i;
	char *paux;
	char **vaux = (char**) malloc(sizeof(char));
	//analisar as strings separadas por espaços
	/*
	 * São dados pela notação:
	 * 1 - peças no tabuleiro
	 * 2 - turno da rodada
	 * 3 - disponibilidade de roque
	 * 4 - disponibilidade de en passant
	 * 5 - quantidade de meio turnos
	 * 6 - quantidade de turno completo
	 */
	for(i = 0; token != NULL; i++)
	{
		paux = (char*)malloc(sizeof(char)*(strlen(token)+1));
		strcpy(paux, token);

		vaux = (char**)realloc(vaux, sizeof(char*)*(i+1));
		vaux[i] = paux;

		token = strtok(NULL, " ");
	}

	strcpy(fen->pieces, vaux[0]);
	fen->turn = *(vaux[1]);
	strcpy(fen->rock, vaux[2]);
	strcpy(fen->pass, vaux[3]);
	fen->halfTurn = atoi(vaux[4]);
	fen->fullTurn = atoi(vaux[5]);

	for(i--; i >= 0; i--)
		free(vaux[i]);
	free(vaux);

	return fen;
}

//função irá eliminar a estrutura fen da memória
void deleteFEN (FEN** fen)
{
	if(fen != NULL && (*fen) != NULL)
	{
		free(*fen);
		*fen = NULL;
	}
}

//função intercala o turno para próxima jogada
void changeTurn (FEN *fen)
{
	if(fen != NULL)
	{
		if(fen->turn == 'w')
			fen->turn = 'b';
		else
			fen->turn = 'w';
	}
}

/**
 * função irá atualizar a situação atual do tabuleiro e suas condições
 * DESCRIÇÃO:
 * 		Função irá percorrer a matriz de tabuleiro e irá iterativamente criando uma nova string
 * 		com as configurações da atual matriz. Em seguida ele irá avaliar segundo a notação anterior
 * 		de roque para criar uma nova string de roque para avaliar a situação atual de condição para
 * 		roque, enserindo a condição apenas para o lado onde não houve alteração. Posteriormente é
 * 		avaliado se houve um movimento de um peão na jogada atual se configura um en passant para
 * 		o adiversário e armazena a nova situação na notação. Por fim são atualizadas os valores de
 * 		meio turno, analizando os parametros da jogada atual (peão, ou captura) e incrementando o
 * 		turno completo após jogada do lado PRETO.
 *
 * 	PARAMETROS:
 * 		FEN *fen - ponteiro para estrutura da notação FEN
 * 		OBJETO *** const table - ponteiro para matriz de tabuleiro
 * 		PLAY play - estrutura que contem dados da jogada
 *
 * 	RETORNO:
 * 		FEN * fen - ponteiro para estrutura da notação FEN atualizada
 */
FEN* updateFEN (FEN* fen, OBJETO *** const table, PLAY play)
{
	// ************************************************* atualizar a configuração do tabuleiro no fen
	char *update = (char*)malloc(sizeof(char));
	*update = '\0';
	int i, j;
	int spaces;
	int size;
	for(i = 0; i < TABLE_ROWS; i++)
	{
		spaces = 0;
		for(j = 0; j < TABLE_COLS; j++)
		{
			//contagem de espaços vazios
			if(table[i][j] == NULL)
				spaces ++;
			//acréscimo de espaços e de peças na notação
			else
			{
				size = strlen(update);
				//se houve espaços entre peças na horizontal acrescental o salto na notação
				if(spaces)
				{
					update = (char*)realloc(update, sizeof(char)*(++size + 1));
					update[size - 1] = '0' + spaces;
					update[size] = '\0';
				}
				//acréscimo das peças na notação
				update = (char*)realloc(update, sizeof(char)*(++size + 1));
				update[size - 1] = getType(table[i][j]);
				update[size] = '\0';
				spaces = 0;
			}
		}
		if(spaces)
		{
			size = strlen(update);
			update = (char*)realloc(update, sizeof(char)*(++size + 1));
			update[size - 1] = '0' + spaces;
			update[size] = '\0';
		}
		//adicionar uma barra para cada linha do tabuleiro
		size = strlen(update);
		update = (char*)realloc(update, sizeof(char)*(++size + 1));
		update[size - 1] = '/';
		update[size] = '\0';
	}

	//eliminar a ultima '/' da string
	size = strlen(update);
	update = (char*)realloc(update, sizeof(char)*(--size + 1));
	update[size] = '\0'; //lembrar que size foi descrecido e não acrescido, portanto é [size]

	int turn = (fen->turn == 'w') ? 1: 0;

	// ********************************* verificar condições de roque futuro
	char *newRock = (char*)malloc(sizeof(char));
	int sizeRock;
	strcpy(newRock, "");
	int adj = 7, count = 0;
	char *paux = fen->rock;

	//analisar roque BRANCO
	if(paux[count] == 'K')
	{
		if(getType(table[TABLE_ROWS - 1][TABLE_COLS - 1]) == 'R' &&
				table[TABLE_ROWS - 1][4] != NULL && getType(table[TABLE_ROWS - 1][4]) == 'K')
		{
			sizeRock = strlen(newRock);
			newRock = (char*)realloc(newRock, sizeof(char)*(++sizeRock + 1));
			newRock[sizeRock - 1] = 'K';
			newRock[sizeRock] = '\0';
		}
		count++;
	}
	if(paux[count] == 'Q')
	{
		if(getType(table[TABLE_ROWS - 1][0]) == 'R' &&
				table[TABLE_ROWS - 1][4] != NULL && getType(table[TABLE_ROWS - 1][4]) == 'K')
		{
			sizeRock = strlen(newRock);
			newRock = (char*)realloc(newRock, sizeof(char)*(++sizeRock + 1));
			newRock[sizeRock - 1] = 'Q';
			newRock[sizeRock] = '\0';
		}
		count++;
	}

	//analisar roque PRETO
	if(paux[count] == 'k')
	{
		if(getType(table[0][TABLE_COLS - 1]) == 'r' &&
				table[0][4] != NULL && getType(table[0][4]) == 'k')
		{
			sizeRock = strlen(newRock);
			newRock = (char*)realloc(newRock, sizeof(char)*(++sizeRock + 1));
			newRock[sizeRock - 1] = 'k';
			newRock[sizeRock] = '\0';
		}
		count++;
	}
	if(paux[count] == 'q')
	{
		if(getType(table[0][0]) == 'r' &&
				table[0][4] != NULL && getType(table[0][4]) == 'k')
		{
			sizeRock = strlen(newRock);
			newRock = (char*)realloc(newRock, sizeof(char)*(++sizeRock + 1));
			newRock[sizeRock - 1] = 'q';
			newRock[sizeRock] = '\0';
		}
		count++;
	}

	//atualizar as disponibilidades de rock
	if((sizeRock = strlen(newRock)))
		strcpy(fen->rock, newRock);
	else
		strcpy(fen->rock , "-");
	free(newRock);

	// ********************************* verificar descartar formação de en passant anterior e verificar a atual
	strcpy(fen->pass, "-");
	//se o turno é do White então os peões estão na linha 5 da matriz, senão, na linha 2 da matriz.
	if(getType(play.obj) == 'p' - turn * 32 && (7 - getObjectRow(play.obj)) == (3 + turn * 1))
	{
//printf("updateFEN_en passant: fromRow (%d) - (7 - Row[%d])\n", play.fromRow, getObjectRow(play.obj));
		//função update será chamada antes de atualizar a coordenada da struct da peça
		int diff = abs(play.fromRow - (7 - getObjectRow(play.obj)));

		//se houve salto de 2 casas do peão
		if(diff == 2)
		{
			char newPass[3];
			newPass[0] = 'a' + getObjectColumn(play.obj);
			newPass[1] = '1' + 2 + !turn * 3;
			newPass[2] = '\0';
			strcpy(fen->pass, newPass);
		}
	}

	// ********************************** verificar o acréscimo do meio turno
		//se movimento foi feito de peão ou foi feito uma captura
	if(getType(play.obj) != 'p' - turn * 32 && getObjectTurn(play.obj) != fen->fullTurn && play.promotion == '-')
		fen->halfTurn ++;
	else
		fen->halfTurn = 0;

	// ********************************** verificar o acréscimo do turno completo
		//se a jogada foi feita pela lado Preto
	if(!turn)
		fen->fullTurn++;

	//guardar a nova notação do estado atual do tabuleiro
	strcpy(fen->pieces, update);

	return fen;
}

//função irá imprimir a notação FEN
void printFEN (FEN *fen)
{
	fprintf(stdout, "%s ", fen->pieces);
	fprintf(stdout, "%c ", fen->turn);
	fprintf(stdout, "%s ", fen->rock);
	fprintf(stdout, "%s ", fen->pass);
	fprintf(stdout, "%d ", fen->halfTurn);
	fprintf(stdout, "%d\n", fen->fullTurn);
}

// ******************************************************** Trabalho 08

int *getPlayTo (char* play)
{
	int *to;
	if(play != NULL)
	{
		to = (int*)malloc(sizeof(int)*2);
		int adj = (play[0] == 'p' || play[0] == 'P') ? 1 : 0;
		if(!(adj && play[strlen(play) - 1] >= 'B' && play[strlen(play) - 1] <= 'R'))
			adj = 0;

		to[1] = play[strlen(play) - 2 - adj] - 'a';
		to[0] = '8' - play[strlen(play) - 1 - adj];
	}
	return to;
}
