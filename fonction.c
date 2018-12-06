
/*fichier fonctions*/

//fonction d'affichage du tableu, affiche toutes les pieces presente sur le tableau, est affiché à nouveau à chaque tours après l'effacement du tableau precédent pour le dynamisme


void affichage(char plateau[5][5][2])
{
	int x, y;
	char lettre = 'A';
	system("clear"); // vide la mémoire d'avant et c'est "clear" sous linus et cls sous windows

  //affichage du plateau
	printf("\n    1    2    3    4    5\n"); //abcsises du plateau
  //structure du plateau
	for(y = 0; y <= 4; y++)
	{
		printf("  --------------------------\n%c ", lettre++); //cadre +  repère en ordonnées
	//affichage des cases du plateau
		for(x = 0; x <= 4; x++)
		{
			printf("| ");
	//cas de la case vide
			if(plateau[y][x][0] == 0) //dans la 3eme dimention de notre tableau se trouve: le type de piece, l'oriantation
			{
				//teste pour l'affichage des +
				if((x == 0) || (x == 4) || (y == 0) || (y == 4))//condition pour etre sur un bors du plateau
				{
					printf("+  ");
				}
				else
				{
					printf("   ");
				}
			}
	//cas où il y aurrai une piece sur la case
			//teste pour affichage des pions
			if(plateau[y][x][0] == 'M') 
			{
				printf("M  ");
			}
			if(plateau[y][x][0] == 'R' || plateau[y][x][0] == 'E') 
			{
			printf("%c", plateau[y][x][0]);
        		printf("%c ", plateau[y][x][1]);	// [1] pour orientation des pièces
      }
		}
		//fermeture de la case
		printf("|\n");
	}
	printf("  --------------------------\n");//derniere ligne non comprit dans la boucle
}

//fonction d'entrée de choix de coups, appelle affichage, deplace, place, rotation, sauvegarde, saisie
short int entre(char plateau [5][5][2], short int bascule,char link[256], short int* pionsE, short int* pionsR, short int *bascsave)
{ 
	char choix;
	short int coup = 0, **pion, victoire = 0;
	
	do
	{
		affichage(plateau); 	//affichage a chaque tour
	//utilisation de la bascule pour determiner à quel des joueurs c'est le tour
		if(bascule)	//bascule = 1 -> rhino
		{
			printf("\n\nc'est aux RHINO de jouer");
			pion = &pionsR;
		}
			
		else if(!bascule)		//bascule = 0 -> elephan
		{
			printf("\n\nc'est aux ELEPH de jouer"); // on saute de deux lignes pour la visibilité des différentes instructions
			pion = &pionsE;
		}
		//affichage des pions en reserve
		printf("\nil vous reste %d pions en dehors\n", **pion);
		//proposition de coups en fonction de la reserve de pions
		if(**pion != 5)
			printf("\nrotation(r)\ndéplacer(d)"); // une rotation compte pour un tour et correspond au changement d'orientation
		if(**pion != 0)
	        	printf("\nposer un pion(p)\n");
    		//proposition de la sauvegarde, disponible à tout moment
        	if(*bascsave == 0)
			printf("\nparametriser la sauvegarde(s)\n\n");
		if(*bascsave == 1)
			printf("\n		sauvegarder (s)\n\n");
        
		
		printf(": ");	
    		saisie(plateau, bascule, link, &choix); //choix de mvt à blinder
    		
		
		// appel des fonction selon choix, blindé	
		if(choix == 'r' && !((*pionsR == 5 && bascule == 1) || (*pionsE == 5 && bascule == 0)))		//si coup == 0 -> erreur donc reprend du début	
      			coup = rotation(plateau, bascule, link);
	
		if(choix == 'd' && !((*pionsR == 5 && bascule == 1) || (*pionsE == 5 && bascule == 0)))
			coup = deplace(plateau, bascule, link,&pionsR, &pionsE);

		if(choix == 'p' && ((bascule == 1 && *pionsR > 0)||(bascule == 0 && *pionsE > 0)))
			coup = place(plateau, bascule, link, &pion, &pionsR, &pionsE);
        
    		
		//differentes saves
		if(choix == 's' && *bascsave == 1)
    		{      
        		sauvegarde(plateau, bascule, link);
       			printf("\npartie sauvegardée sur : %s", link);
    		}
    		else if(choix == 's' && *bascsave == 0)
    		{
      			para_save(plateau, bascule, link);
      			(*bascsave) = 1;
    		}
        
	}while(!coup);

	return victoire;
}

