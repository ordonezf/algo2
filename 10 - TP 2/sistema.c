#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"
#include "twitter.h"

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef enum err // "Codigos" de error
{
	NO_ERROR, ERROR_COMANDO_INVALIDO, ERROR_TWIT_ID_INVALIDO, 
	ERROR_TWIT_DEMASIADO_LARGO, MEM_ERROR
} errores;

const char* mensajes_error[] = // Mensajes asociados al codigo de error
{	
	"OK", "ERROR_COMANDO_INVALIDO", "ERROR_TWIT_ID_INVALIDO", 
	"ERROR_TWIT_DEMASIADO_LARGO", "ERROR: memoria insuficiente. Cerrando"
};

// firma de las funciones asociadas a los comandos
typedef errores (*COMANDO)(sistema_t*, char**, size_t);

struct sistema {
	twitter_t* twitter;
	COMANDO funcion_actual;
};

/* ******************************************************************
 *                        IMPLEMENTACION
 * *****************************************************************/

// Crea el sistema de comandos.
// Post: Devuelve el sistema o NULL en caso de error.
sistema_t* sistema_crear()
{
	sistema_t* sistema = malloc(sizeof(sistema_t));
	if (!sistema) return NULL;

	twitter_t* twitter = twitter_crear();
	if (!twitter){
		free(sistema);
		return NULL;
	}

	sistema->twitter = twitter;
	sistema->funcion_actual = NULL;
	return sistema;
}

// Destruye el sistema de comandos
// Pre: el sistema fue creado.
void sistema_destruir(sistema_t* sistema)
{
	twitter_destruir(sistema->twitter);
	free(sistema);
}

/* ******************************************************************
 *                          COMANDOS 
 * *****************************************************************/

#define MAX_LONG_MENSAJE 150
#define CANT_PARAM_TWITTEAR 2
#define POS_AUTOR 0
#define POS_MENSAJE 1
// Lleva a cabo el comando twittear.
// Recibe el sistema, los parametros y su cantidad.
// Pre: el sistema fue creado.
// Post: Devuelve errores que debera manipular el que llamo a esta funcion.
errores comando_twittear(sistema_t* sistema, char** parametros, size_t cant_parametros)
{
	if (cant_parametros != CANT_PARAM_TWITTEAR) return ERROR_COMANDO_INVALIDO;
	
	// El parametro autor vino sin arroba.
	if (*parametros[POS_AUTOR] != '@') return ERROR_COMANDO_INVALIDO;

	// Tweet demasiado largo
	if (strlen(parametros[POS_MENSAJE]) > MAX_LONG_MENSAJE) 
		return ERROR_TWIT_DEMASIADO_LARGO;

	// Twitteamos
	if (!twitter_twittear(sistema->twitter, parametros[POS_AUTOR], parametros[POS_MENSAJE]))
		return MEM_ERROR;
	return NO_ERROR;
}

#define CANT_PARAM_FAVORITO 1
#define POS_ID 0
// Lleva a cabo el comando favorito.
// Recibe el sistema, los parametros y su cantidad.
// Pre: el sistema fue creado.
// Post: Devuelve errores que debera manipular el que llamo a esta funcion.
errores comando_favorito(sistema_t* sistema, char** parametros, size_t cant_parametros)
{
	if (cant_parametros != CANT_PARAM_FAVORITO) return ERROR_COMANDO_INVALIDO;

	// Conversion y comprobacion del Id
	char* punt_final;
	long id = strtol(parametros[POS_ID], &punt_final, 10);
	if (*punt_final != '\0') return ERROR_COMANDO_INVALIDO; // El parametro no es un nro
	if (id < 0) return ERROR_TWIT_ID_INVALIDO; // Los ids no son negativos.

	// Favoriteamos
	if (!twitter_favorito(sistema->twitter, (size_t)id))
		return ERROR_TWIT_ID_INVALIDO; // No existia el id
	return NO_ERROR;
}

