#ifndef PARTICIPANTE_H
#define PARTICIPANTE_H

#include <stdbool.h>
#include <stddef.h>
#include "pila.h"


/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 ******************************************************************/

typedef struct persona persona_t;

 /*******************************************************************
 *                           PRIMITIVAS                             *
 *******************************************************************/

// Recibe nombre y crea una persona, inicializa una pila vacia de donaciones.
persona_t *crear_persona(char* nombre);

// Destruye la pila y la estructura.
void destruir_persona(void* persona);

// Recibe una persona y devuelve su nombre.
char* ver_nombre(persona_t* persona);

//Recibe una persona y devuelve una pila con sus donaciones.
pila_t* ver_donaciones(persona_t* persona);

// Devuelve por pantalla una lista con la donaciones hechas por la persona
// en orden inverso y el total de estas. Se desapila la pila.
void mostrar_donaciones(pila_t *pila);

// Se agrega una donacion a la pila de una persona.
void agregar_donacion(persona_t* persona, char* monto);

// Devuelve true si el participante dono, false si no. 
bool participante_dono(persona_t* persona);


#endif //PARTICIPANTE_H