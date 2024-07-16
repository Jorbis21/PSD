#include "server.h"
#include <pthread.h>
#include <string.h>
/** Shared array that contains all the games. */
tGame games[MAX_GAMES];
tGameState gameState[MAX_GAMES];

pthread_mutex_t reg_m;

int endOfGame(int gameId){
	if(games[gameId].player1Stack <= 0 || games[gameId].player2Stack <= 0)
		return TRUE;
	else
		return FALSE;
}

tPlayer roundWinner(int gameId){
	tPlayer winner;
	int player1Points = calculatePoints(&(games[gameId].player1Deck));
	int player2Points = calculatePoints(&(games[gameId].player2Deck));

	if(player1Points > GOAL_GAME && player2Points > GOAL_GAME)
		winner = noOne;
	else if(player1Points > GOAL_GAME)
		winner = player2;
	else if(player2Points > GOAL_GAME)
		winner = player1;
	else if(player1Points == player2Points)
		winner = noOne;
	else{
		if(player1Points > player2Points)
			winner = player1;
		else
			winner = player2;
	}

	return winner;
}

void endOfRound(int gameId){
	/*	i) Comprueba el ganador de la ronda
		ii) Actualiza los stacks (en caso de no haber ganador no se suma ni resta nada)
		iii) Comprueba si la partida a acabado
			a) Si la partida no ha acabado se prepara para la siguiente ronda actualizando barajas y decidiendo que jugador la comenzará*/
	tPlayer winnerPlayer = roundWinner(gameId);

	if(winnerPlayer == player1){
		games[gameId].player1Stack += games[gameId].player1Bet;
		games[gameId].player2Stack -= games[gameId].player1Bet;
	} else if(winnerPlayer == player2){
		games[gameId].player2Stack += games[gameId].player2Bet;
		games[gameId].player1Stack -= games[gameId].player2Bet;
	}

	games[gameId].endOfGame = endOfGame(gameId);
	if(!games[gameId].endOfGame){//preparamos para la siguiente ronda
		clearDeck(&(games[gameId].player1Deck));
		clearDeck(&(games[gameId].player2Deck));
		initDeck(&(games[gameId].gameDeck));
		games[gameId].currentPlayer = rand() % 2 == 0 ? player1 : player2;
	} else {
		printf("GAME %d FINISHED\n", gameId);
		printf("%s was the winner [%d]\n", games[gameId].player1Stack <= 0? games[gameId].player2Name : games[gameId].player1Name ,gameId);
	}
	pthread_cond_signal(&(games[gameId].st));
}

void initGame(tGame *game, tGameState *gameState){

	// Init players' name
	memset(game->player1Name, 0, STRING_LENGTH);
	memset(game->player2Name, 0, STRING_LENGTH);

	// Alloc memory for the decks
	clearDeck(&(game->player1Deck));
	clearDeck(&(game->player2Deck));
	initDeck(&(game->gameDeck));

	// Bet and stack
	game->player1Bet = DEFAULT_BET;
	game->player2Bet = DEFAULT_BET;
	game->player1Stack = INITIAL_STACK;
	game->player2Stack = INITIAL_STACK;

	// Init current player
	game->currentPlayer = noOne;

	// Game status variables
	game->endOfGame = FALSE;
	*gameState = gameEmpty;
}

void initServerStructures(struct soap *soap){

	if (DEBUG_SERVER)
		printf("Initializing structures...\n");

	// Init seed
	srand(time(NULL));

	// Init each game (alloc memory and init)
	for (int i = 0; i < MAX_GAMES; i++){
		games[i].player1Name = (xsd__string)soap_malloc(soap, STRING_LENGTH);
		games[i].player2Name = (xsd__string)soap_malloc(soap, STRING_LENGTH);
		allocDeck(soap, &(games[i].player1Deck));
		allocDeck(soap, &(games[i].player2Deck));
		allocDeck(soap, &(games[i].gameDeck));
		initGame(&(games[i]), &(gameState[i]));
	}
}

