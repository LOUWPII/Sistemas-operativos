/**************************************************************
* Nombre: Juan Felipe Gómez López
* Fecha: 28/10/2025
* Asignatura: Sistemas Operativos
* Tema: Paralelización con hilos POSIX — condición de carrera
* Descripción:
*   Este programa crea 20 hilos POSIX que calculan en paralelo los
*   cuadrados de números enteros y los acumulan en una variable
*   global. Sin embargo, el acceso no protegido a la variable
*   compartida 'acumulado' genera una condición de carrera que
*   puede producir resultados inconsistentes.
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

int acumulado = 0; // Variable global compartida (presenta condición de carrera)

void *cuadrados(void *x) {
    int xi = (intptr_t)x; // Convierte el argumento void* a entero usando intptr_t
    acumulado += xi * xi; // Actualiza la variable global sin sincronización → condición de carrera
    return NULL; // Retorna null
}

int main(int argc, char *argv[]) {
    pthread_t hilos[20];  //Arreglo para almacenar los identificadores de los 20 hilos

    for (int i = 0; i < 20; i++) { // Crea 20 hilos en paralelo
        pthread_create(&hilos[i], NULL, cuadrados, (void *)(intptr_t)(i + 1)); // Pasa (i+1) como argumento al hilo
    }

    for (int i = 0; i < 20; i++){ // Espera la finalización de todos los hilos
        void *resultado;                            
        pthread_join(hilos[i], &resultado);  // Sincroniza cada hilo antes de continuar
    }

    printf("Suma de cuadrados = %d\n", acumulado); // Muestra el valor acumulado

    return 0;                                       
}
