#ifndef HASH_H
#define HASH_H

#include <stdbool.h>
#include <stddef.h>

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 ******************************************************************/

typedef struct hash hash_t;
typedef struct hash_iter hash_iter_t;
typedef void (*hash_destruir_dato_t)(void *);

/********************************************************************
 *                     PRIMITIVAS DEL HASH                          *
 *******************************************************************/

// Crea un hash. Recibe una funcion de destruccion
// Pre: destruir_dato es una función capaz de destruir
// los datos del hash, o NULL en caso de que no se la utilice.
// Post: devuelve un hash vacio.
hash_t *hash_crear(hash_destruir_dato_t destruir_dato);

// Guarda el dato dentro del hash asociandolo a la clave.
// Pre: El hash fue creado.
// Post: devuelve true si pudo guardar, false si no.
bool hash_guardar(hash_t *hash, const char *clave, void *dato);

// Borra la clave y devuelve su dato asociado.
// Pre: El hash fue creado.
// Post: Devuelve el dato asociado a la clave o NULL si
// esta no pertenece al hash.
void *hash_borrar(hash_t *hash, const char *clave);

// Obtiene el valor asociado a una clave.
// Pre: El hash fue creado.
// Post: Devuelve el dato asociado a la clave o NULL si
// esta no pertenece al hash.
void *hash_obtener(const hash_t *hash, const char *clave);

// Se fija si una clave esta en el hash.
// Pre: El hash fue creado.
// Post: Devuelve true o false dependiendo de si la clave
// esta o no.
bool hash_pertenece(const hash_t *hash, const char *clave);

// Devuelve la cantidad de elementos en el hash.
// Pre: El hash fue creado.
// Post: devuelve cuantos elementos tiene el hash, 0 si esta vacio.
size_t hash_cantidad(const hash_t *hash);

// Destruye el hash.
// Pre: El hash fue creado.
// Post: Se destruye el hash y sus datos, y se libera la memoria.
void hash_destruir(hash_t *hash);


/*******************************************************************
 *                    PRIMITIVAS DEL ITERADOR                      *
 ******************************************************************/

// Crea un iterador basado en el hash recibido por parametro.
// Pre: El hash fue creado.
// Post: Devuelve una estructura que itera sobre el hash.
hash_iter_t *hash_iter_crear(const hash_t *hash);

// Avanza a la siguiente posicion.
// Pre: El hash y el iterador fueron creados.
// Post: Devuelve true o false dependiendo de si pudo
// avanzar o no.
bool hash_iter_avanzar(hash_iter_t *iter);

// Devuelve la clave actual.
// Si el iterador se encuentra al final devuelve NULL.
// Pre: El hash y el iterador fueron creados.
// Post: Devuelve la clave en donde esta parado el iterador. No
// se puede modificar.
const char *hash_iter_ver_actual(const hash_iter_t *iter);

// Se fija si el iterador esta al final.
// Pre: El hash y el iterador fueron creados.
// Post: Devuele true si esta al final, caso contrario,
// devuelve false.
bool hash_iter_al_final(const hash_iter_t *iter);

// Destruye el iterador.
// Pre: El hash y el iterador fueron creados.
// Post: Destruye el iterador y libera memoria.
void hash_iter_destruir(hash_iter_t* iter);

#endif // HASH_H