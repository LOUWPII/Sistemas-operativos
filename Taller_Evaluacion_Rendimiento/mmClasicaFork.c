/*#######################################################################################
#* Fecha: 30 Octubre 2025
#* Autor: Xamuel Pérez Madrigal, Juan Felipe Gómez López, David Beltrán
#* Programa:
#*      Multiplicación de Matrices algoritmo clásico
#* Versión:
#*      Paralelismo con Procesos Fork 
######################################################################################*/

// Inclusión de interfaces esenciales para el programa.
#include <stdio.h>    // Para funciones estándar de entrada y salid
#include <stdlib.h>   // Para funciones de propósito general (calloc, exit)
#include <unistd.h>   // Para funciones de sistemas operativos, como fork()
#include <sys/wait.h> // Para la función wait(), usada para esperar a los procesos hijos
#include <sys/time.h> // Para funciones de medición de tiempo (no usadas directamente aquí, pero incluidas)
#include <time.h>     // Para la función time(), usada en srand()


// Aqui se hace inclusión de cabeceras de funciones propias para modularizar el proyecto
#include "operacionesFork.h"     // Funciones específicas para la multiplicación paralela con fork
#include "operacionesTiempo.h"   // Funciones para iniciar/finalizar la medición de tiempo
#include "operacionesMatrices.h" // Funciones para inicializar e imprimir matrices

int main(int argc, char *argv[]) {
    // 1. Verificación de argumentos de entrada
    if (argc != 3) {
        // Si no se reciben exactamente 3 argumentos (nombre del ejecutable, N, Hilos),
        // muestra un mensaje de uso y termina el programa.
        printf("\n \t\tUse: $./nom_ejecutable Size Hilos \n");
        exit(0);
    }
    
    // 2. Inicialización de variables y asignación de memoria
    // N es el tamaño de la matriz (N x N)
    int N = (int)atoi(argv[1]);
    // num_P es el número de procesos hijos a crear para el paralelismo
    int num_P = (int)atoi(argv[2]);

    // Asignación dinámica de memoria para las matrices A, B y C (Resultado).
    // Se usa calloc para inicializar toda la memoria a cero.
    double *matA = (double *)calloc(N * N, sizeof(double));
    double *matB = (double *)calloc(N * N, sizeof(double));
    double *matC = (double *)calloc(N * N, sizeof(double));

    // Inicializa la semilla para la función rand() usando el tiempo actual
    srand(time(0)); 
    
    // Inicializamos las matrices A y B con valores aleatorios
    iniMatrix(matA, matB, N);
    // Impresión de las matrices A y B si es necesario 
    impMatrix(matA, N, 0); // Imprime matriz A
    impMatrix(matB, N, 1); // Imprime matriz B

    // Calcula cuántas filas se asignarán a cada proceso hijo dividiendo el tamaño N entre el número de procesos a crear
    int rows_per_process = N / num_P;

    // Inicia la medición de tiempo. Esta función registra el tiempo de inicio.
    InicioMuestra();

    // 3. Creación y asignación de trabajo a los procesos hijos
    for (int i = 0; i < num_P; i++) {
        // La llamada a fork() duplica el proceso actual
        // El valor de 'pid' será 0 en el proceso hijo, y el PID del hijo en el padre.
        pid_t pid = fork();
        
        if (pid == 0) { 
            // AQUI SE ESTPA EN EL PROCESO HIJO
            
            // Define el rango de filas que este proceso hijo calculará.
            int start_row = i * rows_per_process;
            // Determina la fila final. El último proceso toma las filas restantes.
            int end_row = (i == num_P - 1) ? N : start_row + rows_per_process;
            
            // Llama a la función que realiza la multiplicación de matrices
            // El hijo solo calcula el rango de filas que le fue asignado.
            multiplicarMatrix(matA, matB, matC, N, start_row, end_row); 
            
            // Bloque de código para imprimir la porción de la matriz C calculada por el hijo solo si N < 9
            if (N < 9) {
                printf("\nChild PID %d calculated rows %d to %d:\n", getpid(), start_row, end_row - 1);
                for (int r = start_row; r < end_row; r++) {
                    for (int c = 0; c < N; c++) {
                        // El proceso hijo lee y escribe directamente en la memoria compartida (matC)
                        printf(" %.2f ", matC[N * r + c]);
                    }
                    printf("\n");
                }
            }
            // El proceso hijo termina exitosamente
            exit(0); 
        } else if (pid < 0) {
            // Manejo de error si fork() falla
            perror("fork failed");
            exit(1);
        }
    }
    
    // 4. Sincronización que unicamente realiza el padre
    for (int i = 0; i < num_P; i++) {
        // El proceso padre espera a que cada uno de sus procesos hijos termine.
        // Esto garantiza que toda la multiplicación de matrices esté completa antes de medir el tiempo final.
        wait(NULL); 
    }
    
    // Finaliza la medición de tiempo. Imprime el tiempo total transcurrido.
    FinMuestra(); 

    // 5. Liberación de Recursos
    // Libera la memoria asignada a las matrices.
    free(matA);
    free(matB);
    free(matC);

    return 0;
}
