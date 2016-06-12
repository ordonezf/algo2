#include "cola.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*******************************************************************
 *                        PRUEBAS UNITARIAS                        *
 ******************************************************************/


/* Función auxiliar para imprimir si estuvo OK o no. */
void print_test(char* name, bool result)
{
	printf("%s: %s\n", name, result? "OK" : "ERROR");
}
void pruebas_cola_generales(){
	//1
	cola_t *cola = cola_crear();
	print_test("Se creo la cola", cola != NULL);
	//2
	print_test("Cola esta vacia", cola_esta_vacia(cola));
	//3
	int num = 5;
	int* mun = &num;
	cola_encolar(cola, mun);
	print_test("Cola no esta vacia", !cola_esta_vacia(cola));
	//4
	print_test("Primer elemento cola igual a 5",cola_ver_primero(cola) == mun);
	//5
	int aux = *((int*) cola_desencolar(cola));
	print_test("Desencole y es igual a 5", aux == num);
	//6
	print_test("Cola esta vacia", cola_esta_vacia(cola));
	//7
	void* nada = cola_desencolar(cola);
	print_test("Desencolar cola vacia igual a NULL",nada == NULL);
	//8
	print_test("cola vacia", cola_esta_vacia(cola));
	cola_t *cola_1 = cola_crear();
	cola_encolar(cola, cola_1);
	print_test("Encolar otra cola", !cola_esta_vacia(cola));
	//9
	print_test("Primer elemento igual a cola", cola_ver_primero(cola) == cola_1);
	//10
	print_test("Desencolar cola, igual a cola", cola_desencolar(cola) == cola_1);
	//11
	cola_destruir(cola, NULL);
	cola_destruir(cola_1, NULL);
	print_test("Destruir las dos colas", true);
	//12
	cola_t *cola_2 = cola_crear();
	char h = 'h', o ='o', l = 'l';
	char* h_ = &h;
	char* o_ = &o;
	char* l_ = &l;
	//13
	cola_encolar(cola_2, h_);
	print_test("Encolar igual h", cola_ver_primero(cola_2) == h_);
	cola_encolar(cola_2, o_);
	cola_encolar(cola_2, l_);
	//14
	print_test("Desencolar igual h", *((char*) cola_desencolar(cola_2)) == h);
	//15
	print_test("Desencolar igual o", *((char*) cola_desencolar(cola_2)) == o);
	//16
	print_test("Desencolar igual l", *((char*) cola_desencolar(cola_2)) == l);
	//17
	print_test("Cola esta vacia", cola_esta_vacia(cola_2));
	//18
	for (int i = 0; i < 500; i++){
		cola_encolar(cola_2, h_);
	}
	cola_destruir(cola_2,NULL);
	print_test("Destruir cola con 500 elementos", true);

}

void destruir_punto(void* dato){
	free(dato);
}

typedef struct punto{
	int x;
	int y;
} punto;


void pruebas_cola_destruir(){
	//19
	punto* p = malloc(sizeof(punto));
	p->x = 1;
	p->y = 2;

	cola_t *cola =cola_crear();
	cola_encolar(cola, p);
	print_test("Ver primero igual a struct punto", cola_ver_primero(cola) == p);
	cola_desencolar(cola);
	//20
	print_test("Desencolar struct punto, cola vacia", cola_esta_vacia(cola));
	//21
	cola_encolar(cola, p);
	cola_destruir(cola, destruir_punto);
	print_test("Destruir cola con struct punto encolado", true);
	//22
	cola_t* cola_1 = cola_crear();
	for(int i = 0; i<500; i++){
		if (i%2==0)
			cola_encolar(cola_1, p);
		else
			cola_desencolar(cola_1);
	}
	print_test("Intercalar encolar y desencolar", cola_esta_vacia(cola_1));
	cola_destruir(cola_1, destruir_punto);
	destruir_punto(p);

}

/* Ejecuta todas las pruebas. */
void pruebas_cola()
{
	pruebas_cola_generales();
	pruebas_cola_destruir();
}

/* Programa principal. */
int main(void)
{
	pruebas_cola();
	return 0;
}
