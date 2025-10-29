/**************************************************************
* Nombre: Juan Felipe Gómez López
* Fecha: 28/10/2025
* Asignatura: Sistemas Operativos
* Tema: Sincronización de hilos con mutex lock/unlock
* Descripción:
*   Este programa crea tres hilos que simulan tareas concurrentes.
*   Cada hilo incrementa un contador compartido protegido con un mutex
*   para evitar condiciones de carrera, e imprime mensajes indicando
*   el inicio y fin de su ejecución.
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

pthread_t tid[3];    // Identificadores de los tres hilos
int counter = 0;                                      // Contador compartido entre los hilos
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex que protege el acceso al contador

void* compute(void *arg) {
    unsigned long i = 0;    // Variable para la iteración
    int idjoblocal;                                   // Almacena el ID local del trabajo

    pthread_mutex_lock(&counter_mutex);  // Bloquea el mutex para modificar el contador
    counter += 1;  // Incrementa el contador compartido
    idjoblocal = counter;  // Copia el valor actual del contador
    pthread_mutex_unlock(&counter_mutex); // Libera el mutex para permitir acceso a otros hilos

    printf("\nJob %d has started\n", idjoblocal);  // Muestra el inicio del trabajo del hilo

    for (i = 0; i < (0xFFFFFFF); i++);  // Bucle que simula una tarea pesada

    printf("\nJob %d has finished\n", idjoblocal); // Muestra la finalización del trabajo

    return NULL; // Retorna NULL al terminar el hilo
}

int main(void) {
    int i = 0; // Índice para crear los hilos
    int error; // Código de error para verificar creación de hilos

    while (i < 3) {  // Bucle para crear los tres hilos
        error = pthread_create(&(tid[i]), NULL, &compute, NULL); // Crea un hilo que ejecuta compute()
        if (error != 0)
            printf("\nThread can't be created: [%s]", strerror(error)); // Muestra error si no se crea correctamente
        i++;
    }

    pthread_join(tid[0], NULL); // Espera a que finalice el hilo 0
    pthread_join(tid[1], NULL); // Espera a que finalice el hilo 1
    pthread_join(tid[2], NULL); // Espera a que finalice el hilo 2

    return 0;                                     
}
