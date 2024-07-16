#include "worker.h"
#include "mpi.h"

void workerInit(int rank, int worldWidth, int worldHeight){
    int fin = 0, filas, tag = 1, filaIndice, tamMax;
    unsigned short *areaRecv, *areaSend;
    
    //malloc del area
    MPI_Recv(&tamMax, 1, MPI_INT, MASTER, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    areaRecv = malloc((tamMax+2) * worldWidth * sizeof(unsigned short));
    areaSend = malloc((tamMax+2) * worldWidth * sizeof(unsigned short));

    while (!fin){
        //recv del numero de filas
        MPI_Recv(&filas, 1, MPI_INT, MASTER, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (filas == 0){
            fin = 1;
        } else {
            //recv indice
            MPI_Recv(&filaIndice, 1, MPI_INT, MASTER, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //recv area
            MPI_Recv(areaRecv, worldWidth, MPI_UNSIGNED_SHORT, MASTER, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(areaRecv + worldWidth, filas * worldWidth, MPI_UNSIGNED_SHORT, MASTER, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(areaRecv + worldWidth * (filas + 1), worldWidth, MPI_UNSIGNED_SHORT, MASTER, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
            //calculo del nuevo area
            updateWorld(areaRecv, areaSend, worldWidth, filas+2);

            //send area, se envian las filas de en medio, quitando el top y el bottom
            MPI_Send(&filas, 1, MPI_INT, MASTER, tag, MPI_COMM_WORLD);
            MPI_Send(&filaIndice, 1, MPI_INT, MASTER, tag, MPI_COMM_WORLD);
            MPI_Send(areaSend + worldHeight, filas * worldWidth, MPI_UNSIGNED_SHORT, MASTER, tag, MPI_COMM_WORLD);
        }
    }
    
    //free del area
    free(areaRecv);
    free(areaSend);
}