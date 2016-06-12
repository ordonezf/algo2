#ifndef TP1_H
#define TP1_H

#include <stdbool.h>
#include <stddef.h>
#include "cola.h"
#include "pila.h"
#include "lista.h"
#include "participante.h"


/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 ******************************************************************/

typedef struct sistema sistema_t;

 /*******************************************************************
 *                           PRIMITIVAS                             *
 *******************************************************************/
// Crea las estructuras necesarias para guardar la informacion. Si alguna falla
// devuelve NULL.
sistema_t* crear_sistema();
// Cuando se falla en la creacion de alguna estructura o cuando el usuario
// decide terminar la ejecucion del programa se destruye el sistema.
void sistema_destruir(sistema_t* sistema);

// Verifica si la persona se encuentra en el sistema. Devuele la persona 
// si se encuentra, NULL si no.
persona_t* esta_en_sistema(char* nombre, lista_t *lista);

// Inscribe personas cuando son nominadas por alguien. Las agrega a la lista
// y a la cola para tirarse el balde.
void inscripcion(persona_t* persona, char* monto, sistema_t* sistema);

// Recibe un nombre, un monto, una cola y una lista. Si los datos son validos
// lo ingresa al sistema, guarda su donacion y lo pone en el ultimo lugar en la cola.
// Si es invalido, devuelve por pantalla ERROR1.
// Si falla la creacion de alguna estructura devuelve false. Si todo sale bien, true.
bool donar(char *nombre, char* monto, sistema_t* sistema);

// Recibe el nombre del nominador y los nominados. Verifica que
// los nominados no esten en el sistema y los agrega. En caso contrario,
// avisa por pantalla cual de los nominados presenta un error.
// Devuelve false si falla la crecion de alguna estructura. Si todo sale bien, true.
bool nominar(char* nombre, char* nominados, sistema_t* sistema);

// Recibe el nombre de la persona, su decision, una lista y una cola.
// Verifica si puede tirarse el balde o no. 
// Devuelve diferentes errores dependiendo de la eleccion y la posicion de la
// persona en la cola.
void tirar_balde(char* nombre, char* eleccion, sistema_t* sistema);

// El usuario ingresa una linea de texto. Si es EOF sale del ciclo y libera memoria
// Si no es EOF se separa la cadena en nombre, comando y una tercer variable que
// depende del comando.
void ingreso_al_sistema();




#endif //TP1_H