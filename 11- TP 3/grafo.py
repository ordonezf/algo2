#!/usr/bin/env python
# -*- coding: utf-8 -*-

class GrafoError(Exception): pass
class VerticeRepetidoError(GrafoError):pass
class AristaRepetidaError(GrafoError): pass
class VerticeInexistenteError(GrafoError): pass
class AristaInexistenteError(GrafoError): pass

class Vertice(object):
	"""Vertice a utilizar con la clase Grafo"""

	def __init__(self, id, dato=None):
		"""Crea un Vertice a partir de un id y opcionalmente
		guarda un dato.
		"""
		self.id = id
		self.dato = dato
		# Adyacentes son las aristas salientes en digrafos.
		self.adyacentes = {}

	def _agregar_coneccion(self, vertice, dato_arista=None):
		"""A utilizar por la clase Grafo.
		Crea una arista que va de self a vertice (Obj)
		guardando el dato (opcional) de la arista.
		Ojo: Si se usa de forma incorrecta sobreescribe el dato
		Pre: El vertice es de la clase Vertice
		"""
		self.adyacentes[vertice] = dato_arista

	def _eliminar_coneccion(self, vertice):
		"""A utilizar por la clase Grafo.
		Elimina la arista que va de self a vertice (Obj)
		Pre: Existe la arista. El vertice es de la clase Vertice.
		"""
		del self.adyacentes[vertice]

	def _obtener_dato_arista(self, vertice):
		"""A utilizar por la clase Grafo.
		Devuelve el dato de la arista que va de self a vertice (Obj)
		Pre: Existe la arista. El vertice es de la clase Vertice.
		"""
		return self.adyacentes[vertice]

	def esta_conectado_a(self, vertice):
		"""Devuelve True si hay una arista que va de self a vertice
		Pre: El vertice es de la clase Vertice.
		"""
		return vertice in self.adyacentes

	def obtener_id(self):
		"""Devuelve el id"""
		return self.id

	def obtener_dato(self):
		"""Devuelve el dato"""
		return self.dato

	def obtener_adyacentes(self):
		"""Devuelve una lista de los vertices adyacentes"""
		return self.adyacentes.keys()

	def cantidad_adyacentes(self):
		"""Devuelve la cantidad de vertices adyacentes"""
		return len(self.adyacentes)

	def iter_de_adyacentes(self):
		"""Devuelve un iterador de los vertices adyacentes"""
		return self.adyacentes.iterkeys()


class Grafo(object):
	"""
	TDA Grafo dirigido o no, ponderado o no, con libertad para guardar datos.
	(Utilizarlo ponderado significa guardar el peso como dato de la arista)
	Restriccion:
		-Sin aristas paralelas.
	"""
	
	def __init__(self, dirigido=False):
		"""Crea un grafo vacio. Por defecto no dirigido."""
		self.es_dirigido = dirigido
		self.vertices = {}
		self.cant_aristas = 0

	def agregar_vertice(self, id_vertice, dato_vertice=None):
		"""Agrega un vertice al grafo. Recibe el id a asignarle.
		Recibe opcionalmente un dato a guardar en el vertice.
		Pre: el vertice no existia todavia.
		"""
		if id_vertice in self.vertices:
			raise VerticeRepetidoError("El vertice:'%s' YA existe" % str(id_vertice))

		self.vertices[id_vertice] = Vertice(id_vertice, dato_vertice)

	def agregar_arista(self, id_vertice_inicial, id_vertice_final, dato_arista=None):
		"""Agrega una nueva arista desde v_inicial a v_final.
		Recibe opcionalmente un dato a guardar en la arista.
		Pre: Los vertices deben existir y la coneccion no se debe repetir.
		"""
		vertice_inicial = self.obtener_vertice(id_vertice_inicial)
		vertice_final = self.obtener_vertice(id_vertice_final)

		if vertice_inicial.esta_conectado_a(vertice_final):
			raise AristaRepetidaError("La arista %s -> %s YA existe" % 
							(str(id_vertice_inicial), str(id_vertice_final)))

		vertice_inicial._agregar_coneccion(vertice_final, dato_arista)
		if not self.es_dirigido:
			vertice_final._agregar_coneccion(vertice_inicial, dato_arista)

		self.cant_aristas += 1

	def eliminar_vertice(self, id_vertice):
		"""Elimina el vertice y todas las aristas conectadas a el.
		Pre: El vertice debe existir
		"""
		try:
			vertice = self.vertices.pop(id_vertice)
		except KeyError:
			raise VerticeInexistenteError("El vertice:'%s' NO existe" % str(id_vertice))

		if not self.es_dirigido: 
			for adyacente in vertice.iter_de_adyacentes():
				adyacente._eliminar_coneccion(vertice)
		else: # O(n)
			for posible_vecino in self:
				if posible_vecino.esta_conectado_a(vertice):
					posible_vecino._eliminar_coneccion(vertice)
					self.cant_aristas -= 1

		self.cant_aristas -= vertice.cantidad_adyacentes()

	def eliminar_arista(self, id_vertice_inicial, id_vertice_final):
		"""Elimina la arista que va desde v_inicial a v_final.
		Pre: Deben existir los vertices y la coneccion entre ellos.
		"""
		vertice_inicial = self.obtener_vertice(id_vertice_inicial)
		vertice_final = self.obtener_vertice(id_vertice_final)

		if not vertice_inicial.esta_conectado_a(vertice_final):
			raise AristaInexistenteError("La arista %s -> %s NO existe" % 
							(str(id_vertice_inicial), str(id_vertice_final)))

		vertice_inicial._eliminar_coneccion(vertice_final)
		if not self.es_dirigido:
			vertice_final._eliminar_coneccion(vertice_inicial)

		self.cant_aristas -= 1

	def obtener_vertice(self, id_vertice):
		"""Devuelve el Vertice asociado al id
		Pre: El vertice debe existir.
		"""
		try:
			return self.vertices[id_vertice]
		except KeyError:
			raise VerticeInexistenteError("El vertice:'%s' NO existe" % str(id_vertice))

	def obtener_dato_arista(self, id_vertice_inicial, id_vertice_final): #eq a obtener_peso
		"""Devuelve el dato de la arista entre v_inicial y v_final.
		Pre: Deben existir los vertices y la coneccion entre ellos.
		"""
		vertice_inicial = self.obtener_vertice(id_vertice_inicial)
		vertice_final = self.obtener_vertice(id_vertice_final)

		if not vertice_inicial.esta_conectado_a(vertice_final):
			raise AristaInexistenteError("La arista %s -> %s NO existe" % 
							(str(id_vertice_inicial), str(id_vertice_final)))

		return vertice_inicial._obtener_dato_arista(vertice_final)

	def lista_de_vertices(self):
		"""Devuelve una lista de todos los objetos Vertice del grafo"""
		return self.vertices.values()

	def cantidad_vertices(self):
		"""Devuelve la cantidad de vertices"""
		return len(self.vertices)

	def cantidad_aristas(self):
		"""Devuelve la cantidad de aristas"""
		return self.cant_aristas

	def __iter__(self):
		"""Iterador de los objetos Vertice asociados a los ids"""
		return self.vertices.itervalues()

	def __contains__(self, item):
		"""Verificacion de inclusion de vertices (id u objeto)"""
		if isinstance(item, Vertice):
			item = item.obtener_id()
		return item in self.vertices