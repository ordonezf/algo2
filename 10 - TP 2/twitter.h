#ifndef TWITTER_H
#define TWITTER_H

#include <stdbool.h>
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct twitter twitter_t;

/* ******************************************************************
 *                    PRIMITIVAS DEL TWITTER
 * *****************************************************************/

// Crea el twitter.
// Post: Devuelve el twitter o NULL en caso de error.
twitter_t* twitter_crear();

// Destruye el twitter.
// Pre: Twitter fue creado.
void twitter_destruir(twitter_t* twitter);

// Crea un nuevo tweet con el autor y mensaje especificado.
// Pre: Twitter fue creado.
// Post: Devuelve false en caso de error.
bool twitter_twittear(twitter_t* twitter, char* autor, char* mensaje);

// Incrementa la cantida de favoritos del tweet con el id especficado.
// Pre: Twitter fue creado.
// Post: Devuelve false en caso de ser un id invalido.
bool twitter_favorito(twitter_t* twitter, size_t id);

// Lleva a cabo la busqueda cronologica del termino especificado.
// Recibe el usuario o hashtag a buscar y la cantidad de resultados.
// Si cantidad es 0 se listaran todos los resultados.
// Imprimira por salida estandar los resultados en sentido cronologico.
// Pre: Twitter fue creado.
void twitter_buscar_cronologico(twitter_t* twitter, char* busqueda, size_t cantidad);

// Lleva a cabo la busqueda por popularidad del termino especificado.
// Recibe el usuario o hashtag a buscar y la cantidad de resultados.
// Si cantidad es 0 se listaran todos los resultados.
// Imprimira por salida estandar los resultados de mayor a menor en popularidad.
// Pre: Twitter fue creado.
// Post: Devuelve false en caso de error.
bool twitter_buscar_popular(twitter_t* twitter, char* busqueda, size_t cantidad);

#endif //TWITTER_H