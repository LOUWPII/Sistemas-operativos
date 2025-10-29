/*
Nombre: Juan Felipe Gómez López
Fecha: 28/10/2025
Asignatura: Sistemas operativos
Tema: POSIX - creación y terminación de hilos

Descripción:

Este programa crea dos hilos de ejecución que se ejecutan concurrentemente, 
donde cada hilo imprime un mensaje diferente ("Thread 1" o "Thread 2") 
utilizando la biblioteca pthreads de POSIX.
*/



/*Se incluyen las librerías necesarias:
* <stdio.h> : entrada y salida para imprimir en consola
* <stdlib.h> : Creación de void *ptr y char
* <pthread.h> : Creación de hilos 
*/
#include <stdio.h>   
#include <stdlib.h>  
#include <pthread.h>  

void *print_message_function( void *ptr ) {
    char *message;                   // Variable para guardar el puntero a la cadena de caracteres
    message = (char *) ptr;          // Convertir de void* el cual es un puntero genérico a char* para realizar la impresión correcta
    printf("%s \n", message);        // Se imprime el mensaje que le corresponde al hilo
}

void main() {
    pthread_t thread1, thread2;      // Se crean las variables para los IDs de los hilos
    char *message1 = "Thread 1";     // mensaje del hilo 1
    char *message2 = "Thread 2";     // Mensaje del hilo 2
    int iret1, iret2;                // Variables para guardar el retorno de la funcion pthread_create

    /* Create independent threads each of which will execute function */
    /* Parámetros: &threadx - donde se guarda el ID del hilo
     * NULL - para atributos por defecto, en este caso se crea un hilo normal sin detalles avanzados
     * print_message_function: función que ejecutará el hilo
     * (void*) message1: argumento para la función como un puntero por defecto. 
    */
    iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1); 
    iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2); // Crear hilo 2


    printf("Thread 1 returns: %d\n", iret1); // Mostrar lo que devolvió la función del hilo 1
    printf("Thread 2 returns: %d\n", iret2); // Mostrar lo que devolvió la función del hilo 2
    exit(0);                                 
}
