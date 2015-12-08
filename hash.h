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

#ifndef __HASH_H_
#define __HASH_H_

#include "integration.h"

unsigned long long int genCode (OBJETO *** const table, FEN *fen);
int insertHash (LIST** hash, unsigned long long int cod);

#endif
