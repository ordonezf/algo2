#include "cola.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*******************************************************************
 *Nombre: Francisco Joaquin Ordoñez Goncalves                      *
 *Padron: 96478                                                    *
 ******************************************************************/

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 ******************************************************************/

 typedef struct nodo_cola{
 	void *dato;
 	struct nodo_cola *proximo;
 }nodo_cola_t;

 struct cola{
 	nodo_cola_t *ultimo;
 	nodo_cola_t *primero;
 };

 /*******************************************************************
 *                        FUNCIONES AUXILIARES                      *
 *******************************************************************/

 /*******************************************************************
 *                        IMPLEMENTACION                            *
 *******************************************************************/

 // Crea una cola.
// Post: devuelve una nueva cola vacía.
cola_t* cola_crear(){
	cola_t* cola = malloc(sizeof(nodo_cola_t));
	if (cola == NULL)
		return NULL;
	cola->ultimo = NULL;
	cola->primero = NULL;
	return cola;
}

// Destruye la cola. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la cola llama a destruir_dato.
// Pre: la cola fue creada. destruir_dato es una función capaz de destruir
// los datos de la cola, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la cola.
void cola_destruir(cola_t *cola, void destruir_dato(void*)){
	if (destruir_dato == NULL){
		while (cola->primero != NULL){
			cola_desencolar(cola);
		}
	}
	else{
		while (cola->primero != NULL){
			nodo_cola_t *nodo_prox = cola->primero->proximo;
			destruir_dato(cola->primero);
			cola->primero = nodo_prox;
		}
	}
	free(cola);	
}

// Devuelve verdadero o falso, según si la cola tiene o no elementos encolados.
// Pre: la cola fue creada.
bool cola_esta_vacia(const cola_t *cola){
	return (cola->primero == NULL);
}

// Agrega un nuevo elemento a la cola. Devuelve falso en caso de error.
// Pre: la cola fue creada.
// Post: se agregó un nuevo elemento a la cola, valor se encuentra al final
// de la cola.
bool cola_encolar(cola_t *cola, void* valor){
	nodo_cola_t* nodo_nuevo = malloc(sizeof(nodo_cola_t));

	if (nodo_nuevo == NULL)
		return false;

	nodo_nuevo->dato = valor;
	nodo_nuevo->proximo = NULL;

	if (cola_esta_vacia(cola)){
		cola->primero = nodo_nuevo;
		cola->ultimo = nodo_nuevo;
		return true;
	}
	else{
		cola->ultimo->proximo = nodo_nuevo;
		cola->ultimo = nodo_nuevo;
		return true;
	}
}

// Obtiene el valor del primer elemento de la cola. Si la cola tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el primer elemento de la cola, cuando no está vacía.
void* cola_ver_primero(const cola_t *cola){
	if (cola_esta_vacia(cola))
		return NULL;
	else
		return cola->primero->dato;
}

// Saca el primer elemento de la cola. Si la cola tiene elementos, se quita el
// primero de la cola, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la cola fue creada.
// Post: se devolvió el valor del primer elemento anterior, la cola
// contiene un elemento menos, si la cola no estaba vacía.
void* cola_desencolar(cola_t *cola){
	if (cola_esta_vacia(cola))
		return NULL;

	void *aux = cola->primero->dato;
	nodo_cola_t *nodo_prox = cola->primero->proximo;
	free(cola->primero);
	cola->primero = nodo_prox;
	return aux;
	
}