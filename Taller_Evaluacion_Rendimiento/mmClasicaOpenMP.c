/*#######################################################################################
 #* Fecha: 30 Octubre 2025
 #* Autor: Xamuel Pérez Madrigal, Juan Felipe Gómez López, David Beltrán
 #* Programa: 
 #* Multiplicación de Matrices algoritmo clásico
 #* Versión:
 #*	 Paralelismo con OpenMP
######################################################################################*/

// Inclusión de interfaes esenciales.
#include <stdio.h>    // Para funciones estándar de entrada y salida
#include <stdlib.h>   // Para funciones de propósito general como calloc, exit
#include <string.h>   // Para manipulación de cadenas de caracteres 
#include <time.h>     // Para la función time(), usada en srand()
#include <sys/time.h> // Para funciones de medición de tiempo
#include <omp.h>      // La interfaz de OpenMP

// Inclusión de cabeceras de funciones propias
#include "operacionesMatrices.h" // Funciones para inicializar e imprimir matrices.
#include "operacionesTiempo.h"   // Funciones para iniciar/finalizar la medición de tiempo.

int main(int argc, char *argv[]) {
    // 1. Verificación de argumentos de entrada
    if (argc != 3) {
        // Si no se reciben exactamente 3 argumentos, muestra un mensaje de uso y termina.
        printf("\n Use: $./clasicaOpenMP SIZE Hilos \n\n");
        exit(0);
    }

    // 2. Inicialización de variables y asignación de memoria
    // N es el tamaño de la matriz (N x N)
    int N = atoi(argv[1]);
    // TH (Thread count) es el número de hilos a usar.
    int TH = atoi(argv[2]);

    // Asignación dinámica de memoria para las matrices A, B y C (Resultado).
    // Se usa calloc para inicializar toda la memoria a cero.
    double *matrixA = (double *)calloc(N * N, sizeof(double));
    double *matrixB = (double *)calloc(N * N, sizeof(double));
    double *matrixC = (double *)calloc(N * N, sizeof(double));

    // Inicializa la semilla para rand() para asegurar valores aleatorios diferentes cada vez.
    srand(time(NULL));

    // 3. Configuración del Paralelismo
    // Configura explícitamente el número de hilos que OpenMP debe usar
	// EL numero de hilos es el que se ingresa en la ejecución
    omp_set_num_threads(TH);

    // Inicializa las matrices A y B con valores aleatorios.
    iniMatrix(matrixA, matrixB, N);

    // Muestra las matrices A y B.
    impMatrix(matrixA, N, 0);
    impMatrix(matrixB, N, 1);

    // Inicia la medición de tiempo 
    InicioMuestra();

    // 4. Multiplicación Paralela
    // Llama a la función que contiene la directiva OpenMP para dividir el trabajo
    // entre los TH hilos configurados en el paso 3.
    // Nota: El paralelismo real ocurre dentro de la función multiMatrix, ya que hay 
    //Se incluyen las directivas #pragma para que el compilador sepa que habrá ejeución paralela
    multiMatrix(matrixA, matrixB, matrixC, N);

    // Finaliza la medición de tiempo. Esta función calcula e imprime el tiempo total.
    FinMuestra();

    // Imprime la matriz resultante C
    impMatrix(matrixC, N, 0);

    // 5. Liberación de Recursos
    // Libera la memoria asignada a las matrices, evitando fugas de memoria.
    free(matrixA);
    free(matrixB);
    free(matrixC);

    return 0; // Termina el programa exitosamente.
}
