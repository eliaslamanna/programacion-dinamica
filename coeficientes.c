#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "lista.h"
#include <stdbool.h>
#include <string.h>

/* Calcula los coeficientes del binomio de newton (a+b)^n por programacion dinamica 
 * a partir de saber que:
	(a + b)^n = sum_{k=0}^n Combinatorio(n, k) a^k * b^(n-k) y que:
	Combinatorio(n,k) = Combinatorio(n-1, k-1) + Combinatorio(n-1, k) Para 0 < k < n y
	Combinatorio(n,0) = Combinatorio(n,n) = 1 */
lista_t* calcular_coeficientes_optimizada(unsigned int n){
	if (n == 0) return NULL;
	
	unsigned long vector_coeficientes[n+1];
	unsigned long vector_actual[n+1];
	
	//Cargo solo la primer columna	
	vector_coeficientes[0] = 1;
	vector_coeficientes[1] = 1;
	
	for (unsigned int i = 1; i < n; i++){
		//cargo valores iniciales
		vector_actual[0] = 1;
		vector_actual[i+1] = 1;
		for (unsigned int k = 1; k <= i; k++){
			vector_actual[k] = vector_coeficientes[k-1] + vector_coeficientes[k];
		}

		//ahora el vector anterior es el vector actual:
		for (unsigned int k = 0; k <= i+1; k++)
			vector_coeficientes[k] = vector_actual[k];
	}
	
	//me quedo con la ultima columna y pongo los valores en una lista
	lista_t* coef = lista_crear();
	if (!coef) return NULL;
	
	for (unsigned int k = 0; k <= n; k++){
		unsigned long* valor = malloc (sizeof(unsigned long));
		if (!valor){
			lista_destruir(coef, free);
			return NULL;
		}
		*valor = vector_coeficientes[k];
		lista_insertar_ultimo(coef, valor);
	}
	return coef;
}

/* Calcula los coeficientes del binomio de newton (a+b)^n por programacion dinamica 
 * a partir de saber que:
	(a + b)^n = sum_{k=0}^n Combinatorio(n, k) a^k * b^(n-k) y que:
	Combinatorio(n,k) = Combinatorio(n-1, k-1) + Combinatorio(n-1, k) Para 0 < k < n y
	Combinatorio(n,0) = Combinatorio(n,n) = 1 */
lista_t* calcular_coeficientes(unsigned int n){
	if (n == 0) return NULL;
	
	unsigned long matriz_coeficientes[n][n+1];

	//Cargo valores iniciales
	for (unsigned int i = 0; i < n; i++){
		matriz_coeficientes[i][0] = 1;
	}
	
	for (unsigned int i = 0; i < n; i++){
		matriz_coeficientes[i][i+1] = 1;
	}
	
	//calculo el resto de la matriz:
	for (unsigned int i = 1; i < n; i++){
		for (unsigned int k = 1; k <= i; k++){
			matriz_coeficientes[i][k] = matriz_coeficientes[i-1][k-1] + matriz_coeficientes[i-1][k];
		}
	}
	
	//me quedo con la ultima columna y pongo los valores en una lista
	lista_t* coef = lista_crear();
	if (!coef) return NULL;
	
	for (unsigned int k = 0; k <= n; k++){
		unsigned long* valor = malloc (sizeof(unsigned long));
		if (!valor){
			lista_destruir(coef, free);
			return NULL;
		}
		*valor = matriz_coeficientes[n-1][k];
		lista_insertar_ultimo(coef, valor);
	}
	return coef;
}


int main (int argc, char** argv){
	//Recibe un parametro numerico (grado del binomio) y un parametro
	//opcional: -m para indicar que se usa la version optimizada en memoria
	if (argc > 3 || argc < 2 ){
		return 1;
	}
	
	bool mejorada = argc == 3 && strcmp("-m", argv[2]) == 0;
		
	unsigned int grado = atoi(argv[1]);
	//obtengo los coeficientes
	
	lista_t* coeficientes;
	if (mejorada)
		coeficientes = calcular_coeficientes_optimizada(grado);
	else
		coeficientes = calcular_coeficientes(grado);
	
	//itero la lista e imprimo el resultado
	lista_iter_t* iter = lista_iter_crear(coeficientes);
	unsigned int k = 0;
	bool primero = true;
	while (!lista_iter_al_final(iter)){
		unsigned long* coeficiente = lista_iter_ver_actual(iter);
		if (!primero)
			printf(" + ");
		primero = false;
		printf("%lu (a^%u b^%u)", *coeficiente, grado - k, k);
		k++;
		lista_iter_avanzar(iter);
	}
	printf("\n");
	//destruyo las estructuras
	lista_iter_destruir(iter);
	lista_destruir(coeficientes, free);
	return 0;
}
