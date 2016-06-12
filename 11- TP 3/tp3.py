#!/usr/bin/env python
# -*- coding: utf-8 -*-

from sistema import Sistema, SistemaError
from sys import argv

USO = """
USO: tp3.py <nombre_archivo_grafo>
El archivo debe tener el sig. formato:

	"<Cantidad_de_vertices>"
	"<id_vertice> <nombre_vertice>"
	"............................."
	"<Cantidad_de_aristas>"
	"<id_arista> <id_vertice_inicial> <id_vertice_final>"
	"..................................................."
"""

def main():
	"""Programa principal para el sistema de consultas."""
	if len(argv) != 2: 
		print USO
	else:
		try:
			s = Sistema(argv[1])
		except SistemaError, e:
			print e
			return 1
		s.ejecutar()
	return 0

if __name__ == '__main__':
	main()