#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <pthread.h>

using namespace std;

#define tam 4

typedef struct
{
    int x_i, x_f, y_i, y_f;
} limites;

int** crear_matriz(int _tam)
{
    int i, j;
    int** mat =  new int*[_tam];
    for(i = 0; i < _tam; i++)
    {
        mat[i] = new int[_tam];
        for(j = 0; j < _tam; j++)
            mat[i][j] = 0;
    }
    return mat;
}

int**& leer_matriz(const char* txt)
{
    ifstream leer(txt);
    string linea,aux;
    int** matriz=new int*[tam];
    int i,j,k;
    for(i=0,k=0;i<tam;i++)
        matriz[i]=new int[tam];
    while(!leer.eof())
    {
        getline(leer,linea);
        aux.clear();
        if(!linea.size())
            return matriz;
        for(j=0,i=0;j<linea.size();j++)
        {
            if(linea[j]!=' ')
                aux+=linea[j];
            else
            {
                matriz[k][i++]=atoi(aux.c_str());
                aux.clear();
            }
        }
        matriz[k][i]=atoi(aux.c_str());
        k++;
    }
    return matriz;
}

void llenar_matriz(int** mat)
{
    int i, j;
    for(i = 0; i < tam; i++)
        for(j = 0; j < tam; j++)
            cin>>mat[i][j];
}

void llenar_aleatorio(int** mat, limites l_mat, int a, int b)
{
    int i, j;
    for(i = l_mat.x_i; i < l_mat.x_f; i++)
        for(j = l_mat.y_i; j < l_mat.y_f; j++)
        {
            srand(9);
            mat[i][j] = (a + (a - b) * (rand() / RAND_MAX));
        }

}

void rellenar_matriz(int** mat, limites l_mat, int num)
{
    int i, j;
    for(i = l_mat.x_i; i < l_mat.x_f; i++)
        for(j = l_mat.y_i; j < l_mat.y_f; j++)
            mat[i][j] = num;
}

void print_matriz(int** mat)
{
    int i, j;
    for(i = 0; i < tam; i++)
    {
        for(j = 0; j < tam; j++)
            cout<<mat[i][j]<<" ";
        cout<<endl;
    }
    cout<<endl;
}

void sumar_matriz(int** mat1, int** mat2, int** res, limites l_mat1, limites l_mat2, limites l_res)
{
    int x = l_mat1.x_f - l_mat1.x_i;
    int y = l_mat1.y_f - l_mat1.y_i;
    int i, j;
    for(i = 0; i < x; i++)
        for(j = 0; j < y; j++)
            res[i + l_res.x_i][j + l_res.y_i] = mat1[i + l_mat1.x_i][j + l_mat1.y_i] + mat2[i + l_mat2.x_i][j+ l_mat2.y_i];
}

void restar_matriz(int** mat1, int** mat2, int** res, limites l_mat1, limites l_mat2, limites l_res)
{
    int x = l_mat1.x_f - l_mat1.x_i;
    int y = l_mat1.y_f - l_mat1.y_i;
    int i, j;
    for(i = 0; i < x; i++)
        for(j = 0; j < y; j++)
            res[i + l_res.x_i][j + l_res.y_i] = mat1[i + l_mat1.x_i][j + l_mat1.y_i] - mat2[i + l_mat2.x_i][j+ l_mat2.y_i];
}

void subdividir(limites l_mat, int i, int j, limites* sub_mat)
{
    int n_x = l_mat.x_i + (l_mat.x_f - l_mat.x_i) / 2;
    int n_y = l_mat.y_i + (l_mat.y_f - l_mat.y_i) / 2;
    (*sub_mat) = l_mat;
    if(i == 0)
        sub_mat->x_f = n_x;
    else
        sub_mat->x_i = n_x;
    if(!j)
        sub_mat->y_f = n_y;
    else
        sub_mat->y_i = n_y;
}