//fonction de déplacement, coups possible si reserve du joueur<5
short int deplace(char plateau[5][5][2], short int bascule, char link[256], short int **pionR, short int **pionE)
{
	short int casx, casy, casxx, casyy;
	char orientation;

	//selection de la case où se trouve le pion	printf("piece à déplacer :\n");
	entrecase(plateau, bascule, link, &casx, &casy);	//test de la validité du coups
	if(((bascule == 1) && (plateau[casy][casx][0] != 'R')) || ((bascule == 0) && (plateau[casy][casx][0] != 'E'))) //blindage de la case
	{
		printf("case n'est pas de votre équipe (entré)");
		getchar();
		return 0;
	}
	
//selection de la case où l'on veut deplacer le pion	printf("lieu de déplacement :\n");
	entrecase(plateau, bascule, link, &casxx, &casyy);
	//test de la validité du coup
  //cas non valide
	if(!((((casxx-casx == 1)||(casx-casxx == 1)) && (casyy-casy == 0)) || (((casyy-casy == 1)||(casy-casyy == 1)) && (casxx-casx == 0)))) 	
	{
		printf("\ncase non valide(entré)");
		getchar();
		return 0;
	}
  //cas valide
	if(plateau[casyy][casxx][0] != 0)
    	{
        	if(casxx-casx == 1)
			orientation = 'd';
		if(casx-casxx == 1)
			orientation = 'g';
		if(casyy-casy == 1) 
			orientation = 'b';
		if(casy-casyy == 1)
			orientation = 'h';
		printf("--orient : %c", orientation);
    //cas ou cases de deplacement non vide
   	 //test de resistance
   		 //cas deplacement possible
		if((calculateur(plateau, orientation, casxx, casyy) < 10)&&(orientation == plateau[casy][casy][1]))
		{
			deplaceur(plateau, orientation, casx, casy, bascule,  &pionR, &pionE);
			return 1;//bien passé
		}
			//cas deplacement impossible
		printf("\nresisstance trop forte");
		getchar();
		return 0;
    }
  //mise a zero
    else
    {
        plateau[casyy][casxx][0] = plateau[casy][casx][0];
        plateau[casy][casx][0] = 0;
        plateau[casyy][casxx][1] = plateau[casy][casx][1];
        plateau[casy][casx][1] = 0;
    }
}

