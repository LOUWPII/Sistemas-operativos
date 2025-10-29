|/************************************************************** 
 Nombre: Juan Felipe Gómez Lópwz
 Fecha: 28/10/2025
 Asignatura: Sistemas operativos
 Tema: HIlos con API POSIX
 Descripción:
   El programa crea varios hilos POSIX que muestran su número e ID y aumentan un contador compartido protegido por un mutex. 
   Sin embargo, puede presentar condiciones de carrera porque todos los hilos comparten la misma dirección de la variable i, 
   lo que causa lecturas incorrectas de su valor, y porque el programa imprime el valor final del contador antes de que todos los 
   hilos terminen su ejecución.
*************************************************************/

/*Se incluyen las librerías necesarias:
* <stdio.h> : entrada y salida para imprimir en consola
* <stdlib.h> : Creación de void *ptr y char
* <pthread.h> : Creación de hilos
*/

#include <stdio.h>     
#include <stdlib.h> 
#include <pthread.h>    
#include <stddef.h>

#define NTHREADS 10                     // Se define el número de hilos a crear
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger 'counterSe inicializa un mutex para proteger el counter y prevenir condiciones de carrera
int counter = 0;                         // Counter como variable compartida por los hilos

void *thread_function(void *arg) {
    int i = *(int *)(arg);               // Obtener el número de hilo desde el argumento (cast de void* a int*)
    printf("Thread number: %d | Thread ID %ld\n", i, pthread_self()); // Mostrar número e ID del hilo
    pthread_mutex_lock(&mutex1);         // Bloquear el mutex para bloquear la parte del código a otros hilos y solo un hilo modificaá la variable a la vez
    counter++;                           // Incrementar el contador compartido
    pthread_mutex_unlock(&mutex1);       // Desbloquear el mutex para que llegue otro hilo y modifique la variable
}

void main() {
    pthread_t thread_id[NTHREADS];       // Arreglo de tamaño NTHREADS para almacenar los IDs de los hilos
    int i, j;                            // Variable i para recorrer el arreglo y crear un hilo, la variable j no se usa 

    for(i = 0; i < NTHREADS; i++)
    {
        pthread_create(&thread_id[i], NULL, thread_function, &i); // Crear hilo en la posicion i del arreglo pasando la direccion de memoria de i como parámetro
    }

    printf("Final counter value: %d\n", counter); // Mostrar el valor final de 'counter'
}
