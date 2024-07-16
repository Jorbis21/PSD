#include "serverGame.h"
#include <pthread.h>

tPlayer getNextPlayer (tPlayer currentPlayer){

	tPlayer next;

		if (currentPlayer == player1)
			next = player2;
		else
			next = player1;

	return next;
}

void initDeck (tDeck *deck){

	deck->numCards = DECK_SIZE;

	for (int i=0; i<DECK_SIZE; i++){
		deck->cards[i] = i;
	}
}

void clearDeck (tDeck *deck){

	// Set number of cards
	deck->numCards = 0;

	for (int i=0; i<DECK_SIZE; i++){
		deck->cards[i] = UNSET_CARD;
	}
}

void printSession (tSession *session){

		printf ("\n ------ Session state ------\n");

		// Player 1
		printf ("%s [bet:%d; %d chips] Deck:", session->player1Name, session->player1Bet, session->player1Stack);
		printDeck (&(session->player1Deck));

		// Player 2
		printf ("%s [bet:%d; %d chips] Deck:", session->player2Name, session->player2Bet, session->player2Stack);
		printDeck (&(session->player2Deck));

		// Current game deck
		if (DEBUG_PRINT_GAMEDECK){
			printf ("Game deck: ");
			printDeck (&(session->gameDeck));
		}
}

void initSession (tSession *session){

	clearDeck (&(session->player1Deck));
	session->player1Bet = 0;
	session->player1Stack = INITIAL_STACK;

	clearDeck (&(session->player2Deck));
	session->player2Bet = 0;
	session->player2Stack = INITIAL_STACK;

	initDeck (&(session->gameDeck));
}

unsigned int calculatePoints (tDeck *deck){

	unsigned int points;

		// Init...
		points = 0;

		for (int i=0; i<deck->numCards; i++){

			if (deck->cards[i] % SUIT_SIZE < 9)
				points += (deck->cards[i] % SUIT_SIZE) + 1;
			else
				points += FIGURE_VALUE;
		}

	return points;
}

unsigned int getRandomCard (tDeck* deck){

	unsigned int card, cardIndex, i;

		// Get a random card
		cardIndex = rand() % deck->numCards;
		card = deck->cards[cardIndex];

		// Remove the gap
		for (i=cardIndex; i<deck->numCards-1; i++)
			deck->cards[i] = deck->cards[i+1];

		// Update the number of cards in the deck
		deck->numCards--;
		deck->cards[deck->numCards] = UNSET_CARD;

	return card;
}

struct sockaddr_in playerSocketName(tString *name, unsigned int clientLength, int socketfd, int *socketPlayer,  int num){
	int msgLength;						/** Tamaño de los mensajes */
	struct sockaddr_in playerAddress;	/** Direccion del jugador */

	//Cogemos el tamaño de la estructura del jugador
	clientLength = sizeof(playerAddress);

	//Accept
	*socketPlayer = accept(socketfd, (struct sockaddr *) &playerAddress, &clientLength);

	//Comprobamos el socket del jugador
	if(*socketPlayer < 0)
		showError("ERROR while accepting");

	//Iniciamos y leemos el nombre del jugador
	memset(name, 0, sizeof(name));
	msgLength = recv(*socketPlayer, name, sizeof(name) - 1, 0);

	//Comprobamos los bytes leidos
	if (msgLength < 0)
		showError("ERROR while reading from socket");

	//Mostramos el nombre del jugador 1
	printf("El nombre del jugador %d es: %s\n",num, name);

	return playerAddress;
}

unsigned int recepcionUint(int socketPlayer){
	unsigned int tmp;
	int msgLength;
	msgLength = recv(socketPlayer, &tmp, sizeof(tmp), 0);
	if(msgLength < 0)
		showError("ERROR while reading from socket");
	return tmp;
}
void envioUint(unsigned int x, int socketPlayer){
	unsigned int tmp = x;
	int msgLength;
	msgLength = send(socketPlayer, &tmp, sizeof(tmp), 0);
	if(msgLength < 0)
		showError("ERROR while writing to socket");
}

