/*=============================================================================
*
*    oO  Snake code  Oo
*
==============================================================================
*
* File : main.c
* Date : 20 Décembre 2019
* Author : Tran Alexia
*
==============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "snakeAPI.h"
#include "ret_type.h"
#include "clientAPI.h"
#include "fonctionTran.h"


int main(int argc,char* argv[]){

	char* serverName="li2051-215.members.linode.com";
  //char* PlayerName="PetitSerpent(TRAN)";
  char* gameType="SUPER_PLAYER difficulty=2 timeout=100 seed=109";
	char gameName[100];
	int port= strtol(argv[2],NULL,10);

	int  sizeX, sizeY, nbWalls, start;
	int* walls;
	t_return_code cond; /*My and my oponent's condition*/
	t_move my_move, op_move, last_move; /*my move and my oponent's move*/

	int turn_counter=-1;/*on le nombre de tour commence à 0 mais je mets -1 ici car sinon y a un décalage de 1 je sais pas pourquoi*/
	int snake_length=1; /*les longueurs de serpent commence à 1*/

	int etat1=0; /*etat de montée 0 ou de descente 1*/
	int etat2=1; /*etat droite 0 ou gauche 1*/

	t_position mySnake[200], opSnake[200]; /*Les positions de mon snake et de celui
	de l'adversaire, je considère la case 0 comme la tête des serpents*/

/*connection au serveur*/
	connectToServer(argv[1], port, "PetitSerpent");
	while (1){
		waitForSnakeGame(argv[3], gameName, &sizeX, &sizeY,&nbWalls);
		printf("%d\n", sizeX);
		walls=(int*) malloc(4*nbWalls*sizeof (int));
		t_case arene[sizeX][sizeY];

	/*initialisation des bordures de l'arene*/
		initArene(sizeX, sizeY, arene);

	/*récupère aussi les coordonnées des murs*/
		start=getSnakeArena(walls);
		if(start==0){ /*je commence*/
			mySnake[0].x=2;
			mySnake[0].y=sizeY/2;
			opSnake[0].x=sizeX-3;
			opSnake[0].y=sizeY/2;
			arene[2][sizeY/2].snake=1;
			arene[sizeX-3][sizeY/2].snake=1;
		}
		else{ /*mon adversaire commence*/
			mySnake[0].x=sizeX-3;
			mySnake[0].y=sizeY/2;
			opSnake[0].x=2;
			opSnake[0].y=sizeY/2;
			arene[2][sizeY/2].snake=1;
			arene[sizeX-3][sizeY/2].snake=1;
		}
	 /*passage à 1 des directions interdites dans chaque position*/
		insertWalls(sizeX, sizeY, arene, walls, nbWalls);

		cond=NORMAL_MOVE;

		while(cond==NORMAL_MOVE){
			if(start==0){ /*mon tour*/
				turn_counter++;
				printf("%d\n", turn_counter );
				if(turn_counter%10==0){
					snake_length++;
					printf("%d\n", snake_length);
				}
				if (cond == NORMAL_MOVE){
					printArena();
				}
				else{
					printf("Mother Effing LOOSER\n");
				}


				//my_move=moveMySnake();
				//my_move=moveToMake(mySnake, sizeX, sizeY, arene, turn_counter);
	/*détermination des état*/
				/*je suis en etat de montée et que je ne suis pas tout en haut*/
				if((etat1==0)&&(mySnake[0].y!=0)){
					etat1=0; /*je reste en etat de montée*/
				}
				/*si je suis en montée mais que j'ai atteint en haut*/
				else if((etat1==0)&&(mySnake[0].y==0)){
					etat1=1; /*je passe à l'état de descente*/
				}
				/*si je suis en etat de descente et que je ne suis pas tout en bas*/
				else if((etat1==1)&&(mySnake[0].y!=sizeY-1)){
					etat1=1; /*je reste en etat de descente*/
				}
				else if((etat1==1)&&(mySnake[0].y==sizeY-1)){
					etat1=0; /*je passe à l'état de montée*/
				}

				/*si j'ai une priorité à droite et que j'ai atteint un coin à droite*/
				if((etat2==0)&&(mySnake[0].x==sizeX-1)){
					etat2=1; /*priorité à droite*/
				}
				else if((etat2==1)&&(mySnake[0].x==0)){ /*priorité à gauche et atteint la bordure gauche*/
					etat2=0;
				}

				my_move=moveToMake(mySnake, opSnake, sizeX, sizeY, arene, etat1, etat2, last_move);
				last_move=my_move;
				cond=sendMove(my_move);
				SnakeMov(mySnake, my_move, snake_length, sizeX, sizeY, arene);
				//printf("%d\n", cond);
				printf("etat1 %d etat2 %d\n", etat1, etat2);
				//printf("compteur %d bool snake %d\n", arene[sizeX-3][0].compteur, arene[sizeX-3][0].snake);
				printf("snake x %d y %d\n", mySnake[0].x, mySnake[0].y);
				//printf("last_move %d my move %d\n", last_move, my_move);
				//printf("compteur %d\n", arene[5][18].compteur);

			}
			else{ /*tour de l'adversaire*/
				cond=getMove(&op_move);
				SnakeMov(opSnake, op_move, snake_length, sizeX, sizeY, arene);
				if (cond == NORMAL_MOVE){
					 printArena();
				}
				else{
					printf("YOU'RE THE WINNER\n");
				}
			}

			start=!start;
		}
	}
/*se déconnecter du serveur*/
	closeConnection();
	free(walls);
	return 0;
}
