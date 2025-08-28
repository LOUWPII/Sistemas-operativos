/******************************************************** BIENVENIDO A PIPESOFT***********************************************************
Nombre: Juan Felipe Gómez López 
Asignatura: Sistemas Operativos 
Desafio: challenge02 

*/

#include <string.h>//--> Librería para usar operaciones de cadenas
#include <stdio.h>// --> Librería para entrada y salida
#include <stdlib.h>// --> LIbrería para uso de memoria dinámica

int main() {
   char *q = NULL; //--> Declaracion de apuntador e inicializacion en NULL
   printf("Requesting space for \"Goodbye\" \n");
   q = (char *)malloc(strlen("Goodbye")+1);//--> Reserva de memoria de tipo char con el tamaño de la cadena a alacenar (devuelve el puntero)
   if (!q) {
     perror("Failed to allocate space because");//--> Si q está apuntando en NULL se imprime el mensaje seguido de la descripcion que lo causó
     exit(1);
   }

   printf("About to copy \"Goodbye\" to q at address %p \n",(void*)q); //--> Se imprime la direccion de memoria en donde se copiará la cadena
   strcpy(q, "Goodbye"); //--> Se copia la cadena en el bloque de memoria reservado
   printf("String copied\n");
   printf("%s \n", q);//--> Se imprime la cadena copiada en q.
   return 0;
}

