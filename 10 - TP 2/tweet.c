#include <stdlib.h>
#include <stdio.h>
#include "tweet.h"
char *strdup(const char *s);

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 ******************************************************************/

struct tweet{
	char* mensaje;
	char* autor;
	size_t fav;
	size_t id;
};

/********************************************************************
 *                      IMPLEMENTACION TWEET                        *
 *******************************************************************/

// Crea el tweet. Se inicializa con 0 favoritos.
// Recibe el autor, el mensaje y el id.
// Devuelve el tweet o NULL en caso de error.
tweet_t* tweet_crear(const char* autor, const char* mensaje, size_t id)
{
	tweet_t* tweet = malloc(sizeof(tweet_t));
	char* copia_mensaje = strdup(mensaje);
	char* copia_autor = strdup(autor);
	if (!tweet || !copia_autor || !copia_mensaje)
	{
		free(tweet);
		free(copia_mensaje);
		free(copia_autor);
		return NULL;
	}
	tweet->mensaje = copia_mensaje;
	tweet->autor = copia_autor;
	tweet->id = id;
	tweet->fav = 0;
	return tweet;
}

// Devuelve el mensaje del tweet.
// Pre: El tweet fue creado.
const char* tweet_mensaje(const tweet_t* tweet)
{
	return tweet->mensaje;
}

// Devuelve el autor del tweet.
// Pre: El tweet fue creado.
const char* tweet_autor(const tweet_t* tweet)
{
	return tweet->autor;
}

// Devuelve la cantidad de favoritos de un tweet.
// Pre: El tweet fue creado.
size_t tweet_favorito(const tweet_t* tweet)
{
	return tweet->fav;
}

// Devuelve el id del tweet.
// Pre: El tweet fue creado.
size_t tweet_id(const tweet_t* tweet)
{
	return tweet->id;
}

// Incrementa en 1 los favoritos del tweet.
// Pre: El tweet fue creado.
void tweet_incrementar_favorito(tweet_t* tweet)
{  
	tweet->fav++;
}

// Imprime el tweet por salida estandar.
// Se imprime siguiendo el orden: id, autor, mensaje.
// Pre: El tweet fue creado.
void tweet_imprimir(const tweet_t* tweet)
{
	printf("%zu %s %s\n", tweet->id, tweet->autor, tweet->mensaje);
}

// Destruye el tweet.
// Pre: El tweet fue creado.
void tweet_destruir(tweet_t* tweet)
{
	free(tweet->mensaje);
	free(tweet->autor);
	free(tweet);
}
