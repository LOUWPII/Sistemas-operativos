/*#######################################################################################
 #* Fecha: 30 Octubre 2025
 #* Autor: Xamuel Pérez Madrigal, Juan Felipe Gómez López, David Esteban Beltrán
 #* Programa:
 #*      Multiplicación de Matrices algoritmo matriz Transpuesta (Filas x Filas) 
 #* Versión:
 #*      Paralelismo con OpenMP
 #######################################################################################*/

// Inclusión de interfaces esenciales.
#include <stdio.h>    // Para funciones estándar de entrada y salida
#include <stdlib.h>   // Para funciones de propósito general como calloc, exit
#include <string.h>   // Para manipulación de cadenas de caracteres
#include <time.h>     // Para la función time(), usada en srand()
#include <sys/time.h> // Para funciones de medición de tiempo
#include <omp.h>      // La librería OpenMP para el paralelismo en CPU.

// Inclusión de cabeceras de funciones propias
#include "operacionesMatrices.h" // Funciones para inicializar, transponer, e imprimir matrices.
#include "operacionesTiempo.h"   // Funciones para iniciar/finalizar la medición de tiempo.

int main(int argc, char *argv[]) {
    // 1. Verificación de argumentos de entrada
    if(argc != 3){
        // Si no se reciben exactamente 3 argumentos, muestra un mensaje de uso y termina.
        printf("\n Use: $./clasicaOpenMP SIZE Hilos \n\n");
        exit(0);
    }

    // 2. Inicialización de variables y asignación de memoria
    int N = atoi(argv[1]);  // N: Tamaño de la matriz (N x N)
    int TH = atoi(argv[2]); // TH: Número de hilos a usar.

    // Asignación dinámica de memoria para las matrices A, B y C (Resultado).
    double *matrixA = (double *)calloc(N*N, sizeof(double));
    double *matrixB = (double *)calloc(N*N, sizeof(double));
    double *matrixC = (double *)calloc(N*N, sizeof(double));
    
    // 3. Configuración
    // Inicializa la semilla para rand().
    srand(time(NULL));
    // Configura explícitamente el número de hilos que OpenMP debe usar.
    omp_set_num_threads(TH);

    // Inicializa las matrices A y B con valores aleatorios.
    iniMatrix(matrixA, matrixB, N);

    // Muestra las matrices iniciales 
    impMatrix(matrixA, N, 0);
    impMatrix(matrixB, N, 1);

    // Inicia la medición de tiempo (registra el tiempo de inicio).
    InicioMuestra();

    // 4. Multiplicación Paralela con Transposición
    multiMatrixTrans(matrixA, matrixB, matrixC, N);

    // Finaliza la medición de tiempo. Calcula e imprime el tiempo total de ejecución.
    FinMuestra();

    // Imprime la matriz resultante C.
    impMatrix(matrixC, N, 0);

    // 5. Liberación de Recursos
    // Libera la memoria asignada a las matrices.
    free(matrixA);
    free(matrixB);
    free(matrixC);
    
    return 0; // Termina el programa exitosamente.
}
