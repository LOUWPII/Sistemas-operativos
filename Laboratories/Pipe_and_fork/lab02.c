/*
Nombre: Juan Felipe Gómez López
Asignatura: Sistemas Operativos
Tema: Clonación de procesos

*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){

  printf("===================BIENVENIDO A PIPESOFT=======================\n\n");
  printf("=== Inicio del proceso principal ===\n");
  //Se clona el proceso
  int procesoID = fork();

  //Si devuelve cero quiere decir que el hijo se creó correctamente
  if(procesoID==0){
        printf("---> Proceso hijo recien creado \n");

 }else if(procesoID > 0){
 //Si devuelve un numero mayor que cero quiere decir que para el padre el hijo se creó bien
        printf("---> Proceso padre\n");
 }

 //Este ciclo for se ejecuta dos veces ya que ambos procesos se encargan de realizar la misma tarea
 printf("imprimiendo......\n");
 for (int i = 0;i < 5;i++){
   printf(" \t\t == %d == \n", i);
 }

 printf("FIN...........\n");

 return 0;
}
