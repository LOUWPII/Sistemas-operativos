/*
*****************************BIENVENIDO A PIPESOFT**********************************
Nombre: Juan Felipe Gómez López
Actividad: Teoría del rendimiento
Asignatura: Sistemas Operativos
*/

#ifndef _MODULO_H_ //SI el modulo.h no está definido  el compilador define el modulo.h  y mete el contenido el .h en cada uno de los .c que hagan #include "modulo.h"
#define _MODULO_H_
#include <sys/time.h>// Para que la estructura timeval pueda definirse sin problemas

/* 
 * Este archivo de las cabeceras declara el protorip de funciones  que el resto de los .c usan.
 * Se incluye <sys/time.h> para que la estructura struct timeval esté disponible
 * cuando se usa extern struct timeval ini, fin  es para evitar el error de multiple definition y para decirle al compilador que busque la variable global en el archivo donde se declararon
 * Si no se usa extern saldria un error diciendo que en ningun momento se definieron estas estructuras
 */

void InicioMuestra();

void FinMuestra();

void iniMatriz(int n, double *m1, double *m2);

void imprMatrices(int n, double *matriz);

extern struct timeval ini, fin;

#endif
