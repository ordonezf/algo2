#include "entrada.h"
#include "sistema.h"

#define POS_COMANDO 0

/* Programa principal */
int main(void)
{
	entrada_t* entrada = entrada_crear(POS_COMANDO);
	if (!entrada) return 1;

	sistema_t* sistema = sistema_crear();
	if (!sistema)
	{
		entrada_destruir(entrada);
		return 1;
	}

	while (entrada_leer(entrada, cantidad_parametros, (void*)sistema) &&
		   ejecutar_comando(sistema, entrada_comando(entrada), 
					entrada_parametros(entrada), entrada_cant_parametros(entrada)));

	sistema_destruir(sistema);
	entrada_destruir(entrada);
	return 0;
}