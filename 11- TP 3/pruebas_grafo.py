#!/usr/bin/env python
# -*- coding: utf-8 -*-
import time
from grafo import *

global resultados_pruebas
resultados_pruebas = { "OK" : 0, "ERROR" : 0 }

def correr_prueba(caso_prueba, descripcion):
        ''' Comprueba la igualdad pasada por parámetro como caso_prueba y 
        muestra la descripción y su resultado en pantalla. 
        Si no se cumple la igualdad se suma uno a la clave correspondiente en el 
        diccionario resultados_pruebas.
        caso_prueba es una igualdad a la que se le va a aplicar assert.
        descripcion es un texto descriptivo con el que se va a imprimir el 
        resultado de la operación.
        resultados_pruebas es un diccionario con las claves "OK" y "ERROR", que
        identifican valores numéricos con la cantidad de pruebas pasadas y 
        falladas, respectivamente.
        '''
        try:
                assert caso_prueba
                print "%s: OK" % descripcion
                resultados_pruebas["OK"] += 1
        except AssertionError:
                print "%s: ERROR" % descripcion
                resultados_pruebas["ERROR"] += 1


def prueba_creacion_grafo_no_dirigido():
        g = Grafo(dirigido = False)
        correr_prueba(g.es_dirigido == False, "Creacion de grafo no dirigido")
        correr_prueba(g.cantidad_aristas() == 0, "Cantidad de aristas es 0")
        correr_prueba(g.cantidad_vertices() == 0, "Cantidad de vertices es 0")

def prueba_agregar_vertices_grafo_no_dirigido():
        g = Grafo(dirigido = False)
        for x in xrange(6):
                g.agregar_vertice(x, str(x))
        
        correr_prueba(g.cantidad_vertices() == 6, "Cantidad vertices igual a 6")
        correr_prueba(g.cantidad_aristas() == 0, "Cantidad aristas igual a 0")
        for x in xrange(6):
                ok = x in g
                V = g.obtener_vertice(x)
                ok = isinstance(V,Vertice)
                ok = V in g
                ok = V.obtener_id() == x
                ok = V.obtener_dato() == str(x)
        correr_prueba(ok, "Los vertices agregados pertenecen al grafo")
        try: g.agregar_vertice(2)
        except VerticeRepetidoError: pass
        correr_prueba(True , "No se puede agregar vertice repetido")

def prueba_eliminar_vertices_grafo_no_dirigido():
        #Sin aristas
        g = Grafo(dirigido = False)
        for x in xrange(6):
                g.agregar_vertice(x, str(x))

        correr_prueba(g.cantidad_vertices() == 6, "Cantidad de vertices igual a 6")
        g.eliminar_vertice(5)
        correr_prueba(g.cantidad_vertices() == 5, "Cantidad de vertices igual a 5")
        for i in xrange(5):
                g.eliminar_vertice(i)
        correr_prueba(g.cantidad_vertices() == 0, "Luego de borrar todo, cantidad de vertices igual a 0")
        try: g.eliminar_vertice(7)
        except VerticeInexistenteError: pass
        correr_prueba(True, "No se puede eliminar vertice inexistente")

        
