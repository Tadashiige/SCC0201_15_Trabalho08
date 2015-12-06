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
 * Biblioteca para gerenciamento de objetos do jogo
 */

#ifndef __OBJETO_H_
#define __OBJETO_H_

#include <stdlib.h>
#include <stdio.h>
#include "integration.h"
#include "FEN.h"

//função irá criar e alocar o objeto de peças do xadrez
OBJETO* createObject (char type, char *position, funcPtr mov);

//função irá apagar e liberar o objeto de peças do xadrez
void deleteObject (OBJETO **obj);

//função irá mudar o tipo da peça junto com a forma de movimentação
void changeType (OBJETO *obj, char type, funcPtr mov);

//função irá mudar a posição da peça
void changePosition (OBJETO *obj, char *position);

//função irá marcar a peça como capturada
void captured (OBJETO *obj);

//funções de gets e sets de atributos
char getType (OBJETO *obj);
int getValue (OBJETO *obj);
char* getPosition (OBJETO *obj);
funcPtr getFunctionMov(OBJETO *obj);
int getActive (OBJETO *obj);
int getObjectTurn (OBJETO *obj);
void setObjectTurn (OBJETO *obj, int fullTurn);

char** getList (OBJETO *obj);
void setList (OBJETO *obj, char** list);
int getNList (OBJETO *obj);
void setNList (OBJETO *obj, int size);

//funções de print sobre objetos e suas coleções
void printObject (OBJETO *const obj);
void printCollectionObject (OBJETO **const collectionObj, int size);

//função buscar pelo rei do turno
OBJETO *getKingTable (OBJETO *** const table, int turn);

//função elimina toda lista da peça para recepção de uma nova
char** clearList (OBJETO *obj);

#endif
