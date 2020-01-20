/*=============================================================================
*
*    oO  Movements' code Oo
*
==============================================================================
*
* File : fonctionTran.c
* Date : 20 Décembre 2019
* Author : Tran Alexia
*
==============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include "snakeAPI.h"
#include "fonctionTran.h"

t_move moveMySnake(){
	int a;
	t_move m;
	printf("Direction: NORTH(0) EAST(1) SOUTH(2) WEST(3)\n");
	scanf("%d", &a);
/*user wants to go North*/
	if(a==0){
		m=NORTH;
	}
/*user wants to go East*/
	else if(a==1){
		m=EAST;
		//return NORMAL_MOVE;
	}
/*user wants to go South*/
	else if(a==2){
		m=SOUTH;
		//return NORMAL_MOVE;
	}
/*user wants to go West*/
	else if(a==3){
		m=WEST;
		//return NORMAL_MOVE;
	}
	else{
    	scanf("%d", &a);
  }
	return m;
}

/*initialisation des bordures des arenes en passant à 1 les directions interdites*/
void initArene(int sizeX, int sizeY, t_case arene[sizeX][sizeY]){
  int i,j;
/*initialisation des variables n s w e à 0 pour toutes les cases de l'arène*/
	for(i=0; i<sizeX; i++){
		for(j=0; j<sizeY; j++){
			arene[i][j].n=0;
			arene[i][j].s=0;
			arene[i][j].w=0;
			arene[i][j].e=0;
			arene[i][j].compteur=0;
      arene[i][j].snake=0;
		}
	}

/*régle pour les cases en bordure de l'arène*/
/*pour les cases de la colonne 0, passage de la variable w à 1 pour interdir
	le mouvement vers l'ouest*/
	for(i=0; i<sizeY; i++){
		arene[0][i].w=1;
		arene[0][i].compteur++;
	}
/*pour les cases de la ligne 0, passage de la variable n à 1 pour interdir
	le mouvement vers le nord*/
	for(i=0; i<sizeX; i++){
		arene[i][0].n=1;
		arene[i][0].compteur++;
	}
	/*pour les cases de la dernière colonne, passage de la variable e à 1 pour interdir
	le mouvement vers l'est*/
	for(i=0; i<sizeY; i++){
		arene[sizeX-1][i].e=1;
		arene[sizeX-1][i].compteur++;
	}
/*pour les cases de la dernière ligne, passage de la variable s à 1 pour interdir
	le mouvement vers le sud*/
	for(i=0; i<sizeX; i++){
		arene[i][sizeY-1].s=1;
		arene[i][sizeY-1].compteur++;
	}
}

/*passage à 1 des directions interdites à cause des murs*/
void insertWalls(int sizeX, int sizeY,t_case arene[sizeX][sizeY], int* walls,int nbWalls){
	int x1;
	int y1;
	int x2;
	int y2;
	for (int i=0;i<nbWalls;i++){
		x1=walls[4*i];
		y1=walls[4*i+1];
		x2=walls[4*i+2];
		y2=walls[4*i+3];
		if (x1==x2){
			if (y1<y2){
				arene[x1][y1].s=1;
				arene[x2][y2].n=1;
				arene[x1][y1].compteur++;
				arene[x2][y2].compteur++;
			}
			else{
				arene[x1][y1].n=1;
				arene[x2][y2].s=1;
				arene[x1][y1].compteur++;
				arene[x2][y2].compteur++;
			}
		}
		else if(y1==y2){
			if (x1<x2){
				arene[x1][y1].e=1;
				arene[x2][y2].w=1;
				arene[x1][y1].compteur++;
				arene[x2][y2].compteur++;
			}
			else{
				arene[x1][y1].w=1;
				arene[x2][y2].e=1;
				arene[x1][y1].compteur++;
				arene[x2][y2].compteur++;
			}
		}

	}
}

