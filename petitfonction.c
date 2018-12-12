
//fonction de victoire, depend du joueur dont c'est le tour au moment de la victoire
void victoire(char basculeprime)
{	//affichage du message de victoire
  	//partie modulable
	switch(basculeprime)
	{
		case 'R':
			printf("\nles Rhinos ");
			break;
		case 'E':
			printf("\nles Eleph ");
			break;
		default:
			return;
	}
  	//partie comune
	printf("vous avez gagné\n");
	getchar();
  //sortie du programme comme fin de la partie
	perror("fin du programme");
	exit(0);
}

//fonction qui modifie l'orientation d'une piece
void orien_rotation(char plateau[5][5][2],short int bascule, char link[256], short int casx, short int casy)
{
	char rot;
	//boucle dont la sortie demande la saisie d'une orientation 'valable'
	while (rot != 'd' && rot != 'g' && rot!= 'h' && rot!= 'b')
	{
		printf("\nquelle orientation voulez vous lui donner ?\nbas(b)\nhaut(h)\ngauche(g)\ndroite(d)\n:");
        	saisie(plateau, bascule, link, &rot);              //demande l'orientation à blinder
	}
	//afectation de la nouvelle orientation
        plateau[casy][casx][1] = rot;   //changement de rotation du pion
}

//fonction de choix de case
void entrecase(char plateau[5][5][2], short int bascule, char link[256], short int *casx, short int *casy)
{
	char casyprime;
	short int casxprime;

  	do{
        	printf("quel est l'abssices de votre case ? :");
        	scanf("%d", &casxprime);
		getchar();
          	if(casxprime == 27)
          	{
            		quitter(plateau, bascule, link);
          	}
        }while(!(casxprime < 5 && casxprime > 0));
		(*casx) = --casxprime;

  	do{
        	printf("quel est l'ordonné de votre case ? :");
        	saisie(plateau, bascule, link, &casyprime);
  	}while(!(casyprime <! 'A' || casyprime >! 'E'));

	*casy = casyprime - 65;
}

//fonction de rotation, appelle entrecase, orien_rotation
short int rotation(char plateau [5][5][2], short int bascule, char link[256])
{
	short int casx, casy;
	//choix de la case
	entrecase(plateau, bascule, link, &casx, &casy);

	if(((plateau[casy][casx][0] != 'R') && (bascule == 1)) || ((plateau[casy][casx][0] != 'E') && (bascule == 0)))
	{
		printf("\nentrée non valide");
		return 0;
	}

	orien_rotation(plateau, bascule, link, casx, casy);

	return 1;
}

//fonction d'affichage des regels en debut de partie
void regles()
{

    	system("clear");
    	printf("\n\n\n                  		VOUS ETES SUR SIAM  \n\n\n\n         LE JEU TELLEMENT BIEN QUE MEME MACRON A LE TEMPS D'Y JOUER");
    	getchar();
	system("clear");
	printf("Au début de la partie, chaque joueur choisit son équipe :\n -les éléphants\n -les rhinocéros.\nLe jeu commence avec les trois montagnes placées au centre du plateau (3B ;3C ;3D).\nLes pions de chacun des joueurs en dehors du plateau. \n\nDans le siam, les éléphants blancs sont les premiers à jouer. \n\nÀ chaque tour le joueur ne peut effectuer qu’une seule et unique action.\n\nLes diffèrent coup ou action possibles : \n- Poser un pion sur le plateau. \n- Déplacer un pion sur le plateau. \n- Effectuer une rotation d’un pion sur le plateau. \n\nLes conditions pour poser un pion:\n- Le joueur doit avoir au moins un pion à l’extérieur du plateau.\n- Le pion ne peut être poser que sur les cases extérieures du plateau (ligne A ou E, ou colonne 1 ou 5).\n- La case doit être vide ou répondre aux exigences de la poussé afin de déplacer le pion qui l’occupe pour pouvoir y poser un pion.\n\nLes conditions de déplacement:\n- Les pions peuvent se déplacer sur les 4 cases qui leurs sont adjacentes.\n- Si la case est libre le déplacement est possible.\n- Si la case est occupée, le pion déplacer vas tenter une poussée.\n\nLa rotation permet de modifier l’orientation du pion, 4 orientations sont possibles : haut, bas, droite, gauche.\n");
  	printf("\n\n----Quitter à tout moment grace à ESC+ENTER-----\n");
  	getchar();
}
void para_chargement(char plateau[5][5][2], short int bascule, char link[256])
{
 	char choix;
  	short int erreur, n = 0;
  	struct dirent *lecture;
  	DIR* dossier = NULL;
  	dossier = opendir(".");

  	system("clear");
  	printf("	----PARAMETRE DE CHARGEMENT----\n\n\n");

  	if(dossier != NULL)
  	{
    		do
    		{
			n = 0;
			printf("\n\n");
    			while((lecture = readdir(dossier)))
    			{
    	    			if(strstr(lecture->d_name, ".save"))
				{
					printf("-------------------------------------------------\n");
					printf(" %d -- %s\n", n + 1, lecture->d_name);
					n++;
				}

    			}
			printf("-------------------------------------------------");
    			rewinddir(dossier);
    			printf("\nlequel ? : ");
    			saisie(plateau, bascule,link, &choix);
    		}while(!(choix <= 100 && choix > 0));

	//passe devant tous les autres fichier
		for(n = 0;(lecture = readdir(dossier)) && n < 100;)
    		{
    	    		if(strstr(lecture->d_name, ".save"))
			{
				if( n == choix-1)
				{
					link = lecture->d_name;
					break;
				}
				n++;
			}
    		}

    		printf("Vous chargerez le fichier : %s\n", link);
		getchar();
    		closedir(dossier);
  	}
}





