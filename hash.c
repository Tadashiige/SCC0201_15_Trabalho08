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
 * Biblioteca para funcionalidade de Hashing
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "integration.h"
#include "hash.h"





//Função de relações entre tipos de peças com o valor de ID
int getID (char type)
{
	int id = 0;

	switch(type)
	{
	case 'p':
		id = 1;
		break;
	case 'P':
		id = 2;
		break;
	case 'n':
		id = 3;
		break;
	case 'N':
		id = 4;
		break;
	case 'b':
		id = 5;
		break;
	case 'B':
		id = 6;
		break;
	case 'r':
		id = 7;
		break;
	case 'R':
		id = 8;
		break;
	case 'q':
		id = 9;
		break;
	case 'Q':
		id = 10;
		break;
	case 'k':
		id = 11;
		break;
	case 'K':
		id = 12;
		break;
	default:
		break;
	}
	return id;
}






//Função de relações entre lados de roques possíveis
int getRockID (char r)
{
	int id = 0;
	switch(r)
	{
	case 'K':
		id = 1;
		break;
	case 'k':
		id = 2;
		break;
	case 'Q':
		id = 3;
		break;
	case 'q':
		id = 4;
		break;
	default:
		break;
	}
	return id;
}







//Função geradora de código Hash para notação Fen para roques
	/*
	 * OBS.: ele será usado como parte integrande de um Hash geral, e será um dos termos do polimômio, portanto
	 * Sua soma não irá afetar o nível de colisão, pois sua soma será no máximo 10.
	 */
int getRoqueCod(char* roque)
{
	int sum = 0;
	int i;
	for(i = 0; i < strlen(roque); i++)
		sum += getRockID(roque[i]);

	return sum;
}







//Função geradora de código Hash para notação Fen para en passant
/*
 * OBS.:	O código gerado será um termo do polinômio da geração de Hashing geral, portanto
 * localmente se faz geração por polinômio com base em número primo mais próximo do valor máximo.
 * Assim elegeu-se o primo 7 das 8 possibilidade de cada coordenada de en passant (a_8 x 1_8).
 */
int getPassantCod(char* passant)
{
	int sum = 0;
	sum = passant[0] - 'a';
	sum *= 7;
	sum = passant[1] - '1';

	return sum;
}






/**
 * Função geradora de código Hashing geral da situação do tabuleiro
 *
 * DESCRIÇÃO:
 * 		O código hashing é gerado por meio de formação polinomial com
 * 		número primo 29 de fator de coeficiente. Analisa-se cada peça
 * 		do tabuleiro presente e não cada casa do tabuleiro. Para tanto,
 * 		os dados serão gerados por meio do produto entre a casa da peça
 * 		e o seu valor de identificação, formando um número identificador
 * 		com baixa colisão e com menos espaços para armazenamento do que
 * 		hashing de string. Elegeu-se o primo 29 como produtor de hashing
 * 		por ele se o mínimo hashing primo próximo a metade das casas
 * 		existentes (64/2 = 32 -> 29), tornando a colisão mais difícil.
 *
 * 		Após a análise do tabuleiro soma-se o formação polinomial de
 * 		hashing com os códigos gerados para roque, en passant e turno,
 * 		com número primo geradora 67, que é o primo mais próximo de 64 e
 * 		maior que ele, fazendo a soma com o código do tabuleiro não
 * 		sofrer colisão com facilidade.
 *
 * 	PARAMETROS:
 * 		OBJETO *** const table - tabuleiro de objetos
 * 		FEN *fen - ponteiro para estrutura de FEN
 *
 * 	RETORNO:
 * 		unsigned long long int cod - código hashing gerado
 */
unsigned long long int genCode (OBJETO *** const table, FEN *fen)
{
	unsigned long long int cod = 0;

	if(fen != NULL && table != NULL)
	{
		int house, i, x = 1, y = 1;
		for (x = 0; x < TABLE_ROWS; x++)
			for(y = 0; y < TABLE_COLS; y++)
			{
				if(table[x][y] == NULL)
					continue;

				house = (x)*TABLE_COLS + (y+1);

				house *= getID(getType(table[x][y]));
				cod = cod*29 + house;

			}
		cod += 67 * (67 * (67 * getRoqueCod(fen->rock) + (fen->turn == 'w')?2:1) + getPassantCod (fen->pass));
	}

	return cod;
}






/**
 * Função para inserção de hashing na tabela e conhecer o número de suas ocorrências.
 *
 * DESCRIÇÃO:
 * 		Adotou-se a tipagem unsigned long long int (8 BYTES), pois seu valor máximo é capaz de
 * 		comportar a formação polinomial hipotético crítico formado for
 *
 * 		f(x) = f(x-1)*29 + casa[max64] * ID_Peça[max12]
 *
 * 		Sendo 29 elegido por ser o menor primo mais próximo de metade de casas do tabuleiro (64).
 * 		Com isso ainda há a complementação dos códigos de roque, en passant e turno que possui
 * 		metodo próprio de hashing que é regido por:
 *
 * 		g(x) = 67 ( 67 ( 67 (cod_Roque[max10]) + cod_turn[max2] ) +  cod_enPassant[max8*7 + 8])
 *
 * 		que é gerado por número primo 67 maior que 64 e próximo dele. Os primos foram escolhidos
 * 		com base a 64, pois esse valor é único para peças, não havendo a mesma casa atribuida para
 * 		mais peças ao mesmo tempo.
 *
 *
 * 		A tabela hashing é do tipo encadeado para proporcionar flexibilidade no tamanho da tabela,
 * 		pois as combinações possíveis são muito extensas. Assim para evitar este consumo de memória
 * 		adotou-se o sistema de tabela com listas ligadas (circulares) com tabela com tamanho fixo
 * 		max1024 (base binária _ kilo).
 *
 * 		Tido o código gerado e decido a qual lista inserir chamar a função que irá inserir o código
 * 		caso não existe na tabela ou incrementa-se as ocorrencias do mesmo código na tabela.
 *
 *
 * 	PARAMETROS:
 * 		LIST **hash - vetor de listas
 * 		unsigned long long int cod - código hashing gerado da condição do tabuleiro
 *
 * 	RETORNO:
 * 		int num - ocorrencias do codigo dado na tabela.
 */
int insertHash (LIST **hash, unsigned long long int cod)
{
	int num = 0;

	int pos = (cod % HASH_MAX);
//printf("inserir na posição: %d\n", pos);
	num = addHash(hash[pos], cod);

	return num;
}
