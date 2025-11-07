/*******************************************************************************************
* Autor: Juan Felipe Gómez López
* Programa: lab05_sumatoria_paralela.c
* Fecha: 6 de noviembre 2025
* Tema: Programación Paralela - Introducción OpenMP y Medición de Tiempo
* ---------------------------------------------------------------------------------------
* Resumen
* Este programa calcula la sumatoria de la función 'funcion(i) que se encarga de calcular la funcion
* seno de forma paralela, utilizando OpenMP. Recibe como parámetros el número de hilos (T) y 
* el número de repeticiones (N).
* Utiliza la cláusula 'reduction(+: sumatoria)' para garantizar la exactitud del resultado
* y emplea 'gettimeofday' para medir el tiempo total de ejecución.
**********************************************************************************************/

// ----------------------------------------------------------------------------------
// 1. INCLUSIÓN DE INTERFACES

#include <omp.h>      // Directivas de OpenMP para el paralelismo.
#include <stdio.h>    // Operaciones de entrada y salida.
#include <stdlib.h>   // Funciones de utilidad como atoi, exit.
#include <time.h>     // Para funciones de tiempo
#include <sys/time.h> // Define el struct timeval, esencial para la medición de tiempo.
#include <math.h>     // Para el uso de la función seno.


// Propósito: Define el cálculo por iteración, que consiste en retornar sin(i).
// Es el trabajo que se distribuye entre los hilos.
double funcion(int i){

    int inicio = i * (i + 1) / 2;
    int final = inicio + 1;
    double valor = 0;
    
    // Acumula el valor de sin(i) dados los límites
    for(int j = inicio; j < final; j++)
        valor += sin(i);
    
    return valor;
}

// Variables globales para guardar los puntos de tiempo de inicio y fin.
struct timeval inicio, fin;

//Registra el tiempo de inicio de la medición utilizando gettimeofday.
void InicioMuestra(){
    gettimeofday(&inicio, NULL);
}


// Esta función registra el tiempo final, calcula y retorna el tiempo transcurrido en segundos.
double FinMuestra(){
    gettimeofday(&fin, NULL);
    // Calcula la diferencia en segundos y microsegundos.
    long sec  = fin.tv_sec  - inicio.tv_sec;
    long usec = fin.tv_usec - inicio.tv_usec;
    // Retorna el tiempo total como un valor double en segundos.
    return (double)sec + (double)usec / 1e6;
}


int main(int argc, char *argv[]) {
    double sumatoria = 0; // Variable acumuladora inicializada a cero.
    int i;
    double tiempo_total;

    // Se esperan 3 argumentos: ./ejecutable numHilos numRepeticiones
    if (argc != 3) {
        printf("Error: Faltan parámetros.\n");
        printf("\tUso correcto: $ %s <numHilos> <numRepeticiones>\n", argv[0]);
        printf("\tEjemplo: $ %s 4 300000\n", argv[0]);
        exit(1);
    }

    // Cantidad de hilos a usar para el paralelismo.
    int numHilos = atoi(argv[1]);
    // Cantidad de veces que se ejecutará el ciclo for.
    int n = atoi(argv[2]); 
    
    // Configura OpenMP para usar el número de hilos especificado.
    omp_set_num_threads(numHilos);
    
    InicioMuestra(); // Se inicia medición del tiempo

	//La directiva de openMP para que el compilador sepa que habrá un bloque paralelo
    #pragma omp parallel
    {
        // Bloque ejecutado por el Hilo Maestro para imprimir la configuración.
        #pragma omp master
        printf("\n--- Ejecución de Sumatoria ---\n");
        #pragma omp master
        printf("Hilos: %d | Repeticiones (N): %d\n", omp_get_num_threads(), n);

        // Distribuye el ciclo entre los hilos para realizar la sumatoria de forma concurrente.
        // reduction(+: sumatoria): Evita la condición de carrera y consolida el resultado final.
        #pragma omp for reduction(+ : sumatoria)
        for (i = 0; i < n; i++) {
            sumatoria += funcion(i);
        }
    } // Aquí finaliza la region paralela y los hilos se sincronizan

    tiempo_total = FinMuestra(); //Finaliza medición de tiempo

    printf("\n--- Resultados ---\n");
    printf("La sumatoria total es: %.6f\n", sumatoria);
    printf("El tiempo total de ejecución fue: %.6f segundos.\n", tiempo_total);
    printf("------------------\n");

    return 0;
}
