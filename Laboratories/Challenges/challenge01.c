/******************************************************** BIENVENIDO A PIPESOFT***********************************************************
Nombre: Juan Felipe Gómez López
Asignatura: Sistemas Operativos
Desafio: challenge01 

*/


		
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // para malloc y free

int main() {

    char *p = (char*) malloc(50 * sizeof(char)); //--> Se reserva espacio de memoria para la cadena de caracteres
    char *q = (char*) malloc(50 * sizeof(char));

    if (p == NULL || q == NULL) {
        printf("EL apuntador no está apuntando a alguna posicion valida de memoria\n");//-->verificar si los apuntadores estan apuntando a NULL
	return 1;
    }

    printf("Address of p = %p\n", (void*)p); //--> De acuerdo a la consulta realizada, cuando se imprime una dirección de memoria se usa (void*)p

    strcpy(p, "Hello, I'm the best in Operating Systems!!!"); //-->Se pega la cadena de caracteres en el bloque de memoria que se reservó 
    printf("%s\n", p); //--> Se imprime la cadena de caracteres

    printf("About to copy \"Goodbye\" to q\n");
    strcpy(q, "Goodbye"); //--> Se copia la cadena goodbye en el bloque de memoria que se reservó para q

    printf("String copied\n");
    printf("%s\n", q);//--> Se imprime la cadena de caracteres que se encuentra en el bloque de memoria

    free(p);
    free(q);
}
