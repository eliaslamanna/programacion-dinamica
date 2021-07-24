#include <stdio.h>
#include <string.h>
#include "lista.h"
#include <stdlib.h>

void agregar_siguientes(lista_t** siguientes){
	int* aux;
	
	
	//Agrego vecinos al 0 (1,2):
	aux = malloc (sizeof(int));	
	*aux = 1;
	lista_insertar_ultimo(siguientes[0], aux);
	aux = malloc (sizeof(int));	
	*aux = 2;
	lista_insertar_ultimo(siguientes[0], aux);
	
	//Agrego vecinos al 1 (0,2,4):
	aux = malloc (sizeof(int));	
	*aux = 0;
	lista_insertar_ultimo(siguientes[1], aux);
	aux = malloc (sizeof(int));	
	*aux = 2;
	lista_insertar_ultimo(siguientes[1], aux);
	aux = malloc (sizeof(int));	
	*aux = 4;
	lista_insertar_ultimo(siguientes[1], aux);
	
	//Agrego vecinos al 2: (0,1,3,5)
	aux = malloc (sizeof(int));	
	*aux = 0;
	lista_insertar_ultimo(siguientes[2], aux);
	aux = malloc (sizeof(int));	
	*aux = 1;
	lista_insertar_ultimo(siguientes[2], aux);
	aux = malloc (sizeof(int));	
	*aux = 3;
	lista_insertar_ultimo(siguientes[2], aux);
	aux = malloc (sizeof(int));	
	*aux = 5;
	lista_insertar_ultimo(siguientes[2], aux);
	
	//Agrego vecinos al 3: (2,6)
	aux = malloc (sizeof(int));	
	*aux = 2;
	lista_insertar_ultimo(siguientes[3], aux);
	aux = malloc (sizeof(int));	
	*aux = 6;
	lista_insertar_ultimo(siguientes[3], aux);
	
	//Agrego vecinos al 4 (1,5,7):
	aux = malloc (sizeof(int));	
	*aux = 1;
	lista_insertar_ultimo(siguientes[4], aux);
	aux = malloc (sizeof(int));	
	*aux = 5;
	lista_insertar_ultimo(siguientes[4], aux);
	aux = malloc (sizeof(int));	
	*aux = 7;
	lista_insertar_ultimo(siguientes[4], aux);
	
	//Agrego vecinos al 5: (2,4,6,8)
	aux = malloc (sizeof(int));	
	*aux = 2;
	lista_insertar_ultimo(siguientes[5], aux);
	aux = malloc (sizeof(int));	
	*aux = 4;
	lista_insertar_ultimo(siguientes[5], aux);
	aux = malloc (sizeof(int));	
	*aux = 6;
	lista_insertar_ultimo(siguientes[5], aux);
	aux = malloc (sizeof(int));	
	*aux = 8;
	lista_insertar_ultimo(siguientes[5], aux);
	
	//Agrego vecinos al 6 (3,5,9):
	aux = malloc (sizeof(int));	
	*aux = 3;
	lista_insertar_ultimo(siguientes[6], aux);
	aux = malloc (sizeof(int));	
	*aux = 5;
	lista_insertar_ultimo(siguientes[6], aux);
	aux = malloc (sizeof(int));	
	*aux = 9;
	lista_insertar_ultimo(siguientes[6], aux);
	
	//Agrego vecinos al 7: (4,8)
	aux = malloc (sizeof(int));	
	*aux = 4;
	lista_insertar_ultimo(siguientes[7], aux);
	aux = malloc (sizeof(int));	
	*aux = 8;
	lista_insertar_ultimo(siguientes[7], aux);
	
	//Agrego vecinos al 8 (5,7,9):
	aux = malloc (sizeof(int));	
	*aux = 5;
	lista_insertar_ultimo(siguientes[8], aux);
	aux = malloc (sizeof(int));	
	*aux = 7;
	lista_insertar_ultimo(siguientes[8], aux);
	aux = malloc (sizeof(int));	
	*aux = 9;
	lista_insertar_ultimo(siguientes[8], aux);
	
	//Agrego vecinos al 9: (6,8)
	aux = malloc (sizeof(int));	
	*aux = 6;
	lista_insertar_ultimo(siguientes[9], aux);
	aux = malloc (sizeof(int));	
	*aux = 8;
	lista_insertar_ultimo(siguientes[9], aux);
}

