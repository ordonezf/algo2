#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 ******************************************************************/

/* La lista está planteada como una lista de punteros genéricos. */

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/********************************************************************
 *                    PRIMITIVAS DE LA COLA                         *
 *******************************************************************/
// Crea una Lista
// Post: devuelve una nueva lista vacia.
lista_t *lista_crear();

// Devuelve verdadero o falso, segun si la lista tiene o no elementos.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento en la primera posicion de la lista. 
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: Se agrego un nuevo elemento a la lista, dato se encuentra al principio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento en la ultima posicion de la lista. 
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: Se agrego un nuevo elemento a la lista, dato se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca el primer elemento de la lista y lo devuelve. Si esta vacia, devuelve NULL.
// Pre: la lista fue creada.
// Post: Se devuelve el primer elemento de la lista cuando no esta vacia, tamaño--.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista y lo devuelve. Si esta vacia,
// devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvio el primer elemento de la lista, cuando no esta vacia. 
void *lista_ver_primero(const lista_t *lista);

// Obtiene el largo de la lista.
// Pre: la lista fue creada.
// Post: devuelve el largo de la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la funcion destruir_dato por parametro,
// para cada uno de los elementos de la lista se la llama.
// Pre: la lista fue creada. destruir_dato es una funcin capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

/*******************************************************************
 *                    PRIMITIVAS DEL ITERADOR                      *
 ******************************************************************/

// Crea un iterador.
// Pre: la lista fue creada. 
// Post: devuelve un iterador con acual apuntando al primero de la lista.
lista_iter_t *lista_iter_crear(const lista_t *lista);

// Avanza una posicion en el iterador. Devuelve true o false.
// Pre: el iterador fue creado.
// Post: Devuele true si la posicion siguiente a actual no es NULL.
// En caso contrario, false.
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve el elemento actual en donde el iterador esta parado.
// Pre: el iterador fue creado.
// Post: devuelve el elemento actual del iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve true o false dependiendo si se recorrio todo el iterador.
// Pre: el iterador fue creado.
// Post: si se recorrio todo el iterador devuelve true, si no, false.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se elimina el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un elemento en la lista. Devuelve true o false.
// Pre: la lista y el iterador fueron creados.
// Post: Inserta un elemento en la posicion actual del iterador. Devuelve true
// si pudo, false si no.
bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato);

// Borra un elemento de la lista.
// Pre: la lista y el iterador fueron creados.
// Post: Borra y devuelve un elemento de la lista, actual pasa a ser
// el siguiente del elemento borrado. Devuelve NULL si paso el final o esta vacia.
void *lista_borrar(lista_t *lista, lista_iter_t *iter);

// Itera la lista de principio a fin, si visitar no es NULL, se la aplica
// a cada elemento.
// Pre: la lista fue creada.
// Post: Itera la lista de principio a fin o hasta que visitar lo detiene.
void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void* extra);

#endif //LISTA_H