/*#######################################################################################
#* Fecha: 30 Octubre 2025
#* Autor: Xamuel Pérez Madrigal, Juan Felipe Gómez López, David Beltrán
#* Programa:
#*      Multiplicación de Matrices algoritmo clásico
#* Versión:
#*      Paralelismo con Hilos Pthreads Posix 
######################################################################################*/


// Inclusión de interfaces esenciales.
#include <stdio.h>    // Para funciones estándar de entrada y salida
#include <pthread.h>  // La interfaz para el manejo de hilos posix
#include <unistd.h>   // Para funciones de sistemas operativos
#include <stdlib.h>   // Para funciones de propósito general como calloc, exit
#include <time.h>     // Para funciones de tiempo
#include <sys/time.h> // Para funciones de medición de tiempo

// Inclusión de cabeceras de funciones propias
#include "operacionesMatrices.h" // Funciones para inicializar e imprimir matrices.
#include "operacionesHilos.h"    // Contiene la función que ejecutan los hilos y la estructura 'parametros'.
#include "operacionesTiempo.h"   // Funciones para iniciar y finalizar la medición de tiempo.

// 1. Variables globales
// Las matrices se declaran como globales para que todos los hilos puedan acceder a ellas directamente
double *matrixA;
double *matrixB;
double *matrixC;
// Mutex para proteger secciones críticas si varios hilos escribieran en el mismo lugar
pthread_mutex_t MM_mutex;

int main(int argc, char *argv[]) {
    // 2. Verificación y lectura de argumentos
    // Verifica que se ingresen exactamente 3 argumentos: ./ejecutable N T
    if (argc != 3) {
        printf("Uso correcto:\n  $ ./ejecutableMM tamMatriz numHilos\n");
        printf("Ejemplo: ./ejecutableMM 4 2\n");
        exit(EXIT_FAILURE);
    }

    int N = atoi(argv[1]);         // N: Tamaño de la matriz (N x N)
    int n_threads = atoi(argv[2]); // n_threads: Número de hilos a crear

    printf("\n--- Inicio Multiplicacion Matrices ---\n");
    printf("Tamaño de matriz: %d x %d\n", N, N);
    printf("Cantidad de hilos: %d\n\n", n_threads);

    // Declaración de un array para almacenar los IDs de los hilos y sus atributos
    pthread_t p[n_threads];
    pthread_attr_t atrMM;

    // 3. Asignación de memoria (variables globales)
    // Asignación dinámica de memoria para las matrices, accesible por todos los hilos.
    matrixA = (double *)calloc(N * N, sizeof(double));
    matrixB = (double *)calloc(N * N, sizeof(double));
    matrixC = (double *)calloc(N * N, sizeof(double));

    // Verificar si hubo errores al asignar memoria
    if (!matrixA || !matrixB || !matrixC) {
        printf("Error al asignar memoria para las matrices.\n");
        exit(EXIT_FAILURE);
    }

    // 4. Inicialización y pre-ejecución
    // Inicializa las matrices A y B con valores aleatorios
    iniMatrix(matrixA, matrixB, N);

    // Muestra las matrices A y B 
    printf("Matriz A generada:\n");
    impMatrix(matrixA, N, 0);
    printf("Matriz B generada:\n");
    impMatrix(matrixB, N, 1);

    // Inicia la medición de tiempo.
    InicioMuestra();

    // 5. Inicialización de pthreads y atributos
    // Inicializa el mutex (bloqueo)
    pthread_mutex_init(&MM_mutex, NULL);
    // Inicializa el objeto de atributos para los hilos
    pthread_attr_init(&atrMM);
    // Configura los hilos para que sean joinable, permitiendo que el hilo principal espere por ellos.
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);

    // Asigna memoria para el array de estructuras que contienen los parámetros de cada hilo.
    struct parametros *datos = malloc(n_threads * sizeof(struct parametros));

    printf("\nCreando hilos...\n");

    // 6. Creación de Hilos y Asignación de Tareas
    for (int j = 0; j < n_threads; j++) {
        // Prepara la estructura 'parametros' para el hilo j
        datos[j].idH = j;          // Identificador del hilo (0 a n_threads-1)
        datos[j].nH = n_threads;   // Total de hilos
        datos[j].N = N;            // Tamaño de la matriz

        // Muestra qué rango de filas procesará este hilo.
        printf(" > Hilo %d creado (procesará filas %d a %d)\n",
            j, (N/n_threads)*j, (N/n_threads)*(j+1)-1);

        // Crea el hilo:
        // &p[j]: Donde se almacena el ID del nuevo hilo.
        // &atrMM: Atributos del hilo.
        // multiplicarMatrix: La función que el hilo comenzará a ejecutar.
        // (void *)&datos[j]: El argumento que se pasa a la función (sus parámetros de trabajo).
        pthread_create(&p[j], &atrMM, multiplicarMatrix, (void *)&datos[j]);
    }

    // 7. Sincronización
    // Espera a que todos los hilos terminen su ejecución.
    for (int j = 0; j < n_threads; j++) {
        pthread_join(p[j], NULL);
    }

    // Finaliza la medición del tiempo total de ejecución.
    FinMuestra();

    // 8. Resultados y Limpieza
    printf("\nMatriz resultante C = A x B:\n");
    impMatrix(matrixC, N, 0);

    // Liberación de memoria de las matrices y la estructura de parámetros.
    free(matrixA);
    free(matrixB);
    free(matrixC);
    free(datos);

    // Destrucción de atributos y mutex de Pthreads
    pthread_attr_destroy(&atrMM);
    pthread_mutex_destroy(&MM_mutex);

    printf("\n--- Finalizacion Exitosa ---\n");

    // Termina el hilo principal.
    pthread_exit(NULL);
    return 0;
}