def prueba_agregar_aristas_grafo_no_dirigido():
        g = Grafo(dirigido = False)
        for x in xrange(6):
                g.agregar_vertice(x, str(x))

        correr_prueba(g.cantidad_aristas() == 0, "Cantidad aristas igual a 0")
        g.agregar_arista(0, 1, 10)
        g.agregar_arista(1, 2, 20)
        g.agregar_arista(2, 3, 30)
        correr_prueba(g.cantidad_aristas() == 3, "Cantidad aristas igual a 3")
        correr_prueba(g.obtener_dato_arista(1, 2) == 20, "Peso de arista 1-2 igual a 20")
        correr_prueba(g.obtener_dato_arista(2, 1) == 20, "Peso de arista 2-1 igual a 20")
        correr_prueba(g.obtener_dato_arista(2, 3) == 30, "Peso de arista 2-3 igual a 30")
        correr_prueba(g.obtener_dato_arista(3, 2) == 30, "Peso de arista 3-2 igual a 30")
       
        v2 = g.obtener_vertice(2)
        correr_prueba(v2.cantidad_adyacentes() == 2, "Vertices adyacentes a 2 igual a 2")
        v = [1, 3]
        for x in v2.obtener_adyacentes():
                ok = x.obtener_id() in v
                
        correr_prueba(ok, "Vertices adyacentes a 2 son 3 y 1")
        try: g.agregar_arista(2,6)
        except VerticeInexistenteError: pass
        correr_prueba(True, "No se puede agregar una arista a un vertice no existente")

        try: g.agregar_arista(1,0)
        except AristaRepetidaError: pass
        correr_prueba(True, "No se puede agregar una arista repetida")

        try: g.obtener_dato_arista(1,5)
        except AristaInexistenteError: pass
        correr_prueba(True, "No se puede conseguir dato de arista inexistente")


def prueba_eliminar_aristas_grafo_no_dirigido():
        g = Grafo(dirigido = False)
        for x in xrange(6):
                g.agregar_vertice(x, str(x))
        g.agregar_arista(0, 1, 10)
        g.agregar_arista(1, 2, 20)
        g.agregar_arista(2, 3, 30)

        v2 = g.obtener_vertice(2)
        correr_prueba(v2.cantidad_adyacentes() == 2, "Antes de borrar, 2 tiene 2 adyacentes")
        correr_prueba(g.cantidad_aristas() == 3, "Cantidad de aristas igual a 3")
        g.eliminar_arista(0, 1)
        correr_prueba(g.cantidad_aristas() == 2, "Cantidad de aristas igual a 2")
        g.eliminar_arista(1, 2)
        correr_prueba(v2.esta_conectado_a(1) == False, "Despues de eliminar 1-2, 2 no esta conectado a 1")
        g.eliminar_arista(2, 3)
        correr_prueba(v2.cantidad_adyacentes() == 0, "Despues de borrar, 2 tiene 0 adyacentes")
        correr_prueba(g.cantidad_aristas() == 0, "Cantidad de aristas igual a 0")
        try: g.eliminar_arista(9,1)
        except VerticeInexistenteError: pass
        correr_prueba(True, "No se puede eliminar arista de un vertice no existente")

        try: g.eliminar_arista(4,4)
        except AristaInexistenteError: pass
        correr_prueba(True, "No se puede eliminar arista inexistente")


def prueba_creacion_grafo_dirigido():
        g = Grafo(dirigido = True)
        correr_prueba(g.es_dirigido == True, "\nCreacion de grafo dirigido")
        correr_prueba(g.cantidad_aristas() == 0, "Cantidad de aristas es 0")
        correr_prueba(g.cantidad_vertices() == 0, "Cantidad de vertices es 0")

def prueba_aristas_grafo_dirigido():
        g = Grafo(dirigido = True)
        for x in xrange(6):
                g.agregar_vertice(x,str(x))
        
        g.agregar_arista(1, 2, 10)
        g.agregar_arista(2, 1, 20)
        g.agregar_arista(1, 0, "ida")
        g.agregar_arista(0, 1, "vuelta")
        
        correr_prueba(g.obtener_dato_arista(1, 0) != g.obtener_dato_arista(0, 1), "Arista 0-1 distinta a 1-0")
        correr_prueba(g.cantidad_vertices() == 6 and g.cantidad_aristas() == 4, "6 vertices y 4 aristas")
        
        g.eliminar_vertice(0)
        correr_prueba(g.cantidad_aristas() == 2, "Elimino vertice 0, quedan 2 aristas")
   
