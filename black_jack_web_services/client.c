#include "client.h"

unsigned int readBet (){

	int isValid, bet=0;
	xsd__string enteredMove;

		// While player does not enter a correct bet...
		do{

			// Init...
			enteredMove = (xsd__string) malloc (STRING_LENGTH);
			bzero (enteredMove, STRING_LENGTH);
			isValid = TRUE;

			printf ("Enter a value:");
			fgets(enteredMove, STRING_LENGTH-1, stdin);
			enteredMove[strlen(enteredMove)-1] = 0;

			// Check if each character is a digit
			for (int i=0; i<strlen(enteredMove) && isValid; i++)
				if (!isdigit(enteredMove[i]))
					isValid = FALSE;

			// Entered move is not a number
			if (!isValid)
				printf ("Entered value is not correct. It must be a number greater than 0\n");
			else
				bet = atoi (enteredMove);

		}while (!isValid);

		printf ("\n");
		free (enteredMove);

	return ((unsigned int) bet);
}

unsigned int readOption (){
	unsigned int op;
	do{
		printf ("What is your move? Press %d to hit a card and %d to stand\n", PLAYER_HIT_CARD, PLAYER_STAND);
		op = readBet();
		if ((op != PLAYER_HIT_CARD) && (op != PLAYER_STAND))
			printf ("Wrong option!\n");			
	} while ((op != PLAYER_HIT_CARD) && (op != PLAYER_STAND));
	return op;
}

int main(int argc, char **argv){
	struct soap soap;					/** Soap struct */
	char *serverURL;					/** Server URL */
	blackJackns__tMessage playerName;	/** Player name */
	blackJackns__tBlock gameStatus;		/** Game status */
	int resCode, gameId, finPartida = 0;/** Result and gameId */
	
	// Init gSOAP environment
	soap_init(&soap);

	// Obtain server address
	serverURL = argv[1];

	// Allocate memory
	allocClearMessage (&soap, &(playerName));
	allocClearBlock (&soap, &gameStatus);
			
	// Check arguments
	if (argc !=2) {
		printf("Usage: %s http://server:port\n",argv[0]);
		exit(0);
	}

	do{
		printf("Inserte su nombre: ");
		fgets(playerName.msg, STRING_LENGTH - 1, stdin);
		playerName.__size = strlen(playerName.msg)-1;
		soap_call_blackJackns__register(&soap, serverURL, "", playerName, &resCode);
		if(resCode == ERROR_NAME_REPEATED)
			printf("Nombre repetido, pruebe con otro.\n");
		if(resCode == ERROR_SERVER_FULL)
			printf("Servidor lleno, pruebe mas tarde.\n");
	}while(resCode == ERROR_NAME_REPEATED || resCode == ERROR_SERVER_FULL);
	gameId = resCode;
	printf("Register hecho en partida %d\n", gameId);
	//while(gameStatus.code != GAME_LOSE || gameStatus.code != GAME_WIN){
	while(!finPartida){
		//Obtener el estado de la partida (getStatus) e imprimir estado
		soap_call_blackJackns__getStatus(&soap, serverURL, "", gameId, playerName, &gameStatus);
		if(!(gameStatus.code == GAME_LOSE || gameStatus.code == GAME_WIN))
			printStatus(&gameStatus, DEBUG_CLIENT);
		while(gameStatus.code == TURN_PLAY){
			soap_call_blackJackns__play(&soap, serverURL, "", playerName, gameId, readOption(), &gameStatus);
			soap_call_blackJackns__getStatus(&soap, serverURL, "", gameId, playerName, &gameStatus);
			if(!(gameStatus.code == GAME_LOSE || gameStatus.code == GAME_WIN))
				printStatus(&gameStatus, DEBUG_CLIENT);
		}

		finPartida = gameStatus.code == GAME_LOSE || gameStatus.code == GAME_WIN;
	}
	printf("\nFIN DE LA PARTIDA\n");
	if(gameStatus.code == GAME_LOSE){
		printf("\nHas perdido el juego, te quedaste sin fichas\n");
	} else {
		printf("\nHas ganado el juego, el rival se quedó sin fichas\n");
	}
	
  	return 0;
}