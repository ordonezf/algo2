#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear();

// Devuelve verdadero o falso, según si la lista tiene o no elementos.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento a la ultima posicion de la lista.
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Agrega un nuevo elemento a la primera posicion de la lista.
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra al principio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su dato, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el dato del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el dato del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el dato del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Devuelve el largo de la lista.
// Pre: la lista fue creada.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

// Crea un iterador basado en la lista recibida por parametro.
// Pre: la lista fue creada.
// Post: Devuelve un iterador posicionado en el primer elemento.
lista_iter_t *lista_iter_crear(const lista_t *lista);

// Avanza al iterador en una posicion. Devuelve falso si no pudo avanzar,
// es decir, el iterador ya se encuentra al final.
// Pre: el iterador fue creado.
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve el dato del elemento sobre el cual esta parado el iterador.
// Si el iterador se encuentra al final devuelve NULL.
// Pre: el iterador fue creado.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve verdadero o falso, según si el iterador esta parado al final.
// Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
void lista_iter_destruir(lista_iter_t *iter);

// Agrega un nuevo elemento a la posicion en la que esta parado el iterador
// en su respectiva lista. Devuelve falso en caso de error.
// Pre: la lista y el iterador fueron creados.
// El iterador esta "basado" en la lista que se pasa por parametro.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra en
// la posicion actual del iterador.
bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato);

// Saca el elemento de la lista sobre el cual se encuentra el iterador.
// Si el iterador se encuentra al final devuelve NULL.
// Pre: la lista y el iterador fueron creados.
// El iterador esta "basado" en la lista que se pasa por parametro.
// Post: se devolvió el dato del elemento actual del iterador, la lista
// contiene un elemento menos, si la lista no estaba vacía.
// El iterador se encuentra parado sobre el siguiente del actual anterior.
void *lista_borrar(lista_t *lista, lista_iter_t *iter);

// Itera la lista llamando a la funcion "visitar" por cada elemento de la misma.
// La función de callback "visitar" recibe el dato y un puntero extra, 
// y devuelve true si se debe seguir iterando, false en caso contrario.
// Pre: la lista fue creada. "visitar" es una funcion capaz de utilizar y/o
// modificar los datos de la lista.
void lista_iterar(lista_t *lista, bool (*visitar)(void *dato, void *extra), void *extra);

#endif // LISTA_H