//fonction de placement des pieces, apelle enrtecase, orien_rotation, angle, calculateur
short int place(char plateau[5][5][2], short int bascule, char link[256], short int ***pion, short int **pionR, short int **pionE)
{
	short int casx, casy;
	char orientation, rot;

	entrecase(plateau, bascule, link, &casx, &casy);
	//test si case valide
	if(casx == 4 || casx == 0 || casy == 4 || casy == 0) 
	{
		printf("--case valide");
  //cas case vide
		if(plateau[casy][casx][0] == 0) 
		{
			printf("--cas case vide");
			if(bascule == 0)
				plateau[casy][casx][0] = 'E';
			if(bascule == 1)
				plateau[casy][casx][0] = 'R';

			orien_rotation(plateau, bascule, link, casx, casy);
			(***pion)--;	//on retire 1 pion du deck
	
			return 1;	//on return 1 car bien passé
		}
	//cas case non vide
		else if(plateau[casy][casx][0] != 0) 
        	{
			printf("--cas cas non vide");
    //cas angle
                	if((casx == 0 || casx == 4) && (casy == 0 || casy == 4))
               		{                       
                        angle(plateau, bascule, link, casx, casy, pionR, pionE);
                        	(***pion)--;
                       		return 1;//bien passé
               		 }
        		
			//cas ou c'est sur les lignes et colones		
			if(casx == 0)
      				orientation = 'd';
			if(casx == 4)
				orientation = 'g';
			if(casy == 0)
				orientation = 'b';
			if(casy == 4)
				orientation = 'h';
			printf("\norientation du mvt : %c", orientation);
			
      //choix de l'orientaion de la piece
      //boucle dont seul un choix  'valable' permet la sortie
			while (rot != 'd' && rot != 'g' && rot!= 'h' && rot!= 'b')
		        {
                		printf("\nquelle orientation voulez vous lui donner ?\nbas(b)\nhaut(h)\ngauche(g)\ndroite(d)\n:");
				saisie(plateau, bascule, link, &rot); //saisie de l'orientation
       	 		}
			
	//cas d'une case non vide
      //test des resistance
			if ((calculateur(plateau, orientation, casx, casy) < 10)&&(rot == orientation))
        //cas valide        	
			{
				printf("res > 10");
      		//déplacement de la piece
		    		deplaceur(plateau, orientation,casx, casy, bascule, &pionR, &pionE);
              			if(bascule)
			                plateau[casy][casx][0] = 'R';
        			else if(!bascule)
               				 plateau[casy][casx][0] = 'E';
    				plateau[casy][casx][1] = rot; 

				(***pion)--; 

				return 1;//bien passé
               		}
    		//cas no valide
                   	printf("\nentrée non valide");
          		getchar();

              		return 0;
		}
	
	}
	//cas case non valide 
	//test de validité, doit etre sur un bord
	else if(!(casx == 4 || casx == 0 || casy == 4 || casy == 0))	
	{
		printf("\ncase non valide"); 
		getchar();	
		
		return 0;
	}
}

// fonction de l'angle dans la poussé, appelle calculateur
short int angle(char plateau [5][5][2], short int bascule, char link [256], short int casx, short int casy, short int **pionR, short int **pionE)
{
	char orientation;
	short int res; 
  do{
    
		printf("dans quel sens voulez vous pousser ?");
		if(casx == 0)
			printf("\nvers la droite (d)");
		if(casx == 4)
			printf("\nvers la gauche (g)");
		if(casy == 0)
			printf("\nvers le bas (b)\n:");
		if(casy == 4)
			printf("\nvers le haut (h)\n:");
  	saisie(plateau, bascule, link, &orientation);
	//test validité
	}while(orientation != 'd' && orientation != 'g' && orientation != 'b' && orientation != 'h');
  //cas coup possible
	if(calculateur(plateau, orientation, casx, casy) < 10)
	{
		printf("coup joué !");
    deplaceur(plateau, orientation, casx, casy, bascule, &pionR, &pionE);           
		return 1;
	}
  //cas coups non valide
	printf("resisstance trop forte");
	getchar();
	return 0;
}