#define CANT_PARAM_BUSCAR 3 
#define POS_BUSCADO 0
#define POS_ORDEN 1
#define POS_CANTIDAD 2
// Lleva a cabo el comando buscar.
// Recibe el sistema, los parametros y su cantidad.
// Pre: el sistema fue creado.
// Post: Devuelve errores que debera manipular el que llamo a esta funcion.
errores comando_buscar(sistema_t* sistema, char** parametros, size_t cant_parametros)
{
	if (cant_parametros != CANT_PARAM_BUSCAR) return ERROR_COMANDO_INVALIDO;
	
	// Conversion y comprobacion de la cantidad a buscar.
	char* punt_final;
	long cantidad = strtol(parametros[POS_CANTIDAD], &punt_final, 10);
	if (*punt_final != '\0') return ERROR_COMANDO_INVALIDO; // El parametro no es un nro
	if (cantidad < 0) return ERROR_COMANDO_INVALIDO; // Cantidad no puede ser negativo

	// El termino buscado no es ni usuario ni hashtag
	if (*parametros[POS_BUSCADO] != '@' && *parametros[POS_BUSCADO] != '#')
		return ERROR_COMANDO_INVALIDO; 

	// Defino el tipo de busqueda a efectuar y busco.
	if (strcmp(parametros[POS_ORDEN], "cronologico") == 0)
		// No falla
		twitter_buscar_cronologico(sistema->twitter, parametros[POS_BUSCADO], (size_t)cantidad);
	else if (strcmp(parametros[POS_ORDEN], "popular") == 0){
		if (!twitter_buscar_popular(sistema->twitter, parametros[POS_BUSCADO], (size_t)cantidad))
			return MEM_ERROR;
	}else
		return ERROR_COMANDO_INVALIDO;
	return NO_ERROR;
}

/* ******************************************************************
 *                    EJECUTADOR DE COMANDOS
 * *****************************************************************/

struct comandos{
	char* nombre;
	COMANDO funcion;
	int cant_parametros;
	bool eliminar_chars_finales;
};

#define CANT_COMANDOS 3
struct comandos lista_de_comandos[CANT_COMANDOS] = 
{
	{"twittear", comando_twittear, CANT_PARAM_TWITTEAR, false},
	{"favorito", comando_favorito, CANT_PARAM_FAVORITO, true},
	{"buscar", comando_buscar, CANT_PARAM_BUSCAR, true}
};

// Para usar con la entrada.
// Define el comportamiento de la entrada dependiendo del comando.
// Recibe el comando, ptro a bool para definir eliminacion de espacios finales vacios
// y el sistema de comandos.
// Pre: El sistema fue creado.
// Post: Devolvera la cantidad de parametros a esperar y modificara el booleano
// recibido por parametro.
int cantidad_parametros(char* comando, bool* eliminar_chars_finales, void* sistema)
{
	for (size_t i = 0; comando && i < CANT_COMANDOS; i++)
		if (strcmp(lista_de_comandos[i].nombre, comando) == 0)
		{
			*eliminar_chars_finales = lista_de_comandos[i].eliminar_chars_finales;
			((sistema_t*)sistema)->funcion_actual = lista_de_comandos[i].funcion;
			return lista_de_comandos[i].cant_parametros;
		};
	// si llegamos aca no es un comando conocido! -> que termine lo antes posible
	((sistema_t*)sistema)->funcion_actual = NULL;
	*eliminar_chars_finales = false;
	return 0;
}

// Ejecuta el comando correspondiente a su nombre.
// Recibe el sistema, el comando, los parametros y su cantidad.
// Devuelve false en caso de producirse un error y necesitar la finalizacion del programa
// Pre: El sistema fue creado.
// Post: se ejecuto el comando y el resultado se imprimio por la salida estandar.
bool ejecutar_comando(sistema_t* sistema, char* comando, char** parametros, size_t cant_parametros)
{
	errores error = ERROR_COMANDO_INVALIDO;
	if (sistema->funcion_actual)
		error = sistema->funcion_actual(sistema, parametros, cant_parametros);

	if (error > NO_ERROR) printf("%s\n", mensajes_error[error]);
	if (error == MEM_ERROR) return false;
	return true;
}
