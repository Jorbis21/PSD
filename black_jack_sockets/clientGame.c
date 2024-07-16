#include "clientGame.h"

void showCode (unsigned int code){

	tString string;

		if (DEBUG_CLIENT){
			// Reset
			memset (string, 0, STRING_LENGTH);
			switch(code){
				case TURN_BET:
					strcpy (string, "TURN_BET");
					break;

				case TURN_BET_OK:
					strcpy (string, "TURN_BET_OK");
					break;

				case TURN_PLAY:
					strcpy (string, "TURN_PLAY");
					break;

				case TURN_PLAY_HIT:
					strcpy (string, "TURN_PLAY_HIT");
					break;

				case TURN_PLAY_STAND:
					strcpy (string, "TURN_PLAY_STAND");
					break;

				case TURN_PLAY_OUT:
					strcpy (string, "TURN_PLAY_OUT");
					break;

				case TURN_PLAY_WAIT:
					strcpy (string, "TURN_PLAY_WAIT");
					break;

				case TURN_PLAY_RIVAL_DONE:
					strcpy (string, "TURN_PLAY_RIVAL_DONE");
					break;

				case TURN_GAME_WIN:
					strcpy (string, "TURN_GAME_WIN");
					break;

				case TURN_GAME_LOSE:
					strcpy (string, "TURN_GAME_LOSE");
					break;

				default:
					strcpy (string, "UNKNOWN CODE");
					break;
			}

			printf ("Received:%s\n", string);
		}
}

unsigned int readBet (){

	int isValid, bet=0;
	tString enteredMove;

		// While player does not enter a correct bet...
		do{

			// Init...
			bzero (enteredMove, STRING_LENGTH);
			isValid = TRUE;

			printf ("Enter a bet:");
			fgets(enteredMove, STRING_LENGTH-1, stdin);
			enteredMove[strlen(enteredMove)-1] = 0;

			// Check if each character is a digit
			for (int i=0; i<strlen(enteredMove) && isValid; i++)
				if (!isdigit(enteredMove[i]))
					isValid = FALSE;

			// Entered move is not a number
			if (!isValid)
				printf ("Entered bet is not correct. It must be a number greater than 0\n");
			else
				bet = atoi (enteredMove);

		}while (!isValid);

		printf ("\n");

	return ((unsigned int) bet);
}

unsigned int readOption (){

	int isValid, option = 0;
	tString enteredMove;

		// While player does not enter a correct bet...
		do{
			// Init...
			bzero (enteredMove, STRING_LENGTH);
			isValid = TRUE;

			printf ("\nPress %d to hit a card and %d to stand:", PLAYER_HIT_CARD, PLAYER_STAND);
			fgets(enteredMove, STRING_LENGTH-1, stdin);
			enteredMove[strlen(enteredMove)-1] = 0;

			// Check if each character is a digit
			for (int i=0; i<strlen(enteredMove) && isValid; i++)
				if (!isdigit(enteredMove[i]))
					isValid = FALSE;

			if (!isValid)
				printf ("Wrong option!\n");
			else{

				// Conver entered text to an int
				option = atoi (enteredMove);

				if ((option != PLAYER_HIT_CARD) && (option != PLAYER_STAND)){
					printf ("Wrong option!\n");
					isValid = FALSE;
				}
			}
		}while (!isValid);

		printf ("\n");

	return ((unsigned int) option);
}
unsigned int recepcionUint(int socketfd){
	unsigned int tmp;
	int msgLength;
	msgLength = recv(socketfd, &tmp, sizeof(tmp), 0);
	if(msgLength < 0)
		showError("ERROR while reading from socket");
	return tmp;
}
void envioUint(unsigned int x, int socketfd){
	unsigned int tmp = x;
	int msgLength;
	msgLength = send(socketfd, &tmp, sizeof(tmp), 0);
	if(msgLength < 0)
		showError("ERROR while writing to socket");
}
tDeck recepcionDeck(int socketfd){
	int msgLength;
	tDeck d;
	d.numCards = recepcionUint(socketfd);
	msgLength = recv(socketfd, &d.cards, sizeof(unsigned int) * d.numCards, 0);
	if(msgLength < 0)
		showError("ERROR while reading from socket");
	return d;
}