//fonction d'ouverture d'un jeux sauvegardé
void chargement(char plateau[5][5][2], short int *bascule, short int *pionsE, short int *pionsR, char link[256])
{
    	char cache;
    	short int code, m, n, t;
	printf("%s", link);

 	FILE* save = NULL;
    	save = fopen(link, "r"); // ouverture du fichier en mode lecture
    	fseek(save, 0, SEEK_SET);
	printf("%s", link);

  //cas de sauvegarde lisible
    if(save != NULL)
    {	//balayage du plateau pour placement des information sauvegardé
        for(n = 0; n <= 4; n++)
            for(m = 0; m <= 4; m++)
            {
              for(t = 0; t <= 1; t++)
                {
                        fscanf(save, "%c", &cache);
                        if(cache == 'O')
                        	plateau[n][m][t] = 0;
                        else
                        	plateau[n][m][t] = cache;
                        	printf("--%d%c", plateau[n][m][t], cache);
                }
             	if(plateau[n][m][0] == 'R')
                  	(*pionsR)--;
             	if(plateau[n][m][0] == 'E')
                  	(*pionsE)--;
            }

      	fscanf(save, "%d", &cache);
      	if(cache == 'R')
        	(*bascule) = 0;
      	else if (cache == 'E')
        	(*bascule) = 1;
      	//fermeture de save
        fclose(save);
    }
  	//cas fichier non lisible ou non existant
    	else
    	{
        	printf("\nerreur d'ouverture de fichier");
        	getchar();
        	return;
    	}
}