void initDeck(blackJackns__tDeck *deck){

	deck->__size = DECK_SIZE;

	for (int i = 0; i < DECK_SIZE; i++)
		deck->cards[i] = i;
}

void clearDeck(blackJackns__tDeck *deck){

	// Set number of cards
	deck->__size = 0;

	for (int i = 0; i < DECK_SIZE; i++)
		deck->cards[i] = UNSET_CARD;
}

tPlayer calculateNextPlayer(tPlayer currentPlayer){
	return ((currentPlayer == player1) ? player2 : player1);
}

unsigned int getRandomCard(blackJackns__tDeck *deck){

	unsigned int card, cardIndex, i;

	// Get a random card
	cardIndex = rand() % deck->__size;
	card = deck->cards[cardIndex];

	// Remove the gap
	for (i = cardIndex; i < deck->__size - 1; i++)
		deck->cards[i] = deck->cards[i + 1];

	// Update the number of cards in the deck
	deck->__size--;
	deck->cards[deck->__size] = UNSET_CARD;

	return card;
}

void copyGameStatusStructure(blackJackns__tBlock *status, char *message, blackJackns__tDeck *newDeck, int newCode){

	// Copy the message
	status->msgStruct.msg = (xsd__string)malloc(STRING_LENGTH);
	memset((status->msgStruct).msg, 0, STRING_LENGTH);
	strcpy((status->msgStruct).msg, message);
	(status->msgStruct).__size = strlen((status->msgStruct).msg);

	// Copy the deck, only if it is not NULL
	if (newDeck->__size > 0)
		memcpy((status->deck).cards, newDeck->cards, DECK_SIZE * sizeof(unsigned int));
	else
		(status->deck).cards = NULL;

	(status->deck).__size = newDeck->__size;

	// Set the new code
	status->code = newCode;
}

unsigned int calculatePoints(blackJackns__tDeck *deck){

	unsigned int points = 0;

	for (int i = 0; i < deck->__size; i++)
	{

		if (deck->cards[i] % SUIT_SIZE < 9)
			points += (deck->cards[i] % SUIT_SIZE) + 1;
		else
			points += FIGURE_VALUE;
	}

	return points;
}

int nextGameIndex(){
	int i = 0;
	while (i < MAX_GAMES && gameState[i] != gameWaitingPlayer){
		i++;
	}

	if (i == MAX_GAMES){ 
		i = 0;
		while (i < MAX_GAMES && gameState[i] != gameEmpty)
		{
			i++;
		}
	}

	return i;
}