def prueba_volumen_grafo_no_dirigido():
        print "\nPruebas de volumen grafo no dirigido:"
        g = Grafo(dirigido = False)
        start = time.time()
        n = 100000
        for x in xrange(n):
                g.agregar_vertice(x,str(x))
        
        correr_prueba(g.cantidad_vertices() == n, "Cantidad de vertices igual a 100.000")
        for x in xrange(n):
                assert x in g
                V = g.obtener_vertice(x)
                assert isinstance(V,Vertice)
                assert V in g
                assert V.obtener_id() == x
                assert V.obtener_dato() == str(x)

        for x in xrange(n-1):
                g.agregar_arista(x,x+1, str(x))

        assert g.cantidad_vertices() == n
        correr_prueba(g.cantidad_aristas() == n-1, "Agregar aristas igual a 99.999")

        for x in xrange(n-1):
                v_A = g.obtener_vertice(x)
                v_B = g.obtener_vertice(x+1)
                assert v_A.esta_conectado_a(v_B)
                assert v_B.esta_conectado_a(v_A)
                assert g.obtener_dato_arista(x,x+1) == str(x)
                g.eliminar_arista(x+1,x)
                g.eliminar_vertice(x)

        assert n-1 in g
        assert g.cantidad_vertices() == 1
        assert g.cantidad_aristas() == 0

        g.eliminar_vertice(n-1)

        correr_prueba(g.cantidad_vertices() == 0 and g.cantidad_aristas() == 0, "Despues de eliminar, 0 vertices y 0 aristas")
        print "Tarda %i segundos" %int(time.time() - start)
        
def prueba_volumen_grafo_dirigido():
        print "\nPruebas de volumen grafo dirigido:"
        g = Grafo(dirigido = True)
        start = time.time()
        n = 10000
        for x in xrange(n):
                g.agregar_vertice(x,str(x))
        
        assert g.cantidad_vertices() == n
        assert g.cantidad_aristas() == 0
        for x in xrange(n):
                assert x in g
                V = g.obtener_vertice(x)
                assert isinstance(V,Vertice)
                assert V in g
                assert V.obtener_id() == x
                assert V.obtener_dato() == str(x)

        for x in xrange(n-1):
                g.agregar_arista(x,x+1, str(x))

        correr_prueba(g.cantidad_vertices() == n, "Cantidad de vertices igual a 10.000")
        correr_prueba(g.cantidad_aristas() == n-1, "Cantidad de aristas igual a 9.999")

        for x in xrange(n-1):
                v_A = g.obtener_vertice(x)
                v_B = g.obtener_vertice(x+1)
                assert v_A.esta_conectado_a(v_B)
                assert not v_B.esta_conectado_a(v_A)
                assert g.obtener_dato_arista(x, x+1) == str(x)

                try: g.eliminar_arista(x+1, x)
                except AristaInexistenteError: pass

                g.eliminar_arista(x, x+1)
                g.eliminar_vertice(x)

        assert n-1 in g
        g.eliminar_vertice(n-1)
        
        correr_prueba(g.cantidad_vertices() == 0 and g.cantidad_aristas() == 0, "Despues de eliminar, 0 vertices y 0 aristas")

        print "Tarda %i segundos" %int(time.time() - start)
        

if __name__ == '__main__':
        prueba_creacion_grafo_no_dirigido()
        prueba_agregar_vertices_grafo_no_dirigido()
        prueba_eliminar_vertices_grafo_no_dirigido()
        prueba_agregar_aristas_grafo_no_dirigido()
        prueba_eliminar_aristas_grafo_no_dirigido()

        prueba_creacion_grafo_dirigido()
        prueba_aristas_grafo_dirigido()
        
        prueba_volumen_grafo_no_dirigido()
        prueba_volumen_grafo_dirigido()
        
        print "\nPruebas corridas: %d OK, %d errores." % \
            (resultados_pruebas["OK"], resultados_pruebas["ERROR"])
