#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define MAIN_PIPE "/tmp/parque_pipe"
#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: ./agente <nombre_agente>\n");
        return 1;
    }

    char *nombre = argv[1];
    char pipe_respuesta[100];

    // Construir ruta del pipe personal: /tmp/<nombre>_pipe
    strcpy(pipe_respuesta, "/tmp/");
    strcat(pipe_respuesta, nombre);
    strcat(pipe_respuesta, "_pipe");

    // Crear pipe personal
    if (mkfifo(pipe_respuesta, 0666) == -1) {
        perror("[AGENTE] Advertencia (pipe puede existir)");
    }

    // Enviar registro inicial al controlador
    int fd_envio = open(MAIN_PIPE, O_WRONLY);
    if (fd_envio == -1) {
        perror("[AGENTE] Error abriendo pipe principal");
        exit(1);
    }

    char mensaje[BUFFER_SIZE];
    strcpy(mensaje, "REGISTER:");
    strcat(mensaje, nombre);
    strcat(mensaje, ":");
    strcat(mensaje, pipe_respuesta);

    write(fd_envio, mensaje, strlen(mensaje));
    close(fd_envio);
    printf("[AGENTE %s] Registrado. Esperando respuesta...\n", nombre);

    // Leer respuesta del controlador
    int fd_lectura = open(pipe_respuesta, O_RDONLY);
    if (fd_lectura == -1) {
        perror("[AGENTE] Error abriendo su pipe personal");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    int n = read(fd_lectura, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
        printf("[AGENTE %s] Respuesta recibida: %s\n", nombre, buffer);
    }
    close(fd_lectura);
    unlink(pipe_respuesta); // eliminar su pipe personal

    // --- LECTURA DE CSV DE RESERVAS ---
    FILE *f = fopen("reservas.csv", "r");
    if (f == NULL) {
        perror("[AGENTE] No se pudo abrir reservas.csv");
        return 1;
    }

    char linea[128];
    fgets(linea, sizeof(linea), f); // saltar encabezado

    while (fgets(linea, sizeof(linea), f)) {
        char nombre_visita[50];
        int hora, personas;

        if (sscanf(linea, "%[^,],%d,%d", nombre_visita, &hora, &personas) == 3) {
            // Crear mensaje: RESERVA:<nombre>:<hora>:<personas>
            char msg[BUFFER_SIZE];
            char temp[16];

            strcpy(msg, "RESERVA:");
            strcat(msg, nombre_visita);
            strcat(msg, ":");

            sprintf(temp, "%d", hora);
            strcat(msg, temp);
            strcat(msg, ":");

            sprintf(temp, "%d", personas);
            strcat(msg, temp);

            // Enviar al controlador
            int fd_env = open(MAIN_PIPE, O_WRONLY);
            if (fd_env != -1) {
                write(fd_env, msg, strlen(msg));
                close(fd_env);
                printf("[AGENTE %s] Enviada reserva: %s\n", nombre, msg);
            } else {
                perror("[AGENTE] No se pudo abrir pipe principal para enviar reserva");
            }
        }
    }

    fclose(f);
    printf("[AGENTE %s] Finalizó el envío de reservas.\n", nombre);
    return 0;
}
