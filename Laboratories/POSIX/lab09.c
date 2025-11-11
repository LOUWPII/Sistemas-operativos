/*********************************************************************************************
 * Nombre: Juan Felipe Gómez López
 * Fecha: 28/10/2025
 * Tema: Posix para la creación de hilos concurrentes
 *
 * Descripción:
 *   Lectura de un vector desde fichero y búsqueda paralela del máximo
 *   usando hilos POSIX. Cada hilo procesa un subintervalo y devuelve
 *   su máximo parcial, el hilo principal combina los parciales.
 *********************************************************************************************/

#include <stdio.h>     // std I/O para in y out 
#include <stdlib.h>    // malloc, free, atoi, exit
#include <pthread.h>   // POSIX threads

typedef struct {   // estructura de argumentos para cada hilo
    int inicio;  // índice inicial del subarray que procesa el hilo
    int fin; // índice final del subarray que procesa el hilo
    int *vector; // puntero al array completo leído desde fichero
    int maxparcial; // lugar donde el hilo guarda su máximo local
} param_H;

void *buscarMax(void *parametro) {
    param_H *argumentos = (param_H *)parametro;  // cast de void* a param_H*
    argumentos->maxparcial = argumentos->vector[argumentos->inicio]; // inicializa máximo parcial
    for (int i = argumentos->inicio + 1; i < argumentos->fin; i++)  // recorrer solo el subintervalo asignado
        if (argumentos->vector[i] > argumentos->maxparcial)   // actualizar máximo parcial si procede
            argumentos->maxparcial = argumentos->vector[i]; // asignar nuevo máximo parcial
    pthread_exit(0); // terminar hilo
    return NULL;                                                  
}

/* Nota: la función maximoValor debe implementar la creación de hilos,
   particionar el vector en subintervalos, arrancar los hilos con
   argumentos param_H, recoger los máximos parciales y devolver el máximo final. */

int main(int argc, char *argv[]) {
    FILE *fichero;  // descriptor del fichero de entrada
    int n, nhilos, i, ret; // variables auxiliares
    int *vec; // vector dinámico que contendrá los datos leídos
    int maximo; // máximo final

    if (argc != 3) { // validar argumentos de línea de comando
        fprintf(stderr, "Error: número de argumentos incorrecto.\n"); // mensaje de uso
        fprintf(stderr, "Uso: %s <fichero> <número_de_hilos>\n", argv[0]); // mostrar uso correcto
        exit(EXIT_FAILURE); // terminar con error
    }

    fichero = fopen(argv[1], "r"); // abrir fichero de entrada
    if (fichero == NULL) { // comprobar apertura correcta
        perror("Error al abrir fichero"); // imprimir error de sistema
        exit(EXIT_FAILURE); // terminar con error
    }

    ret = fscanf(fichero, "%d", &n); // leer cantidad de elementos
    if (ret != 1) {  // validar lectura
        fprintf(stderr, "Error al leer tamaño del vector.\n"); // mensaje de error
        fclose(fichero); // cerrar fichero
        exit(EXIT_FAILURE);  // terminar con error
    }

    nhilos = atoi(argv[2]); // convertir número de hilos
    if (nhilos <= 0) {  // validar número de hilos
        fprintf(stderr, "Error: número de hilos debe ser > 0.\n"); // mensaje de validación
        fclose(fichero);  // cerrar fichero
        exit(EXIT_FAILURE); // terminar con error
    }

    vec = malloc(sizeof(int) * n);  // reservar memoria para el vector
    if (vec == NULL) {  // comprobar asignación
        fprintf(stderr, "Error: no se pudo asignar memoria.\n"); // mensaje de error
        fclose(fichero);  // cerrar fichero
        exit(EXIT_FAILURE); // terminar con error
    }

    for (i = 0; i < n; i++) { // leer los n elementos del fichero
        ret = fscanf(fichero, "%d", &vec[i]); // leer elemento i
        if (ret != 1) { // validar lectura individual
            fprintf(stderr, "Error al leer elemento %d.\n", i); // mensaje de error con índice
            free(vec); // liberar memoria
            fclose(fichero);   // cerrar fichero
            exit(EXIT_FAILURE); // terminar con error
        }
    }

    /* Llamar a la función que crea hilos y calcula el máximo usando el vector */
    maximo = maximoValor(vec, n, nhilos); // maximoValor no está implementada en este fichero

    printf("Máximo: %d\n", maximo); // imprimir resultado final

    free(vec); // liberar memoria del vector
    fclose(fichero); // cerrar fichero
    return 0;                                             
}
