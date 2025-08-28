/******************************************************** BIENVENIDO A PIPESOFT***********************************************************
Nombre: Juan Felipe Gómez López 
Asignatura: Sistemas Operativos 
Desafio: challenge03 

*/

#include <stdlib.h>
#include <stdio.h>


int main() {
  int *ptr;//--> Creacion del apuntador de tipo entero
  ptr = malloc(15 * sizeof(*ptr)); /* a block of 15 integers, retorna un puntero a un bloque de memoria de tamaño 15 enteros */ 
  if (ptr != NULL) {// --> Si ptr no es NULL...
    *(ptr + 5) = 480; /* assign 480 to sixth integer */
    printf("The value of the sixth integer is %d \n", *(ptr+5));// --> Se imprime el contenido de ptr en la posicion ptr + 5
}
return 0;
}

