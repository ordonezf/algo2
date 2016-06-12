/*
 * Gracias a Margarita Manterola y Maximiliano Curia
 * por las pruebas de hash, en las cuales estan
 * basadas las siguientes pruebas de ABB.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "abb.h"

/* ******************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

/* Función auxiliar para imprimir si estuvo OK o no. */
void print_test(char* name, bool result)
{
	printf("%s: %s\n", name, result? "OK" : "ERROR");
}

int intcmp(const char* clave_izq, const char* clave_der)
{
	//char *endptr1, *endptr2;
	
	// YO lo uso, asi que no tendria que haber errores aca!
	long izq = strtol(clave_izq, NULL, 10);
	long der = strtol(clave_der, NULL, 10);

	return (int)(izq - der);
}

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/


/* Prueba que las primitivas de la lista funcionen correctamente. */
void prueba_crear_abb_vacio()
{
	abb_t* abb = abb_crear(strcmp, NULL);

	print_test("Prueba abb crear abb vacio", abb);
	print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
	print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
	print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
	print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

	abb_destruir(abb);
}

void prueba_iterar_abb_vacio()
{
	abb_t* abb = abb_crear(strcmp, NULL);
	abb_iter_t* iter = abb_iter_in_crear(abb);
	print_test("Prueba abb iter crear iterador abb vacio", iter);
	print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
	print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
	print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

	abb_iter_in_destruir(iter);
	abb_destruir(abb);
}

void prueba_abb_insertar()
{
	abb_t* abb = abb_crear(strcmp, NULL);

	char *clave1 = "perro", *valor1 = "guau";
	char *clave2 = "gato", *valor2 = "miau";
	char *clave3 = "vaca", *valor3 = "mu";

	/* Inserta 1 valor y luego lo borra */
	print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
	print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
	print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
	print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
	print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
	print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
	print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

	/* Inserta otros 2 valores y no los borra (se destruyen con el abb) */
	print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
	print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
	print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
	print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
	print_test("Prueba abb pertenece clave2, es true", abb_pertenece(abb, clave2));

	print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
	print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
	print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
	print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
	print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));

	abb_destruir(abb);
}

void prueba_abb_reemplazar()
{
	abb_t* abb = abb_crear(strcmp, NULL);

	char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
	char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

	/* Inserta 2 valores y luego los reemplaza */
	print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
	print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
	print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
	print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
	print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
	print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
	print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

	print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
	print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
	print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
	print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
	print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
	print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
	print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

	abb_destruir(abb);
}

void prueba_abb_reemplazar_con_destruir()
{
	abb_t* abb = abb_crear(strcmp, free);

	char *clave1 = "perro", *valor1a, *valor1b;
	char *clave2 = "gato", *valor2a, *valor2b;

	/* Pide memoria para 4 valores */
	valor1a = malloc(10 * sizeof(char));
	valor1b = malloc(10 * sizeof(char));
	valor2a = malloc(10 * sizeof(char));
	valor2b = malloc(10 * sizeof(char));

	/* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
	print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
	print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
	print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
	print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
	print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
	print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
	print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

	print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
	print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
	print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
	print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
	print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
	print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
	print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

	/* Se destruye el abb (se debe liberar lo que quedó dentro) */
	abb_destruir(abb);
}

