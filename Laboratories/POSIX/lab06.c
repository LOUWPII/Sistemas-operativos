/**************************************************************
* Nombre: Juan Felipe Gómez López
* Fecha: 28/10/2025
* Asignatura: Sistemas Operativos
* Tema: Paralelización con hilos POSIX — suma de cuadrados con mutex
* Descripción:
*   Programa que crea 20 hilos POSIX; cada hilo calcula el cuadrado
*   de un número y lo acumula en una variable global protegida por
*   un mutex para evitar condiciones de carrera.
*************************************************************/

#include <stdio.h>        // Entrada/salida estándar
#include <stdlib.h>       // Utilidades estándar
#include <stdint.h>       // intptr_t para conversión segura void* <-> entero
#include <pthread.h>      // API POSIX threads

int acumulado = 0;  // Acumulador global para la suma de cuadrados
pthread_mutex_t acumuladoMutex = PTHREAD_MUTEX_INITIALIZER; // Mutex que protege 'acumulado'

void *cuadrados(void *x) {  // Función ejecutada por cada hilo
    int xi = (intptr_t)x;  // Conversión del argumento a entero
    pthread_mutex_lock(&acumuladoMutex); // BLOQUEO: entrar en sección crítica
    acumulado += xi * xi; // ACCESO PROTEGIDO: actualizar acumulador
    pthread_mutex_unlock(&acumuladoMutex); // DESBLOQUEO: salir de sección crítica
    return NULL;     // Retornar NULL 
}

int main(int argc, char *argv[]) { // Función principal
    pthread_t hilos[20]; // Arreglo para los identificadores de 20 hilos

    for (int i = 0; i < 20; i++) {  // Crear 20 hilos
        if (pthread_create(&hilos[i], NULL, cuadrados, (void *)(intptr_t)(i + 1)) != 0) // Crear hilo con argumento (i+1)
            fprintf(stderr, "Error: no se pudo crear el hilo %d\n", i + 1); // Reporte de falla en creación
    }

    for (int i = 0; i < 20; i++) {  // Esperar la finalización de todos los hilos
        void *resultado;                          
        pthread_join(hilos[i], &resultado); // Unir hilo i al hilo principal
    }

    printf("Suma de cuadrados = %d\n", acumulado); // Imprimir resultado final pero ahora seguro por mutex y join

    return 0;                                     
}
