#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "hash.h"
#include "lista.h"
char *strdup(const char *s);

#define TAM_INICIAL 97
#define PROMEDIO_IDEAL 0.8
#define MAX_FACTOR_CARGA 2.0
#define MIN_FACTOR_CARGA 0.4

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 ******************************************************************/

typedef struct nodo_hash{
	char* clave;
	void* dato;
} nodo_hash_t;

struct hash{
 	lista_t** listas;
 	size_t tam;
 	size_t cant;
 	hash_destruir_dato_t destruir_dato;
};

struct hash_iter{
	lista_iter_t* lista_iter;
	size_t indice_actual;
	const hash_t* hash;
};

typedef void (*dest_nodo)(nodo_hash_t*, hash_destruir_dato_t);

 /*******************************************************************
 *                        IMPLEMENTACION HASH                       *
 *******************************************************************/

 /*******************************************************************
 *                       Funciones auxiliares                      */

// Recibe el nodo y opcionalmente una funcion de destruccion.
// Destruye el nodo.
void destruir_nodo(nodo_hash_t* nodo, hash_destruir_dato_t destruir_dato)
{
	if (destruir_dato) destruir_dato(nodo->dato);
	free(nodo->clave);
	free(nodo);
}

// Efectua el hashing sobre la clave.
// Devuelve un indice.
// Cortesia de DEKHash.
size_t f_hash(const char* clave, size_t largo, size_t tam)
{
	size_t h = largo;
	
	for(size_t i = 0; i < largo; clave++, i++)
		h = ((h << 5) ^ (h >> 27)) ^ (*clave);
	
	return h % tam;
}

// Busca un nodo con la clave especificada.
// Si borrar_de_lista es true, se borrara el elemento de la lista.
// y se eliminara la lista si esta vacia.
// Post: Devuelve el nodo o NULL si no lo encontro.
nodo_hash_t* buscar_nodo(const hash_t* hash, const char* clave, bool borrar_de_lista)
{
	size_t indice = f_hash(clave, strlen(clave), hash->tam);
	if (!hash->listas[indice]) return NULL; // no hay lista todavia.

	lista_iter_t* iter = lista_iter_crear(hash->listas[indice]);
	if (!iter) return NULL; //feo esto

	while (!lista_iter_al_final(iter) && 
			(strcmp(((nodo_hash_t*)lista_iter_ver_actual(iter))->clave, clave) != 0))
        lista_iter_avanzar(iter);

    nodo_hash_t* nodo = lista_iter_ver_actual(iter); //si es null no molesta.
    if (borrar_de_lista)
    {
    	lista_borrar(hash->listas[indice], iter);
    	if (lista_esta_vacia(hash->listas[indice]))
    	{
    		lista_destruir(hash->listas[indice], NULL);
    		hash->listas[indice] = NULL;
    	}
    }
    lista_iter_destruir(iter);
	return nodo;
}

// Destruye el arreglo de listas.
// Recibe opcionalmente funcion de destruccion de nodos y de datos.
void destruir_listas(lista_t** listas, size_t tam, dest_nodo destruir_nodo, hash_destruir_dato_t destruir_dato)
{
	for (size_t i = 0; i < tam; i++)
	{
		if (!listas[i]) continue; // No es lista.

		while (!lista_esta_vacia(listas[i]))
		{
			nodo_hash_t* nodo = lista_borrar_primero(listas[i]);
			if (destruir_nodo) destruir_nodo(nodo, destruir_dato);
		}
		lista_destruir(listas[i], NULL);
	}
	free(listas);
}

void determinar_redimension(hash_t* hash);

 /*                       Fin de f. auxiliares                     *
 *******************************************************************/

// Crea un hash. Recibe una funcion de destruccion
// Pre: destruir_dato es una función capaz de destruir
// los datos del hash, o NULL en caso de que no se la utilice.
// Post: devuelve un hash vacio.
hash_t *hash_crear(hash_destruir_dato_t destruir_dato)
{
	hash_t* hash = malloc(sizeof(hash_t));
	lista_t** listas = calloc(TAM_INICIAL, sizeof(lista_t*));
	if (!hash || !listas){
	    free(hash);
	    free(listas);
	    return NULL;
	}

	hash->tam = TAM_INICIAL;
	hash->cant = 0;
	hash->listas = listas;
	hash->destruir_dato = destruir_dato;
	return hash;
}

