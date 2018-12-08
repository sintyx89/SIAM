	/*fichier main*/

//biblio classique
#include <stdio.h>
#include <stdlib.h>

//biblio chaine
#include <string.h>

//couleurs windows
//#include <window.h>

//fichiers
#include <sys/types.h>
#include <dirent.h>

//réseaux
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



#include "headers.h" //implémentation du fichier de prototypes
#include "fonction.c" 
#include "petitfonction.c"
#include "reseaux.c" 

//#define BLANC = '\033[0;31m'
//#define 


void main()
{
	char plateau[5][5][2] = {0}, sauv, linksave[256] = {"SIAM.save"}, reseaux;/*tableau avec toutes les positions des pions
				      j'ai choisie les position sous forme matriciel
				      donc plateau[y][x] le [2] est pour contenir 
				      le type et l'orientation*/
	
	short int pionsE = 5, pionsR = 5, bascule = 0, victoire = 0, bascsave = 0; /* la bascule est pour savoir
		 c'est le tour de qui
	 les pions sont pour savoir 
	 combien de pions on peut jouer*/ 
	
	
	//on place les montagnes aux positions initiales
	plateau[1][2][0] = 'M';
	plateau[2][2][0] = 'M';
	plateau[3][2][0] = 'M';

	regles();

    
	do{
		printf("Voulez-vous jouer avec une autre machine sur le reseau local ? (O/N) :");
		saisie(plateau, bascule, linksave, &reseaux);
		minmaj(&reseaux);

	}while(reseaux == 'O' && reseaux == 'N');
	
	//si on joue sur 2 machines
	if(reseaux == 'O')
	{
		do{
			printf("Voulez vous etres le serveur(s) ou le client(c) : ");
			saisie(plateau, bascule, linksave, &reseaux);
		}while(reseaux == 's' && reseaux == 'c');

		if(reseaux == 's')
		{
			server(plateau);
		}
		else if (reseaux == 'c')
		{
			client(plateau);
		}
	}
	
	//si on joue à 2 sur une seule machine
	if(reseaux == 'N')
	{
  		do{
        		printf("Voulez-vous charger une partie précédente ? (O/N) : ");
        		saisie(plateau, bascule, linksave, &sauv);
      			minmaj(&sauv);
    		}while(sauv != 'O' && sauv != 'N');

    		if(sauv == 'O')
    		{
		    	para_chargement(plateau, bascule, linksave);
			chargement(plateau, &bascule, &pionsE, &pionsR, linksave);
    		}

		while(!victoire)
		{

			entre(plateau, bascule, linksave, &pionsE, &pionsR, &bascsave);

			if(!bascule)
			{
				bascule = 1;
				continue;
			}
			else if(bascule)
			{
				bascule = 0;
				continue;
			}
		}
	}
}
