		/*fichier prototypes*/


void affichage(char plateau[5][5][2]);

short int entre(char plateau [5][5][2], short int bascule, char link[100], short int* pionsE, short int* pionsR, short int *bascsave);

short int deplace(char plateau[5][5][2], short int bascule, char link[100], short int **pionR, short int **pionE);

short int place(char plateau[5][5][2], short int bascule, char link[100], short int ***pion, short int **pionR, short int **pionE);

short int angle(char plateau [5][5][2], short int bascule, char link[100], short int casx, short int casy, short int **pionR, short int **pionE);

short int calculateur(char plateau[5][5][2], char orientation, short int casx, short int casy);

void deplaceur(char plateau[5][5][2], char orientation, short int casx, short int casy, short int bascule, short int ***pionR, short int ***pionE);

	//fichier prototypes petit fonction

void victoire(char basculeprime);

void orien_rotation(char plateau[5][5][2], short int bascule, char link[100], short int casx, short int casy);

void entrecase(char plateau[5][5][2], short int bascule, char link[100], short int *casx, short int *casy);

short int rotation(char plateau [5][5][2], short int bascule, char link[100]);

void regles();

void chargement(char plateau[5][5][2], short int *bascule, short int *pionsR, short int *pionsE, char link[100]);

void para_save(char plateau[5][5][2], short int bascule, char link[100]);

void sauvegarde(char plateau[5][5][2], short int bascule, char link[100]);

//void Color(int couleurDuTexte,int couleurDeFond);

void saisie(char plateau[5][5][2], short int bascule, char link[100],char *ptchoix);

void minmaj(char *cara);

void quitter(char plateau[5][5][2], short int bascule, char link[100]);



