/*
*Se declaran las librerias necesarias
*stdio.h para entrada y salida de datos por consola
*stdlib.h para uso de memoria 
*sys/time.h para la estructura .
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "modulo.h"

/*
 * En este archivo están las implementaciones de las funciones definidas en el modulo.h
 * En este .c no se  definen las variables globales mA/mB/mC/ini/fin, porque en la versión que se está utilizando esas se definen en el main (modulo_GomezLopez.c).
 * Aquí se asume que el header declara esas variables con extern para poder usarlas.
 */

/* 
 * InicioMuestra:
 * Captura el tiempo actual y lo guarda en la variable global 'ini'.
 * Se usa gettimeofday que da precisión en microsegundos.
 * No retorna nada, es solo para preparar el cronómetro.
 */
void InicioMuestra(){
        gettimeofday(&ini, (void *)0);
}

/* 
 * FinMuestra:
 * Captura el tiempo actual en 'fin' y calcula la diferencia con 'ini'.
 * El cálculo se hace en microsegundos para mostrar un valor entero grande.
 * Se imprime el tiempo transcurrido en microsegundos.
 * La idea es saber el tiempo de ejecución de multiplicar las matrices
 */
void FinMuestra(){
        gettimeofday(&fin, (void *)0);
        fin.tv_usec -= ini.tv_usec;
        fin.tv_sec -= ini.tv_sec;
        double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec);
        printf("%9.0f \n", tiempo);
}

/*
 * iniMatriz:
 * Inicializa las dos matrices m1 y m2 de tamaño n*n.
 * Como las matrices están en memoria lineal un malloc que reservó n*n espacios, aquí simplemente se trabaja con ellas como si fueran arreglos de una dimensión.
 */

void iniMatriz(int n, double *m1, double *m2){
        for(int i=0; i<n*n; i++){
                m1[i] = i*2.3 + 1.3;
                m2[i] = i*2.3;
        }
};

/*
 * imprMatrices:
 * Imprime la matriz si n < 9
 * Al imprimir, cada n elementos se hace un salto de línea.
 */
void imprMatrices(int n, double *matriz){
    if(n<9){
        printf("\n#######################################################\n");
        for(int i=0; i<n*n; i++){
                if(i%n==0) printf("\n");
                printf(" %f ", matriz[i]);
   }
    } else {
        printf("\n#######################################################\n");
    }
};
