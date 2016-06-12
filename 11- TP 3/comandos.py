#!/usr/bin/env python
# -*- coding: utf-8 -*-

from grafo import Grafo, Vertice
from BFS import BFS
from heapq import heapify, heappushpop, heappop
from collections import deque

def _primeros_k_max(diccionario, k):
	"""Devuelve una lista con las mayores k claves (tomando como
	comparacion su valor en el diccionario) en orden decreciente.
	Pre: El diccionario existe, k es un numero entero positivo.
	"""
	items = [(x,y) for y,x in diccionario.items()]
	heap_min = list(items[:k])
	heapify(heap_min)

	for i in xrange(k, len(items)):
		valor = items[i][0]
		min_max_valor = heap_min[0][0]
		if valor > min_max_valor:
			heappushpop(heap_min, items[i])

	resultado = deque()
	while heap_min: #inserto a la izq de menor a mayor.
		resultado.appendleft(heappop(heap_min)[1])

	return list(resultado)

def recomendar(grafo, id_vertice, cantidad):
	"""Recomienda vertices a nivel 2 (desde id_vertice) con mayor 
	cantidad de conexiones a vertices a nivel 1.
	Recibe el grafo, el id del vertice y la cantidad maxima a recomendar.
	Devuelve una lista con los vertices recomendados.
	Pre: El grafo y el id del vertice existen. La cantidad es un entero 
	mayor a cero. El vertice debe pertenecer al grafo.
	"""
	vertice = grafo.obtener_vertice(id_vertice)
	rec = {}

	for vecino in vertice.iter_de_adyacentes():
		for segundo_vecino in vecino.iter_de_adyacentes():
			if vertice.esta_conectado_a(segundo_vecino) or \
				segundo_vecino == vertice:
				continue
			rec[segundo_vecino] = rec.get(segundo_vecino, 0) + 1

	return _primeros_k_max(rec, cantidad)

def _actualizar_difusores(iterable, posibles, originales, nuevos):
	"""Actualiza el diccionario de posibles difusores con los
	difusores de iterable, exceptuando si estan en originales
	o nuevos. Aparte, verifica y agrega los nuevos difusores
	a la lista de nuevos y devuelve los agregados en esta iteracion.
	"""
	inicio = len(nuevos)
	for difusor in iterable:
		for vecino in difusor.iter_de_adyacentes():
			if vecino in originales or vecino in nuevos:
				continue
			posibles[vecino] = posibles.get(vecino, 0) + 1

			if vecino.cantidad_adyacentes()/2.0 < posibles[vecino]:
				nuevos.append(vecino)
				posibles.pop(vecino)

	return nuevos[inicio:]

def difundir(grafo, ids_vertices):
	"""Calcula la difusion de informacion en un grafo. 
	Un vertice comparte la informacion solamente si mas de la 
	mitad de sus adyacentes tambien la comparten.
	Recibe el grafo y una lista de ids de los vertices difusores.
	Devuelve una lista con los vertices que compartieron la 
	informacion (sin los originales).
	Pre: el grafo y los ids de los vertices existen. Los vertices
	pertenecen al grafo en cuestion.
	"""
	nuevos_dif = []
	posibles_dif = {}
	primeros_dif = [grafo.obtener_vertice(v_id) for v_id in ids_vertices]

	agregados = _actualizar_difusores(
					primeros_dif, posibles_dif, primeros_dif, nuevos_dif)
	while agregados:
		agregados = _actualizar_difusores(
						agregados, posibles_dif, primeros_dif, nuevos_dif)

	return nuevos_dif

def centralidad(grafo, cantidad):
	"""Calcula los vertices mas "populares"/"centrales" del grafo.
	Calculando los caminos minimos de todos los vertices contra 
	todos los vertices (sin repetir), los vertices mas centrales
	son por los que pasan la mayor cantidad de caminos minimos.
	OBS: Los vertices raiz y destino de los caminos minimos no
	se cuentan en el camino que los une.
	Pre: El grafo existe. La cantidad es un entero mayor a 0.
	"""
	pasadas = {v:0 for v in grafo}
	vertices = grafo.lista_de_vertices()

	for i in xrange(len(vertices)):
		bfs = BFS(grafo, vertices[i])
		for j in xrange(i+1, len(vertices)):
			# [1:-1] porque no cuenta la raiz y destino
			for v in bfs.camino_minimo(vertices[j])[1:-1]:
				pasadas[v] += 1

	return _primeros_k_max(pasadas, cantidad)

def camino(grafo, id_vertice_inicial, id_vertice_final):
	"""Calcula el camino minimo desde el vertice inicial
	al vertice final. Devuelve la lista de los vertices (Obj)
	que los unen (incl. inicial y final).
	Pre: el grafo y los ids de los vertices existen. Los vertices
	pertenecen al grafo en cuestion.
	"""
	v_inicial = grafo.obtener_vertice(id_vertice_inicial)
	v_final = grafo.obtener_vertice(id_vertice_final)
	return BFS(grafo, v_inicial).camino_minimo(v_final)

def diametro(grafo):
	"""Calcula el maximo camino minimo del grafo.
	Devuelve el camino: lista de vertices (Obj).
	Pre: El grafo existe.
	"""
	vertices = grafo.lista_de_vertices()
	maximo = 0

	for i in xrange(len(vertices)):
		bfs = BFS(grafo, vertices[i])
		for j in xrange(i+1, len(vertices)):
			distancia = bfs.distancia_a(vertices[j])
			if distancia > maximo:
				maximo = distancia
				camino = bfs.camino_minimo(vertices[j])

	return camino

