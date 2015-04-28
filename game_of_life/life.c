#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define tam 10
#define tam_i tam
#define tam_j tam

char** leer_universo()
{
	int  i, j, k;
    char** universo;
    universo = (char **) malloc(tam_i * sizeof(char *));
	for(i = 0; i < tam_i; i++)
			universo[i] = (char *) malloc(tam_j * sizeof(char));
	i = 0;
	j = 0;
	k = 0;
	FILE *file;
	char buffer[100];
	file = fopen("universo.txt", "rt");
	while(feof(file) == 0)
	{
		fscanf(file, "%s" ,buffer);
		for(j = 0; j < tam_j; j++)
		{
			universo[i][j] = buffer[j];
			//printf("%c-%c ", buffer[j], universo[i][j]);
		}
		//printf("\n");
		i++;
		//for(i = 0; i < tam_i; i++)
			//for(j = 0; j < tam_j; j++)	
			
	}
	fclose(file);
	return universo;
}

void print_universo(char** universo)
{
	int i, j;
	for(i = 0; i < tam_i; i++)
	{
		for(j = 0; j < tam_j; j++)
			printf("%c", universo[i][j]); ;
		printf("\n");
	}
}

int modulo(int a, int b)
{
	if(a < 0)
		return (a+b) % b;
	return a % b;
}

char evaluar(char** universo, int x, int y)
{
	int x_pos = modulo(x + 1, tam_i);
	int x_neg = modulo(x - 1, tam_i);
	int y_pos = modulo(y + 1, tam_j);
	int y_neg = modulo(y - 1, tam_j);
	int contador = 0;
	contador += (universo[x_neg][y_neg] == '1') ? 1 : 0;
	contador += (universo[x_neg][y] == '1') ? 1 : 0;
	contador += (universo[x_neg][y_pos] == '1') ? 1 : 0;
	contador += (universo[x][y_neg] == '1') ? 1 : 0;
	contador += (universo[x][y_pos] == '1') ? 1 : 0;
	contador += (universo[x_pos][y_neg] == '1') ? 1 : 0;
	contador += (universo[x_pos][y] == '1') ? 1 : 0;
	contador += (universo[x_pos][y_pos] == '1') ? 1 : 0;
	if(contador < 2 || contador > 3)
		return '0';
	if(contador == 2 || contador == 3)
		return '1';
}

void game_of_life(char** universo, int generaciones)
{
	int i, j, k;
	int rank, size;
	int bloque, n_i, n_j;
	char** n_universo;
	char** tmp;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	bloque = tam_i / size;
	
	char vector_universo[tam_i * tam_j];
	char vector_bloque[bloque * tam_j];

	if(!rank)
	{
		k = 0;
		for(i = 0; i < tam_i; i++)
		{
			for(j = 0; j < tam_j; j++)
				vector_universo[k++] = universo[i][j];
		}
	}

	MPI_Datatype blocktype;
    MPI_Datatype blocktype2;

    MPI_Type_vector(bloque, tam_j, tam_j, MPI_CHAR, &blocktype2);
    MPI_Type_create_resized(blocktype2, 0, sizeof(char), &blocktype);
    MPI_Type_commit(&blocktype);

    int disps[size];
    int counts[size];
    for(i = 0; i < size; i++)
    {
        disps[i] = 2* i * tam_j;
		counts[i] = 1;
    }

    MPI_Scatterv(vector_universo, counts, disps, blocktype, vector_bloque, bloque * tam_j, MPI_CHAR, 0, MPI_COMM_WORLD);

    for(k = 0; k < size; k++)
    {
        if(k == rank)
        {
            printf("Rank = %d\n", rank);
            /*if(!rank)
            {
                printf("Global matrix: \n");
                for(i = 0; i < tam_i; i++)
                {
                    for(j = 0; j < tam_j; j++)
                        printf("%c ",vector_universo[i*tam_j+j]);
                    printf("\n");
                }
            }*/
            printf("Local Matrix:\n");
            for(i = 0; i < bloque; i++)
            {
                for(j = 0; j < tam_j; j++)
                {
                	//n_i = (rank+i);
                	//n_j = j;
                    printf("%c ",vector_bloque[i*tam_j+j]);
                    //printf("%c ", universo[n_i][n_j]);
                }
                printf("\n");
            }
            printf("\n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }


	/*bloque = tam_i / size;
	from = rank *  bloque;
	to = ((rank + 1) *  bloque) - 1;


	if(!rank)
	{
		n_universo = (char **) malloc(tam_i * sizeof(char *));
		for(i = 0; i < tam_i; i++)
			n_universo[i] = (char *) malloc(tam_j * sizeof(char));
	}

	tmp = (char **) malloc()

	MPI_Scatter(universo, tam_i * tam_j / size, MPI_CHAR, universo[from], tam_i * tam_j / size, MPI_CHAR, 0, MPI_COMM_WORLD);

	for(i = from; i < to; i++)
	{
		for(j = 0; j < tam_j; j++)
		{
			n_universo[i][j] = evaluar(universo, i, j);
		}
	}

	if(!rank)
	{
		MPI_Gather(n_universo[from], tam_i * tam_j / size, MPI_CHAR, n_universo, tam_i * tam_j / size, MPI_CHAR, 0, MPI_COMM_WORLD);
		game_of_life(n_universo, generaciones - 1);
	}*/
}

int main(int argc, char* argv[])
{
	int rank, size, generaciones, i;
	char** universo = leer_universo();
	//print_universo(universo);

	MPI_Init(&argc, &argv);
	game_of_life(universo, generaciones);
	MPI_Finalize();
	return 0;
}