#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*******************************************************************
 *                        AUXILIARES                               *
 ******************************************************************/

void destruir_punto(void* dato){
	free(dato);
}

typedef struct punto{
	int x;
	int y;
} punto;

void* crear_punto(){
	punto *p = malloc(sizeof(punto));
	if (p == NULL)
		return NULL;
	p->x = 1;
	p->y = 2;
	return p;
}

bool contar(void* dato, void* cont){
	if (dato != NULL){
		int *cuenta = (int*)cont;
		(*cuenta)++;
		return true;
	}
	else
		return false;
}

/*******************************************************************
 *                        PRUEBAS UNITARIAS                        *
 ******************************************************************/

/* Función auxiliar para imprimir si estuvo OK o no. */
void print_test(char* name, bool result)
{
	printf("%s: %s\n", name, result? "OK" : "ERROR");
}

void pruebas_generales(){
//1
	lista_t *lista = lista_crear();
	print_test("Se creo la lista", lista != NULL);
//2
	print_test("La lista esta vacia", lista_esta_vacia(lista));
//3
	int num = 5;
	int *mun = &num;
	lista_insertar_primero(lista,&num);
	print_test("Insertar elemento, lista no vacia", !lista_esta_vacia(lista));
//4
	print_test("Primer elemento igual a 5", *((int*) lista_ver_primero(lista)) == num);
//5
	int num_1 = 9;
	int *mun_1 = &num_1;
	lista_insertar_ultimo(lista, mun_1);
	print_test("Longitud de la lista igual a 2", lista_largo(lista) == 2);
//6
	lista_borrar_primero(lista);
	print_test("Borrar primer elemento, nuevo primero es 9", *((int*) lista_ver_primero(lista)) == num_1);
//7
	lista_insertar_primero(lista, mun);
	print_test("Insertar primero, nuevo primero es 5", *((int*) lista_ver_primero(lista)) == num);
//8
	for(int x = 0; x<100; x++)
		lista_insertar_ultimo(lista, mun);
	print_test("Longitud de la lista es de 102 elementos", lista_largo(lista) == 102);
//9
	lista_destruir(lista, NULL);
	print_test("Se destruyo la lista", true);
//10
	lista_t *lista_1 = lista_crear();
	punto *p = crear_punto();
	lista_insertar_ultimo(lista_1, p);
	print_test("Insertar struct punto ultimo, nuevo primero es punto", lista_ver_primero(lista_1) == p);
//11
	print_test("Borrar primer elemento, igual a struct punto", lista_borrar_primero(lista_1) == p);
//12
	print_test("Borrar primer elemento en lista vacia igual a NULL", lista_borrar_primero(lista_1) == NULL);
//13
	lista_insertar_ultimo(lista_1, p);
	lista_destruir(lista_1, destruir_punto);
	print_test("Destruir lista con estructura punto", true);
//14
	lista_t *lista_2 = lista_crear();
	for (int i = 0; i < 100; i++){
		if (i % 2 == 0)
			lista_insertar_primero(lista_2, (void*) "prueba");
		else
			lista_borrar_primero(lista_2);
	}
	print_test("Intecaralar insertar y borrar", lista_largo(lista_2) == 0);
//15
	for (int j = 0; j<100; j++)
		if (j % 2 == 0)
			lista_insertar_primero(lista_2,(void*) "hola");
		else
			lista_insertar_ultimo(lista_2,(void*) "hola");
	while (lista_largo(lista_2) != 0)
		lista_borrar_primero(lista_2);
	print_test("Borrar muchos elementos uno a uno", lista_largo(lista_2) == 0);
	lista_destruir(lista_2, NULL);
}


