#include "master.h"
#include <SDL2/SDL.h>
#include <sys/time.h>
#include "mpi.h"

void cataclismo(unsigned short* world, int worldWidth, int worldHeight){
    int i;
    for(i=0; i < worldHeight; i++){
        //primera celda de la fila
        world[i*worldWidth] = CELL_CATACLYSM;//(i,0)
        
        //ultima celda de la fila
        world[i*worldWidth + worldWidth-1] = CELL_CATACLYSM;//(i, last)
    }
}

unsigned short* filaSuperior(unsigned short* world, int filaActual, int worldWidth, int worldHeight){
    return (filaActual == 0)? (world + (worldHeight-1)*worldWidth) : world + (worldWidth * (filaActual - 1));
}

unsigned short* filaInferior(unsigned short* world, int filaActual, int worldWidth, int worldHeight, int tamArea){
    return (filaActual+tamArea==worldHeight)? world: world + (worldWidth * (filaActual+tamArea));
}

int sendToWorker(int proceso, int reparto, unsigned short* currentWorld, int worldWidth, int worldHeight, int tag, int filaIndice){
    //primeramente le mandamos las filas que va a tener que procesar y el índice de la primera fila que va a procesar
    MPI_Send(&reparto, 1, MPI_INT, proceso, tag, MPI_COMM_WORLD);
    MPI_Send(&filaIndice, 1, MPI_INT, proceso, tag, MPI_COMM_WORLD);
    //después mandamos la fila superior, el area y la fila inferior
    MPI_Send(filaSuperior(currentWorld, filaIndice,worldWidth, worldHeight),worldWidth, MPI_UNSIGNED_SHORT, proceso, tag, MPI_COMM_WORLD);
    MPI_Send(currentWorld + (worldWidth * filaIndice), worldWidth * reparto, MPI_UNSIGNED_SHORT, proceso, tag, MPI_COMM_WORLD);
    MPI_Send(filaInferior(currentWorld, filaIndice,worldWidth, worldHeight, reparto),worldWidth, MPI_UNSIGNED_SHORT, proceso, tag, MPI_COMM_WORLD);
    return reparto;
}

