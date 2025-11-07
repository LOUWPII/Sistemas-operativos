/*******************************************************************************************
* Autor: Juan Felipe Gómez López
* Programa: lab01.c
* Fecha: 6 de noviembre 2025
* Tema: Programación Paralela - Introducción OpenMP
* ---------------------------------------------------------------------------------------
* Resumen
* Programa crea hilos de ejecución para cada core
* Imprime cadena de caracteres por cada llamada.
**********************************************************************************************/

#include <omp.h>   // <-- Header necesario para invocar API OpenMP
#include <stdio.h> // Interfaz para operaciones de entrada y salida
#include <stdlib.h> 

int main(int argc, char *argv[]){
    // 1. Mensaje de Configuración Inicial (Ejecutado por UN SOLO Hilo)
    int max_hilos = omp_get_max_threads();
    printf("OpenMP ejecutando con %d hilos\n", max_hilos);

    // 2. Directiva de Paralelismo
    // #pragma omp parallel: Esta es la directiva clave. Le indica al compilador
    // que debe crear un equipo de hilos.
    // El número de hilos por default es igual al número de núcleos de la CPU.
    #pragma omp parallel
    {
        // 3. Código Paralelo
        // omp_get_thread_num(): Función de OpenMP que devuelve el ID único.
        // del hilo que está ejecutando esta línea de código.
        int hilo_actual = omp_get_thread_num();
        printf("Hello World desde el thread %d\n", hilo_actual);
        
        // } // Fin del bloque paralelo. Al llegar aquí, los hilos se sincronizan.
    } 

    return 0;
}