void pruebas_con_iteradores(){
//16
	lista_t *lista_1 = lista_crear();
	lista_iter_t *iter_1 = lista_iter_crear(lista_1);
	print_test("Creacion de iter y de lista", iter_1 != NULL && lista_1 != NULL);
//17
	lista_iter_destruir(iter_1);
	lista_destruir(lista_1, NULL);
	print_test("Destruccion de lista e iter vacios", true);
//18
	lista_t *lista = lista_crear();
	void* array[] = {"leche", "huevos", "pan"};
	for (int i = 0; i<3; i++)
		lista_insertar_ultimo(lista, array[i]);
	lista_iter_t *iter = lista_iter_crear(lista);
	bool ok = true;
	for (int j = 0; j<3;j++){
		ok = lista_iter_ver_actual(iter) == (void*) array[j];
		lista_iter_avanzar(iter);
	}
	print_test("El iterador avanza y ve actual", ok);
//19
	print_test("Iterador esta al final", lista_iter_al_final(iter));
//20
	print_test("Iterador esta al final, igual a NULL", lista_iter_ver_actual(iter) == NULL);

	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
//21
	lista_t *lista_2 = lista_crear();
	lista_insertar_primero(lista_2, array[0]);
	lista_iter_t *iter_2 = lista_iter_crear(lista_2);
	lista_insertar_primero(lista_2, array[1]);
	ok = true;
	ok = lista_iter_ver_actual(iter_2) != lista_ver_primero(lista_2);
	print_test("Insertar con primitiva de lista, iter se rompe", ok);
	lista_iter_destruir(iter_2);
//22
	lista_iter_t *iter_3 = lista_iter_crear(lista_2);
	ok = true;
	ok = lista_iter_ver_actual(iter_3) == lista_ver_primero(lista_2);
	print_test("Crear nuevo iterador para lista anterior, funciona", ok);
	lista_iter_destruir(iter_3);
	lista_destruir(lista_2, NULL);
//23
	lista_t *lista_3 = lista_crear();
	for (int i = 0; i<50; i++)
		lista_insertar_ultimo(lista_3, array[2]);
	lista_insertar_primero(lista_3, array[0]);
	int cont = 0;
	void *contador = &cont;
	lista_iterar(lista_3, contar, contador);
	print_test("Iterar lista, contador igual a largo", lista_largo(lista_3) == cont);
//24
	print_test("Despues de iterar, primero sigue siendo el mismo", lista_ver_primero(lista_3) == array[0]);
	lista_destruir(lista_3, NULL);
//25
	lista_t *lista_4 = lista_crear();
	cont = 0;
	contador = &cont;
	lista_iterar(lista_4, contar, contador);
	print_test("Iterar una lista vacia", lista_largo(lista_4) == cont);
	lista_destruir(lista_4, NULL);
}

void prueba_insertar_borrar(){
//26
	void* array[] = {"leche", "huevos", "pan"};
	lista_t *lista = lista_crear();
	lista_iter_t *iter = lista_iter_crear(lista);
	print_test("Insertar elemento en lista vacia", lista_insertar(lista, iter, array[0]));
//27
	print_test("Primero igual a elemento", lista_ver_primero(lista) == array[0]);
//28
	print_test("Insertar elemento cuando actual es primero", lista_insertar(lista, iter, array[1]));
//29
	print_test("Nuevo elemento es el primero", lista_iter_ver_actual(iter) == lista_ver_primero(lista));
//30
	for (int i = 0; i<3; i++)
		lista_insertar_primero(lista, array[i]);
	lista_iter_t *iter_1 = lista_iter_crear(lista);
	lista_iter_avanzar(iter_1);
	print_test("Insertar elemento en el medio de una lista", lista_insertar(lista, iter_1, array[2]));
//31
	print_test("Ver actual es igual al elemento recien insertado", lista_iter_ver_actual(iter_1) == array[2]);
//32
	print_test("Se modifica el largo de la lista",lista_largo(lista) == 6);
//33
	while(!lista_iter_al_final(iter_1))
		lista_iter_avanzar(iter_1);
	print_test("Agregar elemento cuando actual apunta a NULL", lista_insertar(lista, iter_1, array[1]));
//34
	print_test("Actual es el elemento agregado", lista_iter_ver_actual(iter_1) == array[1]);
//35
	print_test("Borrar elemento cuando actual apunta al ultimo", lista_borrar(lista, iter_1) == array[1]);
//36
	print_test("Al borrar un elemento la lista se achica", lista_largo(lista) == 6);

//37
	lista_t * lista_1 = lista_crear();
	for (int j = 0; j<3; j++){
		lista_insertar_primero(lista_1, array[j]);}
	lista_iter_t *iter_2 =lista_iter_crear(lista_1);
	print_test("Borrar elemento cuando actual apunta al primero", lista_borrar(lista_1, iter_2) == array[2]);
//38
	lista_iter_avanzar(iter_2);
	print_test("Borrar elemento cuando actual apunta al medio", lista_borrar(lista_1, iter_2) == array[0]);
	lista_borrar(lista_1, iter_2);
//39
	print_test("Borrar elemento en lista vacia", lista_borrar(lista_1, iter_2) == NULL);
//40
	lista_iter_destruir(iter);
	lista_iter_destruir(iter_1);
	lista_destruir(lista, NULL);
	lista_iter_destruir(iter_2);
	lista_destruir(lista_1, NULL);
	print_test("Se destruyeron listas e iteradores", true);

}