unsigned int stackNbet(int socketPlayer, unsigned int *stack){
	unsigned int msgUi, bet = 0;
	int msgLength;

	//envio del turno al jugador 1
	envioUint(TURN_BET, socketPlayer);
	
	//envio del stack del jugador 1
	envioUint(*stack, socketPlayer);
	
	//recepcion apuesta
	bet = recepcionUint(socketPlayer);

	while (!(bet <= *stack && bet > 0 && bet <= MAX_BET))
	{
		envioUint(TURN_BET, socketPlayer);
		envioUint(*stack, socketPlayer);
		bet = recepcionUint(socketPlayer);
	}
	envioUint(TURN_BET_OK, socketPlayer);

	//o restamos el stack o nos quedamos con la apuesta minima (supongo lo primero)
	*stack -= bet;

	return bet;
}
void envioDeck(tDeck *deck, int socketPlayer){
	int msgLength;
	msgLength = send(socketPlayer, &deck->numCards, sizeof(unsigned int), 0);
	msgLength = send(socketPlayer, &deck->cards, sizeof(unsigned int) * deck->numCards, 0);
	if(msgLength < 0)
		showError("ERROR while writing to socket");
}
void envioInformacion(int jugA, tDeck d, unsigned int points){
	//Envio al jugA y jugB los puntos de jugA
	envioUint(points, jugA);

	//Envio del deck de jugA a jugA y a jugB

	envioDeck(&d, jugA);
}
void turnPlay(tDeck *d, tDeck *gameDeck,int jugA, int jugB){
	//juega jugA, jugB permanece pasivo
	unsigned int points, msgLength, i, playerMove;

	//mano inicial del jugA
	initDeck(&(*d));
	d->numCards=0;
	for(i = 0; i < 2; i++){
		d->cards[i] = getRandomCard(&(*gameDeck));
		d->numCards++;
	}
	//A jugA, TURN_PLAY y a jugB TURN_PLAY_WAIT
	envioUint(TURN_PLAY, jugA);
	envioUint(TURN_PLAY_WAIT, jugB);
	points = calculatePoints(d);
	envioInformacion(jugA, *d, points);
	envioInformacion(jugB, *d, points);
		
	playerMove = recepcionUint(jugA);
	while (playerMove == TURN_PLAY_HIT && points <= GOAL_GAME){
		d->cards[d->numCards] = getRandomCard(&(*gameDeck));
		d->numCards++;
		points = calculatePoints(d);
		if(points > GOAL_GAME){
			envioUint(TURN_PLAY_OUT, jugA);
			envioInformacion(jugA, *d, points);
		}
		else{
			envioUint(TURN_PLAY, jugA);
			envioUint(TURN_PLAY_WAIT, jugB);
			envioInformacion(jugA, *d, points);
			envioInformacion(jugB, *d, points);
			playerMove = recepcionUint(jugA);
		}
	}

	envioUint(TURN_PLAY_RIVAL_DONE, jugB);
	envioInformacion(jugB, *d, points);
}
tPlayer roundWinner(tDeck *player1Deck, tDeck *player2Deck){
	tPlayer winner;
	unsigned player1Points = calculatePoints(player1Deck);
	unsigned player2Points = calculatePoints(player2Deck);

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
unsigned int endOfSession(tSession *partida, int socketPlayer1, int socketPlayer2){
	unsigned int end = FALSE;

	if(partida->player1Stack==0){
		envioUint(TURN_GAME_LOSE, socketPlayer1);
		envioUint(TURN_GAME_WIN, socketPlayer2);
		end = TRUE;
	} else if(partida->player2Stack==0){
		envioUint(TURN_GAME_LOSE, socketPlayer2);
		envioUint(TURN_GAME_WIN, socketPlayer1);
		end = TRUE;
	}	

	return end;
}

void *logicaThread(void *threadArgs){
	tThreadArgs *args;
	tSession partida;
	int socketPlayer1;					/** Socket descriptor for player 1 */
	int socketPlayer2;					/** Socket descriptor for player 2 */
	unsigned int endOfGame = FALSE;
	tPlayer currentPlayer, winnerPlayer;

	args = (tThreadArgs *) threadArgs;
	partida = args->partida;
	socketPlayer1 = args->socketPlayer1;
	socketPlayer2 = args->socketPlayer2;

	// Seed
	srand(time(0));

	//Empieza el bucle de la partida
	currentPlayer = player1;
	while(!endOfGame){
		//Envio del turno y stack, y recepcion de la apuesta
		partida.player1Bet = stackNbet(socketPlayer1, &partida.player1Stack);
		partida.player2Bet = stackNbet(socketPlayer2, &partida.player2Stack);

		//cuidado con el esto que lo carga el diablo
		//if(SERVER_DEBUG) printSession(&partida);
		
		//En este punto empieza la jugada para los jugadores (ver 4d)
		if (currentPlayer == player1){
			turnPlay(&partida.player1Deck, &partida.gameDeck, socketPlayer1, socketPlayer2);
			if(SERVER_DEBUG) printSession(&partida);
			turnPlay(&partida.player2Deck, &partida.gameDeck, socketPlayer2, socketPlayer1);
			if(SERVER_DEBUG) printSession(&partida);
		}
		else{
			turnPlay(&partida.player2Deck, &partida.gameDeck, socketPlayer2, socketPlayer1);
			if(SERVER_DEBUG) printSession(&partida);
			turnPlay(&partida.player1Deck, &partida.gameDeck, socketPlayer1, socketPlayer2);
			if(SERVER_DEBUG) printSession(&partida);
		}

		winnerPlayer = roundWinner(&partida.player1Deck, &partida.player2Deck);

		if(winnerPlayer == noOne){//Caso empate o si ambos se han pasado de 21
			//ronda "anulada", les devolvemos las fichas
			partida.player1Stack += partida.player1Bet;
			partida.player2Stack += partida.player2Bet;
		}//Las siguientes dos casuisticas dependen de si cogemos la menor de las apuestas o la que cada uno ponga (suponemos esto segundo)
		else if(winnerPlayer == player1){
			partida.player1Stack = partida.player1Stack + partida.player1Bet + partida.player2Bet;
		}
		else if(winnerPlayer == player2){
			partida.player2Stack = partida.player2Stack + partida.player1Bet + partida.player2Bet;
		}

		//Cambio de turno
		currentPlayer = getNextPlayer(currentPlayer);

		//Comprobación de fin de partida
		endOfGame = endOfSession(&partida, socketPlayer1, socketPlayer2);
	}

	close(socketPlayer1);
	close(socketPlayer2);
	return 0;
}
int main(int argc, char *argv[]){

	int socketfd;						/** Socket descriptor */
	struct sockaddr_in serverAddress;	/** Server address structure */
	unsigned int port;					/** Listening port */
	struct sockaddr_in player1Address;	/** Client address structure for player 1 */
	struct sockaddr_in player2Address;	/** Client address structure for player 2 */
	int socketPlayer1;					/** Socket descriptor for player 1 */
	int socketPlayer2;					/** Socket descriptor for player 2 */
	tSession partida;
	unsigned int clientLength;			/** Length of client structure */
	unsigned int i;

	//Cosas de los threads
	tThreadArgs threadArgs[MAX_CONNECTIONS];	/** Thread parameters */
	pthread_t threadID[MAX_CONNECTIONS];		/** Thread ID */

	unsigned int conexion;
		// Seed
	srand(time(0));
			// Check arguments
	if (argc != 2) {
		fprintf(stderr,"ERROR wrong number of arguments\n");
		fprintf(stderr,"Usage:\n$>%s port\n", argv[0]);
		exit(1);
	}
	//Creamos socket del server
	socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//Comprobamos  
	if(socketfd < 0)
		showError("ERROR while opening socket");
	//Iniciamos la estructura del servidor
	memset(&serverAddress, 0, sizeof(serverAddress));

	//Cogemos el puerto de los parametros
	port = atoi(argv[1]);

	//Rellenamos la estructura del servidor
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(port);

	//Binding of isaac
	if(bind(socketfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
		showError("ERROR while binding");

	//Listen
	if (listen(socketfd, MAX_CONNECTIONS) < 0)
		showError("ERROR while listening");


	//Iniciamos la partida
	initSession(&partida);
	conexion = 0;
	while(TRUE){
		//Inicializacion de los sockets y obtencion de los nombres de los jugadores
		player1Address = playerSocketName(&partida.player1Name, clientLength, socketfd, &socketPlayer1, 1);
		player2Address = playerSocketName(&partida.player2Name, clientLength, socketfd, &socketPlayer2, 2);
		threadArgs[conexion].socketPlayer1 = socketPlayer1;
		threadArgs[conexion].socketPlayer2 = socketPlayer2;
		threadArgs[conexion].partida = partida;//&

		if (pthread_create(&threadID[conexion], NULL, logicaThread, &threadArgs[conexion]) != 0)
			showError("Error while creating thread");

		conexion++;
	}

	for (i=0; i <= conexion; i++){
		pthread_join(threadID[i], NULL);
		exit(0);
	}

	return 0;
}