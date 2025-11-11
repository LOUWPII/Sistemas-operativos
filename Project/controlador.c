#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define MAIN_PIPE "/tmp/parque_pipe"
#define BUFFER_SIZE 256
#define MAX_RESERVAS 100

// --- ESTRUCTURA DE DATOS ---
typedef struct {
    char nombre[50];
    int hora_llegada;
    int cantidad_personas;
} Reserva;

Reserva reservas[MAX_RESERVAS];
int total_reservas = 0;


// --- PROGRAMA PRINCIPAL ---

int main() {
    int fd_main;
    char buffer[BUFFER_SIZE];

    // Crear el pipe principal si no existe
    if (mkfifo(MAIN_PIPE, 0666) == -1) {
        perror("[CONTROLADOR] Advertencia (pipe puede existir)");
    }

    printf("[CONTROLADOR] Esperando registros de agentes o reservas...\n");

    // Abrir pipe principal
    fd_main = open(MAIN_PIPE, O_RDONLY);
    if (fd_main == -1) {
        perror("[CONTROLADOR] Error abriendo pipe principal");
        exit(1);
    }

    // Bucle principal
    while (1) {
        int n = read(fd_main, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';

            printf("[CONTROLADOR] Recibido: %s\n", buffer);

            // --- REGISTRO DE AGENTE ---
            if (strncmp(buffer, "REGISTER:", 9) == 0) {
                char nombre[50] = "";
                char pipe_agente[100] = "";
                int i = 9, j = 0;

                while (buffer[i] != ':' && buffer[i] != '\0') {
                    nombre[j++] = buffer[i++];
                }
                nombre[j] = '\0';

                if (buffer[i] == ':') i++;
                int k = 0;
                while (buffer[i] != '\0') {
                    pipe_agente[k++] = buffer[i++];
                }
                pipe_agente[k] = '\0';

                printf("[CONTROLADOR] Agente registrado: %s\n", nombre);
                printf("[CONTROLADOR] Pipe respuesta: %s\n", pipe_agente);

                int fd_resp = open(pipe_agente, O_WRONLY);
                if (fd_resp != -1) {
                    char respuesta[] = "HORA_ACTUAL=7";
                    write(fd_resp, respuesta, strlen(respuesta));
                    close(fd_resp);
                    printf("[CONTROLADOR] Respuesta enviada a %s\n", nombre);
                } else {
                    perror("[CONTROLADOR] Error abriendo pipe del agente");
                }
            }

            // --- RESERVA ---
            else if (strncmp(buffer, "RESERVA:", 8) == 0) {
                char nombre[50];
                int hora, personas;

                // Parsear formato: RESERVA:<nombre>:<hora>:<personas>
                if (sscanf(buffer + 8, "%[^:]:%d:%d", nombre, &hora, &personas) == 3) {
                    strcpy(reservas[total_reservas].nombre, nombre);
                    reservas[total_reservas].hora_llegada = hora;
                    reservas[total_reservas].cantidad_personas = personas;
                    total_reservas++;

                    printf("[CONTROLADOR] Nueva reserva: %s - %d personas a las %d:00\n",
                           nombre, personas, hora);
                } else {
                    printf("[CONTROLADOR] Error: formato de reserva inválido.\n");
                }
            }

            // --- SALIDA ---
            else if (strcmp(buffer, "exit") == 0) {
                printf("[CONTROLADOR] Comando de salida recibido. Cerrando...\n");
                break;
            }

            // --- MENSAJE DESCONOCIDO ---
            else {
                printf("[CONTROLADOR] Mensaje no reconocido.\n");
            }
        }
    }

    close(fd_main);
    unlink(MAIN_PIPE);
    printf("[CONTROLADOR] Servidor finalizado correctamente.\n");
    return 0;
}