int blackJackns__register(struct soap *soap, blackJackns__tMessage playerName, int *result){
	int gameIndex, cmp;
	playerName.msg[playerName.__size] = 0;

	pthread_mutex_lock(&reg_m);

	gameIndex = nextGameIndex();
	if (gameIndex == MAX_GAMES){
		pthread_mutex_unlock(&reg_m);
		*result = ERROR_SERVER_FULL;
		return SOAP_OK;
	}

	if (gameState[gameIndex] == gameEmpty){
		strcpy(games[gameIndex].player1Name, playerName.msg);
		gameState[gameIndex] = gameWaitingPlayer;
	}
	else if (gameState[gameIndex]  == gameWaitingPlayer){

		if ((strcmp(playerName.msg, games[gameIndex].player1Name)) == 0){ 
			*result = ERROR_NAME_REPEATED;
			pthread_mutex_unlock(&reg_m);
			return SOAP_OK;
		}
		else{	
			strcpy(games[gameIndex].player2Name, playerName.msg);
			gameState[gameIndex] = gameReady;
			games[gameIndex].currentPlayer = rand() % 2 == 0 ? player1 : player2;//inicializamos el jugador que va a empezar, previamente ha sido noOne (ver initGame)
			pthread_cond_signal(&(games[gameIndex].st));//esto no se si es legal pero de momento va
		}
	}
	
	if (DEBUG_SERVER)
		printf("[Register] Registering new player -> [%s:%d]\n", playerName.msg, gameIndex);
	
	*result = gameIndex;

	pthread_mutex_unlock(&reg_m);

	return SOAP_OK;
}
void makePlayerDeck(int gameId, tPlayer player){
	int i;
	if(player == player1){
		if(games[gameId].player1Deck.__size == 0){
			for(i=0; i<2;i++){
				games[gameId].player1Deck.cards[i] = getRandomCard(&(games[gameId].gameDeck));
				games[gameId].player1Deck.__size++;
			}
		} else  {
				games[gameId].player1Deck.cards[games[gameId].player1Deck.__size] = getRandomCard(&(games[gameId].gameDeck));
				games[gameId].player1Deck.__size++;
		}
	} else {
		if(games[gameId].player2Deck.__size == 0){
			for(i=0; i<2;i++){
				games[gameId].player2Deck.cards[i] = getRandomCard(&(games[gameId].gameDeck));
				games[gameId].player2Deck.__size++;
			}
		} 
		else {
			games[gameId].player2Deck.cards[games[gameId].player2Deck.__size] = getRandomCard(&(games[gameId].gameDeck));
			games[gameId].player2Deck.__size++;
		}
	}
}

int blackJackns__getStatus(struct soap *soap, int gameId, blackJackns__tMessage playerName, blackJackns__tBlock *block){
	char* msg = malloc(STRING_LENGTH);
	int points, fichas, code;

	playerName.msg[playerName.__size] = 0;
	allocClearBlock (soap, block);
	pthread_mutex_lock(&(games[gameId].st_m));

	while (((strcmp(playerName.msg,games[gameId].player1Name) == 0) && games[gameId].currentPlayer != player1)
			|| ((strcmp(playerName.msg,games[gameId].player2Name) == 0) && games[gameId].currentPlayer != player2)){
		pthread_cond_wait(&(games[gameId].st), &(games[gameId].st_m));
	}

	if (games[gameId].currentPlayer == player1){
		if(games[gameId].endOfGame){
			code = games[gameId].player1Stack <= 0 ? GAME_LOSE : GAME_WIN;
			copyGameStatusStructure(block, msg, &(games[gameId].player1Deck), code);
			gameState[gameId]--;
			if(gameState[gameId]==gameEmpty){
				initGame(&(games[gameId]), &(gameState[gameId]));
			} else {
				games[gameId].currentPlayer = calculateNextPlayer(player1);
			}
			pthread_cond_signal(&(games[gameId].st));

		} else {
			if(games[gameId].player1Deck.__size == 0){
				makePlayerDeck(gameId, player1);
			} 
			points = calculatePoints(&(games[gameId].player1Deck));
			fichas = games[gameId].player1Stack;

			if(points>GOAL_GAME){
				sprintf(msg, "\nHas perdido la ronda, te pasaste, tienes %d puntos\n", points);
				code = TURN_WAIT;
				copyGameStatusStructure(block, msg, &(games[gameId].player1Deck), code);
				endOfRound(gameId);
			} else {
				sprintf(msg, "\nTU TURNO\nTienes %d fichas\nTu puntuacion es: %d", fichas, points);
				code = TURN_PLAY;
				copyGameStatusStructure(block, msg, &(games[gameId].player1Deck), code);
			}
		}
	}
	else{
		if (games[gameId].endOfGame){
			code = games[gameId].player2Stack <= 0 ? GAME_LOSE : GAME_WIN;
			copyGameStatusStructure(block, msg, &(games[gameId].player2Deck), code);
			gameState[gameId]--;
			if(gameState[gameId]==gameEmpty){
				initGame(&(games[gameId]), &(gameState[gameId]));
			} else {
				games[gameId].currentPlayer = calculateNextPlayer(player2);
			}
			pthread_cond_signal(&(games[gameId].st));
		} else {
			if(games[gameId].player2Deck.__size == 0){
				makePlayerDeck(gameId, player2);
			} 
			points = calculatePoints(&(games[gameId].player2Deck));
			fichas = games[gameId].player2Stack;

			if(points>GOAL_GAME){
				sprintf(msg, "\nHas perdido la ronda, te pasaste, tienes %d puntos\n", points);
				code = TURN_WAIT;
				copyGameStatusStructure(block, msg, &(games[gameId].player2Deck), code);
				endOfRound(gameId);
			} else {
				sprintf(msg, "\nTU TURNO\nTienes %d fichas\nTu puntuacion es: %d", fichas, points);
				code = TURN_PLAY;
				copyGameStatusStructure(block, msg, &(games[gameId].player2Deck), code);
			}
		}
	}
	

	pthread_cond_signal(&(games[gameId].st));
	pthread_mutex_unlock(&(games[gameId].st_m));

	return SOAP_OK;
}

