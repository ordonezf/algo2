#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pila.h"

#define TAM_MIN 50 // Tamanio minimo e inicial de la pila

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct _pila
{
	void** datos;
	size_t tamanio;
	size_t cantidad;
};

/* *****************************************************************
 *                        IMPLEMENTACION
 * *****************************************************************/

// Crea una pila.
// Post: devuelve una nueva pila vacía.
pila_t* pila_crear()
{
	pila_t* pila = malloc(sizeof(pila_t));
	if (pila == NULL)
		return NULL;
		
	pila->datos = malloc(TAM_MIN * sizeof(void*));
	if (pila->datos == NULL)
	{
		free(pila);
		return NULL;
	}
	
	pila->tamanio = TAM_MIN;
	pila->cantidad = 0;
	return pila;
}

// Redimensiona la pila al nuevo tamanio.
// nuevo_tamanio debe ser mayor o igual a TAM_MIN, definido arriba
// Devuelve verdadero o falso dependiendo de su resultado.
// Si es verdadero pila contendrá el puntero a datos actualizado
// Si es falso, en cambio, se mantendrá su estado original.
// Pre: la pila fue creada
// Post: la pila es de tamanio igual a nuevo_tamanio
bool pila_redimensionar(pila_t *pila, size_t nuevo_tamanio)
{
	if (nuevo_tamanio < TAM_MIN) return false;
	
	void** re_datos = realloc(pila->datos, nuevo_tamanio * sizeof(void*));
	
	if (re_datos == NULL) return false; // nuevo_tamanio nunca deberia ser 0
		
	pila->datos = re_datos;
	pila->tamanio = nuevo_tamanio;
	return true;
}

// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.
void pila_destruir(pila_t *pila)
{
	free(pila->datos);
	free(pila);
}

// Devuelve verdadero o falso, según si la pila tiene o no elementos apilados.
// Pre: la pila fue creada.
bool pila_esta_vacia(const pila_t *pila)
{
	return pila->cantidad == 0;
}

// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void *valor)
{
	if (pila->cantidad == pila->tamanio) // esta lleno
	{
		if (!pila_redimensionar(pila, pila->tamanio * 2))
			return false;
	}
	
	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}

// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope. Si está vacía devuelve NULL.
// Pre: la pila fue creada.
// Post: se devolvió el valor del tope de la pila, cuando la pila no está
// vacía, NULL en caso contrario.
void* pila_ver_tope(const pila_t *pila)
{
	if (pila->cantidad == 0) return NULL;
	
	return pila->datos[(pila->cantidad)-1];
}

// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve 
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacía, se devuelve el valor del tope anterior 
// y la pila contiene un elemento menos.
void* pila_desapilar(pila_t *pila)
{
	if (pila->cantidad == 0) return NULL;
		
	if (pila->tamanio / 4.0 >= pila->cantidad)
		pila_redimensionar(pila, pila->tamanio / 2);
	
	pila->cantidad--;
	return pila->datos[pila->cantidad]; //ya que achicamos antes no necesitamos restar 1
}
