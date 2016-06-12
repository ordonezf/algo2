#include "heap.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* ******************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

/* Función auxiliar para imprimir si estuvo OK o no. */
void print_test(char* name, bool result)
{
	printf("%s: %s\n", name, result? "OK" : "ERROR");
}

void ptr_swap(void** x, void** y)
{
	void* temp = *x;
	*x = *y;
	*y = temp;
}

int wrapper_strcmp(const void *a, const void *b){
	return strcmp((char*) a ,(char*) b);
}

int str_intcmp(const void* clave_izq, const void* clave_der)
{
	long izq = strtol(clave_izq, NULL, 10);
	long der = strtol(clave_der, NULL, 10);

	return (int)(izq - der);
}

void pruebas_str_intcmp()
{
	assert(str_intcmp("0","0000000") == 0);
	assert(str_intcmp("1","0") > 0);
	assert(str_intcmp("-20","0") < 0);
	assert(str_intcmp("-0","+0") == 0);
	assert(str_intcmp("-20","+20") < 0);
	assert(str_intcmp("0000001000","00999") > 0);
}

int intcmp(const void* int_1, const void* int_2)
{
	return (int)(*(int*)int_1 - *(int*)int_2);
}

void prueba_intcmp()
{
	int x = 4;
	int y = 5;
	assert(intcmp(&x,&y) < 0);
	assert(intcmp(&y,&x) > 0);

	int a = 0;
	int b = 0;
	assert(intcmp(&a,&b) == 0);
	
	int h = 1000;
	int n = -1000;
	assert(intcmp(&h, &n) > 0);
	assert(intcmp(&n, &h) < 0);
}

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

void prueba_crear_heap_vacio()
{
	heap_t* heap = heap_crear(wrapper_strcmp);

	print_test("Prueba heap crear heap vacio", heap);
	print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
	print_test("Prueba heap ver maximo es NULL", !heap_ver_max(heap));
	print_test("Prueba heap desencolar es NULL", !heap_desencolar(heap));
	print_test("Prueba heap esta vacio es TRUE", heap_esta_vacio(heap));

	heap_destruir(heap, NULL);
}

void prueba_heap_basico()
{
	heap_t* heap = heap_crear(wrapper_strcmp);

	print_test("Prueba heap cantidad es 0", heap_cantidad(heap) == 0);
	print_test("Prueba heap esta vacio es true", heap_esta_vacio(heap));

	char* b = "b";
	print_test("Prueba heap encolamos -b-", heap_encolar(heap, b));
	print_test("Prueba heap cantidad es 1", heap_cantidad(heap) == 1);
	print_test("Prueba heap heap no esta vacio", !heap_esta_vacio(heap));
	print_test("Prueba heap max es igual a b", heap_ver_max(heap) == b);

	char* s = "s";
	print_test("Prueba heap encolamos -s-", heap_encolar(heap, s));
	print_test("Prueba heap max es igual a s", heap_ver_max(heap) == s);
	print_test("Prueba heap desencolar es igual a s", heap_desencolar(heap) == s);

	print_test("Prueba heap maximo es b", heap_ver_max(heap) == b);
	print_test("Prueba heap desencolar es igual a b", heap_desencolar(heap) == b);
	print_test("Prueba heap esta vacio es true", heap_esta_vacio(heap));

	// Probar que no se rompa vaciandolo constantemente
	char* letras[] = {"a","X","5","!"};

	bool ok = true;
	for (size_t i = 0; i < sizeof(letras)/sizeof(*letras); i++)
	{
		ok = ok && heap_encolar(heap, letras[i]);
		ok = ok && heap_ver_max(heap) == letras[i];
		ok = ok && heap_cantidad(heap) == 1;
		ok = ok && !heap_esta_vacio(heap);
		ok = ok && heap_desencolar(heap) == letras[i];
		ok = ok && heap_esta_vacio(heap);
		ok = ok && heap_cantidad(heap) == 0;
	}
	print_test("Prueba heap vaciar sucesivamente", ok);
	heap_destruir(heap, NULL);
}

void prueba_heap_pocos_elementos()
{
	prueba_intcmp();
	heap_t* heap = heap_crear(intcmp);

	int valores[] = {-20,-1,0,1,2,3,4,50,666};

	bool ok = true;
	size_t i;
	for (i = 0; i < sizeof(valores)/sizeof(*valores); i++)
	{
		ok = ok && heap_encolar(heap, valores + i);
		ok = ok && heap_ver_max(heap) == valores + i;
		ok = ok && heap_cantidad(heap) == i + 1;
		ok = ok && !heap_esta_vacio(heap);
		if (!ok) break;
	}

	print_test("Prueba heap encolamos elementos y comprobamos", ok);
	if (!ok) printf("Fallo en el valor: %d\n", valores[i]);

	ok = true;
	while (i--)
	{
		ok = ok && heap_desencolar(heap) == valores + i;
		ok = ok && heap_cantidad(heap) == i;
		ok = ok && (i >= 1? !heap_esta_vacio(heap): heap_esta_vacio(heap));
		if (!ok) break;
	}

	print_test("Prueba heap DESencolamos los elementos y comprobamos", ok);
	if (!ok) printf("Fallo en el valor: %d\n", valores[i]);

	heap_destruir(heap, NULL);
}

