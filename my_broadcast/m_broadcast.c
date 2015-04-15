#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void my_MPI_Bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator)
{
    int rank, size;
    MPI_Comm_rank(communicator, &rank);
    MPI_Comm_size(communicator, &size);
    if (rank == root)
    {
        int i;
        for(i = 0; i < size; i++) {
            if (i != rank)
                MPI_Send(data, count, datatype, i, 0, communicator);
        }
    }
    else
    {
        MPI_Recv(data, count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
    }
}

int main(int argc, char* argv[])
{
    int rank, size, i;
    double total_my_bcast_time = 0.0;
    double total_mpi_bcast_time = 0.0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    for(i = 10; i < 10000000; i *= 100)
    {
        int* data =  (int*) malloc(sizeof(int) * i);
        MPI_Barrier(MPI_COMM_WORLD);
        total_my_bcast_time -= MPI_Wtime();
        my_MPI_Bcast(data, i, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        total_my_bcast_time += MPI_Wtime();

        MPI_Barrier(MPI_COMM_WORLD);
        total_mpi_bcast_time -= MPI_Wtime();
        MPI_Bcast(data, i, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        total_mpi_bcast_time += MPI_Wtime();
        if(rank == 0)
            printf("numero de datos: %d, tiempo my_MPI_Bcast: %4.4f, tiempo MPI_Bcast: %4.4f, diferencia %4.4f \n", i, (double) total_my_bcast_time, (double) total_mpi_bcast_time, (double) total_mpi_bcast_time - total_my_bcast_time);
        free(data);
    }
    MPI_Finalize();
    return 0;
}