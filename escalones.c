#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lista.h"
#define MEJORA "-m"

/**	Calcula la cantidad de posibilidades para llegar a un determinado escalon, si se pueden dar una cantidad
	determinada de pasos.**/
unsigned long posibilidades_escalones(unsigned int escalones, unsigned int pasos){	
	unsigned long tabla[escalones];
	//Caso base general
	tabla[0] = 1;
	//En los casos base puedo venir de cualquiera de los anteriores, o puedo ir directo, entonces es la suma de todos los anteriores + 1
	unsigned long suma_inicial = tabla[0];
	for (unsigned int i = 1; i < pasos && i < escalones; i++){ //En caso que me pidan llegar a un escalon menor a las posibilidades de pasos, tambien lo contemplo
		tabla[i] = 1 + suma_inicial;
		suma_inicial += tabla[i];
	}
	
	//En los casos generales, sumo las posibilidades de escalon_actual - pasos posibles
	for (unsigned int escalon_actual = pasos; escalon_actual < escalones; escalon_actual++){
		tabla[escalon_actual] = 0;
		for (unsigned int paso_actual = 1; paso_actual <= pasos; paso_actual++){
			tabla[escalon_actual] += tabla[escalon_actual - paso_actual];
		}
	}
	//devuelvo la cantidad de pasos posibles para el ultimo escalon
	return tabla[escalones-1];
}

bool sumar_todos(void* dato, void* extra){
	unsigned long* valor =  dato;
	unsigned long* suma = extra;
	*suma = *suma + *valor;
	return true;
}


/**	Calcula la cantidad de posibilidades para llegar a un determinado escalon, si se pueden dar una cantidad
	determinada de pasos. Ademas, optimiza el espacio utilizado en memoria. **/
unsigned long posibilidades_escalones_mejorados(unsigned int escalones, unsigned int pasos){
	//Ahora usamos una lista, para que sacar el primero y poner uno nuevo al final sea todo O(1)
	lista_t* anteriores = lista_crear();
	
	//Caso General
	unsigned long* valor = malloc(sizeof(unsigned long));
	*valor = 1;
	lista_insertar_ultimo(anteriores, valor);
	
	//En los casos base puedo venir de cualquiera de los anteriores, o puedo ir directo, entonces es la suma de todos los anteriores + 1
	unsigned long suma_inicial = *valor;
	unsigned long posibilidades = 1;
	for (unsigned int i = 1; i < pasos && i < escalones; i++){
		unsigned long* valor = malloc(sizeof(unsigned long));
		*valor = 1 + suma_inicial;
		suma_inicial += *valor;
		lista_insertar_ultimo(anteriores, valor);
		posibilidades = *valor;
	}	
	
	//En los casos generales, sumo las posibilidades de escalon_actual - pasos posibles
	for (unsigned int escalon_actual = pasos; escalon_actual < escalones; escalon_actual++){
		unsigned long* suma = malloc(sizeof(unsigned long));
		*suma = 0;
		lista_iterar(anteriores, sumar_todos, suma);
		lista_insertar_ultimo(anteriores, suma);
		free(lista_borrar_primero(anteriores));
		posibilidades = *suma; //me guardo el ultimo valor para no tener que buscarlo en la lista despues
	}
	
	lista_destruir(anteriores, free);
	return posibilidades;
}



int main(int argc, char** argv){
	//Reviso de haber obtenido la cantidad de parametros esperada
	if (argc < 3 || argc > 4){
		printf ("Modo de uso: escalones cantidad_escalones pasos_maximo [%s | optimizacion en memoria, opcional]\n", MEJORA);
		return 1;
	}
	
	unsigned long posibilidades;
	unsigned int escalones = atoi(argv[1]);
	unsigned int pasos = atoi(argv[2]);
	
	//Si se recibio el parametro de optimizacion, uso esa funcionalidad, sino el modo simple
	if (argc == 4 && strcmp(argv[3], MEJORA) == 0){
		posibilidades = posibilidades_escalones_mejorados(escalones, pasos);
	}else{
		posibilidades = posibilidades_escalones (escalones, pasos);
	}
	
	printf("La cantidad de formas posibles de llegar hasta el escalon %d pudiendo hacer hasta %d pasos es %lu\n", escalones, pasos, posibilidades);
	return 0;
}
