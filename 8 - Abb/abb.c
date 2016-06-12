#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "abb.h"
#include "pila.h"
char *strdup(const char *s);

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 ******************************************************************/

typedef struct _nodo_abb{
	char* clave;
	void* dato;
	struct _nodo_abb *izq, *der;
} nodo_abb_t;

struct abb{
	nodo_abb_t* raiz;
	size_t cantidad;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
};

struct abb_iter{
	const abb_t* arbol;
	pila_t* pila;
};

typedef void (*dest_nodo)(nodo_abb_t*, abb_destruir_dato_t);
typedef bool (*visitar_t)(const char *, void *, void *);

/********************************************************************
 *                      IMPLEMENTACION ABB                          *
 *******************************************************************/

/*******************************************************************
 *                       Funciones auxiliares                      */

// Crea un nodo.
// Post: Devuelve el nodo creado o NULL si no se creo.
static nodo_abb_t *nodo_crear(const char* clave, void* dato)
{
	nodo_abb_t *nodo = malloc(sizeof(nodo_abb_t));
	char* copia_clave = strdup(clave);
	if (!nodo || !copia_clave)
	{
		free(nodo);
		free(copia_clave);
		return NULL;
	}

	nodo->clave = copia_clave;
	nodo->dato = dato;
	nodo->izq = NULL;
	nodo->der = NULL;
	return nodo;
}

// Recibe el nodo y opcionalmente una funcion de destruccion.
// Destruye el nodo.
static void destruir_nodo(nodo_abb_t* nodo, abb_destruir_dato_t destruir_dato)
{
	if (destruir_dato) destruir_dato(nodo->dato);
	free(nodo->clave);
	free(nodo);
}

// Destruira todos los nodos del arbol, empezando por la raiz.
// Recibe opcionalmente una funcion de destruccion de datos.
static void destruir_nodos_postorder(nodo_abb_t* raiz, abb_destruir_dato_t destruir_dato)
{
	if (!raiz) return;
	destruir_nodos_postorder(raiz->izq ,destruir_dato);
	destruir_nodos_postorder(raiz->der, destruir_dato);
	destruir_nodo(raiz, destruir_dato);
}

// Recibe doble puntero al nodo a borrar. 
// Opcionalmente una funcion de destruccion de nodos.
// Borrara el nodo del arbol, reubicando el resto adecuadamente.
static void borrar_nodo(nodo_abb_t** p_nodo, dest_nodo destruir_nodo)
{
	nodo_abb_t* nodo = *p_nodo;
	nodo_abb_t* prox;

	if (!nodo->izq && !nodo->der) prox = NULL;
	else if (nodo->izq && !nodo->der) prox = nodo->izq;
	else if (!nodo->izq && nodo->der) prox = nodo->der;
	else // busco menor de los mayores
	{
		prox = nodo->der;  
		if (prox->izq == NULL) borrar_nodo(&(nodo->der), NULL); //punteros a memoria dinamica, no locales!
		else
		{
			while ((prox->izq)->izq != NULL) prox = prox->izq;
			nodo_abb_t* temp = prox->izq;
			borrar_nodo(&(prox->izq), NULL); //punteros a memoria dinamica, no locales!
			prox = temp;
		}
		prox->izq = nodo->izq;
		prox->der = nodo->der;
	}

	if (destruir_nodo) destruir_nodo(nodo, NULL);
	*p_nodo = prox;
}

// Busca el nodo recursivamente. Recibe doble puntero al nodo inicial.
// Devuelve un doble puntero al nodo encontrado, o apuntando a NULL si no estaba.
static nodo_abb_t** buscar_lugar(nodo_abb_t** p_nodo, const char* clave, abb_comparar_clave_t cmp)
{
	nodo_abb_t* nodo = *p_nodo; // para facilitar lectura
	if (!nodo || cmp(clave, nodo->clave) == 0) return p_nodo;
	else if (cmp(clave, nodo->clave) < 0) return buscar_lugar(&(nodo->izq), clave, cmp);
	else return buscar_lugar(&(nodo->der), clave, cmp);
}

/*                       Fin de f. auxiliares                      *
 *******************************************************************/

// Crea un Arbol Binario de Busqueda. 
// Recibe una funcion de destruccion y una de comparacion.
// Pre: destruir_dato es una función capaz de destruir
// los datos del abb, o NULL en caso de que no se la utilice.
// cmp es una funcion capaz de comparar claves (devuelve 0 si son 
// iguales, <0 si el izquierdo es menor que el derecho y >0 si es mayor)
// Post: devuelve un abb vacio.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato)
{
	abb_t* arbol = malloc(sizeof(abb_t));
	if (!arbol) return NULL;

	arbol->raiz = NULL;
	arbol->cmp = cmp;
	arbol->destruir_dato = destruir_dato;
	arbol->cantidad = 0;
	return arbol;
}

// Guarda el dato dentro del abb asociandolo a la clave.
// Pre: El abb fue creado.
// Post: devuelve true si pudo guardar, false si no.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato)
{
	nodo_abb_t** punt_a_nodo = buscar_lugar(&(arbol->raiz), clave, arbol->cmp);
	nodo_abb_t* nodo = *punt_a_nodo; //para facilitar lectura.
	if (nodo) //lo encontro
	{
		if (arbol->destruir_dato) arbol->destruir_dato(nodo->dato);
		nodo->dato = dato;
		return true;
	}

	nodo = nodo_crear(clave, dato);
	if (!nodo) return false;

	*punt_a_nodo = nodo; // actualiza todo automatico esto.
	arbol->cantidad++;
	return true;
}

