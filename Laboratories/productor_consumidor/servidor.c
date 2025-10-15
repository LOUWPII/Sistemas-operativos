#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define FIFO_FILE "/tmp/fifo_twoway"

void reverse_string(char *);

int main() {
    int fd;
    char readbuf[80];
    char end[10];
    int to_end;
    int read_bytes;
    
    // Crear el FIFO si no existe, 0640 representa un númer octal para manejar los permisos de archivo
    if (mkfifo(FIFO_FILE, 0640) == -1) {
        // Si ya existe, no es necesariamente un error
        if (errno != EEXIST) {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }
    
    //Se copia ek¿l string end en la variable end
    strcpy(end, "end");
    
    // Abrir el FIFO
    fd = open(FIFO_FILE, O_RDWR); //Se abre el FIFO modo read y write
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    //Un bucle mientras que  la variable to_end no sea igual a end
    while(1) {
	//Se crea la variable para corroborar que si se leyó un mensaje
        read_bytes = read(fd, readbuf, sizeof(readbuf) - 1);
        if (read_bytes == -1) {
            perror("read");
            break;
        }
	//Se pone \0 al final
        readbuf[read_bytes] = '\0';
        //Mensaje de confirmación para mostrar que se recibió el texto
        printf("FIFOSERVER: Received string: \"%s\" and length is %d\n", 
               readbuf, (int)strlen(readbuf));
        //Se asigna a la variable to_end el resltado del strcmp 
        to_end = strcmp(readbuf, end);
        //Si igual a cero - son iguales
        if (to_end == 0) {
            close(fd);
            // Opcional: eliminar el FIFO
            unlink(FIFO_FILE);
            break;
        }
        //Se revierte el mensaje con la función auxiliar
        reverse_string(readbuf);
        printf("FIFOSERVER: Sending Reversed String: \"%s\" and length is %d\n", 
               readbuf, (int)strlen(readbuf));
        //Se escribe y se manda el parámetro del descriptor, el mensaje y el tamaño del mensaje
        if (write(fd, readbuf, strlen(readbuf)) == -1) {
            perror("write");
            break;
        }
        
        sleep(1); // Reducido a 1 segundo para mejor respuesta
    }
    return 0;
}

//FUncipn auxiliar para revertir el mensaje
void reverse_string(char *str) {
    int last, limit, first;
    char temp;
    last = strlen(str) - 1;
    limit = last/2;
    first = 0;
    
    while (first < last) {
        temp = str[first];
        str[first] = str[last];
        str[last] = temp;
        first++;
        last--;
    }
}
