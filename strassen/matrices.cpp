#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

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

void print_matriz2(int** mat, limites l_mat)
{
    int i, j;
    for(i = l_mat.x_i; i < l_mat.x_f; i++)
    {
        for(j = l_mat.y_i; j < l_mat.y_f; j++)
            cout<<mat[i][j]<<" ";
        cout<<endl;
    }
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
    if(!i)
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
        res[l_res.x_i][l_res.y_i] += (mat1[l_mat1.x_i][l_mat1.y_i] * mat2[l_mat2.x_i][l_mat2.y_i]);
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

    /// P1
    rellenar_matriz(aux1, l_aux, 0);
    rellenar_matriz(aux2, l_aux, 0);
    ///(a + d)
    sumar_matriz(mat1, mat1, aux1, l_sub_mat1[0][0], l_sub_mat1[1][1], l_aux);
    ///(e + h)
    sumar_matriz(mat2, mat2, aux2, l_sub_mat2[0][0], l_sub_mat2[1][1], l_aux);
    /// (a + d) * (e + h)
    strassen(aux1, aux2, p_i[0], l_aux, l_aux, l_aux);
    cout<<"p1"<<endl;
    print_matriz2(p_i[0], l_aux);

    /// P2
    rellenar_matriz(aux1, l_aux, 0);
    ///(c + d)
    sumar_matriz(mat1, mat1, aux1, l_sub_mat1[1][0], l_sub_mat1[1][1], l_aux);
    ///(c + d) * e
    strassen(aux1, mat2, p_i[1], l_aux, l_sub_mat2[0][0], l_aux);
    cout<<"p2"<<endl;
    print_matriz2(p_i[1], l_aux);

    /// P3
    rellenar_matriz(aux1, l_aux, 0);
    ///(f - h)
    restar_matriz(mat2, mat2, aux1, l_sub_mat2[0][1], l_sub_mat2[1][1], l_aux);
    ///a * (f - h)
    strassen(mat1, aux1, p_i[2], l_sub_mat1[0][0], l_aux, l_aux);
    cout<<"p3"<<endl;
    print_matriz2(p_i[2], l_aux);

    /// P4
    rellenar_matriz(aux1, l_aux, 0);
    ///(g - e)
    restar_matriz(mat2, mat2, aux1, l_sub_mat2[1][0], l_sub_mat2[0][0], l_aux);
    ///d * (g - e)
    strassen(mat1, aux1, p_i[3], l_sub_mat1[1][1], l_aux, l_aux);
    cout<<"p4"<<endl;
    print_matriz2(p_i[3], l_aux);

    /// P5
    rellenar_matriz(aux1, l_aux, 0);
    ///(a + b)
    sumar_matriz(mat1, mat1, aux1, l_sub_mat1[0][0], l_sub_mat1[0][1], l_aux);
    ///(a + b) * h
    strassen(aux1, mat2, p_i[4], l_aux, l_sub_mat2[1][1], l_aux);
    cout<<"p5"<<endl;
    print_matriz2(p_i[4], l_aux);

    /// P6
    rellenar_matriz(aux1, l_aux, 0);
    rellenar_matriz(aux2, l_aux, 0);
    ///(c - a)
    restar_matriz(mat1, mat1, aux1, l_sub_mat1[1][0], l_sub_mat1[0][0], l_aux);
    ///(e + f)
    sumar_matriz(mat2, mat2, aux2, l_sub_mat2[0][0], l_sub_mat2[0][1], l_aux);
    ///(c - a) * (e + f)
    strassen(aux1, aux2, p_i[5], l_aux, l_aux, l_aux);
    cout<<"p6"<<endl;
    print_matriz2(p_i[5], l_aux);

    ///P7
    rellenar_matriz(aux1, l_aux, 0);
    rellenar_matriz(aux2, l_aux, 0);
    ///(b - d)
    restar_matriz(mat1, mat1, aux1, l_sub_mat1[0][1], l_sub_mat1[1][1], l_aux);
    ///(g + h)
    sumar_matriz(mat2, mat2, aux2, l_sub_mat2[1][0], l_sub_mat2[1][1], l_aux);
    ///(b - d) * (g + h)
    strassen(aux1, aux2, p_i[6], l_aux, l_aux, l_aux);
    cout<<"p7"<<endl;
    print_matriz2(p_i[6], l_aux);


    rellenar_matriz(aux1, l_aux, 0);
    rellenar_matriz(aux2, l_aux, 0);
    ///P1 + P4
    sumar_matriz(p_i[0], p_i[3], aux1, l_aux, l_aux, l_aux);
    ///P5 + P7
    sumar_matriz(p_i[4], p_i[6], aux2, l_aux, l_aux, l_aux);
    ///(P1 + P4) - (P5 + P7)
    restar_matriz(aux1, aux2, res, l_aux, l_aux, l_sub_res[0][0]);

    ///P3 + P5
    sumar_matriz(p_i[2], p_i[4], res, l_aux, l_aux, l_sub_res[0][1]);

    ///P2 + P4
    sumar_matriz(p_i[1], p_i[3], res, l_aux, l_aux, l_sub_res[1][0]);

    rellenar_matriz(aux1, l_aux, 0);
    rellenar_matriz(aux2, l_aux, 0);
    ///P1 - P2
    restar_matriz(p_i[0], p_i[1], aux1, l_aux, l_aux, l_aux);
    ///P3 + P4
    sumar_matriz(p_i[2], p_i[3], aux2, l_aux, l_aux, l_aux);
    ///(P1 - P2) + (P3 + P4)
    sumar_matriz(aux1, aux2, res, l_aux, l_aux, l_sub_res[1][1]);
}

int main()
{
    int** mat1 = leer_matriz("m1.txt");
    print_matriz(mat1);
    int** mat2 = leer_matriz("m2.txt");
    print_matriz(mat2);
    int** mat3 = crear_matriz(tam);

    limites l_mat1, l_mat2, l_mat3;
    l_mat1 = l_mat2 = l_mat3 = {0, tam, 0, tam};

    strassen(mat1, mat2, mat3, l_mat1, l_mat2, l_mat3);
    print_matriz(mat3);
    return 0;
}