/* fonction qui fait grandir les serpents et affecte à la valeur 1 le booleen du snake dans les cases*/
void SnakeMov(t_position snake[200], t_move move, int length, int sizeX, int sizeY, t_case arene[sizeX][sizeY]){
/*décalage de toutes les positions sauf la tete*/
for(int i=0; i<=length-2; i++){
	/*le dernier element du serpent prend la position de l'element précedent*/
	snake[length-1-i].x=snake[length-2-i].x;
	snake[length-1-i].y=snake[length-2-i].y;
	arene[snake[length-2-i].x][snake[length-2-i].y].snake=1; /*presence du snake*/
}
arene[snake[length-1].x][snake[length-1].y].snake=0; /*passage à 0 du booleen du snake de la derniere case car le snake ni est plus*/

/*nouvelle position de la tete*/
	switch (move){
		case NORTH: /*coordonnées x reste la meme, on retire 1 a y*/
			snake[0].x=snake[0].x;
			snake[0].y=snake[0].y-1;
			arene[snake[0].x][snake[0].y].snake=1;
		break;
		case SOUTH:
			snake[0].x=snake[0].x;
			snake[0].y=snake[0].y+1;
			arene[snake[0].x][snake[0].y].snake=1;
		break;
		case EAST: /*deplacement horizontal donc y reste le meme*/
			snake[0].x=snake[0].x+1;
			snake[0].y=snake[0].y;
			arene[snake[0].x][snake[0].y].snake=1;
		break;
		case WEST:
			snake[0].x=snake[0].x-1;
			snake[0].y=snake[0].y;
			arene[snake[0].x][snake[0].y].snake=1;
		break;

	}
}


