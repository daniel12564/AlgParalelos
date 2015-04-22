#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define tam 20

void Build_mpi_type(bool** universo, int i, int j, MPI_Datatype* tipo)
{
	int array_of_blockslengths[3] = {1, 1, 1};
	MPI_Datatype array_of_types[3] = {MPI_BOOL, MPI_INT_ MPI_INT};
	MPI_Aint a_addr, b_addr, c_addr;
	MPI_Aint array_of_displacements[3] = {0};

	MPI_Get_address(universo, &a_addr);
	MPI_Get_address(i, &b_addr);
	MPI_Get_address(j, &c_addr);
	array_of_displacements[1] = b_addr - a_addr;
	array_of_displacements[2] = c_addr - a_addr;
	MPI_Type_create_struct(3, array_of_blockslengths, array_of_displacements, array_of_types, tipo);
}

void set_universo(bool** universo)
{
	int i, j;
	universo = (bool **) malloc(tam * sizeof(bool *));
	for(i = 0; i < tam; i++)
		universo[i] = (bool *) malloc(tam * sizeof(bool));
	for(i = 0; i < tam; i++)
		for(j = 0; j < tam; j++)
			universo[i][j] = (bool) (rand() % 1);
}

void game_of_life(bool** universo, int generaciones)
{
	if(!generaciones)
		return;

	int i, j;
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	bool** n_universo;
	bool* aux;
	n_universo = (bool **) malloc(tam * sizeof(bool *));
	aux = (bool *) malloc(tam * sizeof(bool));
	for(i = 0; i < tam; i++)
		n_universo[i] = (bool *) malloc(tam * sizeof(bool));

	for(i = 0; i < tam; i++)
	{
		if(!rank)
		{
			
			MPI_Scatter();
		}
	}

	game_of_life(n_universo, generaciones - 1);
}

int main(int argc, char* argv[])
{
	int rank, size, generaciones, i, j;
	MPI_Init(&argc, &argv);
	
	game_of_life(generaciones);
	MPI_Finalize();
	return 0;
}