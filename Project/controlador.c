/**************************************************************
* Nombre: Juan Felipe Gómez López, Xamuel Perez, David Beltrán
* Fecha: 12/11/2025
* Asignatura: Sistemas Operativos
* Tema: Concurrencia con hilos POSIX — Controlador de Reservas
* Descripción:
*   Controlador concurrente del sistema de reservas. Cada solicitud
*   de reserva se procesa en un hilo POSIX independiente. Las estructuras
*   globales (aforo y lista de reservas) están protegidas por un mutex.
*   La sección crítica se mantiene mínima para evitar bloqueos innecesarios.
*************************************************************/

#include <stdio.h>       
#include <stdlib.h>      
#include <unistd.h>      
#include <fcntl.h>       
#include <sys/stat.h>    
#include <string.h>      
#include <pthread.h>     
#include <getopt.h>      

#define BUFFER_SIZE 256
#define HORAS 24
#define MAX_RESERVAS 100

// ------------------------------------------------------------
// Estructura que representa una reserva almacenada
// ------------------------------------------------------------
typedef struct {
    char nombre[50];
    int hora_llegada;
    int cantidad_personas;
} Reserva;

// ------------------------------------------------------------
// Variables compartidas entre hilos (protegidas por mutex)
// ------------------------------------------------------------
Reserva reservas[MAX_RESERVAS];
int total_reservas = 0;
int aforo[HORAS] = {0};
int CAPACIDAD_MAX = 0;

pthread_mutex_t mutex_aforo = PTHREAD_MUTEX_INITIALIZER;

// ------------------------------------------------------------
// Argumentos pasados al hilo
// ------------------------------------------------------------
typedef struct {
    char buffer[BUFFER_SIZE];
} HiloArgs;

// ------------------------------------------------------------
// Verifica si la hora está en el rango permitido 7–19
// ------------------------------------------------------------
int hora_valida(int h) {
    return (h >= 7 && h <= 19);
}

// ------------------------------------------------------------
// Función ejecutada por cada hilo POSIX
// ------------------------------------------------------------
void *procesar_reserva(void *arg) {
    HiloArgs *args = (HiloArgs *)arg;
    char buffer[BUFFER_SIZE];
    strcpy(buffer, args->buffer);
    free(args); // liberar memoria asignada al hilo

    // Solo procesar reservas
    if (strncmp(buffer, "RESERVA:", 8) == 0) {

        char nombre[50];
        int hora, personas;

        // Formato: RESERVA:<nombre>:<hora>:<personas>
        if (sscanf(buffer + 8, "%[^:]:%d:%d", nombre, &hora, &personas) == 3) {

            if (!hora_valida(hora)) {
                printf("[HILO] Reserva rechazada (%s): hora no válida\n", nombre);
                pthread_exit(NULL);
            }

            int aceptada = 0; // resultado que se imprimirá fuera del mutex

            // --------------------------------------------------------
            // SECCIÓN CRÍTICA MÍNIMA — solo acceso a datos globales
            // --------------------------------------------------------
            pthread_mutex_lock(&mutex_aforo);

            if (aforo[hora] + personas <= CAPACIDAD_MAX) {
                aforo[hora] += personas;

                strcpy(reservas[total_reservas].nombre, nombre);
                reservas[total_reservas].hora_llegada = hora;
                reservas[total_reservas].cantidad_personas = personas;
                total_reservas++;

                aceptada = 1;
            }

            pthread_mutex_unlock(&mutex_aforo);
            // --------------------------------------------------------
            // FIN SECCIÓN CRÍTICA — impresión fuera del mutex
            // --------------------------------------------------------

            if (aceptada)
                printf("[HILO] Reserva ACEPTADA: %s - %d personas a las %d:00\n",
                       nombre, personas, hora);
            else
                printf("[HILO] Reserva RECHAZADA: %s (Aforo lleno a las %d:00)\n",
                       nombre, hora);
        }
    }

    pthread_exit(NULL);
}

// ------------------------------------------------------------
// FUNCIÓN PRINCIPAL DEL CONTROLADOR
// ------------------------------------------------------------
int main(int argc, char *argv[]) {

    int horaIni = 0, horaFin = 0, segHoras = 0;
    char pipeRecibe[100] = "";

    // ------------------------------------------------------------
    // Lectura de parámetros con getopt()
    // ------------------------------------------------------------
    int opt;
    while ((opt = getopt(argc, argv, "i:f:s:t:p:")) != -1) {
        switch (opt) {
            case 'i': horaIni = atoi(optarg); break;
            case 'f': horaFin = atoi(optarg); break;
            case 's': segHoras = atoi(optarg); break;
            case 't': CAPACIDAD_MAX = atoi(optarg); break;
            case 'p': strcpy(pipeRecibe, optarg); break;
        }
    }

    // Validaciones básicas
    if (!hora_valida(horaIni) || !hora_valida(horaFin) || horaIni >= horaFin) {
        fprintf(stderr, "[ERROR] Horas inválidas. Deben estar entre 7 y 19 y horaIni < horaFin.\n");
        exit(1);
    }
    if (segHoras <= 0 || CAPACIDAD_MAX <= 0) {
        fprintf(stderr, "[ERROR] segHoras o aforo máximo inválidos.\n");
        exit(1);
    }

    // Crear el pipe principal
    if (mkfifo(pipeRecibe, 0666) == -1) {
        perror("[CONTROLADOR] Advertencia: el pipe ya existe");
    }

    printf("\n[CONTROLADOR] Simulación de %d a %d.\n", horaIni, horaFin);
    printf("[CONTROLADOR] Aforo máximo por hora: %d.\n", CAPACIDAD_MAX);
    printf("[CONTROLADOR] Escuchando en: %s\n\n", pipeRecibe);

    // Abrir pipe principal
    int fd_main = open(pipeRecibe, O_RDONLY);
    if (fd_main == -1) {
        perror("[CONTROLADOR] Error abriendo pipe principal");
        exit(1);
    }

    char buffer[BUFFER_SIZE];

    // ------------------------------------------------------------
    // Bucle principal
    // ------------------------------------------------------------
    while (1) {
        int n = read(fd_main, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';

            if (strcmp(buffer, "exit") == 0) {
                printf("[CONTROLADOR] Comando exit recibido. Finalizando...\n");
                break;
            }

            // Crear argumentos dinámicos para el hilo
            HiloArgs *args = malloc(sizeof(HiloArgs));
            strcpy(args->buffer, buffer);

            // Crear hilo
            pthread_t tid;
            if (pthread_create(&tid, NULL, procesar_reserva, args) != 0)
                fprintf(stderr, "[ERROR] No se pudo crear hilo.\n");

            pthread_detach(tid); // El hilo se autolibera
        }
    }

    // Limpieza
    close(fd_main);
    unlink(pipeRecibe);
    pthread_mutex_destroy(&mutex_aforo);

    printf("[CONTROLADOR] Finalizado correctamente.\n");
    return 0;
}
