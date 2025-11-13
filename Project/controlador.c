#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <getopt.h>

#define BUFFER_SIZE 256
#define HORAS 24

typedef struct {
    char nombre[50];
    int hora_llegada;
    int cantidad_personas;
} Reserva;

Reserva reservas[100];
int total_reservas = 0;
int aforo[HORAS] = {0};

// --- Validar rango de hora (7 a 19) ---
int hora_valida(int h) {
    return (h >= 7 && h <= 19);
}

int main(int argc, char *argv[]) {
    int horaIni = 0, horaFin = 0, segHoras = 0, total = 0;
    char pipeRecibe[100] = "";

    int opt;
    while ((opt = getopt(argc, argv, "i:f:s:t:p:")) != -1) {
        switch (opt) {
            case 'i': horaIni = atoi(optarg); break;
            case 'f': horaFin = atoi(optarg); break;
            case 's': segHoras = atoi(optarg); break;
            case 't': total = atoi(optarg); break;
            case 'p': strcpy(pipeRecibe, optarg); break;
            default:
                fprintf(stderr, "Uso: %s -i horaIni -f horaFin -s segHoras -t total -p pipeRecibe\n", argv[0]);
                exit(1);
        }
    }

    // Validaciones
    if (!hora_valida(horaIni) || !hora_valida(horaFin) || horaIni >= horaFin) {
        fprintf(stderr, "[ERROR] Horas inválidas. Rango válido: 7-19\n");
        exit(1);
    }
    if (segHoras <= 0 || total <= 0 || strlen(pipeRecibe) == 0) {
        fprintf(stderr, "[ERROR] Parámetros inválidos. Revise segHoras, total o pipeRecibe.\n");
        exit(1);
    }

    // Crear pipe principal
    if (mkfifo(pipeRecibe, 0666) == -1) {
        perror("[CONTROLADOR] Advertencia (pipe puede existir)");
    }

    printf("[CONTROLADOR] Simulación desde %d hasta %d horas.\n", horaIni, horaFin);
    printf("[CONTROLADOR] 1 hora de simulación = %d segundos reales.\n", segHoras);
    printf("[CONTROLADOR] Aforo máximo: %d personas/hora\n", total);
    printf("[CONTROLADOR] Escuchando en: %s\n", pipeRecibe);

    int fd_main = open(pipeRecibe, O_RDONLY);
    if (fd_main == -1) {
        perror("[CONTROLADOR] Error abriendo pipe principal");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    while (1) {
        int n = read(fd_main, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            printf("[CONTROLADOR] Recibido: %s\n", buffer);

            // --- Mensaje de reserva ---
            if (strncmp(buffer, "RESERVA:", 8) == 0) {
                char nombre[50];
                int hora, personas;

                if (sscanf(buffer + 8, "%[^:]:%d:%d", nombre, &hora, &personas) == 3) {
                    if (!hora_valida(hora)) {
                        printf("[CONTROLADOR] Rechazada reserva '%s' (hora fuera de rango)\n", nombre);
                        continue;
                    }

                    if (aforo[hora] + personas <= total) {
                        aforo[hora] += personas;
                        strcpy(reservas[total_reservas].nombre, nombre);
                        reservas[total_reservas].hora_llegada = hora;
                        reservas[total_reservas].cantidad_personas = personas;
                        total_reservas++;

                        printf("[CONTROLADOR] ✅ Reserva ACEPTADA: %s - %d personas a las %d:00\n",
                               nombre, personas, hora);
                    } else {
                        printf("[CONTROLADOR] ❌ Reserva RECHAZADA: %s (aforo lleno a las %d:00)\n",
                               nombre, hora);
                    }
                }
            }

            // --- Salida ---
            else if (strcmp(buffer, "exit") == 0) {
                printf("[CONTROLADOR] Comando de salida recibido.\n");
                break;
            }
        }
    }

    close(fd_main);
    unlink(pipeRecibe);
    printf("[CONTROLADOR] Finalizado correctamente.\n");
    return 0;
}