t_move moveToMake(t_position snake[200], t_position op_snake[200], int sizeX, int sizeY, t_case arena[sizeX][sizeY], int etat1, int etat2, t_move last_move){
	t_move m;
	/*si je suis en état de montée*/
	if(etat1==0){
		/*si je suis tout en bas*/
		if(snake[0].y==sizeY-1){
			if(last_move!=SOUTH){ /*je viens pas du nord*/
				/*Si je peux aller nord*/
				if((arena[snake[0].x][snake[0].y].n!=1)&&(arena[snake[0].x][snake[0].y-1].compteur<3)){
					m=NORTH;
			  }
				else{ /*si je paux pas aller nord*/
					/*priorité à gauche*/
					if(etat2==1){
						if((last_move!=EAST)&&(arena[snake[0].x-1][snake[0].y].snake!=1)&&(arena[op_snake[0].x-1][op_snake[0].y].snake!=1)){
							m=WEST;
						}
						else{
							m=EAST;
						}
					}
					else{/*priorité à droite*/
						if((last_move!=WEST)){
							m=EAST;
						}
						else{
							m=WEST;
						}
					}

				}
			}
			/*if je viens du nord et si j'ai un priorité à droite, que je peux aller à droite et qu'il n'y a pas de cul de sac*/
			else if((etat2==0)&&(arena[snake[0].x][snake[0].y].e!=1)&&(arena[snake[0].x+1][snake[0].y].compteur<3)){
				m=EAST;
			}
			else{
				if((arena[snake[0].x][snake[0].y].w!=1)&&(arena[snake[0].x-1][snake[0].y].compteur<3)){
					m=WEST;
				}
				else{
					m=SOUTH;
				}
			}
		}
		/*si je ne suis pas tout en bas et si je peux aller au nord et qu'il ni a pas de cul de sac au nord*/
		else if((arena[snake[0].x][snake[0].y].n!=1)&&(arena[snake[0].x][snake[0].y-1].compteur<3)){
			if(last_move!=SOUTH){ /*je viens pas du nord*/
				if((arena[snake[0].x][snake[0].y].n!=1)&&(arena[snake[0].x][snake[0].y-1].compteur<3)){
					m=NORTH;
			  }
				else{ /*peut pas aller au nord*/
					/*priorité à gauche*/
					if(etat2==1){
						if((last_move!=EAST)){
							m=WEST;
						}
						else{
							if(arena[snake[0].x][snake[0].y].e!=1){
								m=EAST;
							}
							else{
								m=SOUTH;
							}
						}
					}
					else{/*priorité à droite*/
						if((last_move!=WEST)){
							m=EAST;
						}
						else{
							if(arena[snake[0].x][snake[0].y].w!=1){
								m=WEST;
							}
							else{
								m=SOUTH;
							}

						}
					}

				}
			}
			else{ /* si je viens du nord*/
				if(etat2==1){ /*priorité à gauche*/
					if(last_move!=EAST){ /*si je viens pas de la gauche*/
						if((arena[snake[0].x][snake[0].y].w!=1)&&(arena[snake[0].x-1][snake[0].y].compteur<3)){
							m=WEST;
						}
						else{
							m=SOUTH;
						}
					}
					else{ /*viens de la gauche*/
						if((arena[snake[0].x][snake[0].y].e!=1)&&(arena[snake[0].x+1][snake[0].y].compteur<3)){
							m=EAST;
						}
						else{
							m=SOUTH;
						}
					}
				}
				else{/*priorité à droite*/
					if(last_move!=WEST){
						m=EAST;
					}
					else{
						if(arena[snake[0].x][snake[0].y].s!=1){
							m=SOUTH;
						}
						else{
							m=WEST;
						}
					}
				}

			}
		}
		else{ /*je peux pas aller au nord ou il y a un cul de sac*/
			if(etat2==0){/*si j'ai un priorité à droite*/
			/*si je peux aller à droite et qu'il n'y a pas de cul de sac à droite*/
				if((arena[snake[0].x][snake[0].y].e!=1)&&(arena[snake[0].x+1][snake[0].y].compteur<3)){
					if(last_move!=WEST){/*si je viens pas de la droite*/
						m=EAST;
					}
					else{/*si je viens de la droite*/
						if((arena[snake[0].x][snake[0].y].w!=1)&&(arena[snake[0].x-1][snake[0].y].compteur<3)){
							m=WEST;
						}
						else{
							m=SOUTH;
						}
					}

				}
				else{
					if((arena[snake[0].x][snake[0].y].w!=1)&&(arena[snake[0].x-1][snake[0].y].compteur<3)){
						m=WEST;
					}
					else{
						m=SOUTH;
					}

				}
			}
			else{ /*priorité à gauche*/
			/*si je peux aller à gauche et qu'il n'y a pas de cul de sac à gauche*/
				if((arena[snake[0].x][snake[0].y].w!=1)&&(arena[snake[0].x-1][snake[0].y].compteur<3)){
					m=WEST;
				}
				else{/*si je peux pas aller à gauche*/
					if(last_move!=WEST){
						m=EAST;
					}
					else{
						if(last_move!=NORTH){
							m=SOUTH;
						}
						else{
							m=NORTH;
						}
					}
				}
			}
		}
	}
	/*si je suis en etat de descente*/
	else{
		/*si je suis tout en haut*/
		if(snake[0].y==0){
			if(last_move!=NORTH){/*si je ne viens pas du sud*/
				/*si je peux aller au sud et y a pas de cul de sac*/
				if((arena[snake[0].x][snake[0].y].s!=1)&&(arena[snake[0].x][snake[0].y+1].compteur<3)){
					m=SOUTH;
				}
				else{/*si je ne peux pas aller au sud*/
					/*priorité à gauche*/
					if(etat2==1){
						if(last_move!=EAST){
							m=WEST;
						}
						else{ /*si je viens de gauche*/
							if(last_move!=WEST){
								m=EAST;
							}
							else{
								m=NORTH;
							}
						}
					}
					else{/*priorité à droite*/
						if(last_move!=WEST){
							m=EAST;
						}
						else{
							if(last_move!=EAST){
									m=WEST;
							}
							else{
								m=NORTH;
							}
						}
					}
				}

			}
			/*si je viens du sud; j'ai un priorité à droite, que je peux aller à droite et qu'il n'y a pas de cul de sac*/
			else if((etat2==0)&&(arena[snake[0].x][snake[0].y].e!=1)&&(arena[snake[0].x+1][snake[0].y].compteur<3)){
				m=EAST;
			}
			else{
				m=WEST;
			}
		}
		/*si je ne suis pas tout en haut et que je suis tout en bas*/
		else if(snake[0].y==sizeY-1){
			if(last_move!=SOUTH){/*si je viens pas d'en haut*/
			/*et que je peux aller en haut*/
				if((arena[snake[0].x][snake[0].y].n!=1)&&(arena[snake[0].x][snake[0].y-1].compteur<3)){
					m=NORTH;
				}
				else{/*peut pas aller en haut*/
					if(last_move!=WEST){/*si je viens pas de la droite*/
						m=EAST;
					}
					else{
						m=WEST;
					}
				}

			}
			/*si je viens d'en haut, et que j'ai un priorité à droite, que je peux aller à droite et qu'il n'y a pas de cul de sac*/
			else if((etat2==0)&&(arena[snake[0].x][snake[0].y].e!=1)&&(arena[snake[0].x+1][snake[0].y].compteur<3)){
				m=EAST;
			}
			else{
				m=WEST;
			}

		}
		/*si au milieu je peux aller au sud et qu'il ni a pas de cul de sac au sud*/
		else if((arena[snake[0].x][snake[0].y].s!=1)&&(arena[snake[0].x][snake[0].y+1].compteur<3)){
			if(last_move!=NORTH){ /*si je ne viens pas du sud*/
				m=SOUTH;
			}
			else{ /*si je viens du sud*/
				if(etat2==0){/*si j'ai un priorité à droite*/
				/*si je peux aller à droite et qu'il n'y a pas de cul de sac à droite*/
					if((arena[snake[0].x][snake[0].y].e!=1)&&(arena[snake[0].x+1][snake[0].y].compteur<3)){
						m=EAST;
					}
					else{
						if(last_move!=EAST){
							m=WEST;
						}
						else{
							m=NORTH;
						}
					}
				}
				else{ /*priorité à gauche*/
				/*si je peux aller à gauche et qu'il n'y a pas de cul de sac à gauche*/
					if((arena[snake[0].x][snake[0].y].w!=1)&&(arena[snake[0].x-1][snake[0].y].compteur<3)){
						m=WEST;
					}
					else{
						if(last_move!=WEST){
							m=EAST;
						}
						else{
							m=NORTH;
						}
					}
				}

			}
		}
		else{ /*je peux pas aller au sud ou il y a un cul de sac*/
			if(etat2==0){/*si j'ai un priorité à droite*/
			/*si je peux aller à droite et qu'il n'y a pas de cul de sac à droite*/
				if((arena[snake[0].x][snake[0].y].e!=1)&&(arena[snake[0].x+1][snake[0].y].compteur<3)){
					if(last_move!=WEST){/*vient pas de la droite*/
						m=EAST;
					}
					else{/*vient de la droite*/
						if(arena[snake[0].x][snake[0].y].w!=1){
							m=WEST;
						}
						else{
							m=NORTH;
						}

					}

				}
				else{
					if(last_move!=EAST){
						m=WEST;
					}
					else{
						m=NORTH;
					}

				}
			}
			else{ /*priorité à gauche*/
			/*si je peux aller à gauche et qu'il n'y a pas de cul de sac à gauche*/
				if((arena[snake[0].x][snake[0].y].w!=1)&&(arena[snake[0].x-1][snake[0].y].compteur<3)){
					if(last_move!=EAST){ /*si je ne viens pas de la gauche*/
						m=WEST;
					}
					else{
						if((arena[snake[0].x][snake[0].y].n!=1)&&(arena[snake[0].x][snake[0].y-1].compteur<3)){
							m=NORTH;
						}
						else{
							m=EAST;
						}

					}

				}
				else{ /*peut pas aller à gauche*/
					if(last_move!= WEST){
						m=EAST;
					}
					else{
						if((arena[snake[0].x][snake[0].y].n!=1)&&(arena[snake[0].x][snake[0].y-1].compteur<3)){
							m=NORTH;
						}
						else{
							m=WEST;
						}
					}
				}
			}
		}
	}
	return m;
}
