#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <getopt.h>

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    char nombre[50] = "";
    char archivo[100] = "";
    char pipeRecibe[100] = "";

    int opt;
    while ((opt = getopt(argc, argv, "s:a:p:")) != -1) {
        switch (opt) {
            case 's': strcpy(nombre, optarg); break;
            case 'a': strcpy(archivo, optarg); break;
            case 'p': strcpy(pipeRecibe, optarg); break;
            default:
                fprintf(stderr, "Uso: %s -s nombre -a archivo -p pipeRecibe\n", argv[0]);
                exit(1);
        }
    }

    if (strlen(nombre) == 0 || strlen(archivo) == 0 || strlen(pipeRecibe) == 0) {
        fprintf(stderr, "[ERROR] Parámetros incompletos.\n");
        exit(1);
    }

    // Crear pipe personal
    char pipe_respuesta[100];
    sprintf(pipe_respuesta, "/tmp/%s_pipe", nombre);
    if (mkfifo(pipe_respuesta, 0666) == -1) {
        perror("[AGENTE] Advertencia (pipe puede existir)");
    }

    // Registro
    int fd_envio = open(pipeRecibe, O_WRONLY);
    if (fd_envio == -1) {
        perror("[AGENTE] Error abriendo pipe principal");
        exit(1);
    }

    char mensaje[BUFFER_SIZE];
    sprintf(mensaje, "REGISTER:%s:%s", nombre, pipe_respuesta);
    write(fd_envio, mensaje, strlen(mensaje));
    close(fd_envio);
    printf("[AGENTE %s] Registrado. Esperando respuesta...\n", nombre);

    // Leer reservas del archivo
    FILE *f = fopen(archivo, "r");
    if (!f) {
        perror("[AGENTE] No se pudo abrir el archivo de reservas");
        unlink(pipe_respuesta);
        exit(1);
    }

    char linea[128];
    while (fgets(linea, sizeof(linea), f)) {
        char grupo[50];
        int hora, personas;

        if (sscanf(linea, "%[^,],%d,%d", grupo, &hora, &personas) == 3) {
            sprintf(mensaje, "RESERVA:%s:%d:%d", grupo, hora, personas);
            int fd_send = open(pipeRecibe, O_WRONLY);
            if (fd_send != -1) {
                write(fd_send, mensaje, strlen(mensaje));
                close(fd_send);
                printf("[AGENTE %s] Enviada: %s\n", nombre, mensaje);
            }
            sleep(1); // pequeña pausa entre envíos
        }
    }

    fclose(f);
    unlink(pipe_respuesta);

	//TEste es temporal, para que el controlador finalice su ejecucion
    int fd_exit = open(pipeRecibe, O_WRONLY);
    if (fd_exit != -1) {
        write(fd_exit, "exit", 4);
        close(fd_exit);
        printf("[AGENTE %s] Enviada señal de salida.\n", nombre);
    }
    printf("[AGENTE %s] Finalizó el envío de reservas.\n", nombre);
    return 0;
}
