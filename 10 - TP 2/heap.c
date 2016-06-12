#include "heap.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define TAM_INICIAL 50
#define FACTOR_MAX 0.9
#define FACTOR_MIN 0.3

#define HIJO_IZQ(padre) (size_t)((padre * 2) + 1)
#define HIJO_DER(padre) (size_t)((padre * 2) + 2)
#define PADRE(hijo) (size_t)((hijo - 1) / 2)

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 ******************************************************************/

struct heap{
	void** datos;
	size_t cant;
	size_t tam;
	cmp_func_t heap_cmp;
};

/*******************************************************************
 *                        FUNCIONES AUXILIARES                     *
 ******************************************************************/

// Se redimensiona el heap. Devuelve true o false si no puede.
// Pre: el heap fue creado.
// Post: tam_nuevo es el nuevo tamaño del heap.
bool heap_redimensionar(heap_t* heap, size_t tam_nuevo){
	void** datos_nuevo = realloc(heap->datos, tam_nuevo * sizeof(void*));
	if (!datos_nuevo) return false;

	heap->datos = datos_nuevo;
	heap->tam = tam_nuevo;
	return true;
}

/* La funcion swap intercambia dos valores. */
void swap(void *datos[], size_t i, size_t j){
    void* aux = datos[i];
    datos[i] = datos[j];
    datos[j] = aux;
}

// Efectua el downheap en el arreglo datos a partir de la pos_inicial.
// Pre: pos_inicial esta en el rango de 0-cant.
void downheap(void** datos, size_t cant, size_t pos_inicial, cmp_func_t cmp){
	size_t padre = pos_inicial, hijo_mayor;

	while ((hijo_mayor = HIJO_IZQ(padre)) < cant) //hijo_mayor = hijo izquierdo
	{
		if (hijo_mayor + 1 < cant &&  // hijo derecho existe -> comparo
			cmp(datos[hijo_mayor + 1], datos[hijo_mayor]) > 0) hijo_mayor++;

		if (cmp(datos[padre], datos[hijo_mayor]) >= 0) break;

		swap(datos, padre, hijo_mayor);
		padre = hijo_mayor;
	}
}

/*******************************************************************
 *                        IMPLEMENTACION                           *
 ******************************************************************/

/* Crea un heap. Recibe como único parámetro la función de comparación a
 * utilizar. Devuelve un puntero al heap, el cual debe ser destruido con
 * heap_destruir(). 
 */
heap_t *heap_crear(cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));
	void** datos = malloc(TAM_INICIAL * sizeof(void*));
	if (!heap || !datos){
		free(heap);
		free(datos);
		return NULL;
	}

	heap->cant = 0;
	heap->tam = TAM_INICIAL;
	heap->heap_cmp = cmp;
	heap->datos = datos;
	return heap;
}

/* Elimina el heap, llamando a la función dada para cada elemento del mismo.
 * El puntero a la función puede ser NULL, en cuyo caso no se llamará.
 * Post: se llamó a la función indicada con cada elemento del heap. El heap
 * dejó de ser válido. */
void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
	if (destruir_elemento)
		for (size_t i = 0; i < heap->cant; i++)
			destruir_elemento(heap->datos[i]);
	
	free(heap->datos);
	free(heap);
}

/* Devuelve la cantidad de elementos que hay en el heap. */
size_t heap_cantidad(const heap_t *heap){
	return heap->cant;
}

/* Devuelve true si la cantidad de elementos que hay en el heap es 0, false en
 * caso contrario. */
bool heap_esta_vacio(const heap_t *heap){
	return heap->cant == 0;
}

/* Agrega un elemento al heap. El elemento no puede ser NULL.
 * Devuelve true si fue una operación exitosa, o false en caso de error. 
 * Pre: el heap fue creado.
 * Post: se agregó un nuevo elemento al heap.
 */
bool heap_encolar(heap_t *heap, void *elem){
	if (heap->cant > heap->tam * FACTOR_MAX)
		if(!heap_redimensionar(heap, heap->tam * 3)) return false;
	
	// Upheap
	size_t hijo = heap->cant++;
	size_t padre = PADRE(hijo);
	while (hijo > 0 && heap->heap_cmp(elem, heap->datos[padre]) > 0){
		heap->datos[hijo] = heap->datos[padre];
		hijo = padre;
		padre = PADRE(hijo);
	}
	heap->datos[hijo] = elem;
	return true;
}

/* Devuelve el elemento con máxima prioridad. Si el heap esta vacío, devuelve
 * NULL. 
 * Pre: el heap fue creado.
 */
void *heap_ver_max(const heap_t *heap){
	if (heap_esta_vacio(heap)) return NULL;
	return heap->datos[0];
}

/* Elimina el elemento con máxima prioridad, y lo devuelve.
 * Si el heap esta vacío, devuelve NULL.
 * Pre: el heap fue creado.
 * Post: el elemento desencolado ya no se encuentra en el heap. 
 */
void *heap_desencolar(heap_t *heap){
	if (heap_esta_vacio(heap)) return NULL;

	void* elem_max = heap->datos[0];
	heap->datos[0] = heap->datos[--heap->cant];
	downheap(heap->datos, heap->cant, 0, heap->heap_cmp);

	if (heap->tam / 2 > TAM_INICIAL && heap->cant < heap->tam * FACTOR_MIN)
		heap_redimensionar(heap, heap->tam / 2);

	return elem_max;
}

/* Función de heapsort genérica. Esta función ordena mediante heap_sort
 * un arreglo de punteros opacos, para lo cual requiere que se
 * le pase una función de comparación. Modifica el arreglo "in-place".
 * Notar que esta función NO es formalmente parte del TAD Heap.
 */
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	if (cant <= 1) return;
	// heapify!
	size_t i = PADRE(cant - 1); // ultimo padre
	do { downheap(elementos, cant, i, cmp);
	} while (i--);

	// ordenar el arreglo de heap_max
	for (i = cant - 1; i > 0; i--)
	{
		swap(elementos, 0, i); // (nuevo) maximo al final
		downheap(elementos, i, 0, cmp);
	}
}