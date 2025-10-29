/**************************************************************
* Nombre: Juan Felipe Gómez López
* Fecha: 28/10/2025
* Asignatura: Sistemas Operativos
* Tema: Sincronización de hilos con mutex y variables de condición POSIX
* Descripción:
*   Este programa demuestra cómo dos hilos pueden coordinar su ejecución
*   utilizando un mutex y una variable de condición. Un hilo se detiene 
*   cuando el contador compartido está dentro de un rango definido, 
*   mientras el otro hilo continúa e informa cuando debe reanudarse, 
*   garantizando exclusión mutua y control ordenado de la ejecución.
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t count_mutex      = PTHREAD_MUTEX_INITIALIZER;   // Protege el acceso al contador compartido
pthread_mutex_t condition_mutex  = PTHREAD_MUTEX_INITIALIZER;   // Sincroniza el uso de la variable de condición
pthread_cond_t  condition_cond   = PTHREAD_COND_INITIALIZER;    // Variable de condición para suspender o reanudar hilos

int count = 0;  // Contador compartido entre hilos

#define COUNT_DONE  15   // Valor máximo del contador que finaliza la ejecución
#define COUNT_HALT1  4   // Límite inferior del rango donde el hilo 1 se pausa
#define COUNT_HALT2 11   // Límite superior del rango donde el hilo 1 se pausa

void *count01() {
    for(;;) {  // Bucle continuo hasta que se alcance COUNT_DONE
        pthread_mutex_lock(&condition_mutex);  // Bloquea el mutex antes de verificar la condición
        while (count >= COUNT_HALT1 && count <= COUNT_HALT2)   // Si el contador está dentro del rango de pausa
            pthread_cond_wait(&condition_cond, &condition_mutex); // El hilo espera una señal para continuar
        pthread_mutex_unlock(&condition_mutex);                // Libera el mutex de condición

        pthread_mutex_lock(&count_mutex);  // Bloquea el acceso al contador
        count++;  // Incrementa el contador compartido
        printf("Hilo 1 -> contador = %d\n", count);  // Muestra el valor actual del contador
        pthread_mutex_unlock(&count_mutex);  // Libera el mutex del contador

        if (count >= COUNT_DONE) return NULL; // Termina cuando se alcanza el límite máximo
    }
}

void *count02() {
    for(;;) {  // Bucle continuo hasta alcanzar COUNT_DONE
        pthread_mutex_lock(&condition_mutex);  // Bloquea el mutex de condición
        if (count < COUNT_HALT1 || count > COUNT_HALT2) // Si el contador está fuera del rango de pausa
            pthread_cond_signal(&condition_cond); // Despierta al hilo en espera
        pthread_mutex_unlock(&condition_mutex); // Libera el mutex de condición

        pthread_mutex_lock(&count_mutex); // Bloquea el acceso al contador
        count++; // Incrementa el contador compartido
        printf("Hilo 2 -> contador = %d\n", count);  // Muestra el valor actual del contador
        pthread_mutex_unlock(&count_mutex); // Libera el mutex del contador

        if (count >= COUNT_DONE) return NULL; // Termina cuando se alcanza el valor máximo
    }
}

int main(void) {
    pthread_t thread1, thread2; // Identificadores de los dos hilos

    pthread_create(&thread1, NULL, &count01, NULL); // Crea el primer hilo
    pthread_create(&thread2, NULL, &count02, NULL); // Crea el segundo hilo

    pthread_join(thread1, NULL);  // Espera a que termine el hilo 1
    pthread_join(thread2, NULL);  // Espera a que termine el hilo 2

    return 0;                                                 
}
