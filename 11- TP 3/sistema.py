#!/usr/bin/env python
# -*- coding: utf-8 -*-

from grafo import Grafo, Vertice, GrafoError
from comandos import *
from operator import eq, ne, le

class SistemaError(Exception):pass
class ComandoInvalidoError(SistemaError): pass
class ParametroInvalidoError(SistemaError): pass
class ArchivoInvalidoError(SistemaError): pass

class Sistema(object):
	"""Sistema de consultas para una red de musicos."""

	def __init__(self, archivo):
		"""Inicializa el Sistema. Recibe el archivo del grafo.
		El archivo debe tener el sig. formato:
		"<Cantidad_de_vertices>"
		"<id_vertice> <nombre_vertice>"
		"............................."
		"<Cantidad_de_aristas>"
		"<id_arista> <id_vertice_inicial> <id_vertice_final>"
		"..................................................."
		"""
		self.comando = None
		self.parametros = []
		self.funciones_asociadas = {
			"recomendar": self.comando_recomendar, 
			"difundir": self.comando_difundir, 
			"centralidad": self.comando_centralidad,
			"debilidad": self.comando_debilidad,
			"camino": self.comando_camino,
			"diametro": self.comando_diametro,
			"agrupamiento": self.comando_agrupamiento,
			"distancias": self.comando_distancias,
			"subgrupos": self.comando_subgrupos,
			"articulacion": self.comando_articulacion
		}
		self.grafo = None
		self.musico_a_id_vertice = {}
		self._crear_grafo_desde_archivo(archivo)

	def _crear_grafo_desde_archivo(self, archivo):
		"""Crea el grafo y le carga la configuracion del archivo."""
		self.grafo = Grafo()
		try:
			with open(archivo,"r") as f:
				#agregado de vertices
				for i in xrange(int(f.readline())):
					args = f.readline().split()
					self.musico_a_id_vertice[args[1]] = args[0]
					self.grafo.agregar_vertice(args[0], dato_vertice=args[1])

				#agregado de aristas
				for i in xrange(int(f.readline())):
					args = f.readline().split()
					self.grafo.agregar_arista(args[1], args[2])

		except IOError:
			raise ArchivoInvalidoError("Error de archivo")
		except ValueError:
			raise ArchivoInvalidoError("Las cantidades no son numeros")
		except IndexError:
			raise ArchivoInvalidoError("Formato inadecuado")
		except GrafoError:
			raise ArchivoInvalidoError("Forma un grafo inadecuado")

	def _obtener_id_vertice(self, musico):
		try:
			return self.musico_a_id_vertice[musico]
		except KeyError:
			raise ParametroInvalidoError\
				("El musico %s no existe" % str(musico))

	def _obtener_cantidad(self, numero_en_str):
		if not numero_en_str.isdigit():
			raise ParametroInvalidoError\
				("La cantidad debe ser un numero entero mayor a 0")
		return int(numero_en_str)

	def _verificar_cant_parametros(self, f_operador, cantidad):
		"""Compara utilizando la funcion_operador la cantidad de
		parametros recibidos con la cantidad (rec. por parametro).
		Si es falsa la comparacion levanta ParametroInvalidoError.
		"""
		if f_operador(len(self.parametros), cantidad):
			return

		raise ParametroInvalidoError\
			("La cantidad de paremetros no es la esperada")

	def _leer_entrada(self):
		"""Lee la entrada del usuario y la formatea.
		Devuelve False si no recibio nada o recibio EOF,
		True en otro caso.
		"""
		try:
			entrada = raw_input()
		except EOFError:
			return False
		#Ya que puede venir con comas
		self.parametros = entrada.split(",")
		entrada = " ".join(self.parametros) #agrego whitespace
		self.parametros = entrada.split() #me saca todo.
		try:
			self.comando = self.parametros.pop(0)
		except IndexError:
			return False
		return True

	def _ejecutar_comando(self):
		"""Ejecuta el comando leido desde la entrada estandar."""
		try:
			self.funciones_asociadas[self.comando]()
		except KeyError:
			raise ComandoInvalidoError\
				("El comando %s no es un comando valido" % self.comando)

	def ejecutar(self):
		"""Ejecuta el programa."""
		while self._leer_entrada():
			try:
				self._ejecutar_comando()
			except SistemaError, e:
				print e

	def comando_recomendar(self):
		"""Ejecuta el comando recomendar."""
		self._verificar_cant_parametros(eq, 2)
		id_vertice = self._obtener_id_vertice(self.parametros[0])
		cantidad = self._obtener_cantidad(self.parametros[1])

		for vertice in recomendar(self.grafo, id_vertice, cantidad):
			print vertice.obtener_dato()

	def comando_difundir(self):
		"""Ejecuta el comando difundir."""
		self._verificar_cant_parametros(ne, 0)

		lista_de_ids = []
		# Por si hay musicos repetidos
		for musico in self.parametros:
			if self._obtener_id_vertice(musico) not in lista_de_ids:
				lista_de_ids.append(self._obtener_id_vertice(musico))

		difusores = difundir(self.grafo, lista_de_ids)
		print len(difusores)
		for d in difusores:
			print d.obtener_dato()

	def comando_centralidad(self):
		"""Ejecuta el comando centralidad."""
		self._verificar_cant_parametros(eq, 1)
		cantidad = self._obtener_cantidad(self.parametros[0])

		for vertice in centralidad(self.grafo, cantidad):
			print vertice.obtener_dato()
		
	def comando_debilidad(self): print "No implementado, ver centralidad"

	def comando_camino(self):
		"""Ejecuta el comando camino."""
		self._verificar_cant_parametros(eq, 2)
		v_inicial = self._obtener_id_vertice(self.parametros[0])
		v_final = self._obtener_id_vertice(self.parametros[1])

		if v_inicial == v_final:
			print "Son el mismo!"
			return
		
		camino_minimo = camino(self.grafo, v_inicial, v_final)
		if not camino_minimo:
			print "No hay camino"
		else:
			for v in camino_minimo:
				print v.obtener_dato()

	def comando_diametro(self):
		"""Ejecuta el comando diametro."""
		self._verificar_cant_parametros(eq, 0)

		camino = diametro(self.grafo)
		print len(camino) - 1
		for vertice in camino:
			print vertice.obtener_dato()

	def comando_agrupamiento(self):
		"""Ejecuta el comando agrupamiento."""
		self._verificar_cant_parametros(le, 1)
			
		if len(self.parametros) == 1:
			id_vertice = self._obtener_id_vertice(self.parametros[0])
			print clustering(self.grafo.obtener_vertice(id_vertice))
		else:
			print agrupamiento(self.grafo)

	def comando_distancias(self):
		"""Ejecuta el comando distancias."""
		self._verificar_cant_parametros(eq, 1)
		id_vertice = self._obtener_id_vertice(self.parametros[0])

		for vertices in distancias(self.grafo, id_vertice):
			print "%d:" % len(vertices)
			for v in vertices:
				print v.obtener_dato(),
			print
		
	def comando_subgrupos(self):
		"""Ejecuta el comando subgrupos."""
		self._verificar_cant_parametros(eq, 0)

		for cantidad_vertices in subgrupos(self.grafo):
			print cantidad_vertices

	def comando_articulacion(self):
		"""Ejecuta el comando articulacion."""
		self._verificar_cant_parametros(eq, 0)

		for punto in articulacion(self.grafo):
			print punto.obtener_dato()