void prueba_abb_borrar()
{
	abb_t* abb = abb_crear(strcmp, NULL);

	char *clave1 = "perro", *valor1 = "guau";
	char *clave2 = "gato", *valor2 = "miau";
	char *clave3 = "vaca", *valor3 = "mu";

	/* Inserta 3 valores y luego los borra */
	print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
	print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
	print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));

	/* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
	print_test("Prueba abb pertenece clave3, es verdadero", abb_pertenece(abb, clave3));
	print_test("Prueba abb borrar clave3, es valor3", abb_borrar(abb, clave3) == valor3);
	print_test("Prueba abb borrar clave3, es NULL", !abb_borrar(abb, clave3));
	print_test("Prueba abb pertenece clave3, es falso", !abb_pertenece(abb, clave3));
	print_test("Prueba abb obtener clave3, es NULL", !abb_obtener(abb, clave3));
	print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

	print_test("Prueba abb pertenece clave1, es verdadero", abb_pertenece(abb, clave1));
	print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
	print_test("Prueba abb borrar clave1, es NULL", !abb_borrar(abb, clave3));
	print_test("Prueba abb pertenece clave1, es falso", !abb_pertenece(abb, clave1));
	print_test("Prueba abb obtener clave1, es NULL", !abb_obtener(abb, clave1));
	print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);

	print_test("Prueba abb pertenece clave2, es verdadero", abb_pertenece(abb, clave2));
	print_test("Prueba abb borrar clave2, es valor2", abb_borrar(abb, clave2) == valor2);
	print_test("Prueba abb borrar clave2, es NULL", !abb_borrar(abb, clave3));
	print_test("Prueba abb pertenece clave2, es falso", !abb_pertenece(abb, clave2));
	print_test("Prueba abb obtener clave2, es NULL", !abb_obtener(abb, clave2));
	print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

	abb_destruir(abb);
}

void prueba_abb_borrar_varios()
{
	abb_t* abb = abb_crear(intcmp, NULL);

	const size_t largo = 10;
	char clave_temp[2];
	int valores[] = {5,2,1,0,3,4,7,6,9,8};

	/* Inserta los 10 valores */
	bool ok = true;
	for (size_t i = 0; i < largo; i++) {
		sprintf(clave_temp, "%d", valores[i]);
		ok = abb_guardar(abb, clave_temp, &valores[i]);
		if (!ok) break;
	}
	print_test("Prueba abb insertamos los 10 valores", ok);
	
	// para verificar que borre bien todos los casos! (hoja, 1hijo y 2hijos)
	for (size_t i = 0; i < largo; i++)
	{
		sprintf(clave_temp, "%d", valores[i]);
		ok = ok && abb_pertenece(abb, clave_temp);
		ok = ok && abb_borrar(abb, clave_temp);
		ok = ok && !abb_borrar(abb, clave_temp);
		ok = ok && !abb_pertenece(abb, clave_temp);
		ok = ok && !abb_obtener(abb, clave_temp);
		ok = ok && abb_cantidad(abb) == largo - i - 1;
		if (!ok) break;
	}
	print_test("Prueba abb borramos los 10 valores", ok);

	abb_destruir(abb);
}

void prueba_abb_clave_vacia()
{
	abb_t* abb = abb_crear(strcmp, NULL);

	char *clave = "", *valor = "";

	print_test("Prueba abb insertar clave vacia", abb_guardar(abb, clave, valor));
	print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
	print_test("Prueba abb obtener clave vacia es valor", abb_obtener(abb, clave) == valor);
	print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
	print_test("Prueba abb borrar clave vacia, es valor", abb_borrar(abb, clave) == valor);
	print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

	abb_destruir(abb);
}

void prueba_abb_valor_null()
{
	abb_t* abb = abb_crear(strcmp, NULL);

	char *clave = "", *valor = NULL;

	/* Inserta 1 valor y luego lo borra */
	print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
	print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
	print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
	print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
	print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
	print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

	abb_destruir(abb);
}

bool guardar_en_preorder(abb_t* abb, char* clave, int** arreglo, size_t inicio, size_t fin)
{
	if (fin <= inicio) return true;

    size_t medio = (fin + inicio) / 2;

    if (medio == fin) return true; // quizas innecesario con la comprobacion de arriba..

	sprintf(clave, "%zu", medio);

    return  abb_guardar(abb, clave, (void*)(arreglo[medio])) &&
    		guardar_en_preorder(abb, clave, arreglo, inicio, medio) && 
    		guardar_en_preorder(abb, clave, arreglo, medio + 1, fin);
}

