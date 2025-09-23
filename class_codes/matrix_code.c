//Incluir libreria para entrada y salida de datos
#include <stdio.h>
//Incluir librería para operciones de reserva, asignación y liberación de memoria
#include <stdlib.h>
//Libreria para llenar la matriz con valores aleatorios
#include <time.h>


void printFunction() {
    printf("\n============================\n");
    printf("Bienvenido a PipeSoft - Calculadora de Masa Corporal\n");
    printf("============================\n");
}

// Calcular IMC
float imcFunction(float masa, float altura) {
    return masa / (altura * altura);
}

// Categoría de edad 
char* categoriaEdad(int edad) {
    //Si la edad es menor que 18 es menor de edad
    if (edad < 18) {
        return "menor de edad";
    } else if (edad < 50) { //Si no es menor a 18 pero sí es menor a 50 es adulto
        return "adulto";
    } else { //Si no es ninguna de las dos entonces hace parte de la tercer edad
        return "tercera edad";
    }
}

// Crear matriz NxN
/*Retorna float** ya que matriz es un apuntador que apunta a 
a un arreglo de apuntadores que apuntan a un arreglo del tipo de
dato float.
*/
float** crearMatriz(int N) {
    //Reservamos la memoria para el "arreglo" de apuntadores de apuntadores de tipo float
    float** matriz = (float**)malloc(N * sizeof(float*));
    //Por cada apuntador del arreglo reservamos memoria para almacenar valores de tipo float
    for (int i = 0; i < N; i++) {
        matriz[i] = (float*)malloc(N * sizeof(float));
    }
    //Retornamos la matriz
    return matriz;
}

// Liberar memoria
void liberarMatriz(float** matriz, int N) {
    //Por cada uno de los apuntadores a los valores float se libera la memoria
    for (int i = 0; i < N; i++) {
        free(matriz[i]);
    }
    //Se libera la memoria para el apuntador a los apuntadores
    free(matriz);
}

// Llenar matriz con valores aleatorios, por cada celda i,j de l matriz
//se asigna un valor aleatorio
void llenarMatrix(int N, float** mA) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            //Generamos un valor aleatorio entero de 0 a 99 pero lo
            //multiplicamos por 0.1 y le hacemos un cast a float
            //para generar valores de 0 a 9.9
            mA[i][j] = (float)(rand() % 100) * 0.1f;
        }
    }
}

// Imprimir matriz
void impMatrix(float** mA, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            //Se imprime cada valor i,j de la matriz y se pone %.2f para indicar que se imprime 
            //un flotante y los 2 primeros decimales.
            printf("%.2f ", mA[i][j]);
        }
        printf("\n");
    }
}

// Multiplicar matrices
float** matrixMult(float** mA, float** mB, int N) {
    float** mC = crearMatriz(N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            mC[i][j] = 0.0;
            for (int k = 0; k < N; k++) {
                /*
                  El producto de estos dos elementos se suma a la variable acumuladora mC[i][j]. Esto se repite N veces por cada valor de k
                  hasta que la suma de productos para la posición i,j está completa.
                */
                mC[i][j] += mA[i][k] * mB[k][j];
            }
        }
    }
    return mC;
}

// Menú principal
void mostrarMenu() {
    printf("\n========== MENÚ ==========\n");
    printf("1. Calcular IMC\n");
    printf("2. Multiplicar matrices aleatorias\n");
    printf("3. Salir\n");
    printf("Seleccione una opción: ");
}

int main() {
    int opcion;
    //Creacion de la semilla
    srand(time(NULL)); 

    do {
        //Mostramos el menu
        mostrarMenu();
        //Se recibe la opcion digitada por el usuario
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: {
                printFunction();
                float masa, altura;
                int edad;

                printf("Ingrese su masa (kg): ");
                scanf("%f", &masa);
                printf("Ingrese su altura (m): ");
                scanf("%f", &altura);
                printf("Ingrese su edad (años): ");
                scanf("%d", &edad);
                //Se calcula el IMC con los datos ingresados
                float imc = imcFunction(masa, altura);
                char* categoriaE = categoriaEdad(edad);

                printf("Su IMC es: %.2f\n", imc);
                printf("Se encuentra en la categoría de edad: %s\n", categoriaE);
                break;
            }

            case 2: {
                int N;
                printf("Ingrese el tamaño de las matrices (NxN): ");
                scanf("%d", &N);

                //Se crean las matrices A y B
                float** mA = crearMatriz(N);
                float** mB = crearMatriz(N);

                //Se llena cada una de las matrices llamando a las funciones
                llenarMatrix(N, mA);
                llenarMatrix(N, mB);

                //Imprimimos la matriz A
                printf("\nMatriz A:\n");
                impMatrix(mA, N);

                //Imprimimos la matriz B
                printf("\nMatriz B:\n");
                impMatrix(mB, N);

                //Asignamos a la matriz C el resultado de la multiplicación 
                //de las matrices A y B
                float** mC = matrixMult(mA, mB, N);
                printf("\nResultado de A x B:\n");
                impMatrix(mC, N);

                liberarMatriz(mA, N);
                liberarMatriz(mB, N);
                liberarMatriz(mC, N);
                break;
            }

            case 3:
                printf("Saliendo del programa...\n");
                break;

            default:
                printf("Opción inválida. Intente de nuevo.\n");
                break;
        }

    } while (opcion != 3);

    return 0;
}