//voire si on peut simplifier
//fonction de calcule des resistances
short int calculateur(char plateau[5][5][2], char orientation, short int casx, short int casy)
{
  //* par 10 pour plus de lisbilité toute la res
	short int n, res = 0, sortie = 1;	
    	printf("calculateur");
  //calcule selon l'orientation de la piece qui pousse
	if(orientation == 'd')	
  	//ballayage de la ligne à droite
	       for(n = casx; n <= 4 && sortie; n++)
	       {	
	       	       if(plateau[casy][n][1] == 'g')
			      			 res += 10;
       					if(plateau[casy][n][1] == 'd')
									res -= 10;
								if(plateau[casy][n][0] == 0)
								{
									sortie = 0;
									break;
								}
								if(plateau[casy][n][0] == 'M')
									res += 9;
					}

	if(orientation == 'g')	
    //ballayage de la ligne à gauche
		for(n = casx; n >= 0 && sortie; n--)
		{
			if(plateau[casy][n][1] == 'd')
				res += 10;
			if(plateau[casy][n][1] == 'g')
				res -= 10;
			if(plateau[casy][n][0] == 0)
			{
				sortie = 0;
				break;
			}
			if(plateau[casy][n][0] == 'M')
				res += 9;
		}
	if(orientation == 'h')
    //ballayage de la colone au dessus
		for(n = casy; n >= 0 && sortie; n--)
		{
			if(plateau[n][casx][1] == 'b')
				res+=10;
			if(plateau[n][casx][1] == 'h')
				res-=10;
			if(plateau[n][casx][0] == 0)
			{
				sortie = 0;
				break;
			}
			if(plateau[n][casx][0] == 'M')
				res += 9;
		}
	if(orientation == 'b')
    //ballayage de la colone au dessous
		for(n = casy; n <= 4 && sortie; n++)
		{
			if(plateau[n][casx][1] == 'h')
				res+=10;
			if(plateau[n][casx][1] == 'b')
				res-=10;
			if(plateau[n][casx][0] == 0)
			{
				sortie = 0;
				break;
			}
			if(plateau[n][casx][0] == 'M')
				res +=9;
		}

	return res;//rend la resistance à l'appellant
}

//fonction de déplacement des cases en travaux

