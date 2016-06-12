#!/usr/bin/env python
# -*- coding: utf-8 -*-

from collections import deque
from grafo import Grafo, Vertice

class BFSError(Exception): pass
class VerticeNoPerteneceAlGrafoError(BFSError): pass

class BFS(object):
	"""Implementa el recorrido BFS de un grafo.
	Permite multiples consultas en base al recorrido.
	"""

	def __init__(self, grafo, vertice_raiz):
		"""Recibe el grafo a aplicar BFS y el vertice raiz(Obj).
		Pre: El vertice raiz es de clase Vertice y es parte del grafo.
		"""
		self.grafo = grafo
		self.padre = {}
		self.visitado = {}
		self.distancia = {}
		self.nivel = {}
		self.cola = deque()

		if isinstance(vertice_raiz, Vertice) and vertice_raiz not in grafo:
			raise VerticeNoPerteneceAlGrafoError\
							("El vertice raiz no es parte del grafo")

		for v in grafo:
			self.visitado[v] = False
			self.distancia[v] = -1 #indica inexistencia de camino
			self.padre[v] = None   #indica inexistencia de camino

		self.cola.append(vertice_raiz)
		self.visitado[vertice_raiz] = True
		self.distancia[vertice_raiz] = 0
		self.nivel[self.distancia[vertice_raiz]] = [vertice_raiz]

	def _recorrer(self, vertice_buscado=None, max_nivel=None):
		"""Aplica BFS al grafo recolectando informacion.
		Si recibe algun parametro (opcional), entonces detiene el
		recorrido cuando sabe el resultado de lo pedido.
		Pre: El vertice es de clase Vertice y es parte del grafo.
		"""
		while len(self.cola) != 0:

			if vertice_buscado:
				try:
					if self.visitado[vertice_buscado]:
						return
				except KeyError:
					raise VerticeNoPerteneceAlGrafoError\
							("El vertice buscado no es parte del grafo")
			if max_nivel:
				if max_nivel+1 in self.nivel:
					return

			padre = self.cola.popleft()
			for vertice in padre.iter_de_adyacentes():
				if not self.visitado[vertice]:
					self.cola.append(vertice)
					self.visitado[vertice] = True
					self.padre[vertice] = padre
					self.distancia[vertice] = self.distancia[padre] + 1
					
					if self.distancia[vertice] not in self.nivel:
						self.nivel[self.distancia[vertice]] = []
					self.nivel[self.distancia[vertice]].append(vertice)

	def vertices_a_nivel(self, n):
		"""Devuelve una lista de vertices(Obj) que estan a 
		distancia/nivel n. Si no hay vertices devuelve lista vacia.
		"""
		self._recorrer(vertice_buscado=None, max_nivel=n)
		try:
			return list(self.nivel[n])		
		except KeyError:
			return []

	def cantidad_a_nivel(self, n):
		"""Devuelve la cantidad de vertices que estan a 
		distancia/nivel n. Si no hay vertices devuelve 0.
		"""
		self._recorrer(vertice_buscado=None, max_nivel=n)
		try:
			return len(self.nivel[n])		
		except KeyError:
			return 0

	def __iter__(self):
		"""Iterador que recorre los vertices(Obj) por niveles"""
		self._recorrer()
		for n in self.nivel:
			for v in self.nivel[n]:
				yield v

	def distancia_a(self, vertice):
		"""Devuelve la distancia a un vertice.
		La distancia es -1 si no existe camino que los una.
		Pre: vertice es de clase Vertice y es parte del grafo.
		"""
		self._recorrer(vertice_buscado=vertice)
		return self.distancia[vertice]
	
	def camino_minimo(self, vertice):
		"""Calcula el camino minimo a un vertice
		Devuelve una lista con los vertices(Obj) que forman el camino
		(incluyendo vertice_raiz y destino).
		Si no existe camino o vertice es la raiz, se devuelve lista vacia.
		Pre: vertice es de clase Vertice y es parte del grafo.
		"""
		self._recorrer(vertice_buscado=vertice)
		camino = deque()
		if self.padre[vertice]: # Si es None no habia camino, o era la raiz
			while vertice:
				camino.appendleft(vertice)
				vertice = self.padre[vertice]
		return list(camino)