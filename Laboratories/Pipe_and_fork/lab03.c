/*
Nombre: Juan Felipe Gómez López
Asignatura: Sistemas Operativos
Tema: Pipes - comunicación entre procesos relacionados
*/

// Incluye cabeceras necesarias:
// unistd.h: Cabecera para poder utilizar el API de POSIX  fork, pipe, read, write, close.
// string.h: strlen.
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

	printf("===============================BIENVENIDO A PIPESOFT================================\n\n");
        printf("=== Inicio del proceso principal ===\n");
        int pipefd[2]; // Descriptor de archivo para la tubería: [0] = lectura, [1] = escritura.
        pid_t pHIJO_PID; // Variable para almacenar el PID del hijo.

        //Primero se debe crear el pipe antes de hacer fork, ya que es necesario que ambos procesos compartan el mismo pipe
        if(pipe(pipefd) == -1){
                perror("ERROR al crear el pipe");
                exit(EXIT_FAILURE);
        }

        //Se crea el proceso hijo.
        pHIJO_PID = fork();
        
        // Verificación de error al crear el proceso hijo.
        if(pHIJO_PID == -1){
                perror("ERROR al ejecutar fork");
                exit(EXIT_FAILURE);
        }

        // === Bloque del Proceso hijo el cual es el lector ===
        if(pHIJO_PID == 0){
                
                // Cierra el extremo de escritura, el hijo solo lee.
                close(pipefd[1]);
                
                char mensaje[100]; // Se reserva espacio de memoria para recibir el mensaje.
                int lectBytes;     // Número de bytes leídos.
                
                // Lee del extremo de lectura del pipe.
                lectBytes = read(pipefd[0], mensaje, sizeof(mensaje));
                
		//Si no hubo lectura de bytes entonces
                if(lectBytes == -1){
                        perror("ERROR en lectura del hijo");
                        exit(EXIT_FAILURE);
                }
                
                // Se muestra el mensaje recibido.
                printf("\t#=== Proceso HIJO: recibe mensaje del PADRE %.*s ##\n", lectBytes, mensaje);
                
                // Cierra el extremo de lectura.
                close(pipefd[0]);
        }
        
        // --- Bloque del Proceso PADRE (Escritor) ---
        else { 
                
                // Cierra el extremo de lectura, el padre solo escribe.
                close(pipefd[0]);
                
                char mensajeP[]="Hola soy tu PADRE LUKE!!!"; // Mensaje a enviar.
                int escrBytes; // Número de bytes escritos.
                
                // Escribe el mensaje en el pipe.
                escrBytes = write(pipefd[1], mensajeP, strlen(mensajeP));
                
                if(escrBytes == -1){
                        perror("ERROR en escritura del padre");
                        exit(EXIT_FAILURE);
                }
                
                // Cierra el extremo de escritura.
                close(pipefd[1]);
        }

        // Ambos procesos ejecutan esta línea.
        printf("FIN...........\n");

        return 0;
}