void deplaceur(char plateau[5][5][2], char orientation, short int casx, short int casy, short int bascule, short int ***pionR, short int ***pionE)
{
  printf("--deplaceur");
	short int n, sortie = 1;
	char cache[2], cache2[2];
	cache[0] = plateau[casy][casx][0];	//cache contient la valeur de la case précedente
    	cache[1] = plateau[casy][casx][1];	//cache2 contient la valeur de la case actuelle en transformation!

	plateau[casy][casx][0] = 0;
	plateau[casy][casx][1] = 0;

	switch(orientation)
	{
		case 'd' :
    		
		for(n = casx + 1; n <= 4 && sortie; n++) 
	       	{	 //cas ou case non vide      
           		if(plateau[casy][n][0] != 0) 
		       	{
				if((plateau[casy][n][0] == 'E') && (n == 4))
				{
					(**pionE)++;
					plateau[casy][n][0] = cache[0];
                                        plateau[casy][n][1] = cache[1];
                                        return;
				}
				if((plateau[casy][n][0] == 'R') && (n == 4))
				{
					(**pionR)++;
					plateau[casy][n][0] = cache[0];
                                        plateau[casy][n][1] = cache[1];
                                        return;
				}
				if((plateau[casy][n][0] == 'M') && (n == 4))
					for(n = casx - 1; n >= 0 && sortie; n--)
					
						if(plateau[casy][n][0] == 'M' || plateau[casy][n][0] == 'E')
						victoire(plateau[casy][n][0]);
						
				
        	    	 	cache2[0] = plateau[casy][n][0];
               			cache2[1] = plateau[casy][n][1];
		       		plateau[casy][n][0] = cache[0];
             			plateau[casy][n][1] = cache[1];
		       	}
	//cas case vide
       			if(plateau[casy][n][0] == 0)	
			{
			 	plateau[casy][n][0] = cache[0];
				plateau[casy][n][1] = cache[1];
				sortie = 0;
				break;
			}
			
        		 printf("d");
       //changement de cache
			 cache[0] = cache2[0];
		   	 cache[1] = cache2[1];
		}
		 break;
		
  	
  
	case 'g' :	
    		for(n = casx - 1; n >= 0 && sortie; n--)
	       	{	
		      //cas ou case non vide
           		if(plateau[casy][n][0] != 0) 
	      		{
				if((plateau[casy][n][0] == 'E') &&  (n == 0))
                                {
                                        (**pionE)++;
					plateau[casy][n][0] = cache[0];
                                        plateau[casy][n][1] = cache[1];
                                        return;
                                }
                                if((plateau[casy][n][0] == 'R') && (n == 0))
                                {
                                        (**pionR)++;
					plateau[casy][n][0] = cache[0];
					plateau[casy][n][1] = cache[1];
					return;
                                }
                                if((plateau[casy][n][0] == 'M') && (n == 0))
                                        for(n = casx + 1; n <= 4 && sortie; n++)	
						if(plateau[casy][n][0] == 'M' || plateau[casy][n][0] == 'E')
                                                victoire(plateau[casy][n][0]);
                                        
                                
				
				

             			cache2[0] = plateau[casy][n][0];
                		cache2[1] = plateau[casy][n][1];
                		plateau[casy][n][0] = cache[0];
             			plateau[casy][n][1] = cache[1];
			}
      	//cas case vide
			if(plateau[casy][n][0] == 0)
			{
			 	plateau[casy][n][0] = cache[0];
				plateau[casy][n][1] = cache[1];
				sortie = 0;
				break;
			}
      //changement de cache
			cache[0] = cache2[0]; 
	      		cache[1] = cache2[1];
     		}
		break;

	case 'h':	
    		for(n = casy - 1; n >= 0 && sortie; n--)
		{	
				//cas case non vide
           		if(plateau[n][casx][0] != 0) 
		       	{
				if((plateau[n][casx][0] == 'E') &&  (n == 0))
                                {
                                        (**pionE)++;
                                        plateau[n][casx][0] = cache[0];
                                        plateau[n][casx][1] = cache[1];
                                        return;
                                }
                                if((plateau[n][casx][0] == 'R') && (n == 0))
                                {
                                        (**pionR)++;
                                        plateau[n][casx][0] = cache[0];
                                        plateau[n][casx][1] = cache[1];
                                        return;
                                }
                                if((plateau[n][casx][0] == 'M') && (n == 0))
                                        for(n = casy + 1; n <= 4 && sortie; n++)
						if(plateau[n][casx][0] == 'M' || plateau[n][casx][0] == 'E')
							victoire(plateau[n][casx][0]);

             			cache2[0] = plateau[n][casx][0];
              			cache2[1] = plateau[n][casx][1];
		       		plateau[n][casx][0] = cache[0];
             			plateau[n][casx][1] = cache[1];
		       	}
      //cas case vide
		 	if(plateau[n][casx][0] == 0)	
			{
				plateau[n][casx][0] = cache[0];
				plateau[n][casx][1] = cache[1];
				sortie = 0;
				break;
			}
  //changement de cache     
			cache[0] = cache2[0]; 
	      		cache[1] = cache2[1];
     		}
		break;
  
	case 'b' :
        	
        	for(n = casy + 1; n <= 4 && sortie; n++)
 		{		//cas ou case non vide
        	    	if(plateau[n][casx][0] != 0) 
        	    	{
				if((plateau[n][casx][0] == 'E') &&  (n == 4))
                                {
                                        (**pionE)++;
                                        plateau[n][casx][0] = cache[0];
                                        plateau[n][casx][1] = cache[1];
                                        return;
                                }
                                if((plateau[n][casx][0] == 'R') && (n == 4))
                                {
                                        (**pionR)++;
                                        plateau[n][casx][0] = cache[0];
                                        plateau[n][casx][1] = cache[1];
                                        return;
                                }
                                if((plateau[n][casx][0] == 'M') && (n = 4))
                                        for(n = casy - 1; n >= 0 && sortie; n--)
						if(plateau[n][casx][0] == 'M' || plateau[n][casx][0] == 'E')
							victoire(plateau[n][casx][0]);

        	     		cache2[0] = plateau[n][casx][0];
        	        	cache2[1] = plateau[n][casx][1];
        	        	plateau[n][casx][0] = cache[0];
        	     		plateau[n][casx][1] = cache[1];
        	    	}
        //cas case vide	    
            		if(plateau[n][casx][2] == 0)	
                {
            		    	plateau[n][casx][0] = cache[0];
           	 	    	plateau[n][casx][1] = cache[1];
           		     	sortie = 0;
               	 		break;
           		}
  //changement de cache
        		cache[0] = cache2[0]; 
      			cache[1] = cache2[1];
     		}
		break;
	}
	return;
   	
}