void strassen(int** mat1, int** mat2,int** res, limites l_mat1, limites l_mat2, limites l_res)
{
    limites l_sub_mat1[2][2], l_sub_mat2[2][2], l_sub_res[2][2], l_aux;
    int** p_i[7], **aux1, **aux2;
    int i, j;
    ///caso base
    if((l_mat1.y_f - l_mat1.y_i) == 1)
    {
        res[l_res.x_i][l_res.y_i] += mat1[l_mat1.x_i][l_mat1.y_i] * mat2[l_mat2.x_i][l_mat2.y_i];
        return;
    }
    ///subdivido matrices
    for(i = 0; i < 2; i++)
        for(j = 0; j < 2; j++)
        {
            subdividir(l_mat1, i, j, &l_sub_mat1[i][j]);
            subdividir(l_mat2, i, j, &l_sub_mat2[i][j]);
            subdividir(l_res, i, j, &l_sub_res[i][j]);
        }

    l_aux.x_i = l_aux.y_i = 0;
    l_aux.x_f = l_aux.y_f = (l_mat1.y_f - l_mat1.y_i) / 2;

    for(i = 0; i < 8; i++)
        p_i[i] = crear_matriz(l_aux.x_f);
    aux1 = crear_matriz(l_aux.x_f);
    aux2 = crear_matriz(l_aux.x_f);
    l_aux.x_i = 0;
    for(i = 0; i < 7; i++)
        rellenar_matriz(p_i[i], l_aux, 0);

    ///calculo P1, P2, P3 .... P7
    #define reiniciar rellenar_matriz(aux1, l_aux, 0); rellenar_matriz(aux2, l_aux, 0)

    /// P1
    reiniciar;
    ///(a + d)
    sumar_matriz(mat1, mat1, aux1, l_sub_mat1[0][0], l_sub_mat1[1][1], l_aux);
    ///(e + h)
    sumar_matriz(mat2, mat2, aux2, l_sub_mat2[0][0], l_sub_mat2[1][1], l_aux);
    /// (a + d) * (e + h)
    strassen(aux1, aux2, p_i[0], l_aux, l_aux, l_aux);

    /// P2
    reiniciar;
    ///(c + d)
    sumar_matriz(mat1, mat1, aux1, l_sub_mat1[1][0], l_sub_mat1[1][1], l_aux);
    ///(c + d) * e
    strassen(aux1, mat2, p_i[1], l_aux, l_sub_mat2[0][0], l_aux);

    /// P3
    reiniciar;
    ///(f - h)
    restar_matriz(mat2, mat2, aux2, l_sub_mat2[0][1], l_sub_mat2[1][1], l_aux);
    ///a * (f - h)
    strassen(mat1, aux2, p_i[2], l_sub_mat1[0][0], l_aux, l_aux);

    /// P4
    reiniciar;
    ///(g - e)
    restar_matriz(mat2, mat2, aux2, l_sub_mat2[1][0], l_sub_mat2[0][0], l_aux);
    ///d * (g - e)
    strassen(mat1, aux2, p_i[3], l_sub_mat1[1][1], l_aux, l_aux);

    /// P5
    reiniciar;
    ///(a + b)
    sumar_matriz(mat1, mat1, aux1, l_sub_mat1[0][0], l_sub_mat1[0][1], l_aux);
    ///(a + b) * h
    strassen(aux1, mat2, p_i[4], l_aux, l_sub_mat2[1][1], l_aux);

    /// P6
    reiniciar;
    ///(c - a)
    restar_matriz(mat1, mat1, aux1, l_sub_mat1[1][0], l_sub_mat1[0][0], l_aux);
    ///(e + f)
    sumar_matriz(mat2, mat2, aux2, l_sub_mat2[0][0], l_sub_mat2[0][1], l_aux);
    ///(c - a) * (e + f)
    strassen(aux1, aux2, p_i[5], l_aux, l_aux, l_aux);

    ///P7
    reiniciar;
    ///(b - d)
    restar_matriz(mat1, mat1, aux1, l_sub_mat1[0][1], l_sub_mat1[1][1], l_aux);
    ///(g + h)
    sumar_matriz(mat2, mat2, aux2, l_sub_mat2[1][0], l_sub_mat2[1][1], l_aux);
    ///(b - d) * (g + h)
    strassen(aux1, aux2, p_i[6], l_aux, l_aux, l_aux);


    reiniciar;
    ///P0 + P3 - P4 + P6 = C00
    sumar_matriz(p_i[0], p_i[3], aux1, l_aux, l_aux, l_aux);
    restar_matriz(aux1, p_i[4], aux2, l_aux, l_aux, l_aux);
    sumar_matriz(aux2, p_i[6], res, l_aux, l_aux, l_sub_res[0][0]);

    ///P2 + P4 = C01
    sumar_matriz(p_i[2], p_i[4], res, l_aux, l_aux, l_sub_res[0][1]);

    ///P1 + P3 = C10
    sumar_matriz(p_i[1], p_i[3], res, l_aux, l_aux, l_sub_res[1][0]);

    ///P0 + P2 - P1 + P5 = C11
    sumar_matriz(p_i[0], p_i[2], aux1, l_aux, l_aux, l_aux);
    restar_matriz(aux1, p_i[1], aux2, l_aux, l_aux, l_aux);
    sumar_matriz(aux2, p_i[5], res, l_aux, l_aux, l_sub_res[1][1]);
}

typedef struct
{
    int** mat1, **mat2, **res;
    limites l_mat1, l_mat2, l_res;
} Cparams;

void copiar_matriz(int** mat1, int** mat2, limites l_mat)
{
    int i, j;
    for(i = l_mat.x_i; i < l_mat.x_f; i++)
        for(j = l_mat.y_i; j < l_mat.y_f; j++)
            mat[i][j] = num;
}

void p_strassen(void* params)
{
    Cparams* tmp = (Cparams*) params;
    
    int** mat1, **mat2, **res;
    limites l_mat1, l_mat2, l_res;

    mat1 = (*tmp).mat1;
    mat2 = (*tmp).mat2;
    res = (*tmp).res;
    l_mat1 = (*tmp).l_mat1;
    l_mat2 = (*tmp).l_mat2;
    l_res = (*tmp).l_res;

    print_matriz(mat1);
}

int main()
{
    /// desde archivo
    int** mat1 = leer_matriz("m1.txt");
    print_matriz(mat1);
    int** mat2 = leer_matriz("m2.txt");
    print_matriz(mat2);


    /// aleatorio
    /*srand(time(0));
    int** mat1 = crear_matriz(tam);
    llenar_aleatorio(mat1, l_mat1, 50, 100);
    print_matriz(mat1);
    int** mat2 = crear_matriz(tam);
    llenar_aleatorio(mat2, l_mat1, 50, 100);
    print_matriz(mat2);
    */

    int** mat3 = crear_matriz(tam);
    limites l_mat1, l_mat2, l_mat3;
    l_mat1 = l_mat2 = l_mat3 = {0, tam, 0, tam};

    ///strassen normal
    //strassen(mat1, mat2, mat3, l_mat1, l_mat2, l_mat3);
    ///strassen paralelo
    Cparams* params;
    *params = {mat1, mat2, mat3, l_mat1, l_mat2, l_mat3};
    p_strassen((void*) params);
    print_matriz(mat3);
    return 0;
}