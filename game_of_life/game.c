#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define tam 10
#define tam_i tam
#define tam_j tam

typedef struct My_pair
{
	int pos_i;
	int pos_j;
} My_pair;

MPI_Datatype createRecType()
{
    // Set-up the arguments for the type constructor
    MPI_Datatype new_type;

    int count = 2;
    int blocklens[] = { 1,1 };

    MPI_Aint indices[2];
    indices[0] = (MPI_Aint)offsetof(struct My_pair, pos_i);
    indices[1] = (MPI_Aint)offsetof(struct My_pair, pos_j);

    MPI_Datatype old_types[] = {MPI_INT,MPI_INT};

    MPI_Type_struct(count,blocklens,indices,old_types,&new_type);
    // Create a resized type
    MPI_Datatype resized_new_type;
    MPI_Type_create_resized(new_type, indices[0], (MPI_Aint)sizeof(struct My_pair), &resized_new_type);
    MPI_Type_commit(&resized_new_type);
    // Free new_type as it is no longer needed
    MPI_Type_free(&new_type);

    return resized_new_type;
}

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
			universo[i][j] = buffer[j];
		i++;
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
	/*
	printf("para %d,%d\n", x, y);
	printf("%d,%d:%c %d,%d:%c %d,%d:%c\n", x_neg, y_neg, universo[x_neg][y_neg], x_neg, y, universo[x_neg][y], x_neg, y_pos, universo[x_neg][x_pos]);
	printf("%d,%d:%c %d,%d:%c %d,%d:%c\n", x, y_neg, universo[x][y_neg], x, y, universo[x][y], x, y_pos, universo[x][y_pos]);
	printf("%d,%d:%c %d,%d:%c %d,%d:%c\n", x_pos, y_neg, universo[x_pos][y_neg], x_pos, y, universo[x_pos][y], x_pos, y_pos, universo[x_pos][y_pos]);
	*/
	contador += (universo[x_neg][y_neg] == '1') ? 1 : 0;
	contador += (universo[x_neg][y] == '1') ? 1 : 0;
	contador += (universo[x_neg][y_pos] == '1') ? 1 : 0;
	contador += (universo[x][y_neg] == '1') ? 1 : 0;
	contador += (universo[x][y_pos] == '1') ? 1 : 0;
	contador += (universo[x_pos][y_neg] == '1') ? 1 : 0;
	contador += (universo[x_pos][y] == '1') ? 1 : 0;
	contador += (universo[x_pos][y_pos] == '1') ? 1 : 0;
	//printf("%d\n", contador);
	if(contador < 2 || contador > 3)
	{
		//printf("MUERE\n");
		return '0';
	}
	if(contador == 2 || contador == 3)
	{
		//printf("VIVE\n");
		return '1';
	}
}

void game_of_life(char** universo, int generaciones)
{
	if(generaciones > 0)
	{
		printf("Universo en generacion: %d\n", generaciones);
		print_universo(universo);
	}
	else
		return;
	int i, j, k;
	int rank, size;
	int bloque, n_i, n_j;
	char** n_universo;
	n_universo = (char **) malloc(tam_i * sizeof(char *));
	for(i = 0; i < tam_i; i++)
			n_universo[i] = (char *) malloc(tam_j * sizeof(char));

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	bloque = tam_i / size;
	My_pair vector_universo[tam_i * tam_j];
	My_pair vector_bloque[bloque * tam_j];

	if(!rank)
	{
		k = 0;
		for(i = 0; i < tam_i; i++)
		{
			for(j = 0; j < tam_j; j++)
			{
				My_pair t;
				t.pos_i = i;
				t.pos_j = j;
				vector_universo[k++] = t;
			}
		}
	}

	MPI_Datatype MPI_My_pair = createRecType();
	MPI_Scatter(vector_universo, bloque * tam_j, MPI_My_pair, vector_bloque, bloque * tam_j, MPI_My_pair, 0, MPI_COMM_WORLD);

	for(k = 0; k < size; k++)
    {
        if(k == rank)
        {
            for(i = 0; i < bloque; i++)
            {
                for(j = 0; j < tam_j; j++)
                {
					//printf("%d,%d:%c ",vector_bloque[i*tam_j+j].pos_i, vector_bloque[i*tam_j+j].pos_j, universo[vector_bloque[i*tam_j+j].pos_i][vector_bloque[i*tam_j+j].pos_j]);
                	n_universo[vector_bloque[i*tam_j+j].pos_i][vector_bloque[i*tam_j+j].pos_j] = evaluar(universo, vector_bloque[i*tam_j+j].pos_i, vector_bloque[i*tam_j+j].pos_j);
                	//printf("%c ", evaluar(universo, vector_bloque[i*tam_j+j].pos_i, vector_bloque[i*tam_j+j].pos_j));
                }
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
	if(!rank)
	{
		printf("Nuevo universo\n");
		print_universo(n_universo);
		game_of_life(n_universo, generaciones--);
	}

}

int main(int argc, char* argv[])
{
	int rank, size, generaciones, i;
	char** universo = leer_universo();

	MPI_Init(&argc, &argv);
	game_of_life(universo, 3);
	MPI_Finalize();
	return 0;
}