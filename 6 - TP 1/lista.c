#include "lista.h"
#include <stdlib.h>
#include <stdbool.h>

/*******************************************************************
 *Nombre: Francisco Joaquin Ordoñez Goncalves                      *
 *Padron: 96478                                                    *
 *Ayudante: Dani Soto                                              *
 ******************************************************************/

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 ******************************************************************/

typedef struct nodo_lista{
	void *dato;
	struct nodo_lista *proximo;
}nodo_lista_t;

struct lista{
	nodo_lista_t *primero;
	nodo_lista_t *ultimo;
	size_t largo;
};

struct lista_iter{
	nodo_lista_t *actual;
	nodo_lista_t *anterior;
};

 /*******************************************************************
 *                        IMPLEMENTACION Lista                      *
 *******************************************************************/

// Crea una Lista
// Post: devuelve una nueva lista vacia.
lista_t *lista_crear(){
	lista_t* lista = malloc(sizeof(lista_t));
	if (lista == NULL)
		return NULL;
	lista->ultimo = NULL;
	lista->primero = NULL;
	lista->largo = 0;
	return lista;
}

// Devuelve verdadero o falso, segun si la lista tiene o no elementos.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista){
	return (lista->largo == 0);
}

// Agrega un nuevo elemento en la primera posicion de la lista. 
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: Se agrego un nuevo elemento a la lista, dato se encuentra al principio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato){
	nodo_lista_t *nodo = malloc(sizeof(nodo_lista_t));
	if (nodo == NULL) return false;

	nodo->dato = dato;
	nodo->proximo = lista->primero;

	if (lista_esta_vacia(lista))
		lista->ultimo = nodo;

	lista->primero = nodo;
	lista->largo++;
	return true;
}

// Agrega un nuevo elemento en la ultima posicion de la lista. 
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: Se agrego un nuevo elemento a la lista, dato se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato){
	if (lista_esta_vacia(lista)){
		bool ok;
		ok = lista_insertar_primero(lista, dato);
		return ok;
	}

	nodo_lista_t *nodo = malloc(sizeof(nodo_lista_t));
	if (nodo == NULL) return false;

	nodo->dato = dato;
	nodo->proximo = NULL;
	lista->ultimo->proximo = nodo;
	lista->ultimo = nodo;
	lista->largo++;
	return true;
}

// Saca el primer elemento de la lista y lo devuelve. Si esta vacia, devuelve NULL.
// Pre: la lista fue creada.
// Post: Se devuelve el primer elemento de la lista cuando no esta vacia, tamaño--.
void *lista_borrar_primero(lista_t *lista){
	if (lista_esta_vacia(lista)) return NULL;

	nodo_lista_t *nodo = lista->primero;
	void* valor = nodo->dato;
	lista->primero = lista->primero->proximo;
	free(nodo);
	lista->largo--;
	return valor;
}

// Obtiene el valor del primer elemento de la lista y lo devuelve. Si esta vacia,
// devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvio el primer elemento de la lista, cuando no esta vacia. 
void *lista_ver_primero(const lista_t *lista){
	if (lista_esta_vacia(lista)) return NULL;
	return lista->primero->dato;
}

// Obtiene el largo de la lista.
// Pre: la lista fue creada.
// Post: devuelve el largo de la lista, si esta vacia 0.
size_t lista_largo(const lista_t *lista){
	return lista->largo;
}

// Destruye la lista. Si se recibe la funcion destruir_dato por parametro,
// para cada uno de los elementos de la lista se la llama.
// Pre: la lista fue creada. destruir_dato es una funcin capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *)){
	if (destruir_dato == NULL){
		while (lista->primero != NULL){
			lista_borrar_primero(lista);
		}
	}
	else{
		while (lista->primero != NULL){
			nodo_lista_t *nodo = lista->primero;
			lista->primero = lista->primero->proximo;
			destruir_dato(nodo->dato);
			free(nodo);
		}
	}
	free(lista);
}

 /*******************************************************************
 *                        IMPLEMENTACION ITERADOR                   *
 *******************************************************************/

// Crea un iterador.
// Pre: la lista fue creada. 
// Post: devuelve un iterador con acual apuntando al primero de la lista.
 lista_iter_t *lista_iter_crear(const lista_t *lista){
 	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (iter == NULL) return NULL;

	iter->actual = lista->primero;
	iter->anterior = NULL;
	return iter;
 }