void prueba_heap_destruir()
{
	heap_t* heap = heap_crear(intcmp);

	size_t tam = 20;
	int* valores[tam];

	for (size_t j = 0; j < tam; j++)
	{
		valores[j] = malloc(sizeof(int));
		*valores[j] = j / 2;
	}
	//si todo sale bien quedo: {0,0,1,1,2,2,3,3....9,9}

	bool ok = true;
	size_t i;
	for (i = 0; i < tam; i++)
	{
		ok = ok && heap_encolar(heap, valores[i]);
		ok = ok && *(int*)heap_ver_max(heap) == *valores[i];
		ok = ok && heap_cantidad(heap) == i + 1;
		ok = ok && !heap_esta_vacio(heap);
		if (!ok) break;
	}
	print_test("Prueba heap encolamos elementos y comprobamos", ok);
	if (!ok) printf("Fallo en el valor: %d\n", *valores[i]);

	printf("Prueba heap el destructor se ocupa de los elementos en memoria.\n");
	heap_destruir(heap, free);
}

void prueba_heap_volumen(size_t largo)
{
	heap_t* heap = heap_crear(intcmp);

	int* valores = malloc(largo * sizeof(int));
	if (!valores) return;

	for (size_t j = 0; j < largo; j++)
		valores[j] = j;

	bool ok = true;
	size_t i;
	for (i = 0; i < largo; i++)
	{
		ok = ok && heap_encolar(heap, valores + i);
		ok = ok && heap_ver_max(heap) == valores + i;
		ok = ok && heap_cantidad(heap) == i + 1;
		ok = ok && !heap_esta_vacio(heap);
		if (!ok) break;
	}

	print_test("Prueba heap -Volumen- encolamos elementos y comprobamos", ok);
	if (!ok) printf("Fallo en el valor: %d\n", valores[i]);

	ok = true;
	while (i--)
	{
		ok = ok && heap_desencolar(heap) == valores + i;
		ok = ok && heap_cantidad(heap) == i;
		ok = ok && (i >= 1? !heap_esta_vacio(heap): heap_esta_vacio(heap));
		if (!ok) break;
	}

	print_test("Prueba heap -Volumen- DESencolamos los elementos y comprobamos", ok);
	if (!ok && i < largo) printf("Fallo en el valor: %d\n", valores[i]);

	free(valores);
	heap_destruir(heap, NULL);
}

void prueba_heapsort_basico()
{
	pruebas_str_intcmp();

	size_t tam = 10;
	char* valores[] = {"0","-20","40","1","-666","666","23","13","1","-20"};
	char* ordenados[] = {"-666","-20","-20","0","1","1","13","23","40","666"};

	heap_sort((void**)valores, tam, str_intcmp);

	bool ok = true;
	size_t i;
	for (i = 0; ok && i < tam; i++)
		ok = (str_intcmp(valores[i], ordenados[i]) == 0);

	print_test("Prueba heapsort arreglo basico", ok);
	if (!ok) printf("Fallo en el valor: %s, esperaba: %s\n", valores[i], ordenados[i]);
}

void prueba_heapsort_volumen(size_t largo)
{
	prueba_intcmp();

	int** valores = malloc(largo * sizeof(int*));
	if (!valores) return;

	// inicializamos
	for (size_t i = 0; i < largo; i++)
	{
		valores[i] = malloc(sizeof(int));
		if (!valores[i]) return;
		*valores[i] = i;
	}

	// desordenamos
	// hacer rand % largo al parecer no es muy bueno, pero para esto zafa.
	for (size_t i = 0; i < largo; i++)
		ptr_swap((void**)(valores + i), (void**)(valores + (rand() % largo)));

	heap_sort((void**)valores, largo, intcmp);

	bool ok = true;
	size_t i;
	for (i = 0; ok && i < largo; i++)
		ok = *valores[i] == i;

	print_test("Prueba heapsort Volumen", ok);
	if (!ok) printf("Fallo en el valor: %d, esperaba: %zu\n", *valores[i], i);

	for (i = 0; i < largo; i++) free(valores[i]);
	free(valores);
}

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

int main(int argc, char** argv)
{
	if (argc < 2){
		prueba_crear_heap_vacio();
		prueba_heap_basico();
		prueba_heap_pocos_elementos();
		prueba_heap_destruir();
		prueba_heap_volumen(5000);
		prueba_heapsort_basico();
		prueba_heapsort_volumen(5000);
	} else {
		size_t largo = atoi(argv[1]);
		prueba_heap_volumen(largo);
		prueba_heapsort_volumen(largo);
	}
	return 0;
}