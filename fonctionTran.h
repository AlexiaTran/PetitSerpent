/*=============================================================================
*
*    oO  Header file of fonctions' code  Oo
*
==============================================================================
*
* File : fonctionTran.h
* Date : 20 Décembre 2019
* Author : Tran Alexia
*
==============================================================================
*/
#ifndef __FONCTIONTRAN_H__
#define __FONCTIONTRAN_H__

/*structures*/
typedef struct{
  int x;
  int y;
}t_position;

/*definition d'une structure pour chaque case du tableau représentant l'arène*/
typedef struct{
  int n;
  int e;
  int s;
  int w;
  int compteur; /*compte le nombre de direction interdite, =3 si c'est un cul de sac*/
  int snake; /*1 si un serpent est présent, 0 sinon le booléen du snake*/
}t_case;


/*prototypes*/
t_move moveMySnake();
void initArene(int sizeX, int sizeY, t_case arene[sizeX][sizeY]);
void insertWalls(int sizeX, int sizeY,t_case arene[sizeX][sizeY], int* walls,int nbWalls);
void SnakeMov(t_position snake[200], t_move move, int length, int sizeX, int sizeY, t_case arene[sizeX][sizeY]);
t_move moveToMake(t_position snake[200], t_position op_snake[200], int sizeX, int sizeY, t_case arena[sizeX][sizeY], int etat1, int etat2, t_move last_move);


#endif
