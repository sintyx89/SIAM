void client(char plateau[5][5][2])
{
	short int victoire = 0, bascule, pionR, pionE, save;
	char link[256] = {"SIAM.save"};	


	//créer le socket, AF_INET = TCP->adresse sur 4 octets, SOCK_STREAM = TCPsocket(type de socket)/par flot de donnée
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);	
	char *ip;


	//créer la structure server_adress
	struct sockaddr_in server_adresse;
	//donne le port
	server_adresse.sin_port = htons(9002);//htons convertit un entiers en adresse
	//quels type d'adresse
	server_adresse.sin_family = AF_INET;

	//donner l'ip à laquelle on se coo
	printf("\nIP : ");
	scanf("%c", ip);

	inet_aton(ip, &server_adresse.sin_addr.s_addr);//inet convertit un char en ip
	
	
	//conection
	int bi_statut = bind(server_socket, (struct sockaddr *) &server_adresse, sizeof(server_adresse));
	if(bi_statut == SO_ERROR)
	{
		printf("erreur de connection");
		quitter(plateau, bascule, link);
	}
	int statut = connect(server_socket, &server_adresse.sin_addr.s_addr, sizeof(server_adresse)); 
	if(statut == SO_ERROR)
	{
		printf("erreur de connection");
		quitter(plateau, bascule, link);
	}

	//partie en elle même
	while(!victoire)
	{
		//recoit le plateau
		recv(server_socket, &plateau, sizeof(plateau[5][5][2]), 0);
		recv(server_socket, &pionE, sizeof(pionE), 0);
		recv(server_socket, &pionR, sizeof(pionR), 0);
	
		//entrées
		entre(plateau, bascule, link, &pionE, &pionR, &save);
		if(!bascule)
		{
			bascule = 1;
		}
		else if(bascule)
		{
			bascule = 0;
		}

		//envoie tous
		send(server_socket, &plateau, sizeof(plateau[5][5][2]), 0);
		send(server_socket, &pionE, sizeof(pionE), 0);
		send(server_socket, &pionR, sizeof(pionR), 0);
	}
}


void server(char plateau[5][5][2])
{
	//créer le socket, SOCK_STREAM = TCPsocket(type de socket)
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	short int ip[8];

	//créer la structure server_adress
	struct sockaddr_in server_adresse;
	//donne le port
	server_adresse.sin_port = htons(9002);//htons convertit un entiers en adresse
	//quels type d'adresse
	server_adresse.sin_family = AF_INET;
	//quelle adresse
	server_adresse.sin_addr.s_addr = INADDR_ANY;//n'importe laquelle du server
	
	//lier les 2
	bind(server_socket, (struct socketaddr *) &server_adresse, sizeof(server_adresse));
	//attente de connection
	listen(server_socket, 5);
	
	//accpete la connection
	int client_socket;
	client_socket = accept(server_socket, NULL, NULL);
	

	//le jeu en lui même
	short int victoire = 0, bascule = 1, pionR, pionE, save = 0;
	char link[256] = {"SIAM.save"};

	entre(plateau, bascule, link, &pionE, &pionR, &save);
	send(client_socket, &plateau, sizeof(plateau[5][5][2]), 0);
	send(client_socket, &pionE, sizeof(pionE), 0);
	send(client_socket, &pionR, sizeof(pionR), 0);


	while(!victoire)
	{
		//recoit le plateau
		recv(client_socket, &plateau, sizeof(plateau[5][5][2]), 0);
		recv(client_socket, &pionE, sizeof(pionE), 0);
		recv(client_socket, &pionR, sizeof(pionR), 0);
	
		//entrées
		entre(plateau, bascule, link, &pionE, &pionR, &save);
		if(!bascule)
		{
			bascule = 1;
		}
		else if (bascule)
		{
			bascule = 0;
		}
		//envoie les données
		send(client_socket, &plateau, sizeof(plateau[5][5][2]), 0);
		send(client_socket, &pionE, sizeof(pionE), 0);
		send(client_socket, &pionR, sizeof(pionR), 0);
	}


}