// Avanza una posicion en el iterador. Devuelve true o false.
// Pre: el iterador fue creado.
// Post: Devuele true si la posicion siguiente a actual no es NULL.
// En caso contrario, false.
bool lista_iter_avanzar(lista_iter_t *iter){
	nodo_lista_t *nodo = iter->actual;
	if (iter->actual == NULL) return false;

	iter->anterior = nodo;
	iter->actual = nodo->proximo;
	return true;
}

// Devuelve el elemento actual en donde el iterador esta parado.
// Pre: el iterador fue creado.
// Post: devuelve el elemento actual del iterador. Devuelve NULL si se llego
// al final de la lista.
 void *lista_iter_ver_actual(const lista_iter_t *iter){
 	if (iter->actual == NULL) return NULL;
 	
 	return iter->actual->dato;
 }

// Devuelve true o false dependiendo si se recorrio todo el iterador.
// Pre: el iterador fue creado.
// Post: si se recorrio todo el iterador devuelve true, si no, false.
 bool lista_iter_al_final(const lista_iter_t *iter){
 	if (iter->actual == NULL) return true;
 	
 	return false;
 }

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se elimina el iterador.
void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

// Inserta un elemento en la posicion actual del iterador. Devuelve true o false.
// Pre: la lista y el iterador fueron creados.
// Post: Inserta un elemento en la posicion del iterador,actual se actualiza.
// Devuelve true si puede, false si no.
bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato){
	if (lista_esta_vacia(lista)){
		if(!lista_insertar_primero(lista, dato)) return false;

		iter->actual = lista->primero;
		iter->anterior = lista->ultimo;
		return true;
	}
	nodo_lista_t *nodo = malloc(sizeof(nodo_lista_t));
	if (nodo == NULL) return false;
	nodo->dato = dato;
	lista->largo++;
	if (lista_iter_ver_actual(iter) == NULL){
		nodo->proximo = NULL;
		lista->ultimo->proximo = nodo;
		iter->anterior = lista->ultimo;
		lista->ultimo = nodo;
		iter->actual = lista->ultimo;
		return true;
	}
	else if (iter->actual == lista->primero){
		nodo->proximo = lista->primero;
		iter->anterior = NULL;
		lista->primero = nodo;
		iter->actual = nodo;
		return true;
	}
	else{
		nodo->proximo = iter->actual;
		iter->actual = nodo;
		iter->anterior->proximo = iter->actual;
		return true;
	}
}

// Borra un elemento de la lista.
// Pre: la lista y el iterador fueron creados.
// Post: Borra y devuelve un elemento de la lista, actual pasa a ser
// el siguiente del elemento borrado. Devuelve NULL si paso el final o esta vacia.
void *lista_borrar(lista_t *lista, lista_iter_t *iter){
	if (lista_esta_vacia(lista) || lista_iter_al_final(iter)) return NULL;

	nodo_lista_t *nodo = iter->actual;
	void *valor = nodo->dato;

	if (iter->actual == lista->primero){
		lista->primero = nodo->proximo;
		iter->actual = lista->primero;
		iter->anterior = NULL;
	}
	else if (lista->ultimo == iter->actual){
		lista->ultimo = iter->anterior;
		iter->actual = NULL;
		iter->anterior->proximo = iter->actual;
	}
	else{
		iter->actual = nodo->proximo;
		iter->anterior->proximo = iter->actual;
	}
	free(nodo);
	lista->largo--;
	return valor;
}

// Itera la lista de principio a fin, si visitar no es NULL, se la aplica
// a cada elemento.
// Pre: la lista fue creada.
// Post: Itera la lista de principio a fin o hasta que visitar lo detiene.
void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra)
, void *extra){
	if (visitar == NULL) return;

	//nodo_lista_t *nodo = malloc(sizeof(nodo_lista_t));
	//if (nodo == NULL) return;

	nodo_lista_t *nodo = lista->primero;
	bool ok_visitar = true;
	while(nodo != NULL && ok_visitar){
		ok_visitar = visitar(nodo->dato, extra);
		nodo = nodo->proximo;
	}
	return;
}

