/**************************************************************
* Nombre: Juan Felipe Gómez López
* Fecha: 28/10/2025
* Asignatura: Sistemas Operativos
* Tema: Hilos con API POSIX — variables de condición y mutex
* Descripción:
*   Programa que crea dos hilos: uno imprime un valor compartido
*   y otro lo asigna. La sincronización con mutex y variables
*   de condición está comentada, lo que provoca una condición de
*   carrera (el hilo puede leer el valor antes de que sea cambiado).
*************************************************************/

#include <stdio.h> // Entrada/salida estándar
#include <stdlib.h>  // Funciones estándar
#include <stdint.h>  // Tipos enteros de tamaño fijo
#include <pthread.h> // Librería de hilos POSIX
#include <stdbool.h> // Tipo booleano (true/false)

pthread_cond_t var_cond = PTHREAD_COND_INITIALIZER;  // Variable de condición inicializada
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   // Mutex para coordinar con la condición

int valor = 100; // Variable compartida entre los hilos
bool notificar = false; // Bandera para indicar actualización de 'valor'

void *reportar(void *nousado) {  // Hilo que reporta el valor
/*
    pthread_mutex_lock(&mutex);  // BLOQUEO: esperar señal
    while (!notificar) {
        pthread_cond_wait(&var_cond, &mutex);  // Esperar a que 'asignar' cambie el valor
    }
*/
    printf("El valor es: %d\n", valor); // CONDICIÓN DE CARRERA: puede imprimir valor viejo
/*
    pthread_mutex_unlock(&mutex);  // DESBLOQUEO: liberar el mutex
*/
    return NULL;                                     // Retornar NULL
}

void *asignar(void *nousado) { // Hilo que asigna el nuevo valor
    valor = 20; // Actualiza la variable compartida
/*
    pthread_mutex_lock(&mutex); // BLOQUEO: proteger acceso a 'notificar'
    notificar = true; // Indicar que el valor ha sido actualizado
    pthread_cond_signal(&var_cond); // Señalar al hilo que puede continuar
    pthread_mutex_unlock(&mutex); // DESBLOQUEO: liberar el mutex
*/
    return NULL;                                     // Retornar NULL 
}

int main(int argc, char *argv[]) {                   // Función principal
    pthread_t reporte, asigne;                       // Identificadores de hilos

    pthread_create(&reporte, NULL, reportar, NULL);  // Crear hilo que reporta el valor
    pthread_create(&asigne, NULL, asignar, NULL);    // Crear hilo que asigna el nuevo valor

    void *nousado;                                   // Variable usada para recibir retorno de pthread_join
    pthread_join(reporte, &nousado);                 // Esperar finalización del hilo 'reporte'
    pthread_join(asigne, &nousado);                  // Esperar finalización del hilo 'asigne'

    return 0;                                        // Finalizar el programa correctamente
}
