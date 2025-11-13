/****************************
# Autor: Juan Felipe Gómez López
# Fecha: 11 de noviembre 2025
# Materia: Sistemas Operativos
# Tema: Programa en C que lea del teclado el nombre de un directorio y muestre en
# pantalla el nombre y el tamaño de los ficheros que contiene
******************************/

//Se incluyen las interfaces necesarias 
#include <sys/types.h> //Para tipos de datos que se usan en interfaces como dirent.h
#include <sys/stat.h> // Es la interfaz que define la estructura de datos devueltos por fstat(), istat()
#include <sys/mman.h> //Para declaración de funciones para la gestión de memoria
#include <fcntl.h> // Permite el control de propiedades de un archivo abierto
#include <stdio.h> //Para operaciones de entrada y salida
#include <unistd.h> //Para la definición de tipos de datos y constantes
#include <stdlib.h> //Para la gestión de memoria y cast y exit() para finalizar un proceso
#include <string.h> //Para el manejo de cadenas d ecaracteres
#include <dirent.h> //Para operaciones como closedir(DIR *), y opendir


int main(){
	//Declaración apuntador hacia el el directotio
	DIR *d;
	//Se crean dos cadenas de caracteres para el nombre del fichero y del directorio
	char nomdir[90], nomfich[90];
	//Para manejar y almacenar información sobre el directorio
	struct stat datos;
	//Se crea un apuntador para la estructura de tipo dirent
	struct dirent *direc;
	printf ("Introduzca el Nombre de un Directorio: ");
	//Se recibe en una variable el nombre digitado por el usuario
	fgets (nomdir,sizeof(nomdir),stdin);
	nomdir[strlen(nomdir)-1]='\0'; /*Eliminamos el \n del Nombre del Fichero*/
	if ((d=opendir(nomdir))==NULL){
		printf ("El directorio no existe\n");
		return -1;
	}
	//Mientras que la lectura del directorio no llegue al final 
	while ((direc=readdir(d)) !=NULL) {
		//Se copia en el nombre del fichero el nombre del directorio
		strcpy(nomfich, nomdir);
		//Se concatena con el caracter / a la cadena nomfich
		strcat(nomfich, "/");
		//Se concatena en el nomfich el nombre del directorio
		strcat(nomfich, direc->d_name );
		//Se obtienen los datos/estadísticas del fichero gracias a la interfaz <sys/stat.h>
		stat (nomfich, &datos);
		//El S_ISREG obtiene información de un archivo y se le pasa el atributo st_mode de la estructura stat
		//para saber si el archivo tiene stats regulares o invalidos
		if (S_ISREG(datos.st_mode))
			printf ("Nombre: %s\t| Tamaño: %ld\n", direc->d_name,datos.st_size);
	}/*Fin del While*/
	
	closedir(d);
}
