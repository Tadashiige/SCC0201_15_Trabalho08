/**
 * Saulo Tadashi Iguei NºUsp 7573548
 *
 * DATA entrega limite: 08/12/15
 *
 * SCC0201_01 - ICC2 _ Prof. Moacir
 *
 */

/*
 * Biblioteca para integração de definições e tipagens
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "integration.h"
#include "hash.h"

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
//printf("ID required: id(%c) = %d\n", type, id);
	return id;
}

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

int getRoqueCod(char* roque)
{
	int sum = 0;
	int i;
	for(i = 0; i < strlen(roque); i++)
		sum += getRockID(roque[i]);

	return sum;
}

int getPassantCod(char* passant)
{
	int sum = 0;
	sum = passant[0] - 'a';
	sum *= 7;
	sum = passant[1] - '1';

	return sum;
}

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
//printf("house: %d\n", house);
				house *= getID(getType(table[x][y]));
				cod = cod*29 + house;
//printf("cod[%d][%d] = %llu\n", x+1, y+1, cod);
			}
		cod += 67 * (67 * (67 * getRoqueCod(fen->rock) + (fen->turn == 'w')?2:1) + getPassantCod (fen->pass));
	}
//printf(" >>>>>>>>>> cod final: %llu\n", cod);
	return cod;
}

int insertHash (LIST **hash, unsigned long long int cod)
{
	int num = 0;

	int pos = (cod % HASH_MAX);
//printf("inserir na posição: %d\n", pos);
	num = addHash(hash[pos], cod);

	return num;
}
