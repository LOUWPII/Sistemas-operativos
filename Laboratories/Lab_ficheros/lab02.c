/***************************************
# Autor: Juan Felipe Gómez López
# Fecha: 11 de noviembre 2025
# Materia: Sistemas operativos
# Tema: Sistemas de archivos: Se revisa el directorio y los ficheros que contiene y se revisan las estadísticas de los ficheros
# ademas en pantalla se imprimen estos datos. 
****************************************/
//Se incluyen las interfaces necesarias
#include <stdio.h> //Operaciones de entrada y salida
#include <string.h> //Para las operaciones de cadenas de caracteres
#include <time.h> //Para tipos de datos como la fecha
#include <dirent.h> //Para operaciones como opendir y closedir
#include <fcntl.h> //Permite el control de propiedades de un archivo abierto
#include <unistd.h>//Definicion de tipos de datos y constantes
#include <sys/stat.h> //Es la interfaz que define la estructura de datos devueltos como fstat()
#include <sys/types.h> //Para los tipos de datos que se utilizan en interface como dirent.h


int main () {
	int er;
	//Se crean 3 cadenas de caracteres para el nombre del fichero, el directorio
	char nomdir[100], nomfich[100], resp[30];
	//Se crea la estructura stat para mostrar estadísticas e informacion adicional del directorio
	struct stat atr;
	//Apuntador al directorio
	DIR *d;
	//Apuntador para la estructura de tipo diren
	struct dirent *rd1;
	//Definicion de la variable fecha utilizando la interfaz <time.h>
	time_t fecha;
	//Laboratorio 02
	printf ("Nombre directorio\n");
	//Se obtiene el nombre del directorio que escribió el usuario
	fgets (nomdir, sizeof (nomdir), stdin);
	/* hay que quitar el \n del nombre del directorio*/
	nomdir[strlen(nomdir)-1]='\0';
	fecha=time(&fecha);
	if ((d=opendir(nomdir))==NULL) {
		printf ("No existe ese directorio \n");
	return -1;
	}
	//Si el directorio si se pudo abrir
	else {
	//Mientras que no se llegue al final del directorio
	while (( rd1 =readdir(d)) != NULL) {
	//Si el nombre del directorio es distinto de . o ..
		if ( (strcmp(rd1->d_name, ".")!=0 )&& (strcmp(rd1->d_name, "..")!=0 )){
			//Se copiara en la cadena de caracteres el path o direccion del archivo obteniendo el nombre del fichero y directorio
			strcpy (nomfich, nomdir);
			strcat (nomfich, "/");
			strcat (nomfich, rd1->d_name);
			printf ("fichero :%s:", nomfich);
			//Se asigna en la variable entera er las estadisticas del nomfich
			er=stat (nomfich, &atr);
			printf ("modo :%#o:", atr.st_mode);
			if ((atr.st_mode & 0400) != 0)
				printf (" permiso R para propietario\n");
				//Laboratorio 02
			else
				printf (" No permiso R para propietario\n");
			if (S_ISDIR(atr.st_mode)) 
				printf (" Es un directorio \n");
			//Si las estadísticas estan bien y estan válidas 
			if (S_ISREG(atr.st_mode))
			/* ficheros modificados en los últimos 10 dias */
				if ( (fecha - 10*24*60*60) < atr.st_mtime) {
				    //Se imprime en pantlla la fecha y los segundos obteniendo los datos desde la estructura atr de tipo stat
					printf ("FICHERO:%s: fecha acceso %s, en sgdos %ld\n", 
					rd1->d_name,
					ctime (&atr.st_mtime),atr.st_mtime );
				}
		}
	} /* while*/
	//Se cierra el directorio con closedir gracias a la interfaz dirent.h que permite cerrar y abrir el directorio
	closedir (d);
  }
  return 0;
}/* main*/
