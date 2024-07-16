rm imagenFinal.bmp
make clean
make
mpiexec -np 6 lifeGame 200 200 400 auto imagenFinal.bmp static
