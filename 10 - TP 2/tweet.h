#ifndef TWEET_H
#define TWEET_H

#include <stdlib.h>

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 ******************************************************************/

typedef struct tweet tweet_t;

/********************************************************************
 *                      PRIMITIVAS DEL TWEET                        *
 *******************************************************************/

// Crea el tweet. Se inicializa con 0 favoritos.
// Recibe el autor, el mensaje y el id.
// Devuelve el tweet o NULL en caso de error.
tweet_t* tweet_crear(const char* autor, const char* mensaje, size_t id);

// Devuelve el mensaje del tweet.
// Pre: El tweet fue creado.
const char* tweet_mensaje(const tweet_t* tweet);

// Devuelve el autor del tweet.
// Pre: El tweet fue creado.
const char* tweet_autor(const tweet_t* tweet);

// Devuelve la cantidad de favoritos de un tweet.
// Pre: El tweet fue creado.
size_t tweet_favorito(const tweet_t* tweet);

// Devuelve el id del tweet.
// Pre: El tweet fue creado.
size_t tweet_id(const tweet_t* tweet);

// Incrementa en 1 los favoritos del tweet.
// Pre: El tweet fue creado.
void tweet_incrementar_favorito(tweet_t* tweet);

// Imprime el tweet por salida estandar.
// Se imprime siguiendo el orden: id, autor, mensaje.
// Pre: El tweet fue creado.
void tweet_imprimir(const tweet_t* tweet);

// Destruye el tweet.
// Pre: El tweet fue creado.
void tweet_destruir(tweet_t* tweet);

#endif // TWEET_H