void prueba_abb_volumen(size_t largo, bool debug)
{
	abb_t* abb = abb_crear(intcmp, NULL);

	// Verifico que intcmp funciona bien
	assert(intcmp("0","0000000") == 0);
	assert(intcmp("1","0") > 0);
	assert(intcmp("-20","0") < 0);
	assert(intcmp("-0","+0") == 0);
	assert(intcmp("-20","+20") < 0);
	assert(intcmp("0000001000","00999") > 0);

	size_t digitos = 0;
	for (size_t nro = largo; nro; digitos++) nro /= 10;

	char clave_temp[digitos + 1];
	int** valores = malloc(largo * sizeof(int*));
	if (!valores)
	{
		abb_destruir(abb);
		return;
	}
	// "inicializo" el arreglo
	for (size_t i = 0; i < largo; i++)
	{
		valores[i] = malloc(sizeof(int));
		*(valores[i]) = i;
	}

	/* Inserta 'largo' parejas en el hash */
	bool ok = true;
	ok = guardar_en_preorder(abb, clave_temp, valores, 0, largo);

	if (debug) print_test("Prueba abb almacenar muchos elementos", ok);
	if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

	/* Verifica que devuelva los valores correctos */
	for (size_t i = 0; i < largo; i++) {
		sprintf(clave_temp, "%zu", i);
		ok = abb_pertenece(abb, clave_temp);
		if (!ok) break;
		ok = abb_obtener(abb, clave_temp) == valores[i];
		if (!ok) break;
	}

	if (debug) print_test("Prueba abb pertenece y obtener muchos elementos", ok);
	if (debug) print_test("Prueba abb la cantidad de elementos es correcta", abb_cantidad(abb) == largo);

	/* Verifica que borre y devuelva los valores correctos */
	for (size_t i = 0; i < largo; i++) {
		sprintf(clave_temp, "%zu", i);
		ok = abb_borrar(abb, clave_temp) == valores[i];
		if (!ok) break;
	}

	if (debug) print_test("Prueba abb borrar muchos elementos", ok);
	if (debug) print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

	/* Destruye el abb y crea uno nuevo que sí libera */
	abb_destruir(abb);
	abb = abb_crear(intcmp, free);

	/* Inserta 'largo' parejas en el abb */
	guardar_en_preorder(abb, clave_temp, valores, 0, largo);

	free(valores);
	/* Destruye el abb - debería liberar los enteros */
	abb_destruir(abb);

}

int buscar(const char* clave, char* claves[], size_t largo)
{
	if (clave == NULL) return -1;
	for (size_t i = 0; i < largo; i++) {
		if (strcmp(clave, claves[i]) == 0) return i;
	}
	return -1;
}

void prueba_abb_iterar_ext()
{
	abb_t* abb = abb_crear(strcmp, NULL);

	char *claves[] = {"perro", "gato", "vaca"};
	char *valores[] = {"guau", "miau", "mu"};

	/* Inserta 3 valores */
	print_test("Prueba abb insertar clave1", abb_guardar(abb, claves[0], valores[0]));
	print_test("Prueba abb insertar clave2", abb_guardar(abb, claves[1], valores[1]));
	print_test("Prueba abb insertar clave3", abb_guardar(abb, claves[2], valores[2]));

	// Prueba de iteración sobre las claves almacenadas.
	abb_iter_t* iter = abb_iter_in_crear(abb);
	const char *clave;
	int indice;

	print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

	/* Primer valor */
	clave = abb_iter_in_ver_actual(iter);
	indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
	print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
	print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
	print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
	print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

	/* Segundo valor */
	clave = abb_iter_in_ver_actual(iter);
	indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
	print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
	print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
	print_test("Prueba abb iterador avanzar es true", abb_iter_in_avanzar(iter));
	print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

	/* Tercer valor */
	clave = abb_iter_in_ver_actual(iter);
	indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
	print_test("Prueba abb iterador ver actual, es una clave valida", indice != -1);
	print_test("Prueba abb iterador ver actual, no es el mismo puntero", clave != claves[indice]);
	/* Se anula esta prueba por diferencias de criterios */
	abb_iter_in_avanzar(iter);
	print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

	/* Vuelve a tratar de avanzar, por las dudas */
	print_test("Prueba abb iterador ver actual, es NULL", !abb_iter_in_ver_actual(iter));
	print_test("Prueba abb iterador avanzar es false", !abb_iter_in_avanzar(iter));
	print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

	abb_iter_in_destruir(iter);
	abb_destruir(abb);
}

