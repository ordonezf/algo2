#include "pila.h"
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

void pruebas_pila_con_numeros()
{	puts("Pruebas con numeros \n");

//1
	pila_t *pila = pila_crear();
	print_test("Crear una pila", pila != NULL);
//2
	int numero = 5;
	int *num = &numero;
	pila_apilar(pila, num);
	print_test("Apila un elemento", ! pila_esta_vacia(pila));
//3
	print_test("Ver tope", pila_ver_tope(pila) == num);
//4
	pila_desapilar(pila);
	print_test("Desapila un elemento", pila_esta_vacia(pila) );

//5
	pila_destruir(pila);
	print_test("Destruir pila", true);
//6
	bool ok = true;
	pila_t *pila_1 = pila_crear();
	for (int i = 0; i < 1000; i++){
		int valor_1 = i;
		int *valor_2 = &valor_1;
		ok &= pila_apilar(pila_1, valor_2 );
	}
	print_test("Apila 1000 elementos", ok);
//7
	for (int j = 0; j < 1000; j++){
		pila_desapilar(pila_1);
	}
	print_test("Desapila 1000 elementos", pila_esta_vacia(pila_1));

	pila_destruir(pila_1);
//8
	pila_t *pila_2 = pila_crear();
	for (int u = 0; u<1000000; u++){
		int valor_u = u;
		int *valor_ = &valor_u;
		ok &= pila_apilar(pila_2, valor_);
	}

	pila_destruir(pila_2);
	print_test("Destruye pila con 1000000 de elementos", true);
	puts("");
}

void pruebas_pila_con_strings()
{	puts("Pruebas con arrays \n");

//9
	pila_t *pila = pila_crear();
	int array[] = {1,2,3,4};
	pila_apilar(pila, array);
	print_test("Apilo array", !pila_esta_vacia(pila));
//10
	print_test("Ver tope", pila_ver_tope(pila) == array);
//11
	print_test("Desapilo array y chekeo que sea igual", array == pila_desapilar(pila));
//12
	bool ok = true;
	for (int i = 0; i < 10000; i++)
		ok = pila_apilar(pila, array);
	print_test("Apilo 10000 arrays", ok);
//13
	ok = true;
	for (int j = 0; j < 10000; j++)
		ok &= (array == pila_desapilar(pila));
	print_test("Chekeo que todo lo apilado sea igual", ok);
//14
	print_test("Pila esta vacia", pila_esta_vacia(pila));
//15
	pila_destruir(pila);
	print_test("Destruir pila", true);


}


/* Ejecuta todas las pruebas. */
void pruebas_pila()
{
	pruebas_pila_con_numeros();
	pruebas_pila_con_strings();

}

/* Programa principal. */
int main(void)
{
	pruebas_pila();
	return 0;
}