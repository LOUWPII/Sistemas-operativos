/*
*****************************BIENVENIDO A PIPESOFT**********************************
Nombre: Juan Felipe Gómez López
Actividad: Teoría del rendimiento
Asignatura: Sistemas Operativos
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "modulo.h"

/* 
 * En este archivo se definen las variables globales.
 * Esto evita los errores de multiple definition.
 * Las funciones en modulo.c las ven porque modulo.h las declara con extern.
 */
double *mA, *mB, *mC;

struct timeval ini, fin;


int main(int argc, char *argv[]) {

    /*
     * Verificación básica de argumentos:
     * El programa espera al menos 1 argumento además del nombre del ejecutable,
     * por ejemplo: ./modulo_GomezLopez 3
     */

    if(argc<=2){
        printf("Numero argumentos incorrectos\n");
        printf("\n\t $ejecutable.exe DIM \n");
        return -1;
    }

    //Se le hace un cast al valor que el usuario ingresa para definir el tamaño de la matriz
    int N = (int) atof(argv[1]);
    if(N<=0){
        printf("\n Valores deben ser mayor que cero\n");
        return -1;
    };

    /* 
     * Reserva dinámica de memoria:
     * mA y mB apuntan al bloque de memoria reservado por malloc, mC apunta a un bloque de memoria con calloc 
     * Se guarda N*N doubles, por eso N*N * sizeof(double).
     */

    mA = (double *) malloc(N*N*sizeof(double)); 
    mB = (double *) malloc(N*N*sizeof(double)); 
    mC = (double *) calloc(N*N,sizeof(double)); 

        //Se inicia la  matriz a  y b
        iniMatriz(N, mA, mB);
	//Se imprimen las matrice a y b antes de multiplicar
        imprMatrices(N, mA);
        imprMatrices(N, mB);

     /*
     *  Para cada fila i de A y cada columna j de B se calcula la suma de productos A[i,*] * B[*,j]
     *  Se usan los punteros pA y pB para recorrer la fila de A y la columna de B.
     *  pA = mA + i*N apunta al inicio de la fila i.
     *  pB = mB + j apunta al elemento (0,j) de B; al hacer pB += N se avanza a la siguiente fila,
     *  manteniendo la columna j fija, lo que implementa acceso por columnas en la matriz plana.
     */

     for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            double sumaTemp, *pA, *pB;
            sumaTemp = 0.0;
            pA = mA + i*N;
            pB = mB + j;
            for(int k=0; k<N; k++, pA++, pB+=N){
                sumaTemp += *pA * *pB;
            }
            mC[j+i*N] = sumaTemp;
        }
    }
	//Se imprime la matriz 
        imprMatrices(N, mC);

        printf("\n\tFin del programa.............!\n");
        /* Se libera memoria, devolverle los recursos al sistema*/
        free(mA);
        free(mB);
        free(mC);
        return 0;

}
