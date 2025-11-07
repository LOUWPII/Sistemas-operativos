/*******************************************************************************************
* Autor: Juan Felipe Gómez López
* Programa: lab02.c
* Fecha: 6 de noviembre 2025
* Tema: Programación Paralela - Introducción OpenMP
* ---------------------------------------------------------------------------------------
* Resumen
* El programa crea un equipo de hilos que se encarga de sumar y modificar la variable b compartida
* y demuestra que al existir condiciones de carrera lleva a un resultado impredecible incorecto. 
**********************************************************************************************/


#include <omp.h>   // Header necesario para invocar la API OpenMP
#include <stdio.h> //Interfaz para operaciones de entrada y salida
#include <stdlib.h>

int main(){
    // 1. Declaración e Inicialización de Variables
    int i;
    const int N = 1000; // Límite superior del bucle
    int a = 50;         // Variable inicializada: 50
    int b = 0;          // Variable inicializada: 0

    // 2. Directiva de Paralelismo y Bucles
    // #pragma omp parallel for: Crea el equipo de hilos y distribuye las iteraciones del bucle 'for'
    // entre ellos automáticamente.

    // private(i): i es el contador del bucle y debe ser privado para cada hilo.
    // private(a): Cada hilo recibe su propia copia local de 'a'
    // La variable 'b' es de tipo 'shared'
    #pragma omp parallel for private(i) private(a)
    for (i = 0; i < N; i++){
        // Ocurre una race condition:
        // Múltiples hilos intentan leer y escribir en la variable compartida 'b'
        // al mismo tiempo, llevando a un resultado incorrecto e impredecible.
        b = a + i;
    }

    // 3. Impresión de Resultados
    // Este código es ejecutado solo por el hilo maestro después de que todos los hilos han terminado.
    printf("a = %d b = %d (Se espera a=50 b=1049)\n", a, b);
    
    return 0;
}