// Borra la clave y devuelve su dato asociado.
// Pre: El abb fue creado.
// Post: Devuelve el dato asociado a la clave o NULL si
// esta no pertenece al abb.
void *abb_borrar(abb_t *arbol, const char *clave)
{
	nodo_abb_t** punt_a_nodo = buscar_lugar(&(arbol->raiz), clave, arbol->cmp);
	if (!(*punt_a_nodo)) return NULL;

	void* temp = (*punt_a_nodo)->dato;
	borrar_nodo(punt_a_nodo, destruir_nodo);
	arbol->cantidad--;
	return temp;
}

// Obtiene el valor asociado a una clave.
// Pre: El abb fue creado.
// Post: Devuelve el dato asociado a la clave o NULL si
// esta no pertenece al abb.
void *abb_obtener(const abb_t *arbol, const char *clave)
{
	nodo_abb_t* raiz = arbol->raiz; //hago esto porque arbol es const.
	nodo_abb_t* nodo = *(buscar_lugar(&raiz, clave, arbol->cmp));
	if (!nodo) return NULL;
	return nodo->dato;
}

// Se fija si una clave esta en el abb.
// Pre: El abb fue creado.
// Post: Devuelve true o false dependiendo de si la clave
// esta o no.
bool abb_pertenece(const abb_t *arbol, const char *clave)
{
	nodo_abb_t* raiz = arbol->raiz; //hago esto porque arbol es const.
	return *(buscar_lugar(&raiz, clave, arbol->cmp)) != NULL;
}

// Devuelve la cantidad de elementos en el abb.
// Pre: El abb fue creado.
// Post: devuelve cuantos elementos tiene el abb, 0 si esta vacio.
size_t abb_cantidad(abb_t *arbol)
{
	return arbol->cantidad;
}

// Destruye el abb.
// Pre: El abb fue creado.
// Post: Se destruye el abb y sus datos, y se libera la memoria.
void abb_destruir(abb_t *arbol)
{
	destruir_nodos_postorder(arbol->raiz, arbol->destruir_dato);
	free(arbol);
}

/*******************************************************************
 *                    IMPLEMENTACION ITERADOR                      *
 ******************************************************************/

/*******************************************************************
 *                       Funciones auxiliares                      */

// Recorre el arbol in order llamando a la funcion visitar para cada 
// dato y clave.
bool iterar_in_order(nodo_abb_t* nodo, visitar_t visitar, void* extra)
{
	if (!nodo) return true;

	if (!iterar_in_order(nodo->izq, visitar, extra)) return false;
	if (!visitar(nodo->clave, nodo->dato, extra)) return false;
	return iterar_in_order(nodo->der, visitar, extra);
}

// Apilara toda la rama izquierda del nodo recibido por parametro.
bool apilar_rama_izq(pila_t* pila, nodo_abb_t* nodo)
{
	while (nodo)
	{
		if (!pila_apilar(pila, nodo)) return false;
		nodo = nodo->izq;
	}
	return true;
}

/*                       Fin de f. auxiliares                      *
 *******************************************************************/

/* INTERNO */

// Itera el abb IN ORDER llamando a la funcion "visitar" por cada elemento de la misma.
// La función de callback "visitar" recibe la clave, el dato y un puntero extra,
// y devuelve true si se debe seguir iterando, false en caso contrario.
// Pre: el abb fue creado. "visitar" es una funcion capaz de utilizar y/o
// modificar los datos del abb, no debe modificar la clave.
void abb_in_order(abb_t *arbol, visitar_t visitar, void *extra)
{
	iterar_in_order(arbol->raiz, visitar, extra);
}

/* EXTERNO */

// Crea un iterador basado en el abb recibido por parametro.
// Pre: El abb fue creado.
// Post: Devuelve una estructura que itera IN ORDER sobre el abb.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol)
{
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if (!iter) return NULL;

	pila_t* pila = pila_crear();
	if (!pila)
	{
		free(iter);
		return NULL;
	}
	if (!apilar_rama_izq(pila, arbol->raiz))
	{
		pila_destruir(pila);
		free(iter);
		return NULL;
	}

	iter->arbol = arbol;
	iter->pila = pila;
	return iter;
}

// Avanza a la siguiente posicion.
// Pre: El abb y el iterador fueron creados.
// Post: Devuelve true o false dependiendo de si pudo
// avanzar o no.
bool abb_iter_in_avanzar(abb_iter_t *iter)
{
	if (abb_iter_in_al_final(iter)) return false;
	return apilar_rama_izq(iter->pila, ((nodo_abb_t*)pila_desapilar(iter->pila))->der);
}

// Devuelve la clave actual.
// Si el iterador se encuentra al final devuelve NULL.
// Pre: El abb y el iterador fueron creados.
// Post: Devuelve la clave en donde esta parado el iterador. No
// se puede modificar.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter)
{
	if (abb_iter_in_al_final(iter)) return NULL;
	return ((nodo_abb_t*)pila_ver_tope(iter->pila))->clave;
}

// Se fija si el iterador esta al final.
// Pre: El abb y el iterador fueron creados.
// Post: Devuele true si esta al final, caso contrario,
// devuelve false.
bool abb_iter_in_al_final(const abb_iter_t *iter)
{
	return pila_esta_vacia(iter->pila);
}

// Destruye el iterador.
// Pre: El abb y el iterador fueron creados.
// Post: Destruye el iterador y libera memoria.
void abb_iter_in_destruir(abb_iter_t* iter)
{
	pila_destruir(iter->pila);
	free(iter);
}