def clustering(vertice):
	"""Calcula el coeficiente de clustering de un vertice.
	Obs: Devuelve -1 si el coeficiente no es calculable.
	Pre: EL vertice existe y es de clase Vertice.
	"""
	# Cuento las conexiones (por duplicado)
	aristas_entre_vecinos = 0
	for vecino in vertice.iter_de_adyacentes():
		for segundo_vecino in vecino.iter_de_adyacentes():
			if vertice.esta_conectado_a(segundo_vecino):
				aristas_entre_vecinos += 1

	# Calculo coeficiente
	try:
		ady = vertice.cantidad_adyacentes()
		max_aristas_posibles = ady * (ady-1)
		return aristas_entre_vecinos / float(max_aristas_posibles)
	except ZeroDivisionError:
		return -1 #Vertice con 1 vecino o ningun vecino (no calculable)

def agrupamiento(grafo):
	"""Calcula el coeficiente de agrupamiento promedio del grafo.
	Pre: el grafo existe.
	"""
	suma = 0
	incalculables = 0
	for v in grafo:
		coef = clustering(v)
		if coef == -1:
			incalculables += 1
		else:
			suma += coef

	return suma / (grafo.cantidad_vertices()-incalculables)

def distancias(grafo, id_vertice):
	"""Calcula todos los vertices que estan a diferentes niveles/
	distancias del vertice recibido por parametro.
	Devuelve una lista de listas de vertices(Obj). La 1er lista
	seria la del nivel 1, la segunda del nivel 2, etc..
	Pre: El grafo y el id del vertice existen.
	"""
	vertice = grafo.obtener_vertice(id_vertice)
	bfs = BFS(grafo, vertice)
	vertices_por_nivel = []

	i = 1
	while bfs.cantidad_a_nivel(i): #distinto de 0
		vertices_por_nivel.append(bfs.vertices_a_nivel(i))
		i += 1 

	return vertices_por_nivel

def _DFS_identificar_subgrupo(vertice, visitado, subgrupos, id_subgrupo):
	"""Lleva a cabo la busqueda DFS a partir del vertice
	para identificar su componente conexa.
	Recibe el diccionario de visitado, subgrupos y el id 
	a imponer sobre la componente a calcular.
	Pre: Los diccionarios estan inicializados, vertice es
	de clase Vertice. id_subgrupo "deberia" ser diferente
	a algun id ya existente.
	"""
	visitado[vertice] = True
	subgrupos[id_subgrupo] = subgrupos.get(id_subgrupo, 0) + 1
	for ady in vertice.iter_de_adyacentes():
		if not visitado[ady]:
			_DFS_identificar_subgrupo(ady, visitado, subgrupos, id_subgrupo)


def subgrupos(grafo):
	"""Calcula la cantidad de componentes conexas en el grafo
	y la cantidad de vertices que contiene cada una.
	Devuelve una lista con la cantidad de vertices que 
	contiene cada componente conexa en orden decreciente.
	Pre: El grafo existe.
	"""
	visitado = {v:False for v in grafo}
	subgrupos = {}
	
	id_subgrupo = 0
	for v in grafo:
		if not visitado[v]:
			_DFS_identificar_subgrupo(v, visitado, subgrupos, id_subgrupo)
			id_subgrupo += 1

	return sorted(subgrupos.values(), reverse=True)

def _DFS_ptos_art(v, raiz, padre, visitado, orden, inferior, tiempo, art):
	"""Lleva a cabo la busqueda DFS para identificar los puntos
	de articulacion de la componente conexa en la que se trabaje.
	Recibe el vertice v, la raiz (que debe ser igual a v), los
	diccionarios de pdare, visitado, orden e inferior, el tiempo y 
	la lista art donde se guardaran los ptos de articulacion.
	Pre: Los diccionarios estan inicializados:
		visitado: con todos los vertices en Falso,
		orden: creado vacio,
		inferior: creado vacio,
		padre: con el vertice raiz en None.
	El vertice v es igual a la raiz. Son objetos Vertice.
	El tiempo es un numero incrementable.
	"""
	tiempo += 1
	visitado[v] = True
	orden[v] = tiempo
	inferior[v] = tiempo
	hijos_raiz = 0 #solo sirve para la llamada "base"

	for ady in v.iter_de_adyacentes():
		if not visitado[ady]:
			padre[ady] = v
			_DFS_ptos_art(ady, raiz, padre, visitado, orden, inferior, tiempo, art)
			if inferior[ady] < inferior[v]:
				inferior[v] = inferior[ady]

			if v == raiz:
				hijos_raiz += 1
			else:
				#el padre de ady es v!; y v no es la raiz!
				if inferior[ady] >= orden[v]:
					if v not in art:
						art.append(v)
		else:
			if padre[v] != ady:
				if orden[ady] < inferior[v]:
					inferior[v] = orden[ady]

	if hijos_raiz >= 2:
		assert v == raiz #si entra aca con otro estoy haciendo algo raro!
		if raiz not in art:
			art.append(raiz)

def articulacion(grafo):
	"""Calcula los puntos de articulacion del grafo.
	Devulve una lista de vertices (Obj) que son ptos. art.
	Pre: El grafo existe.
	"""
	visitado = {v:False for v in grafo}
	art = []
	tiempo = 0

	for v in grafo: #calcular art para cada comp. conexa
		if not visitado[v]:
			padre = {v:None}
			orden = {}
			inferior = {}
			_DFS_ptos_art(v, v, padre, visitado, orden, inferior, tiempo, art)

	return art