#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "twitter.h"
#include "tweet.h"
#include "heap.h"
#include "hash.h"
#include "lista.h"

#define TAM_INICIAL 100

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct twitter {
	hash_t* palabras;
	tweet_t** twits;
	size_t tam_twits;
	size_t cant_twits;
};

/* ******************************************************************
 *                        IMPLEMENTACION
 * *****************************************************************/

 /*******************************************************************
 *                       Funciones auxiliares                      */

// Redimensiona el arreglo de twits. Devuelve bool.
// Pre: Los twits fueron creados.
// Post: tam_nuevo es el nuevo tamaño del arreglo de twits.
static bool _redimensionar_twits(twitter_t* twitter, size_t tam_nuevo)
{
	tweet_t** twits_nuevo = realloc(twitter->twits, tam_nuevo * sizeof(tweet_t*));
	if (!twits_nuevo) return false;

	twitter->twits = twits_nuevo;
	twitter->tam_twits = tam_nuevo;
	return true;
}

// Funcion de destruccion de listas sin destruir los elementos de la misma.
static void wrapper_lista_destruir(void* lista)
{
	lista_destruir((lista_t*)lista, NULL);
}

// Agregara al hash, con la palabra como clave, el tweet.
// Si es necesario creará la lista y, si la lista ya existia,
// comprobará si el tweet ya se encontraba para no repetirlo.
// Devuelve false en caso de error.
static bool agregar_al_hash_sin_repetir(hash_t* palabras, char* palabra, tweet_t* tweet)
{
	lista_t* lista_de_tweets = hash_obtener(palabras, palabra);
	if (!lista_de_tweets)
	{
		lista_de_tweets = lista_crear();
		if (!lista_de_tweets) return false;
		hash_guardar(palabras, palabra, (void*)lista_de_tweets);
	}
	else // ya existia (si o si tiene algo), verificar por repeticion.
		if ((tweet_t*)lista_ver_primero(lista_de_tweets) == tweet) return true;

	if (!lista_insertar_primero(lista_de_tweets, (void*)tweet)) return false;
	return true;
}

// Pensado para utilizar con lista_iterar.
// Imprimira el tweet por salida estandar si la cantidad es mayor a 0.
// Si la cantidad es 0, devolvera false para terminar la iteracion.
// En cambio disminuira en 1 la cantidad y devolvera true.
static bool imprimir_tweets(tweet_t* tweet, size_t* cantidad)
{
	if (!((*cantidad)--)) return false;
	tweet_imprimir(tweet);
	return true;
}

// Funcion de comparacion para el heap. (especificamente heap_min)
// Recibe punteros a tweet y compara los favoritos de cada uno.
// Devuelve positivo si A es menor a B, 0 si son iguales
// y negativo si A es mayor a B.
static int comparar_favoritos(const tweet_t* tweet_A, const tweet_t* tweet_B)
{
	return (long)tweet_favorito(tweet_B) - (long)tweet_favorito(tweet_A);
}

/*                       Fin de f. auxiliares                      *
 *******************************************************************/

// Crea el twitter.
// Post: Devuelve el twitter o NULL en caso de error.
twitter_t* twitter_crear()
{
	twitter_t* twitter = malloc(sizeof(twitter_t));
	tweet_t** twits = malloc(TAM_INICIAL * sizeof(tweet_t*));
	if (!twitter || !twits){
		free(twitter);
		free(twits);
		return NULL;
	}

	hash_t* palabras = hash_crear(wrapper_lista_destruir);
	if (!palabras){
		free(twitter);
		free(twits);
		return NULL;
	}

	twitter->twits = twits;
	twitter->tam_twits = TAM_INICIAL;
	twitter->cant_twits = 0;
	twitter->palabras = palabras;
	return twitter;
}

// Destruye el twitter.
// Pre: Twitter fue creado.
void twitter_destruir(twitter_t* twitter)
{
	for (size_t i = 0; i < twitter->cant_twits; i++) tweet_destruir(twitter->twits[i]);
	hash_destruir(twitter->palabras);
	free(twitter->twits);
	free(twitter);
}

