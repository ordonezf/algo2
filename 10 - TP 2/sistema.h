#ifndef SISTEMA_H
#define SISTEMA_H

#include <stdbool.h>
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct sistema sistema_t;

/* ******************************************************************
 *               PRIMITIVAS DEl SISTEMA DE COMANDOS
 * *****************************************************************/

// Crea el sistema de comandos.
// Post: Devuelve el sistema o NULL en caso de error.
sistema_t* sistema_crear();

// Destruye el sistema de comandos
// Pre: el sistema fue creado.
void sistema_destruir(sistema_t* sistema);

/* ******************************************************************
 *                    EJECUTADOR DE COMANDOS
 * *****************************************************************/

// Para usar con la entrada.
// Define el comportamiento de la entrada dependiendo del comando.
// Recibe el comando, ptro a bool para definir eliminacion de espacios finales vacios
// y el sistema de comandos.
// Pre: El sistema fue creado.
// Post: Devolvera la cantidad de parametros a esperar y modificara el booleano
// recibido por parametro.
int cantidad_parametros(char* comando, bool* eliminar_chars_finales, void* sistema);

// Ejecuta el comando correspondiente a su nombre.
// Recibe el sistema, el comando, los parametros y la cantidad de parametros.
// Devuelve false en caso de producirse un error y necesitar la finalizacion del programa
// Pre: El sistema fue creado.
// Post: se ejecuto el comando y el resultado se imprimio por la salida estandar.
bool ejecutar_comando(sistema_t* sistema, char* comando, char** parametros, size_t cant_parametros);

#endif //SISTEMA_H