unsigned long long posibilidades_fuerzaBruta(unsigned int inicial, unsigned int n_pasos, lista_t** siguientes){
	//Si no quedan pasos, 0
	if (n_pasos == 0) return 0;
	
	//Si queda un solo paso, solo puedo ir a los que son directamente vecinos
	if (n_pasos == 1) return lista_largo(siguientes[inicial]);
	
	
	//Sino, voy a cada vecino con 1 paso menos, y sumo las posibilidades desde cada uno de ellos
	unsigned long long posibilidades = 0;
	lista_iter_t* iter = lista_iter_crear(siguientes[inicial]);
	while (!lista_iter_al_final(iter)){
		int* vecino = lista_iter_ver_actual(iter);
		posibilidades += posibilidades_fuerzaBruta(*vecino, n_pasos - 1, siguientes);
		lista_iter_avanzar(iter);
	}
	
	lista_iter_destruir(iter);
	return posibilidades;
}

unsigned long long posibilidades_dinamica(unsigned int inicial, unsigned int n_pasos, lista_t** siguientes){
	
	//Defino la matriz en la que cada lugar (i,j) es la cantidad de posibles caminos a adoptar
	unsigned long long matriz[10][n_pasos + 1];
	
	unsigned int i, j;
	//Guardo los valores base conocidos (cuando tengo 1 solo paso, solo puedo ir a los vecinos directos)
	for (i = 0; i < 10; i++){
		matriz[i][0] = 0;
		matriz[i][1] = lista_largo(siguientes[i]);
	}
	
	//A partir del paso 2, se que puedo ir a tantos vecinos como tenga (reduzco el paso en 1)
	//y desde alli ya se cuantas posibilidades tengo con la cantidad de pasos reducida en 1.
	//Sumo todas esas opciones y listo
	for (j = 2; j <= n_pasos; j++){
		for (i = 0; i < 10; i++){
			unsigned long long aux = 0;
			lista_iter_t* iter = lista_iter_crear(siguientes[i]);
			while (!lista_iter_al_final(iter)){
				int* vecino = lista_iter_ver_actual(iter);
				aux += matriz[*vecino][j - 1];
				lista_iter_avanzar(iter);
			}
			
			lista_iter_destruir(iter);
			matriz[i][j] = aux;
		}
	}
	
	return matriz[inicial][n_pasos];
}


int main (int argc, char* argv[]){
	/*Se recibe por parametro:
	 *	parametro 1: -d o -f, para indicar si se hace por programacion
	 * 	dinamica, o por fuerza bruta, respectivamente.
	 * 	parametro 2: el lugar inicial
	 *  parametro 3: la cantidad de pasos a realizar
	*/
	
	if (argc != 4) {
		printf("Asi no se usa el programa\n");
		return 1;
	}
	
	bool porFuerzaBruta = strcmp(argv[1], "-f") == 0;
	bool porProgramacionDinamica = strcmp(argv[1], "-d") == 0;
	
	if (! (porFuerzaBruta || porProgramacionDinamica)){
		printf("Es necesario indicar el metodo a utilizar\n");
		return 1;
	}
	unsigned int inicio = atoi (argv[2]);
	unsigned int pasos = atoi (argv[3]);
	
	lista_t* siguientes[10];
	for (unsigned int i = 0; i < 10; i++)
		siguientes[i] = lista_crear();
	agregar_siguientes(siguientes);
	
	printf("La cantidad de posibilidades son ");
	if (porProgramacionDinamica)
		printf("%llu\n", posibilidades_dinamica(inicio, pasos, siguientes));
	else
		printf("%llu\n", posibilidades_fuerzaBruta(inicio, pasos, siguientes));
	
	for (unsigned int i = 0; i < 10; i++)
		lista_destruir(siguientes[i], free);
	return 0;
}

//Ejemplo: 1,2: (1,0,1), (1,0,2), (1,2,1), (1,2,3), (1,2,0), (1,2,5), (1,4,1), (1,4,5), (1,4,7) = 9 posibilidades
