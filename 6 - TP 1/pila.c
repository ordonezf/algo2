#include "pila.h"
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
struct _pila
{
    void** datos;
    size_t tamanio;
    size_t cantidad;
};


/*******************************************************************
 *                        FUNCIONES AUXILIARES                     *
 ******************************************************************/
 // Cambia el tamanio de la pila
 // Pre: la pila fue creada
 // Post: la pila cambio de tamanio a nuevo_tam y devuelve true
 // o la pila queda intacta y devuelve false si no se pudo cambiar el tamanio
 // a nuevo_tam
bool pila_redimensionar(pila_t *pila, size_t tam_nuevo){
	void* datos_nuevo = realloc(pila->datos, tam_nuevo * sizeof(void*));
	if (datos_nuevo == NULL)
	    return false;

	pila->datos = datos_nuevo;
	pila->tamanio = tam_nuevo;
	return true;
}


/*******************************************************************
 *                        IMPLEMENTACION                           *
 ******************************************************************/

 // Crea una pila.
 // Post: devuelve una nueva pila vacía.
 pila_t* pila_crear(){
	pila_t* pila = malloc(sizeof(pila_t));
	if (pila == NULL) return NULL;

	pila->tamanio = 10;
	pila->cantidad = 0;
	void** datos = malloc(pila->tamanio * sizeof(void*));
	if (datos == NULL)
	{
	    free(pila);
	    return NULL;
	}
	pila->datos = datos;
	return pila;
 }

// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.
void pila_destruir(pila_t *pila){
	free(pila->datos);
	free(pila);
}

// Devuelve verdadero o falso, según si la pila tiene o no elementos apilados.
// Pre: la pila fue creada.
bool pila_esta_vacia(const pila_t *pila){
	if (pila->cantidad == 0)
		return true;
	else
		return false;
}

// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void *valor){
	int tam_limite = pila->tamanio * 0.8;

	//Si cantidad supera el 80% de tamanio, se duplica la memoria disponible.
	if (pila->cantidad > tam_limite){
		if (! pila_redimensionar(pila, pila->tamanio * 2))
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
void* pila_ver_tope(const pila_t *pila){
	if (pila->cantidad == 0)
		return NULL;
	else
		return pila->datos[pila->cantidad - 1];

}

// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve 
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacía, se devuelve el valor del tope anterior 
// y la pila contiene un elemento menos.
void* pila_desapilar(pila_t *pila){
	if (pila->cantidad == 0)
		return NULL;
	
	pila->cantidad -= 1;
	int tam_limite = pila->tamanio * 0.3;

	//Si cantidad es menor que el 30% de tamanio, se disminuye
	//la memoria disponible a la mitad.
	if(pila->cantidad < tam_limite){
		size_t nuevo_tam = pila->tamanio * 0.5;
		pila_redimensionar(pila, nuevo_tam);
	}
	return pila->datos[pila->cantidad];
}
