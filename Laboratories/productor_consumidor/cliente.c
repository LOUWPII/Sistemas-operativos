/***************************************************
*Tema: Named pipes y fork
*Fecha: 14/10/2023
*Materia: Sistemas operativos
*
***************************************************/

/*
stdio.h: Funciones de entrada y salida printf, fgets

stdlib.h: Funciones generales exit, EXIT_FAILURE

string.h: Manipulación de strings strcpy, strcmp, strlen

unistd.h: Funciones del sistema Unix read, write, close

fcntl.h: Control de archivos open, O_RDWR

sys/stat.h: Operaciones con estados de archivos
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

//Se define la ruta del archivo FIFO que usaran los procesos para comunicarse
#define FIFO_FILE "/tmp/fifo_twoway"

int main() {
    //Descriptor del archivo FIFO
    int fd;
    //flag para saber si terminar
    int end_process;
    //Longitud del string mensaje
    int stringlen;
    //Vriable para saber si se leyeron los datos
    int read_bytes;
    //Bytes leidos del FIFO
    char readbuf[80];
    //String de terminación
    char end_str[5];
    
    //Descripción del cliente para indicar que envía mensajes
    printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n");
    
    // Abrir el FIFO (debe existir, creado por el servidor)
    //Se crea una variable para almacenar lo que devuelve la función open(FIFO_FILE, O_RDWR) que abre el FIFO en lectura y escritura
    fd = open(FIFO_FILE, O_RDWR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    //Copia el string end en l variable end_str
    strcpy(end_str, "end");
    
    //Bucle que se repite hasta que el usuario escriba end
    while (1) {
        //Se le pide al usuario ingresae un string
        printf("Enter string: ");
        if (fgets(readbuf, sizeof(readbuf), stdin) == NULL) {
            perror("fgets");
            break;
        }
        //Calcula la longitud del string leido
        stringlen = strlen(readbuf);
	//Verifica si el último carácter es salto de línea
        if (stringlen > 0 && readbuf[stringlen - 1] == '\n'){
	    // Reemplaza \n por \0
            readbuf[stringlen - 1] = '\0';
        }
        
        //Se almacena en la variable end_process el resultado del strcmp con el readbuf y el end_str
        end_process = strcmp(readbuf, end_str);
        //SI no son iguales
        if (end_process != 0) {
            // Enviar mensaje al servidor
	    //Envía el descriptor del FIFO, los datos a enviar y  la cantidad de bytes a escribit
            if (write(fd, readbuf, strlen(readbuf)) == -1) {
                perror("write");
                break;
            }
	    //Confirmacion de que se envío el mensaje
            printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", 
                   readbuf, (int)strlen(readbuf));
            
            // Recibir respuesta del servidor. Para confirmar se crea una variable que permite saber si se recibieron Bytes o no
            read_bytes = read(fd, readbuf, sizeof(readbuf) - 1);
            if (read_bytes == -1) {
                perror("read");
                break;
            }
	    //Coloca el \0 para  terminar el string recibido
            readbuf[read_bytes] = '\0';
            printf("FIFOCLIENT: Received string: \"%s\" and length is %d\n", 
                   readbuf, (int)strlen(readbuf)); //Se parsea a int la longitud del readbuf
        } else {
            // Enviar mensaje
            if (write(fd, readbuf, strlen(readbuf)) == -1) {
                perror("write");
            }
            printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", 
                   readbuf, (int)strlen(readbuf));
            close(fd);
            break;
        }
    }
    return 0;
}