//teste les anciennes saves et crée un nouveau fichier si besoin
void para_save(char plateau[5][5][2], short int bascule, char link[256])
{
  	char choix;
  	short int erreur, n = 0, m[100];
  	struct dirent *lecture;
  	DIR* dossier = NULL;
  	dossier = opendir(".");

  	system("clear");
  	printf("	----PARAMETRE DE SAUVEGARDE----\n\n\n");
  	do{
  		printf("ancienne partie 'ecraser' ou nouvelle ? (A/N) : ");
  		saisie(plateau, bascule, link, &choix);
  		minmaj(&choix);
  	}while(choix != 'A' && choix != 'N');

	//choix de save ancienne
  	if(choix == 'A' && dossier != NULL)
  	{
    		do
    		{
			n = 0;
			printf("\n\n");
    			while((lecture = readdir(dossier)))
    			{
    	    			if(strstr(lecture->d_name, ".save"))
				{
					printf("-------------------------------------------------\n");
					printf(" %d -- %s\n", n + 1, lecture->d_name);
					n++;
				}

    			}
			printf("-------------------------------------------------");
    			rewinddir(dossier);
    			printf("\nlequel ? : ");
    			scanf("%d", &choix);
    		}while(!(choix <= 100 && choix > 0));

	//passe devant tous les autres fichier
		for(n = 0;(lecture = readdir(dossier)) && n < 100;)
    		{
    	    		if(strstr(lecture->d_name, ".save"))
			{
				if( n == choix-1)
				{
					link = lecture->d_name;
					break;
				}
				n++;
			}
    		}

    		printf("Vous sauverez sur le fichier : %s\n", link);
		getchar();
    		closedir(dossier);
  	}

	//si nouveau lien
  	if(choix == 'N' && dossier != NULL)
  	{
    		do
    		{
      			erreur = 0;
     			rewinddir(dossier);

			//demande nom
      			printf("\nnom du nouveau fichier : ");
  			scanf("%s", link);


			//teste si il y a .save dedans, sinon on rajoute
			if(!strstr(link, ".save"))
        			strcat(link, ".save");

			//analyse si il existe deja
			while(lecture = readdir(dossier))
				if(lecture->d_name == link)
				{
					do{
						printf("\nle fichier existe déjà\nVoulez vous l'écraser ?(O/N) :");
						saisie(plateau, bascule, link, &choix);
						minmaj(&choix);
					}while(choix != 'N' && choix != 'O');

					if(choix == 'N')
						erreur = 1;

				}

      			if(strchr(link, ' '))//test si espace sinon erreur
      			{
        			printf("FAUT PAS D'ESPACES!");
      				erreur = 1;
        			continue;
      			}
		}while(erreur == 1);
	}
	sauvegarde(plateau, bascule, link);
}
//fonction de sauvegarde de l'etat de la partie
void sauvegarde(char plateau[5][5][2], short int bascule, char link[256])
{
    short int code, m, n, t;
    FILE* save = NULL;

    save = fopen(link, "w");
    fseek(save, 0, SEEK_SET);
    printf("partie sauvegardée sur %s", link);

    if(save != NULL)
    {
        //écrire le plateau
        for(n = 0; n <= 4; n++)
            for(m = 0; m <= 4; m++)
              	for(t = 0; t<2; t++)
                {
                    if(plateau[n][m][t] == 0)
                        putc('O', save);
                    else
                        putc(plateau[n][m][t], save);
                }
      if(bascule == 1)
      	putc('R', save);
      else if (bascule == 0)
        putc('E', save);

      	//fermer le fichier
        fclose(save);
      return;
    }
  //cas erreur
    else
    {
        printf("erreur d'ouverture de fichier");
        getchar();
        return;
    }
}

/* WINDOWS
// fonction d'affichage de couleurs
void Color(int couleurDuTexte,int couleurDeFond)
{
        HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}
*/

//fonction de saisie des choix proposé
void saisie(char plateau[5][5][2], short int bascule, char link[256], char *ptchoix)
{
    char entre = 0;
    short int c;
    do
    {
        scanf("%c", &entre);
	getchar();

    	//saisie d'échap pour quitter
        if (entre == 27)
        {
         	quitter(plateau, bascule, link);
        }
    		//saisie d'un choix
        if(entre > 65 && entre < 90)
    		entre += 32;
    }
    while(entre==0);

    /*				//videur de buffer
	while (c != EOF && c != '\n')
		c = getchar();
*/
    //choix retourné à l'appellant
    *ptchoix = entre;
  	return;
}

//donne la version majuscule de la cara
void minmaj(char *cara)
{
  if(*cara >= 97 && *cara <= 122)
    (*cara) -= 32;
}


//quitte le programme en douceur
void quitter(char plateau[5][5][2], short int bascule, char link[256])
{
	char save;

	do{
  		printf("voulez-vous sauvegarder?   oui(1)   non(2) : ");
  		scanf("%c",&save);
		getchar();
	}while(save != '1' && save != '2');

 	if (save == '1')
  	{
     		sauvegarde(plateau, bascule, link);
  	}

  	printf("fermeture du programme, vous retrouverez ce que vous avez entrée\n\n");
	getchar();
	system("clear");
  	exit(0);
}