// Guarda el dato dentro del hash asociandolo a la clave.
// Pre: El hash fue creado.
// Post: devuelve true si pudo guardar, false si no.
bool hash_guardar(hash_t *hash, const char *clave, void *dato)
{
	nodo_hash_t* nodo = buscar_nodo(hash, clave, false);
	if (nodo)
	{
		if (hash->destruir_dato) hash->destruir_dato(nodo->dato);
		nodo->dato = dato;
		return true;
	}

	// Si estamos aca, entonces no estaba la clave
	size_t indice = f_hash(clave, strlen(clave), hash->tam);

	if (!hash->listas[indice]) //no existia la lista todavia.
	{
		hash->listas[indice] = lista_crear();
		if (!hash->listas[indice]) return false;
	}
	nodo = malloc(sizeof(nodo_hash_t));
	char* copia_clave = strdup(clave);
	if (!nodo || !copia_clave){
		free(nodo);
		free(copia_clave);
		if (lista_esta_vacia(hash->listas[indice])) // la creamos recien
			lista_destruir(hash->listas[indice], NULL);
		return false;	
	}

	nodo->clave = copia_clave;
	nodo->dato = dato;
	lista_insertar_ultimo(hash->listas[indice], nodo);
	hash->cant++;
	determinar_redimension(hash);
	return true;
}

// Borra la clave y devuelve su dato asociado.
// Pre: El hash fue creado.
// Post: Devuelve el dato asociado a la clave o NULL si
// esta no pertenece al hash.
void* hash_borrar(hash_t *hash, const char *clave)
{
	nodo_hash_t* nodo = buscar_nodo(hash, clave, true);
	if (!nodo) return NULL;
	void* dato = nodo->dato;
	destruir_nodo(nodo, NULL);
	hash->cant--;
	return dato;
}

// Obtiene el valor asociado a una clave.
// Pre: El hash fue creado.
// Post: Devuelve el dato asociado a la clave o NULL si
// esta no pertenece al hash.
void *hash_obtener(const hash_t *hash, const char *clave)
{
	nodo_hash_t* nodo = buscar_nodo(hash, clave, false);
	if (!nodo) return NULL;
	return nodo->dato;
}

// Se fija si una clave esta en el hash.
// Pre: El hash fue creado.
// Post: Devuelve true o false dependiendo de si la clave
// esta o no.
bool hash_pertenece(const hash_t *hash, const char *clave)
{
	return buscar_nodo(hash, clave, false) != NULL;
}

// Devuelve la cantidad de elementos en el hash.
// Pre: El hash fue creado.
// Post: devuelve cuantos elementos tiene el hash, 0 si esta vacio.
size_t hash_cantidad(const hash_t *hash)
{
	return hash->cant;
}

// Destruye el hash.
// Pre: El hash fue creado.
// Post: Se destruye el hash y sus datos, y se libera la memoria.
void hash_destruir(hash_t *hash)
{
	destruir_listas(hash->listas, hash->tam, destruir_nodo, hash->destruir_dato);
	free(hash);
}

 /*******************************************************************
 *                      IMPLEMENTACION ITERADOR                     *
 *******************************************************************/

 /*******************************************************************
 *                       Funciones auxiliares                      */

// Recibe el iterador y un indice indicando desde donde comenzar a buscar
// la proxima lista.
// Modificara el iterador para seleccionar al proximo iter de lista.
bool seleccionar_proximo_lista_iter(hash_iter_t* iter, size_t indice_principio)
{
	for (size_t i = indice_principio; i < iter->hash->tam; i++)
	{
		if (!iter->hash->listas[i]) continue; //No es lista

		lista_iter_t* nuevo_lista_iter = lista_iter_crear(iter->hash->listas[i]);
		if (!nuevo_lista_iter) return false;

		if (iter->lista_iter) lista_iter_destruir(iter->lista_iter);
		iter->lista_iter = nuevo_lista_iter;
		iter->indice_actual = i;
		return true;
	}
	// LLegar aca significa llegar al final.
	iter->indice_actual = iter->hash->tam;
	if (iter->lista_iter) lista_iter_destruir(iter->lista_iter);
	iter->lista_iter = NULL;
	return true;
}

 /*                       Fin de f. auxiliares                     *
 *******************************************************************/

