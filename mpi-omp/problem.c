#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>

int thread_count;

int main(int argc, char *argv[])
{
    thread_count = 10;
    //thread_count = strtol(argv[1], NULL, 10);;
    
    int tam = 1000000 * thread_count;

    int numprocs, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL)*rank);
    int* rand_nums;
    int local_sum, ii, global_sum;
    int prc = tam / numprocs;

    if(!rank)
    {
        rand_nums = (int*) malloc(sizeof(int) * tam);
        int i;
        for(i = 0; i < tam; i++)
            rand_nums[i] = rand();
        local_sum = 0;
        global_sum = 0;

        //for (i = 0; i < tam; i++)
            //printf("%d ", rand_nums[i]);
        //printf("\n");
    }

    # pragma omp parallel for num_threads(thread_count)\
    private(ii, local_sum)
        for(ii = (prc * rank); ii < (prc * (rank + 1)); ii++)
            local_sum += rand_nums[ii];

    printf("suma local %d del proceso %d \n", local_sum, rank);

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (!rank)
    {
        printf("suma total = %d\n", global_sum);
    }

    MPI_Finalize();

    return 0;
}