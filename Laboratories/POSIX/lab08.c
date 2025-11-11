/**************************************************************
* Nombre: Juan Felipe Gómez López
* Fecha: 28/10/2025
* Asignatura: Sistemas Operativos
* Tema: Hilos con API POSIX — variables de condición y exclusión mutua
* Descripción:
*   Programa que sincroniza dos hilos usando un mutex y una variable
*   de condición. Un hilo asigna un nuevo valor y notifica al otro,
*   mientras el hilo receptor espera la señal para leer el valor
*   actualizado de forma segura.
*************************************************************/
#include <stdio.h>        // Entrada y salida estándar
#include <stdlib.h>       // Funciones generales de utilidad
#include <stdint.h>       // Tipos enteros portables (intptr_t)
#include <pthread.h>      // Biblioteca para manejo de hilos POSIX
#include <stdbool.h>      // tipo de dato booleano

pthread_cond_t var_cond = PTHREAD_COND_INITIALIZER; // Variable de condición inicializada
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex inicializado para coordinar con var_cond

int valor = 100;  // Valor compartido entre hilos
bool notificar = false; // Indica si 'valor' ha sido actualizado

void *reportar(void *nousado) {
    pthread_mutex_lock(&mutex); // Bloquear mutex antes de acceder a variables compartidas
    while (!notificar) { // Esperar hasta que se notifique la actualización
        pthread_cond_wait(&var_cond, &mutex); // Espera la señal y libera temporalmente el mutex
    }
    printf("El valor es: %d\n", valor); // Imprimir el valor actualizado de forma segura
    pthread_mutex_unlock(&mutex); // Desbloquear mutex tras la lectura
    return NULL; // Finalizar hilo 
}

void *asignar(void *nousado) {
    pthread_mutex_lock(&mutex);  // Bloquear mutex antes de modificar variables compartidas
    valor = 20; // Actualizar el valor compartido
    notificar = true; // Indicar que 'valor' ya fue modificado
    pthread_cond_signal(&var_cond); // Enviar señal al hilo en espera
    pthread_mutex_unlock(&mutex); // Liberar mutex para otros hilos
    return NULL;  // Finalizar hilo
}

int main(int argc, char *argv[]) {
    pthread_t reporte, asigne;  // Identificadores de hilos

    pthread_create(&reporte, NULL, reportar, NULL); // Crear hilo que reporta el valor
    pthread_create(&asigne, NULL, asignar, NULL);   // Crear hilo que asigna el valor

    void *nousado;  // Variable auxiliar para pthread_join
    pthread_join(reporte, &nousado);  // Esperar a que termine el hilo de reporte
    pthread_join(asigne, &nousado); // Esperar a que termine el hilo de asignación

    return 0;
}
