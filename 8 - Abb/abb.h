#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 ******************************************************************/

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;
typedef int (*abb_comparar_clave_t)(const char *, const char *);
typedef void (*abb_destruir_dato_t)(void *);

/********************************************************************
 *                      PRIMITIVAS DEL ABB                          *
 *******************************************************************/

// Crea un Arbol Binario de Busqueda. 
// Recibe una funcion de destruccion y una de comparacion.
// Pre: destruir_dato es una función capaz de destruir
// los datos del abb, o NULL en caso de que no se la utilice.
// cmp es una funcion capaz de comparar claves (devuelve 0 si son 
// iguales, <0 si el izquierdo es menor que el derecho y >0 si es mayor)
// Post: devuelve un abb vacio.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

// Guarda el dato dentro del abb asociandolo a la clave.
// Pre: El abb fue creado.
// Post: devuelve true si pudo guardar, false si no.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

// Borra la clave y devuelve su dato asociado.
// Pre: El abb fue creado.
// Post: Devuelve el dato asociado a la clave o NULL si
// esta no pertenece al abb.
void *abb_borrar(abb_t *arbol, const char *clave);

// Obtiene el valor asociado a una clave.
// Pre: El abb fue creado.
// Post: Devuelve el dato asociado a la clave o NULL si
// esta no pertenece al abb.
void *abb_obtener(const abb_t *arbol, const char *clave);

// Se fija si una clave esta en el abb.
// Pre: El abb fue creado.
// Post: Devuelve true o false dependiendo de si la clave
// esta o no.
bool abb_pertenece(const abb_t *arbol, const char *clave);

// Devuelve la cantidad de elementos en el abb.
// Pre: El abb fue creado.
// Post: devuelve cuantos elementos tiene el abb, 0 si esta vacio.
size_t abb_cantidad(abb_t *arbol);

// Destruye el abb.
// Pre: El abb fue creado.
// Post: Se destruye el abb y sus datos, y se libera la memoria.
void abb_destruir(abb_t *arbol);


/*******************************************************************
 *                    PRIMITIVAS DEL ITERADOR                      *
 ******************************************************************/

/* INTERNO */

// Itera el abb IN ORDER llamando a la funcion "visitar" por cada elemento de la misma.
// La función de callback "visitar" recibe la clave, el dato y un puntero extra,
// y devuelve true si se debe seguir iterando, false en caso contrario.
// Pre: el abb fue creado. "visitar" es una funcion capaz de utilizar y/o
// modificar los datos del abb, no debe modificar la clave.
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/* EXTERNO */

// Crea un iterador basado en el abb recibido por parametro.
// Pre: El abb fue creado.
// Post: Devuelve una estructura que itera IN ORDER sobre el abb.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanza a la siguiente posicion.
// Pre: El abb y el iterador fueron creados.
// Post: Devuelve true o false dependiendo de si pudo
// avanzar o no.
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Devuelve la clave actual.
// Si el iterador se encuentra al final devuelve NULL.
// Pre: El abb y el iterador fueron creados.
// Post: Devuelve la clave en donde esta parado el iterador. No
// se puede modificar.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Se fija si el iterador esta al final.
// Pre: El abb y el iterador fueron creados.
// Post: Devuele true si esta al final, caso contrario,
// devuelve false.
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye el iterador.
// Pre: El abb y el iterador fueron creados.
// Post: Destruye el iterador y libera memoria.
void abb_iter_in_destruir(abb_iter_t* iter);

#endif // ABB_H