void doCode(int code, int socketfd){
	//funcion que realiza la acción correspondiente al code enviado por el server
	switch(code){
		case TURN_BET:
			TURN_BET_ACTION(socketfd);
			break;
		case TURN_BET_OK:
			printf("Apuesta válida\n");
			break;
		case TURN_PLAY:
			TURN_PLAY_ACTION(socketfd);
			break;
		case TURN_PLAY_OUT:
			TURN_PLAY_OUT_ACTION(socketfd);
			break;
		case TURN_PLAY_WAIT:
			TURN_PLAY_WAIT_ACTION(socketfd);
			break;
		case TURN_PLAY_RIVAL_DONE:
			TURN_PLAY_RIVAL_DONE_ACTION(socketfd);
			break;
		case TURN_GAME_WIN:
			printf("Enhorabuena, ganaste la partida\n");
			endOfGame = TRUE;
			break;
		case TURN_GAME_LOSE:
			printf("Ouch, perdiste, otra vez será socio\n");
			endOfGame = TRUE;
			break;
		default: break;
	}
}

void TURN_BET_ACTION(int socketfd){
	unsigned int stack = recepcionUint(socketfd);
	printf("Tienes %d fichas.\n", stack);
	envioUint(readBet(), socketfd);
}
void TURN_PLAY_OUT_ACTION(int socketfd){
	unsigned int points;
	tDeck d;
	points = recepcionUint(socketfd);//aquí es el punto donde puede recibir TURN_PLAY_WAIT
	//pondria aquí a comprobarlo con un if pero TURN_PLAY_WAIT = 6 y eso puede ser un valor de puntos
	d = recepcionDeck(socketfd);
	printf("Te pasaste de la puntación máxima\n");
	printf("Tienes %d puntos.\n", points);
	printFancyDeck(&d);
}
void TURN_PLAY_RIVAL_DONE_ACTION(int socketfd){
	unsigned int points;
	tDeck d;
	points = recepcionUint(socketfd);
	d = recepcionDeck(socketfd);
	printf("El rival ha terminado con %d puntos y las siguientes cartas\n", points);
	printFancyDeck(&d);
}
void TURN_PLAY_ACTION(int socketfd){
	unsigned int points;
	tDeck d;
	points = recepcionUint(socketfd);//aquí es el punto donde puede recibir TURN_PLAY_WAIT
	//pondria aquí a comprobarlo con un if pero TURN_PLAY_WAIT = 6 y eso puede ser un valor de puntos
	d = recepcionDeck(socketfd);
	printf("-----JUEGA-----\n");
	printf("Tienes %d puntos.\n", points);
	printFancyDeck(&d);
	if(readOption())
		envioUint(TURN_PLAY_HIT, socketfd);
	else
		envioUint(TURN_PLAY_STAND, socketfd);
}
void TURN_PLAY_WAIT_ACTION(int socketfd){
	unsigned int points;
	tDeck d;
	points = recepcionUint(socketfd);
	d = recepcionDeck(socketfd);
	printf("-----ESPERA-----\n");
	printf("El rival tiene %d puntos.\n", points);
	printFancyDeck(&d);
}

int main(int argc, char *argv[]){

	int socketfd;						/** Socket descriptor */
	unsigned int port;					/** Port number (server) */
	struct sockaddr_in server_address;	/** Server address structure */
	char* serverIP;						/** Server IP */
	tString playerName;					/** Name of the player */
	unsigned int code;					/** Code */
	int msgLength;						/** Tamaño del mensaje */
	unsigned int stack;					/** Numero de fichas disponibles */

		// Check arguments!
		if (argc != 3){
			fprintf(stderr,"ERROR wrong number of arguments\n");
			fprintf(stderr,"Usage:\n$>%s serverIP port\n", argv[0]);
			exit(0);
		}

		// Get the server address
		serverIP = argv[1];

		// Get the port
		port = atoi(argv[2]);

		//Creamos socket cliente
		socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		//Comprobamos que el socket se haya creado bien
		if(socketfd < 0)
			showError("ERROR while creating the socket");
		
		//Rellenamos la estructura del socket
		memset(&server_address, 0, sizeof(server_address));
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = inet_addr(serverIP);
		server_address.sin_port = htons(port);

		//Conectamos con el server
		if(connect(socketfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
			showError("ERROR while establishing connection");
		
		//Incializcion del nombre
		printf("Inserte su nombre: ");
		memset(playerName,0,sizeof(playerName));
		fgets(playerName, sizeof(playerName) - 1, stdin);

		//Enviamos el mensaje
		msgLength = send(socketfd, playerName, strlen(playerName) - 1, 0);

		//Comprobamos los bytes enviados
		if(msgLength < 0)
			showError("ERROR while writing from the socket");

		endOfGame = FALSE;
		//Aqui esperamos a la recepcion del estado de la partida
		while (!endOfGame){
			code = recepcionUint(socketfd);
			doCode(code, socketfd);
		}
	
	close(socketfd);
	return 0;		

}