void pruebas_iterador_en_primero(){
//41
	void* array[] = {"leche", "huevos", "pan", "agua"};
	lista_t *lista = lista_crear();
	lista_iter_t *iter = lista_iter_crear(lista);
	for(int i = 0; i<3; i++)
		lista_insertar(lista, iter, array[i]);
	lista_iter_destruir(iter);
	lista_iter_t *iter_1 = lista_iter_crear(lista);
	lista_insertar(lista, iter_1, array[3]);
	print_test("Insertar elemento en lista no vacia, actual apunta a primero", lista_iter_ver_actual(iter_1) == lista_ver_primero(lista));
//42
	print_test("Largo lista igual a 4", lista_largo(lista) == 4);
//43
	print_test("Borrar elemento cuando actual esta en primero", lista_borrar(lista, iter_1) == array[3]);
//44
	print_test("Largo lista igual a 3", lista_largo(lista) == 3);
	lista_iter_destruir(iter_1);
	lista_destruir(lista, NULL);
//45
	lista_t *lista_1 = lista_crear();
	for (int j = 0; j<3; j++)
		lista_insertar_ultimo(lista_1,array[j]);
	lista_iter_t *iter_2 = lista_iter_crear(lista_1);
	lista_insertar(lista_1, iter_2, array[3]);
	print_test("Insertar elemento con iter despues de usar insertar ultimo", lista_iter_ver_actual(iter_2) ==  lista_ver_primero(lista_1));
//46
	print_test("Largo lista igual a 4", lista_largo(lista_1) == 4);
//47
	print_test("Borrar elemento cuando actual esta en primero", lista_borrar(lista_1, iter_2) == array[3]);
//48
	print_test("Largo lista igual a 3", lista_largo(lista_1) == 3);

	lista_iter_destruir(iter_2);
	lista_destruir(lista_1, NULL);
//49
	lista_t *lista_2 = lista_crear();
	lista_insertar_primero(lista_2, array[0]);
	lista_insertar_ultimo(lista_2, array[1]);
	lista_iter_t *iter_3 = lista_iter_crear(lista_2);
	lista_insertar(lista_2, iter_3, array[2]);
	print_test("insertar elemento con iter en la primera posicion", lista_iter_ver_actual(iter_3) == lista_ver_primero(lista_2));
//50
	print_test("Largo lista igual a 3", lista_largo(lista_2) == 3);
//51
	print_test("Borrar elemento cuando actual esta en primero", lista_borrar(lista_2, iter_3) == array[2]);
//52
	print_test("Largo lista igual a 2", lista_largo(lista_2) == 2);
	lista_iter_destruir(iter_3);
	lista_destruir(lista_2, NULL);
//53
	lista_t *lista_3 = lista_crear();
	lista_iter_t * iter_4 = lista_iter_crear(lista_3);
	lista_insertar(lista_3, iter_4, array[0]);
	lista_insertar(lista_3, iter_4, array[1]);
	print_test("Insertar elemento con iter cuando actual apunta a primero", lista_iter_ver_actual(iter_4) == lista_ver_primero(lista_3));
	lista_iter_destruir(iter_4);
	lista_destruir(lista_3, NULL);
}

void pruebas_iterador_en_ultimo(){
//54
	void* array[] = {"leche", "huevos", "pan", "agua"};
	lista_t *lista = lista_crear();
	lista_iter_t *iter = lista_iter_crear(lista);
	for (int i = 0; i<3; i++)
		lista_insertar(lista, iter, array[i]);
	for (int i = 0; i<2; i++)
		lista_iter_avanzar(iter);
	lista_insertar(lista, iter, array[3]);
//55
	print_test("Insertar elemento cuando iterador esta 1 posicion antes del final", lista_iter_ver_actual(iter) == array[3]);
//56
	print_test("Largo lista igual a 4", lista_largo(lista) == 4);
//57
	print_test("Borrar elemento actual", lista_borrar(lista, iter) == array[3]);
//58
	print_test("Primero es sigue siendo el mismo", lista_ver_primero(lista) == array[2]);
//59
	print_test("Largo lista igual a 3", lista_largo(lista) == 3);
	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
//60
	lista_t *lista_1 = lista_crear();
	for (int i = 0; i<3; i++)
		lista_insertar_ultimo(lista_1, array[i]);
	lista_iter_t *iter_1 = lista_iter_crear(lista_1);
	print_test("actual es igual a primero", lista_iter_ver_actual(iter_1) == lista_ver_primero(lista_1));
	for (int j = 0; j<2 ; j++)
		lista_iter_avanzar(iter_1);
//61
	print_test("Actual no apunta a NULL", !lista_iter_al_final(iter_1));
	lista_borrar(lista_1, iter_1);
	print_test("Borrar ultimo elemento, actual apunta a NULL", lista_iter_al_final(iter_1));
//62
	lista_insertar(lista_1, iter_1, array[3]);
	print_test("Insertar cuando iter apunta a ultimo", lista_iter_ver_actual(iter_1) == array[3]);
	lista_iter_destruir(iter_1);
	lista_destruir(lista_1, NULL);


}

/* Ejecuta todas las pruebas. */
void pruebas_lista()
{
	pruebas_generales();
	pruebas_con_iteradores();
	prueba_insertar_borrar();
	pruebas_iterador_en_primero();
	pruebas_iterador_en_ultimo();

}

/* Programa principal. */
int main(void)
{
	pruebas_lista();
	return 0;
}