// Crea un iterador basado en el hash recibido por parametro.
// Pre: El hash fue creado.
// Post: Devuelve una estructura que itera sobre el hash.
hash_iter_t *hash_iter_crear(const hash_t *hash)
{
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if (!iter) return NULL;

	iter->hash = hash;
	iter->indice_actual = 0;
	iter->lista_iter = NULL;

	if (hash_cantidad(hash) == 0) iter->indice_actual = hash->tam; //iter "al final"
	else if (!seleccionar_proximo_lista_iter(iter, iter->indice_actual))
	{
		free(iter);
		return NULL;
	}
	return iter;
}

// Avanza a la siguiente posicion.
// Pre: El hash y el iterador fueron creados.
// Post: Devuelve true o false dependiendo de si pudo
// avanzar o no.
bool hash_iter_avanzar(hash_iter_t *iter)
{	
	if (hash_iter_al_final(iter)) return false;
	lista_iter_avanzar(iter->lista_iter);
	if (!lista_iter_al_final(iter->lista_iter)) return true;
	return seleccionar_proximo_lista_iter(iter, iter->indice_actual + 1);
}

// Devuelve la clave actual.
// Si el iterador se encuentra al final devuelve NULL.
// Pre: El hash y el iterador fueron creados.
// Post: Devuelve la clave en donde esta parado el iterador. No
// se puede modificar.
const char *hash_iter_ver_actual(const hash_iter_t *iter)
{
	if (hash_iter_al_final(iter)) return NULL;
	return ((nodo_hash_t*)lista_iter_ver_actual(iter->lista_iter))->clave;
}

// Se fija si el iterador esta al final.
// Pre: El hash y el iterador fueron creados.
// Post: Devuele true si esta al final, caso contrario,
// devuelve false.
bool hash_iter_al_final(const hash_iter_t *iter)
{
	return iter->indice_actual == iter->hash->tam && iter->lista_iter == NULL;
}

// Destruye el iterador.
// Pre: El hash y el iterador fueron creados.
// Post: Destruye el iterador y libera memoria.
void hash_iter_destruir(hash_iter_t* iter)
{
	if (iter->lista_iter) lista_iter_destruir(iter->lista_iter);
	free(iter);
}

 /*******************************************************************
 *                      AUXILIAR: REDIMENSION                       *
 *******************************************************************/

bool redimensionar(hash_t* hash, size_t nuevo_tam)
{
	lista_t** listas_nuevas = calloc(nuevo_tam, sizeof(lista_t*));
	if (!listas_nuevas) return false;

	hash_iter_t* iter = hash_iter_crear(hash);
	if (!iter) 
	{
		free(listas_nuevas);
		return false;
	}

	while (!hash_iter_al_final(iter))
	{
		nodo_hash_t* nodo = lista_iter_ver_actual(iter->lista_iter);
		size_t indice = f_hash(nodo->clave, strlen(nodo->clave), nuevo_tam);

		if (!listas_nuevas[indice]) //no existia la lista todavia.
		{
			listas_nuevas[indice] = lista_crear();
			if (!listas_nuevas[indice]) // fallo, borro lo nuevo manteniendo lo viejo.
			{
				destruir_listas(listas_nuevas, nuevo_tam, NULL, NULL); //sin dest. nodos
				return false;	
			}
		}
		lista_insertar_ultimo(listas_nuevas[indice], nodo);
		hash_iter_avanzar(iter);
	}
	destruir_listas(hash->listas, hash->tam, NULL, NULL); //sin dest. nodos!
	hash_iter_destruir(iter);
	hash->listas = listas_nuevas;
	hash->tam = nuevo_tam;
	return true;
}

void determinar_redimension(hash_t* hash)
{
	float factor_de_carga = hash->cant / (float)hash->tam;
	if (MIN_FACTOR_CARGA < factor_de_carga &&
		factor_de_carga < MAX_FACTOR_CARGA) return;

	size_t nuevo_tam = hash->cant / PROMEDIO_IDEAL;
	if (nuevo_tam < TAM_INICIAL) return;
	
	redimensionar(hash, nuevo_tam);
}