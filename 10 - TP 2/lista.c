#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "lista.h"

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct nodo
{
	void* dato;
	struct nodo* siguiente;
} nodo_t;

struct lista
{
	nodo_t *primero, *ultimo;
	size_t largo;
};

struct lista_iter
{
	nodo_t *actual, *anterior;
};

/* ******************************************************************
 *                        IMPLEMENTACION
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear()
{
	lista_t* lista = malloc(sizeof(lista_t));
	if (lista == NULL) return NULL;
	
	lista->primero = NULL;
	lista->ultimo = NULL;
	lista->largo = 0;
	return lista;
}

// Devuelve verdadero o falso, según si la lista tiene o no elementos.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista)
{
	return lista->largo == 0;
}

// Agrega un nuevo elemento a la posicion que esta entre el nodo 
// "anterior" y el "actual" de la lista. Devuelve falso en caso de error.
// Recibe opcionalmente el iterador para actualizar su estado.
// Pre: la lista fue creada. anterior y actual son nodos consecutivos de la lista.
// El iterador debe corresponder a los nodos actual y anterior.
// Post: se agregó un nuevo elemento a la lista. Se actualizaron las invariantes.
static bool lista_insertar_entre(lista_t *lista, nodo_t *anterior, nodo_t *actual, void *dato, lista_iter_t *iter)
{
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (nodo == NULL) return false;
	
	nodo->dato = dato;
	nodo->siguiente = actual;

	if (actual == NULL)		// Estamos insertando al final
		lista->ultimo = nodo;

	if (anterior == NULL)	// Estamos insertando al principio
		lista->primero = nodo;
	else
		anterior->siguiente = nodo;

	if (iter) iter->actual = nodo;
	lista->largo++;
	return true;
}

// Agrega un nuevo elemento a la ultima posicion de la lista.
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato)
{
	return lista_insertar_entre(lista, lista->ultimo, NULL, dato, NULL);
}

// Agrega un nuevo elemento a la primera posicion de la lista.
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra al principio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato)
{
	return lista_insertar_entre(lista, NULL, lista->primero, dato, NULL);
}

// Borra el nodo "actual" y saca el elemento del mismo de la lista.
// Si el nodo "actual" es NULL, devuelve NULL.
// Recibe opcionalmente el iterador para actualizar su estado.
// Pre: la lista fue creada. anterior y actual son nodos consecutivos de la lista.
// El iterador debe corresponder a los nodos actual y anterior.
// Post: se devolvió el dato del nodo "actual". Se actualizaron las invariantes.
static void *lista_borrar_actual(lista_t *lista, nodo_t *anterior, nodo_t *actual, lista_iter_t *iter)
{
	if (actual == NULL) return NULL;

	void *dato = actual->dato;
	nodo_t *siguiente = actual->siguiente;

	if (siguiente == NULL)	// Estamos borrando el ultimo
		lista->ultimo = anterior;

	if (anterior == NULL)	// Estamos borrando el primero
		lista->primero = siguiente;
	else
		anterior->siguiente = siguiente;

	if (iter) iter->actual = siguiente;
	free(actual);
	lista->largo--;
	return dato;
}

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su dato, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el dato del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista)
{
	return lista_borrar_actual(lista, NULL, lista->primero, NULL);
}

// Obtiene el dato del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el dato del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista)
{
	if (lista_esta_vacia(lista)) return NULL;
	return lista->primero->dato;
}

// Devuelve el largo de la lista.
// Pre: la lista fue creada.
size_t lista_largo(const lista_t *lista)
{
	return lista->largo;
}

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *))
{
	while (!lista_esta_vacia(lista))
	{
		if (destruir_dato)
			destruir_dato(lista_borrar_primero(lista));
		else
			lista_borrar_primero(lista);
	}
	free(lista);
}

// Crea un iterador basado en la lista recibida por parametro.
// Pre: la lista fue creada.
// Post: Devuelve un iterador posicionado en el primer elemento.
lista_iter_t *lista_iter_crear(const lista_t *lista)
{
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (iter == NULL) return NULL;

	iter->anterior = NULL;
	iter->actual = lista->primero;
	return iter;
}

// Avanza al iterador en una posicion. Devuelve falso si no pudo avanzar,
// es decir, el iterador ya se encuentra al final.
// Pre: el iterador fue creado.
bool lista_iter_avanzar(lista_iter_t *iter)
{
	if (lista_iter_al_final(iter)) return false;
	iter->anterior = iter->actual;
	iter->actual = iter->actual->siguiente;
	return true;
}

// Devuelve el dato del elemento sobre el cual esta parado el iterador.
// Si el iterador se encuentra al final devuelve NULL.
// Pre: el iterador fue creado.
void *lista_iter_ver_actual(const lista_iter_t *iter)
{
	if (lista_iter_al_final(iter)) return NULL;
	return iter->actual->dato;
}

// Devuelve verdadero o falso, según si el iterador esta parado al final.
// Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t *iter)
{
	return iter->actual == NULL;
}

// Destruye el iterador.
// Pre: el iterador fue creado.
void lista_iter_destruir(lista_iter_t *iter)
{
	free(iter);
}

// Agrega un nuevo elemento a la posicion en la que esta parado el iterador
// en su respectiva lista. Devuelve falso en caso de error.
// Pre: la lista y el iterador fueron creados.
// El iterador esta "basado" en la lista que se pasa por parametro.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra en
// la posicion actual del iterador.
bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato)
{
	return lista_insertar_entre(lista, iter->anterior, iter->actual, dato, iter);
}

// Saca el elemento de la lista sobre el cual se encuentra el iterador.
// Si el iterador se encuentra al final devuelve NULL.
// Pre: la lista y el iterador fueron creados.
// El iterador esta "basado" en la lista que se pasa por parametro.
// Post: se devolvió el dato del elemento actual del iterador, la lista
// contiene un elemento menos, si la lista no estaba vacía.
// El iterador se encuentra parado sobre el siguiente del actual anterior.
void *lista_borrar(lista_t *lista, lista_iter_t *iter)
{
	return lista_borrar_actual(lista, iter->anterior, iter->actual, iter);
}

// Itera la lista llamando a la funcion "visitar" por cada elemento de la misma.
// La función de callback "visitar" recibe el dato y un puntero extra, 
// y devuelve true si se debe seguir iterando, false en caso contrario.
// Pre: la lista fue creada. "visitar" es una funcion capaz de utilizar y/o
// modificar los datos de la lista.
void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra)
{
	nodo_t* nodo = lista->primero;
	while (nodo && visitar(nodo->dato, extra))
		nodo = nodo->siguiente;	
}