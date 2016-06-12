#ifndef ENTRADA_H
#define ENTRADA_H

#include <stdbool.h>
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct entrada entrada_t;

// La funcion max_cant_param_t se utiliza para definir la cantidad esperada
// de parametros (aparte del comando), el comportamiento con los espacios vacios
// finales y opcionalmente para hacer cambios con un puntero extra.
// Devuelve la cantidad (maxima) de parametros a necesitar. 
// Devolver "-1" es equivalente a leer todos los parametros posibles.
typedef int (*max_cant_param_t)(char* comando, bool* eliminar_chars_finales, void* extra);

/* ******************************************************************
 *                    PRIMITIVAS DE LA ENTRADA
 * *****************************************************************/

// Crea la entrada. Recibe la posicion (a partir de 0) donde se espera el comando.
// Pre: Posicion del comando es un numero positivo.
// Post: Devuelve la entrada o NULL en caso de error.
entrada_t* entrada_crear(size_t pos_comando);

// Destruye la entrada
// Pre: la entrada fue creada.
void entrada_destruir(entrada_t* entrada);

// Lee la linea de la entrada estandar y la divide en los argumentos correspondientes.
// Recibe, opcionalmente, una funcion del tipo max_cant_param_t (explicada arriba)
// que se llamara apenas se lea el comando, para definir el procedimiento
// a llevar a cabo con los proximos parametros.
// Por defecto, se leeran todos los parametros posibles y se eliminaran los
// caracteres vacios finales.
// Devolvera false en caso de llegar a EOF o haber leido una linea vacia.
// Pre: la entrada fue creada.
// Post: la entrada fue actualizada con los argumentos leidos. (comando y parametros)
bool entrada_leer(entrada_t* entrada, max_cant_param_t max_param, void* extra);

// Devuelve el comando actual.
// Pre: la entrada fue creada.
// Nota: comando es NULL si no existio un argumento en la posicion del comando.
char* entrada_comando(entrada_t* entrada);

// Devuelve la cantidad de parametros actuales.
// Pre: la entrada fue creada.
size_t entrada_cant_parametros(entrada_t* entrada);

// Devuelve los parametros actuales.
// Pre: la entrada fue creada.
char** entrada_parametros(entrada_t* entrada);

#endif //ENTRADA_H
