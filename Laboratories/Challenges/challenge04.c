/******************************************************** BIENVENIDO A PIPESOFT***********************************************************
Nombre: Juan Felipe Gómez López 
Asignatura: Sistemas Operativos 
Desafio: challenge04 

*/

#include <stdlib.h>
#include <stdio.h>

int main() {
   int n, i, *ptr, sum = 0;//--> Declaracion de n(numero de elementos), i(para las iteraciones del bucle), sum(suma de los numeros que digite el usuario
   printf("Enter number of elements: ");
   scanf("%d", &n); //--> scan recibe el numero ingresado por el usuario y lo coloca en la direccion de memoria de la variable n
   ptr = (int*) calloc(n, sizeof(int)); //--> Reserva de memoria de tamaño int, se inicializa en cero.
   if(ptr == NULL) {
     printf("Error! memory not allocated."); //--> Si ptr apunta a NULL se imprime el mensaje
     exit(0);
   }

   
   for(i = 0; i < n; ++i) {//--> Se entra en el ciclo para preguntar por los elementos n veces
      printf("Enter elements: ");
      scanf("%d", ptr + i);//--> scan guarga el valor ingresado en donde este apuntando el apuntador (ptr+i) de acuerdo a la iteracion
      sum += *(ptr + i);//--> Se acumula la suma de cada uno de los valores ingresados en la variable suma
   }

   printf("Sum = %d \n", sum); //--> Se imprime el resultado de la suma 
   free(ptr); //--> liberacion de memoria
   return 0; 
}
