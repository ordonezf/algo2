#include "tp1.h"
#include "participante.h"
#include "lectura.h"
#include "cola.h"
#include "pila.h"
#include "lista.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#define DONAR "donar"
#define NOMINAR "nominar"
#define TIRAR_BALDE "tirar_balde"

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 ******************************************************************/

struct sistema{
	lista_t* lista;
	lista_t* lista_usuario;
	cola_t* cola;
};

 /*******************************************************************
 *                        IMPLEMENTACION                            *
 *******************************************************************/
sistema_t* crear_sistema(){
	sistema_t* sistema = malloc(sizeof(sistema_t));
	if (sistema == NULL) return NULL;

	lista_t* lista = lista_crear();
	if (lista == NULL) return NULL;

	lista_t* lista_usuario = lista_crear();
	if (lista_usuario == NULL) return NULL;

	cola_t* cola = cola_crear();
	if (cola == NULL) return NULL;

	sistema->lista = lista;
	sistema->lista_usuario = lista_usuario;
	sistema->cola = cola;

	return sistema;
}

void sistema_destruir(sistema_t* sistema){
	lista_destruir(sistema->lista_usuario, free);
	cola_destruir(sistema->cola, NULL);
	lista_destruir(sistema->lista, destruir_persona);
	free(sistema);
}

persona_t* esta_en_sistema(char *nombre, lista_t *lista){
	if (lista_esta_vacia(lista)) return NULL;
	persona_t* persona;
	bool ok = false;
	lista_iter_t *iter = lista_iter_crear(lista);
	if (iter == NULL) return NULL;

	while (!lista_iter_al_final(iter)){
		persona = lista_iter_ver_actual(iter);
		char *actual = ver_nombre(persona);
		if(strcmp(actual, nombre) == 0){
			ok = true;
			break;
		}

		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	if (ok) return persona;

	return NULL;
}

void inscripcion(persona_t* persona, char* monto, sistema_t* sistema){
	if (monto != NULL) 
		agregar_donacion(persona, monto);

	lista_insertar_ultimo(sistema->lista, persona);
	cola_encolar(sistema->cola, ver_nombre(persona));
}

bool donar(char *nombre, char* monto, sistema_t* sistema){
	int donacion = 0;
	if (!(donacion = atoi(monto)) || donacion < 0){
		puts("ERROR1");
		return true;
	}
	else
		puts("OK");

	persona_t* persona = esta_en_sistema(nombre, sistema->lista);
	if (persona != NULL)
		agregar_donacion(persona, monto);

	else{
		persona_t *p = crear_persona(nombre);
		if (p == NULL) return false;

		inscripcion(p, monto, sistema);
	}
	return true;
}


bool nominar(char* nombre, char* nominados, sistema_t* sistema){
	persona_t* persona = esta_en_sistema(nombre, sistema->lista);
	if (persona == NULL){
		puts("ERROR2");
		return true;
	}
	if(!participante_dono(persona)){
		puts("ERROR3");
		return true;
	}
	char* n = strtok(nominados, " ");
	while (n != NULL){
		if (esta_en_sistema(n, sistema->lista) != NULL)
			printf("ERROR4 %s\n",n);
		else{
			printf("OK %s\n",n);
			persona_t *persona = crear_persona(n);
			if (persona == NULL) return false;

			inscripcion(persona, NULL, sistema);
		}
		n = strtok(NULL, " ");
	}
	return true;
}

void tirar_balde(char* nombre, char* eleccion, sistema_t* sistema){
	if (cola_esta_vacia(sistema->cola) || strcmp(cola_ver_primero(sistema->cola), nombre) != 0){
		puts("ERROR5");
		return;
	}
	persona_t* p = esta_en_sistema(nombre, sistema->lista);
	if (p == NULL){
		puts("ERROR6");
		return;
	}
	if (!participante_dono(p) && (strcmp(eleccion, (char*) "no") == 0)){
		puts("ERROR6");
		return;
	}
	if ((strcmp(cola_ver_primero(sistema->cola), nombre) == 0) && (strcmp(eleccion, (char*) "si") == 0)){
		mostrar_donaciones(ver_donaciones(p));
		cola_desencolar(sistema->cola);
		return;
	}
	else if (strcmp(eleccion, (char*) "no") == 0){
		cola_desencolar(sistema->cola);
		puts("ERROR8");
	}
}


void ingreso_al_sistema(){
	sistema_t* sistema = crear_sistema();
	if (sistema == NULL) return;

	bool ok = true;
	while (true){
		char* usuario = leer_linea();
		lista_insertar_ultimo(sistema->lista_usuario, usuario);
		if(feof(stdin)) break;

		char* nombre = strtok(usuario, " ");
		char* comando = strtok(NULL, " "); 
		if (strcmp(comando, (char*) DONAR) == 0){
			char* monto = strtok(NULL, " ");
			ok = donar(nombre, monto, sistema);
		}
		else if (strcmp(comando, (char*) NOMINAR) == 0){
			char* nomindados = strtok(NULL, "\0");
			ok = nominar(nombre, nomindados, sistema);
		}
		else if (strcmp(comando, (char*) TIRAR_BALDE) == 0){
			char* eleccion = (strtok(NULL, " "));
			tirar_balde(nombre, eleccion, sistema);
		}
		else
			puts("ERROR7");

		if (!ok) break;
	}

	sistema_destruir(sistema);
}

int main(){
	ingreso_al_sistema();
	return 0;
}