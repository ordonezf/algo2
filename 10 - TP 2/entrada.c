#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "entrada.h"

#define TAMANIO_INICIAL_BUFFER 20
#define FIN_LINEA '\n'
#define FIN_STRING '\0'
#define TAM_INICIAL_PARAMETROS 10
#define ARG_DELIM "\a\b\r\n \r\v\t\f"

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct entrada
{
	size_t pos_comando;
	char* buffer;
	size_t tam_buffer;
	size_t buffer_len; // buffer[buffer_len] == \0 == final
	char* comando;
	char** parametros;
	size_t tam_parametros;
	size_t cant_parametros;
};

/* ******************************************************************
 *                        IMPLEMENTACION
 * *****************************************************************/

/*******************************************************************
 *                       Funciones auxiliares                      */

// Funcion auxiliar para ingreso de texto por teclado.
// Guarda la linea que se leyo hasta el enter en el buffer de la entrada.
// Pre: la entrada fue creada.
// Post: Devuelve falso en caso de llegar al EOF. True si leyo correctamente.
static bool leer_linea(entrada_t* entrada)
{
	int c;
	size_t i;

	for(i = 0; (c = getchar()) != FIN_LINEA; i++)  //leo hasta el enter
	{
		if (c == EOF) return false;

		if (i + 1 == entrada->tam_buffer)  // si llego al tamanio de mi cadena
		{
			char* aux = realloc(entrada->buffer, (entrada->tam_buffer*2) * sizeof(char));
			if (!aux) break;  // si falla realloc, termino aca
			entrada->buffer = aux;
			entrada->tam_buffer *= 2;
		}
		entrada->buffer[i] = (char)c;
	}
	entrada->buffer[i] = FIN_STRING; // es necesario cerrar el string
	entrada->buffer_len = i; // guardo el largo
	return true;
}

// Recibe una cadena y los caracteres iniciales a ignorar.
// Devuelve un puntero a la primer posisicon donde la cadena
// no presenta los caracteres a ignorar.
static char* ignorar_caracteres_iniciales(char* cadena, char* caracteres)
{
	char* car = caracteres;
	while (*car != '\0')
	{
		if (*car == *cadena)
		{
			cadena++;
			car = caracteres;
		}
		else car++;
	}
	return cadena;
}

// Recibe una cadena, su largo y los caracteres finales a eliminar.
// Se intercambiara cada ocurrencia de los caracteres por '\0'
// hasta que aparezca un caracter aceptado.
static void eliminar_caracteres_finales(char* cadena, size_t largo, char* caracteres)
{
	char* letra = cadena + (largo - 1); //vamos hasta el final, antes del \0
	char* car = caracteres;
	while (*car != '\0')
	{
		if (*car == *letra)
		{
			*letra = '\0';
			letra--;
			car = caracteres;
		}
		else car++;
	}
}

/*                       Fin de f. auxiliares                      *
 *******************************************************************/

// Crea la entrada. Recibe la posicion (a partir de 0) donde se espera el comando.
// Pre: Posicion del comando es un numero positivo.
// Post: Devuelve la entrada o NULL en caso de error.
entrada_t* entrada_crear(size_t pos_comando)
{
	entrada_t* entrada = malloc(sizeof(entrada_t));
	char* buffer = malloc(TAMANIO_INICIAL_BUFFER*sizeof(char));
	char** parametros = malloc(TAM_INICIAL_PARAMETROS*sizeof(char*));
	if (!entrada || !buffer || !parametros)
	{
		free(entrada);
		free(buffer);
		free(parametros);
		return NULL;
	}
	entrada->parametros = parametros;
	entrada->tam_parametros = TAM_INICIAL_PARAMETROS;
	entrada->cant_parametros = 0;
	entrada->comando = NULL;
	entrada->pos_comando = pos_comando;
	entrada->buffer = buffer;
	entrada->tam_buffer = TAMANIO_INICIAL_BUFFER;
	entrada->buffer_len = 0;
	return entrada;
}

// Destruye la entrada
// Pre: la entrada fue creada.
void entrada_destruir(entrada_t* entrada)
{
	free(entrada->parametros);
	free(entrada->buffer);
	free(entrada);
}

// Lee la linea de la entrada estandar y la divide en los argumentos correspondientes.
// Recibe, opcionalmente, una funcion del tipo max_cant_param_t (explicada arriba)
// que se llamara apenas se lea el comando, para definir el procedimiento
// a llevar a cabo con los proximos parametros.
// Por defecto, se leeran todos los parametros posibles y se eliminaran los
// caracteres vacios finales.
// Devolvera false en caso de llegar a EOF o haber leido una linea vacia.
// Pre: la entrada fue creada.
// Post: la entrada fue actualizada con los argumentos leidos. (comando y parametros)
bool entrada_leer(entrada_t* entrada, max_cant_param_t max_param, void* extra)
{
	if (!leer_linea(entrada)) return false; //salgo directo si me mandan EOF

	entrada->cant_parametros = 0;
	entrada->comando = NULL;
	int max_cant_param = -1;
	bool eliminar_chars_finales = true;

	char* argumento = strtok(entrada->buffer, ARG_DELIM);
	if (!argumento) return false;   //salgo directo en enter a linea vacia

	for (size_t j = 0; argumento; j++)
	{
		if (j == entrada->pos_comando)
		{
			entrada->comando = argumento;
			if (max_param) max_cant_param = max_param(argumento, &eliminar_chars_finales, extra);
		}
		else
		{
			if (entrada->cant_parametros == entrada->tam_parametros)
			{
				char** temp = realloc(entrada->parametros, (entrada->tam_parametros*2) * sizeof(char*));
				if (!temp) break;
				entrada->parametros = temp;
				entrada->tam_parametros *= 2;
			}
			entrada->parametros[entrada->cant_parametros++] = argumento;
		}
		if (max_cant_param >= 0 && (int)entrada->cant_parametros >= (max_cant_param - 1))
			break;
		argumento = strtok(NULL, ARG_DELIM);
	}

	// Si cortamos antes de leer todo.
	if (argumento && max_cant_param > (int)entrada->pos_comando)
	{
		// Recorro el ultimo argumento ya conocido.
		while (*argumento != '\0') argumento++;

		// Si llegamos al \0 que habiamos puesto nosotros, terminamos
		if (argumento == &(entrada->buffer[entrada->buffer_len]))
			return true; // queda asi, si no terminamos leyendo cosas viejas.

		argumento = ignorar_caracteres_iniciales(++argumento, ARG_DELIM);
		// Argumento es ahora el principio del ultimo argumento (peor caso == \0)
		if (*argumento == '\0') return true;
		entrada->parametros[entrada->cant_parametros++] = argumento;

		if (eliminar_chars_finales)
			eliminar_caracteres_finales(entrada->buffer, entrada->buffer_len, ARG_DELIM);
	}
	return true;
}

// Devuelve el comando actual.
// Pre: la entrada fue creada.
// Nota: comando es NULL si no existio un argumento en la posicion del comando.
char* entrada_comando(entrada_t* entrada)
{
	return entrada->comando;
}

// Devuelve la cantidad de parametros actuales.
// Pre: la entrada fue creada.
size_t entrada_cant_parametros(entrada_t* entrada)
{
	return entrada->cant_parametros;
}

// Devuelve los parametros actuales.
// Pre: la entrada fue creada.
char** entrada_parametros(entrada_t* entrada)
{
	return entrada->parametros;
}