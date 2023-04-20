/*
 *  GRAFO.h - plantilla para los alumnos y alumnas de definicion de la clase
 * GRAFO
 *
 *  Autores : Antonio Sedeno Noda, Sergio Alonso
 *  Cursos  : 2012-2021
 */

#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

const unsigned UERROR = 65000;  // Para ciertos algoritmos es necesario
                                // codificar este valor de error
const int maxint = 1000000;  // Para ciertos algoritmos es necesario codificar
                             // este valor de m�ximo

// definicion de un elemento de la lista de adyacencia

typedef struct {
  unsigned j;  // nodo
  int c;       // atributo para expresar los costes, pesos, las longitudes, las
               // inversiones, etc...
} ElementoLista;

// definicion del tipo de una lista de adyacencia de un nodo
typedef std::vector<ElementoLista> LA_nodo;

class GRAFO {
 public:
  GRAFO(char nombrefichero[], int &errorapertura);
  void actualizar(char nombrefichero[], int &errorapertura);
  bool Es_dirigido();  // devuelve 0 si el grafo es no dirigido y 1 si es dirigido
  void Info_Grafo();           // devuelve información básica del grafo
  void Mostrar_Listas(int l);  // Muestra la lista de adyacencia
  void Mostrar_Matriz();  // Muestra la matriz de adyacencia: nodos y costes
  void RecorridoProfundidad();  // Construye un recorrido en profundidad desde
                                // un nodo inicial
  void RecorridoAmplitud();  // Construye un recorrido en amplitud desde un nodo
                             // inicial
  void Prim();
  ~GRAFO();                  // Destructor del objeto grafo

 private:
  unsigned dirigido_;  // 0 si el grafo es no dirigido y 1 si es dirigido
  unsigned n_;         // numero de NODOS
  unsigned m_;         // numero de ARCOS
  std::vector<LA_nodo>
      LS_;  // Lista de adyacencia de los sucesores: es una matriz dode cada fila
           // posiblemente es posiblemente de distinto tamaño
  std::vector<LA_nodo>
      LP_;  // Lista de adyacencia de los predecesores: es una matriz dode cada
           // fila posiblemente es de distinto tamaño
  std::vector<LA_nodo> A;  // Matriz de adyacencia, tanto adyacencia como costes
  void destroy();     // Destructor del LS, A, y LP, en su caso
  void build(char nombrefichero[85],
             int &errorapertura);  // Crea LS, A y LP en su caso, desde la
                                   // lectura del fichero
  void dfs_num(unsigned i, std::vector<LA_nodo> L, std::vector<bool> &visitado,
               std::vector<unsigned> &prenum, unsigned &prenum_ind,
               std::vector<unsigned> &postnum,
               unsigned &postnum_ind);  // Recorrido en profundidad recursivo
                                        // con recorridos enum y postnum
  void bfs_num(
      unsigned i, std::vector<LA_nodo> L, std::vector<unsigned> &pred,
      std::vector<unsigned> &d);  // Recorrido en amplitud con c�lculo de pred y d
};

#endif