int blackJackns__play(struct soap *soap, blackJackns__tMessage playerName, int gameId, unsigned int playerMove, blackJackns__tBlock *block){
	tPlayer player = games[gameId].currentPlayer;
	if (playerMove == PLAYER_HIT_CARD){
		makePlayerDeck(gameId, player);
	}
	else if (playerMove == PLAYER_STAND){ //aquí tenemos que ver si la ronda ha acabado o sino
		if(games[gameId].player1Deck.__size == 0 || games[gameId].player2Deck.__size == 0){//la ronda no ha acabado
			//code = TURN_WAIT;
			games[gameId].currentPlayer = calculateNextPlayer(player);
			pthread_cond_signal(&(games[gameId].st));
		} else {//la ronda acabó
			endOfRound(gameId);
		}
	}
	return SOAP_OK;
}

void *processRequest(void *soap){
	pthread_detach(pthread_self());

	//printf("Conectando jugador...\n");

	soap_serve((struct soap *)soap);
	soap_destroy((struct soap *)soap);
	soap_end((struct soap *)soap);
	soap_done((struct soap *)soap);

	free(soap);

	return NULL;
}

int main(int argc, char **argv){

	struct soap soap;
	struct soap *tsoap;
	pthread_t tid;
	int port, first = TRUE;
	SOAP_SOCKET m, s;
	// Check arguments
	if (argc != 2)
	{
		printf("Usage: %s port\n", argv[0]);
		exit(0);
	}
	// Inisio de soap
	soap_init(&soap);

	// Configuracion de tiempos
	soap.send_timeout = 60;		// segundos
	soap.recv_timeout = 60;		// segundos
	soap.accept_timeout = 3600; // se para tras 1 hora de espera
	soap.max_keep_alive = 100;

	// Puerto desde el que se escucha
	port = atoi(argv[1]);

	// Binding of isaac
	m = soap_bind(&soap, NULL, port, 100);

	if (!soap_valid_socket(m))
	{
		exit(1);
	}

	initServerStructures(&soap);

	printf("Server is ON!\n");

	while (TRUE)
	{
		// Aceptamos nueva conexion
		s = soap_accept(&soap);

		// Socket invalido
		if (!soap_valid_socket(s))
		{
			if (soap.errnum)
			{
				soap_print_fault(&soap, stderr);
				exit(1);
			}
			fprintf(stderr, "Time out!\n");
			break;
		}

		// Se hace una copia de la estructura soap
		tsoap = soap_copy(&soap);

		if (!tsoap)
		{
			printf("SOAP copy error!\n");
			break;
		}
		// Creacion del hilo cliente
		pthread_create(&tid, NULL, (void *(*)(void *))processRequest, (void *)tsoap);
	}
	// Fin soap
	soap_done(&soap);
	return 0;
}