#include "participante.h"
#include "pila.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 ******************************************************************/

struct persona{
	char* nombre;
	pila_t *donaciones;
};

 /*******************************************************************
 *                        IMPLEMENTACION                            *
 *******************************************************************/

persona_t *crear_persona(char *nombre){
	persona_t* persona = malloc(sizeof(persona_t));
	if (persona == NULL) return NULL;

	pila_t* pila = pila_crear();
	if (pila == NULL) return NULL;

	persona->nombre = nombre;
	persona->donaciones = pila;
	return persona;
}
void destruir_persona(void* persona){
	persona_t *per = (persona_t*) persona;
	pila_destruir(per->donaciones);
	free(per);
	return;
}

char* ver_nombre(persona_t* persona){
	return persona->nombre;
}

pila_t* ver_donaciones(persona_t* persona){
	return persona->donaciones;
}

void mostrar_donaciones(pila_t *pila){
	if (pila_esta_vacia(pila)){
		puts("OK [] TOTAL: 0");
		return;
	}
	else{
		char donaciones[100] = "\0";
		strcat(donaciones, (char*) "[");
		int total = 0;
		while(!pila_esta_vacia(pila)){
			char* d = pila_desapilar(pila);
			strcat(donaciones, d);
			if (!pila_esta_vacia(pila))
				strcat(donaciones, (char*) ",");
			total += atoi(d);
		}
		strcat(donaciones, (char*) "]");
		printf("OK %s TOTAL: %i\n",donaciones, total);
	}
}
void agregar_donacion(persona_t* persona, char* monto){
	pila_apilar(persona->donaciones, monto);
}

bool participante_dono(persona_t* persona){
	return !pila_esta_vacia(persona->donaciones);
}