// Crea un nuevo tweet con el autor y mensaje especificado.
// Pre: Twitter fue creado.
// Post: Devuelve false en caso de error.
bool twitter_twittear(twitter_t* twitter, char* autor, char* mensaje)
{
	if (twitter->cant_twits == twitter->tam_twits) 
		if (!_redimensionar_twits(twitter, twitter->tam_twits * 3)) return false;

	size_t id = twitter->cant_twits++;
	tweet_t* tweet = tweet_crear(autor, mensaje, id);
	if (!tweet) return false;

	if (!agregar_al_hash_sin_repetir(twitter->palabras, autor, tweet)) return false;
	char* palabra = strtok(mensaje, " ");
	while (palabra)
	{
		if (palabra[0] == '@' || palabra[0] == '#')
			if (!agregar_al_hash_sin_repetir(twitter->palabras, palabra, tweet))
				return false;
		palabra = strtok(NULL, " ");
	}

	twitter->twits[id] = tweet;
	printf("OK %zu\n", id);
	return true;
}

// Incrementa la cantida de favoritos del tweet con el id especficado.
// Pre: Twitter fue creado.
// Post: Devuelve false en caso de ser un id invalido.
bool twitter_favorito(twitter_t* twitter, size_t id)
{
	if (id >= twitter->cant_twits) return false;
	tweet_incrementar_favorito(twitter->twits[id]);
	printf("OK %zu\n", id);
	return true;
}

// Lleva a cabo la busqueda cronologica del termino especificado.
// Recibe el usuario o hashtag a buscar y la cantidad de resultados.
// Si cantidad es 0 se listaran todos los resultados.
// Imprimira por salida estandar los resultados en sentido cronologico.
// Pre: Twitter fue creado.
void twitter_buscar_cronologico(twitter_t* twitter, char* busqueda, size_t cantidad)
{
	lista_t* lista = hash_obtener(twitter->palabras, busqueda);
	if (lista){
		size_t max_cant = (cantidad == 0 || cantidad > lista_largo(lista))? lista_largo(lista): cantidad;
		printf("OK %zu\n", max_cant);
		lista_iterar(lista, (bool(*)(void*, void*))imprimir_tweets, &max_cant);
	}
	else printf("OK 0\n");
}

// Lleva a cabo la busqueda por popularidad del termino especificado.
// Recibe el usuario o hashtag a buscar y la cantidad de resultados.
// Si cantidad es 0 se listaran todos los resultados.
// Imprimira por salida estandar los resultados de mayor a menor en popularidad.
// Pre: Twitter fue creado.
// Post: Devuelve false en caso de error.
bool twitter_buscar_popular(twitter_t* twitter, char* busqueda, size_t cantidad)
{
	lista_t* lista = hash_obtener(twitter->palabras, busqueda);
	if (!lista){
		printf("OK 0\n");
		return true;
	}
	size_t max_cant = (cantidad == 0 || cantidad > lista_largo(lista))? lista_largo(lista): cantidad;

	lista_iter_t* iter = lista_iter_crear(lista);
	if (!iter) return false;

	heap_t* heap = heap_crear((int(*)(const void*, const void*))comparar_favoritos);
	if (!heap){
		lista_iter_destruir(iter);
		return false;
	}

	for (size_t i = 0; !lista_iter_al_final(iter); i++, lista_iter_avanzar(iter))
	{
		if (i >= max_cant) // Comparar con el tope
		{
			tweet_t* tweet = lista_iter_ver_actual(iter);
			if (tweet_favorito(tweet) > tweet_favorito((tweet_t*)heap_ver_max(heap)))
				heap_desencolar(heap);
			else
				continue;
		}
		if (!heap_encolar(heap, lista_iter_ver_actual(iter)))
		{
			lista_iter_destruir(iter);
			heap_destruir(heap, NULL);
			return false;
		}
	}
	lista_iter_destruir(iter);

	tweet_t** populares = malloc(max_cant * sizeof(tweet_t*));
	if (!populares){
		heap_destruir(heap, NULL);
		return false;
	}

	for (size_t i = max_cant; i--;)
		populares[i] = heap_desencolar(heap);
	
	printf("OK %zu\n", max_cant);
	for (size_t i = 0; i < max_cant; i++)
		tweet_imprimir(populares[i]);

	heap_destruir(heap, NULL);
	free(populares);
	return true;
}

