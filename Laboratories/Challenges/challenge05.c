/******************************************************** BIENVENIDO A PIPESOFT***********************************************************
Nombre: Juan Felipe Gómez López 
Asignatura: Sistemas Operativos 
Desafio: challenge05 

*/

#include <stdio.h>
#include <stdlib.h>

int main() {

   int *ptr, i , n1, n2; /*--> Se declara el puntero (ptr), i para las iteraciones, n1 para poder recorrer el primer bloque de memoria, 
  n2 para recorrer el segundo bloque de memoria */

   printf("Enter size: "); 
   scanf("%d", &n1); // --> Se recibe el tamaño de enteros.
   ptr = (int*) malloc(n1 * sizeof(int)); // --> reserva de espacio de memoria de acuerdo al numero de enteros a almacenar.
   printf("Addresses of previously allocated memory: ");
   for(i = 0; i < n1; ++i) //--> Se recorrerá cada espacio de memoria y se imprimirá la direccion de memoria
      printf("\n\np = %p\n", ptr+i); //--> IMpresión de la dirección de memoria del espacio de memoria actual

   printf("\n Enter the new size: ");
   scanf("%d", &n2);
   // rellocating the memory
   ptr = realloc(ptr, n2 * sizeof(int));//--> Se realiza un realloc para volver a reservar un nuevo número de espacios de memria para enteros
   printf("Addresses of newly allocated memory: ");
   for(i = 0; i < n2; ++i)//--> Se vuelve a recorrer cada espacio de memoria y se imprime la dirección de memoria
      printf("\n\np = %p\n", ptr+i); //--> Impresión del espacio de memoria
   free(ptr);//--> Liberacion de memoria

}