void prueba_abb_iterar_ext_volumen(size_t largo)
{
	abb_t* abb = abb_crear(intcmp, free);

	// Verifico que intcmp funciona bien
	assert(intcmp("0","0000000") == 0);
	assert(intcmp("1","0") > 0);
	assert(intcmp("-20","0") < 0);
	assert(intcmp("-0","+0") == 0);
	assert(intcmp("-20","+20") < 0);
	assert(intcmp("0000001000","00999") > 0);

	size_t digitos = 0;
	for (size_t nro = largo; nro; digitos++) nro /= 10;

	char clave_temp[digitos + 1];
	int** valores = malloc(largo * sizeof(int*));
	if (!valores)
	{
		abb_destruir(abb);
		return;
	}
	// "inicializo" el arreglo
	for (size_t i = 0; i < largo; i++)
	{
		valores[i] = malloc(sizeof(int));
		*(valores[i]) = i;
	}

	/* Inserta 'largo' parejas en el hash */
	bool ok = true;
	ok = guardar_en_preorder(abb, clave_temp, valores, 0, largo);

	// Prueba de iteración sobre las claves almacenadas.
	abb_iter_t* iter = abb_iter_in_crear(abb);
	print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

	ok = true;
	size_t i;
	const char *clave;
	int *valor;

	// iterar in order -> es el arreglo ordenado!
	for (i = 0; i < largo; i++) {
		if ( abb_iter_in_al_final(iter) ) {
			ok = false;
			break;
		}
		clave = abb_iter_in_ver_actual(iter);
		sprintf(clave_temp, "%zu", i);
		if (clave && strcmp(clave, clave_temp) != 0) {
			ok = false;
			break;
		}
		valor = abb_obtener(abb, clave);
		if ( *valor != i ) {
			ok = false;
			break;
		}
		*valor = largo;
		abb_iter_in_avanzar(iter);
	}
	print_test("Prueba abb iteración en volumen", ok);
	print_test("Prueba abb iteración en volumen, recorrio todo el largo", i == largo);
	if (i != largo) printf("Largo recorrido: %zu\n", i);
	print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

	ok = true;
	for (i = 0; i < largo; i++) {
		if ( *valores[i] != largo ) {
			ok = false;
			break;
		}
	}
	print_test("Prueba abb iteración en volumen, se cambiaron todo los elementos", ok);

	free(valores);
	abb_iter_in_destruir(iter);
	abb_destruir(abb); //se ocupa de los malloc en valores.
}

bool verificar_claves_y_valores(const char* clave, int* dato, size_t* largo)
{
	static int i = 0;

	char clave_temp[10];
	sprintf(clave_temp, "%d", i);

	if (*dato != i || strcmp(clave, clave_temp) != 0)
	{
		printf("Fallo en el valor: %d, con clave: %s\n", *dato, clave);
		printf("Esperaba el valor: %d, con clave: %s\n", i, clave_temp);
		print_test("Prueba abb iter interno, verificar valores y claves", false);
		return false;
	}

	if (++i == *largo)
		print_test("Prueba abb iter interno, verificar valores y claves", true);

	return true;
}

bool multiplicar_dato(const char* clave, int* dato, int* multiplicador)
{
	*dato *= *multiplicador;
	return true;
}

void prueba_abb_iterar_int()
{
	abb_t* abb = abb_crear(intcmp, NULL);

	const size_t largo = 10;
	char clave_temp[2];
	int valores[] = {5,2,1,0,3,4,7,6,9,8};

	/* Inserta los 10 valores */
	bool ok = true;
	for (size_t i = 0; i < largo; i++) {
		sprintf(clave_temp, "%d", valores[i]);
		ok = abb_guardar(abb, clave_temp, &valores[i]);
		if (!ok) break;
	}
	print_test("Prueba abb insertamos los 10 valores", ok);
	
	// Prueba de iteración sobre las claves almacenadas.
	// Debe iterar in order y deben estar todo lo guardado.
	abb_in_order(abb, (bool(*)(const char*, void*, void*))verificar_claves_y_valores, (void*)&largo);

	// Prueba de iteracion con modificacion.
	int multiplicador = 2;
	abb_in_order(abb, (bool(*)(const char*, void*, void*))multiplicar_dato, (void*)&multiplicador);

	int originales[] = {5,2,1,0,3,4,7,6,9,8};
	for (size_t i = 0, ok = true; i < largo; i++)
	{
		sprintf(clave_temp, "%d", originales[i]);
		ok = ok && abb_obtener(abb, clave_temp) == valores + i;
		ok = ok && valores[i] == originales[i] * multiplicador;
		if (!ok) break;
	}
	print_test("Prueba abb iter interno, modificamos y comprobamos los datos", ok);

	abb_destruir(abb);
}


/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

/* Programa principal. */
int main(int argc, char** argv)
{

	if (argc < 2) {
		/* Ejecuta todas las pruebas unitarias. */
		prueba_crear_abb_vacio();
		prueba_iterar_abb_vacio();
		prueba_abb_insertar();
		prueba_abb_reemplazar();
		prueba_abb_reemplazar_con_destruir();
		prueba_abb_borrar();
		prueba_abb_borrar_varios();
		prueba_abb_iterar_int();
		prueba_abb_volumen(5000, true);
		prueba_abb_iterar_ext();
		prueba_abb_iterar_ext_volumen(5000);
	} else {
		size_t largo = atoi(argv[1]);
		prueba_abb_volumen(largo, false);
	}
	return 0;
}