void staticMode(unsigned short* currentWorld, unsigned short* nextWorld, int reparto, int resto, int workers, int worldWidth, int worldHeight){
    MPI_Status status;
    int filasRecv = 0, filasEnv = 0, tag = 1, numFilas, filaIndice;
    for(int proceso = 1; proceso <= workers; proceso++){
        reparto = proceso==workers? reparto+resto : reparto;
        filasEnv += sendToWorker(proceso, reparto, currentWorld, worldWidth, worldHeight, tag, filasEnv);
    }
    while(filasRecv < worldHeight){
        MPI_Recv(&numFilas, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&filaIndice, 1, MPI_INT, status.MPI_SOURCE, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(nextWorld + (worldWidth * filaIndice), worldWidth * numFilas, MPI_UNSIGNED_SHORT, status.MPI_SOURCE, tag, MPI_COMM_WORLD, &status);
        filasRecv += numFilas;
    }
}
void dynamicMode(unsigned short* currentWorld, unsigned short* nextWorld,int workers, int worldWidth, int worldHeight, int grain){
    MPI_Status status;
    int filasRecv = 0, filasEnv = 0, tag = 1, numFilas, filaIndice, reparto;
    for(int proceso = 1; proceso <= workers; proceso++){
        if(filasEnv < worldHeight){
            reparto = (worldHeight - filasEnv) >= grain ? grain : worldHeight - filasEnv;
            filasEnv += sendToWorker(proceso,reparto,currentWorld,worldWidth,worldHeight,tag, filasEnv);
        }
    }
    while(filasRecv < worldHeight){
        MPI_Recv(&numFilas, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&filaIndice, 1, MPI_INT, status.MPI_SOURCE, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(nextWorld + (worldWidth * filaIndice), worldWidth * numFilas, MPI_UNSIGNED_SHORT, status.MPI_SOURCE, tag, MPI_COMM_WORLD, &status);
        filasRecv += numFilas;
        if(filasEnv < worldHeight){
            reparto = (worldHeight - filasEnv) >= grain ? grain : worldHeight - filasEnv;
            filasEnv += sendToWorker(status.MPI_SOURCE,reparto,currentWorld,worldWidth,worldHeight,tag, filasEnv);
        }
    }
}
void masterInit(int workers, int worldWidth, int worldHeight, int estatico, int grain, int iterations, SDL_Window* window, SDL_Renderer* renderer, int autoMode) {
    int reparto, resto, tag = 1, lastCataclismo = 0;
    unsigned short* worldA, *worldB, *currentWorld, *nextWorld;
    char ch;
  
    SDL_Event event;
	struct timeval stop, start;

    int isquit = 0;

    worldA = malloc(sizeof(unsigned short)*worldWidth*worldHeight);
    worldB = malloc(sizeof(unsigned short)*worldWidth*worldHeight);

    clearWorld(worldA, worldWidth, worldHeight);
    clearWorld(worldB, worldWidth, worldHeight);

    initRandomWorld(worldA, worldWidth, worldHeight);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer);		
	drawWorld (worldB, worldA, renderer, 0, worldHeight - 1, worldWidth, worldHeight);		
	SDL_RenderPresent(renderer);
	SDL_UpdateWindowSurface(window);

	if (SDL_PollEvent(& event)) 
		if (event.type == SDL_QUIT) 
		    isquit = 1;

    if(estatico){
        reparto = worldHeight/workers; resto = worldHeight%workers;
        int filasMax= reparto+resto;
        for(int proceso = 1; proceso <= workers; proceso++){
            MPI_Send(&filasMax,1,MPI_INT,proceso,tag,MPI_COMM_WORLD);
        }
    }
    else{
        for(int proceso = 1; proceso <= workers; proceso++){
            MPI_Send(&grain,1,MPI_INT,proceso,tag,MPI_COMM_WORLD);
        }
    }
    for(int i = 0; i < iterations; i++){
        printf("Procesando la %dº iteración\n", (i+1));
        // Clear renderer			
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);

        // Set timer for this iteration
		gettimeofday(&start, NULL);

        currentWorld = (i%2==0)? worldA : worldB;
        nextWorld = (i%2==0)? worldB : worldA;
            
        if(estatico){
            staticMode(currentWorld, nextWorld, reparto, resto, workers, worldWidth, worldHeight);
        }
        else{
            dynamicMode(currentWorld, nextWorld, workers, worldWidth, worldHeight, grain);
        }
        //Cataclismo
        if(lastCataclismo>=ITER_CATACLYSM){
            if(rand()%PROB_CATACLYSM==0){
                cataclismo(nextWorld, worldWidth, worldHeight);
                lastCataclismo=0;
            } else {
                lastCataclismo++;
            }
        } else {
            lastCataclismo++;
        }

        drawWorld(currentWorld, nextWorld, renderer, 0, worldHeight-1 ,worldWidth, worldHeight);
        clearWorld(currentWorld, worldWidth, worldHeight);

        // End timer for this iteration
		gettimeofday(&stop, NULL);
		printf ("%f seconds\n", (double) ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec)/1000000.0);			

		//Update the surface
		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(window);					
			
		// Read event
		if (SDL_PollEvent(& event)) 
			if (event.type == SDL_QUIT) 
				isquit = 1;			
			
		if (!autoMode){
			printf ("Press Enter to continue...");
			ch = getchar();
		}
    }

    int fin = 0;
    for(int proceso = 1; proceso <= workers; proceso++){
        MPI_Send(&fin, 1, MPI_INT, proceso, tag, MPI_COMM_WORLD);
    }
       

    free(worldA);
    free(worldB);
}