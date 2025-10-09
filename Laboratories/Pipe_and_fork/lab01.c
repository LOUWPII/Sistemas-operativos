/*
Nombre: Juan Felipe Gómez López
Asignatura: Sistemas Operativos
Tema: Clonación de procesos

*/


//Se incluyen las librerias necesarias para la implementación correcta del código
//unistd.h: Se incluye la cabecera para poder acceder a la API de POSIX y usar funciones como fork()
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main (){
	printf("=================BIENVENIDO A PIPESOFT===================\n\n");
        printf("\n#=>Inicio del proceso main o proceso principal<=#\n\n");
	//Se clona el proceso y la función fork devuelve 1, 0 o -1
        int procesoID = fork();

       /*/Ambos procesos están ejecutando el mismo código, así que si fork devuelve > 0 
	quiere decir que para el padre se pudo crear el hijo correctamente*/
        if(procesoID>0)
		//Se obtiene el ID del proceso padre
                printf("\t ##==>Proceso Padre con ID %d \n", getpid());
        else if(procesoID == 0){
                //Si devuelve cero entonces se puede saber que el hijo se creó correctamente
                printf("\t ##==>Proceso Hijo con ID %d \n", getpid());
	}
        printf(" A partir de aqui es el proceso main o proceso principal\n\n");
        return 0;
}
