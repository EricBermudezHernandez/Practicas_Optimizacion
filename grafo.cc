/*
 *  GRAFO.CPP - Plantilla para la implementación de la clase GRAFOS
 *
 *
 *  Autores : Antonio Sedeno Noda, Sergio Alonso
 *  Cursos   : 2012-2023
 */

#include "grafo.hpp"

void GRAFO ::destroy() {
  for (unsigned i{0}; i < n_; ++i) {
    LS_[i].clear();
    // A[i].clear();
    if (dirigido_ == 1) {
      LP_[i].clear();
    }
  }
  LS_.clear();
  LP_.clear();
  // A.clear();
}

void GRAFO ::build(char nombrefichero[85], int &errorapertura) {
  ElementoLista dummy, aux;
  std::ifstream textfile;
  textfile.open(nombrefichero, std::ios::in);
  if (textfile.fail()) {
    errorapertura = 1;
  } else {
    errorapertura = 0;
  }
  if (textfile.is_open()) {
    unsigned i, j;
    // leemos por conversion implicita el numero de nodos, arcos y el atributo
    // dirigido
    textfile >> (unsigned &)n_ >> (unsigned &)m_ >> (unsigned &)dirigido_;
    // los nodos internamente se numeran desde 0 a n-1
    // creamos las n listas de sucesores
    LS_.resize(n_);
    // leemos los m arcos
    for (int k{0}; k < m_; ++k) {
      textfile >> (unsigned &)i >> (unsigned &)j >> (int &)dummy.c;
      // damos los valores a dummy.j y dummy.c
      dummy.j = (j - 1);
      // situamos en la posición del nodo i a dummy mediante push_back
      LS_[i - 1].emplace_back(dummy);
      // pendiente de hacer un segundo push_back si es no dirigido. O no.
      if (Es_dirigido()) {  // El grafo es dirigido
        aux.j = (i - 1);  // El predecesor es el propio nodo en el que estábamos
        aux.c = dummy.c;  // El coste es el mismo al tratarse del mismo arco
        LP_.resize(n_);
        LP_[dummy.j].emplace_back(
            aux);  // Actualizamos la lista de predecesores del nodo j, diciendo
                   // que el predecesor es el nodo i
      } else {     // El grafo es no dirigido
        if (i - 1 != dummy.j) {  // Comprobamos que no es un bucle, por que si
                                 // no estaríamos insertando dos veces lo mismo
          aux.j =
              (i -
               1);  // El nodo adyacente es el propio nodo i en el que estábamos
          aux.c = dummy.c;  // El coste es el mismo al tratarse del mismo arco
          LS_[dummy.j].emplace_back(
              aux);  // Actualizamos la lista de adyacencia del nodo sucesor de
                     // j(nodo sucesor de i)
        }
      }
    }
  }
}

GRAFO::~GRAFO() { destroy(); }

GRAFO::GRAFO(char nombrefichero[85], int &errorapertura) {
  build(nombrefichero, errorapertura);
}

void GRAFO::actualizar(char nombrefichero[85], int &errorapertura) {
  // Limpiamos la memoria dinamica asumida en la carga previa, como el
  // destructor
  destroy();
  // Leemos del fichero y actualizamos G con nuevas LS y, en su caso, LP
  build(nombrefichero, errorapertura);
}

bool GRAFO::Es_dirigido() {
  if (dirigido_ == 1) {  // El grafo es dirigido
    return true;
  }
  return false;  // El grafo es no dirigido
}

void GRAFO::Info_Grafo() {
  if (Es_dirigido()) {
    std::cout << "Grafo dirigido  | nodos " << n_ << " | arcos " << m_
              << std::endl;
  } else {
    std::cout << "Grafo no dirigido  | nodos " << n_ << " | aristas " << m_
              << std::endl;
  }
}

void Mostrar_Lista(std::vector<LA_nodo> L) {
  for (int i{0}; i < L.size(); ++i) {
    std::cout << "[" << (i + 1) << "] : ";
    if (L[i].empty()) std::cout << "null";
    for (int j{0}; j < L[i].size(); ++j) {
      std::cout << ((L[i][j].j) + 1) << " : (" << L[i][j].c << ") ";
    }
    std::cout << std::endl;
  }
}

void GRAFO ::Mostrar_Listas(int l) {
  if (l == 0 || l == 1) {
    Mostrar_Lista(LS_);
  } else {
    Mostrar_Lista(LP_);
  }
}

void GRAFO::Mostrar_Matriz() {}  // Muestra la matriz de adyacencia, tanto los
                                 // nodos adyacentes como sus costes

void GRAFO::dfs_num(
    unsigned i, std::vector<LA_nodo> L, std::vector<bool> &visitado,
    std::vector<unsigned> &prenum, unsigned &prenum_ind,
    std::vector<unsigned> &postnum,
    unsigned &postnum_ind) {  // Recorrido en profundidad recursivo con
                              // recorridos enum y postnum

  visitado[i] = true;
  prenum[prenum_ind++] =
      i;  // asignamos el orden de visita prenum que corresponde el nodo i
  for (unsigned j{0}; j < L[i].size(); ++j)
    if (!visitado[L[i][j].j]) {
      dfs_num(L[i][j].j, L, visitado, prenum, prenum_ind, postnum, postnum_ind);
    }
  postnum[postnum_ind++] =
      i;  // asignamos el orden de visita posnum que corresponde al nodo i
}

void GRAFO::RecorridoProfundidad() {
  // creación e inicialización de variables y vectores
  unsigned i, prenum_ind{0}, postnum_ind{0};
  std::vector<bool> visitado(n_, false);
  std::vector<unsigned> prenum(n_), postnum(n_);
  // solicitud al usuario del nodo inicial del recorrido en profundidad
  std::cout << "Elije nodo partida [1-" << n_ << "]: ";
  std::cin >> i;
  dfs_num((i - 1), LS_, visitado, prenum, prenum_ind, postnum, postnum_ind);
  // mostrar en pantalla el preorden
  std::cout << "Orden de visita de los nodos en preorden" << std::endl;
  for (int i{0}; i < prenum_ind; ++i) {
    std::cout << "[" << (prenum[i] + 1) << "]";
    if (i != (prenum_ind - 1)) std::cout << " -> ";
  }
  std::cout << std::endl;
  // mostrar en pantalla el postorden
  std::cout << "Orden de visita de los nodos en postorden" << std::endl;
  for (int j{0}; j < postnum_ind; ++j) {
    std::cout << "[" << (postnum[j] + 1) << "]";
    if (j != (postnum_ind - 1)) std::cout << " -> ";
  }
  std::cout << std::endl;
}

void GRAFO::bfs_num(
    unsigned i,  // nodo desde el que realizamos el recorrido en amplitud
    std::vector<LA_nodo> L,  // lista que recorremos, LS o LP; por defecto LS
    std::vector<unsigned> &pred,  // std::vector de predecesores en el recorrido
    std::vector<unsigned> &d) {   // std::vector de distancias a nodo i+1
                                 // Recorrido en amplitud con la construcción de
                                 // pred y d: usamos la cola
  // Limpiamos la consola
  std::vector<bool> visitado;  // creamos e iniciamos el std::vector visitado
  visitado.resize(n_, false);
  visitado[i] = true;

  pred.resize(n_, 0);  // creamos e inicializamos pred y d
  d.resize(n_, 0);
  pred[i] = i;
  d[i] = 0;

  std::queue<unsigned> cola;  // creamos e inicializamos la cola
  cola.push(i);               // iniciamos el recorrido desde el nodo i+1

  while (!cola.empty()) {  // al menos entra una vez al visitar el nodo i+1 y
                           // continúa hasta que la cola se vacíe
    unsigned k = cola.front();  // cogemos el nodo k+1 de la cola
    cola.pop();                 // lo sacamos de la cola
    // Hacemos el recorrido sobre L desde el nodo k+1
    for (unsigned j{0}; j < L[k].size();
         ++j) {  // Recorremos todos los nodos u adyacentes al nodo k+1

      // Si el nodo u no está visitado
      if (visitado[L[k][j].j] == false) {
        // Lo visitamos
        visitado[L[k][j].j] = true;  // L[k][j].j es el nodo sucesor de L[k]
        // Lo metemos en la cola
        cola.push(L[k][j].j);
        // le asignamos el predecesor
        pred[L[k][j].j] = k;
        // le calculamos su etiqueta distancia
        d[L[k][j].j] =
            (d[k] + 1);  // La distancia que hay al sucesor es la distancia del
                         // predecesor + 1, al ser el sucesor
      }
    }
    // Hemos terminado pues la cola está vacía
  }
}

// Construye un recorrido en amplitud desde un
// nodo inicial
void GRAFO::RecorridoAmplitud() {
  // creación e inicialización de variables y vectores
  unsigned i;  // nodo desde el que realizamos el recorrido en amplitud
  std::vector<unsigned> pred, d;
  // solicitud al usuario del nodo inicial del recorrido en amplitud
  std::cout << "Elije nodo de partida [1- " << n_ << "]: ";
  std::cin >> i;
  std::cout << "Nodo inicial: " << i << std::endl;
  bfs_num((i - 1), LS_, pred, d);
  // mostrar en pantalla la etiqueta distancia
  int distancia_maxima{0};

  for (int z{0}; z < d.size(); ++z) {
    if (distancia_maxima < d[z]) distancia_maxima = d[z];
  }
  std::cout << "\nNodos segun distancia al nodo inicial en numero de aristas\n";
  std::cout << "Distancia 0 aristas : " << i << std::endl;
  for (int i{1}; i <= distancia_maxima; ++i) {
    std::cout << "Distancia " << i << " aristas";
    for (int j{0}; j < d.size(); ++j) {
      if (i == d[j]) {
        std::cout << " : " << (j + 1);
      }
    }
    std::cout << std::endl;
  }

  // mostrar en pantalla los predecesores
  std::cout << "\nRamas de conexión en el recorrido" << std::endl;
  for (int j{0}; j < pred.size(); ++j) {
    if (d[j] != 0) {
      if (d[j] == 1) {
        std::cout << (pred[j] + 1) << " - " << (j + 1) << std::endl;
      } else {
        std::cout << i << " - " << (pred[j] + 1) << " - " << (j + 1)
                  << std::endl;
      }
    }
  }
}

void GRAFO::Prim() {
  // Es posible que en nuestro grafo hayan nodos islas, es decir sin ningun nodo
  // adyacente. En este caso recorremos el vector entero de nodos y vemos sus
  // adyacentes, si vemos que hay un nodo con ningún sucesor es que no tiene
  // adyacentes y es una isla
  int contador{0}, componentes_conexas{1};
  bool conexo{true};
  std::vector<unsigned> islas;
  for (int i{0}; i < LS_.size(); ++i) {
    if (LS_[i].size() != 0) {
      contador++;
    } else {
      islas.emplace_back(i);
      conexo = false;
      componentes_conexas++;
    }
  }
  contador--;  // La solución es n - 1 aristas, decrementamos ya el número de
               // aristas
  // T = ∅
  unsigned T{0}, u{0};  // Aristas que forman parte de la solución y nodo u
  std::vector<unsigned> pred(
      contador + 1);  // Inicializamos el vector con el tamaño de aristas
  std::vector<bool> M(
      contador + 1,
      false);  // Inicializamos el vector con el tamaño de aristas y todo falso
               // ya que todavía no se han visitado los nodos
  // Para todo nodo i de V hacer coste[i] = ∞
  std::vector<int> coste(
      contador + 1, maxint);  // Almacena el coste que incorporamos al arbol si
                              // introducimos un nodo en la solución
  // M = {1}
  M[0] = true;
  // coste[1] = 0
  coste[0] = 0;
  // pred[1] = 1
  pred[0] = 1;
  // Mientras en T no haya n-1 aristas hacer
  std::cout << "Iniciamos desde el nodo 1:" << std::endl;
  while (T != contador) {
    // sea u el último nodo que entró en M, u va a mejorar costes
    // para todo z adyacente a u en V-M hacer
    for (int z{0}; z < LS_[u].size(); ++z) {
      // si coste[z] > w(u, z) entonces
      if ((!M[LS_[u][z].j]) && coste[LS_[u][z].j] > LS_[u][z].c) {
        // coste[z] = w(u,z)
        coste[LS_[u][z].j] = LS_[u][z].c;  // Esta arista es menos costosa
        // pred[z] = u
        pred[LS_[u][z].j] = u;  // cambio el nodo de conexión
      }
    }
    // sea u = nodo con menor coste en V-M
    int coste_menor{maxint};
    for (int z{1}; z < coste.size(); ++z) {
      if (!M[z] && coste_menor > coste[z]) {
        coste_menor = coste[z];
        u = z;
      }
    }
    // M = M U {u}
    M[u] = true;
    // T = T U {(u, pred[u])]
    std::cout << "(" << pred[u] + 1 << ", " << u + 1 << ")"
              << "[" << coste[u] << "];" << std::endl;
    T++;  // Insertamos arista
  }
  // Imprimir la solución:
  int peso_mst{0};
  for (int i{0}; i < coste.size(); ++i) {
    if (coste[i] != maxint) {
      peso_mst += coste[i];
    }
  }
  std::cout << "El peso del MST encontrado es: " << peso_mst << std::endl
            << std::endl;

  // En el caso de que no sea conexo:
  if (!conexo) {
    for (int i{0}; i < islas.size(); ++i) {
      std::cout << "Reiniciamos desde el nodo " << (islas[i] + 1) << ":"
                << std::endl;
      std::cout << "Esta componente conexa tiene un único nodo, y por tanto, "
                   "no se añade arista alguna"
                << std::endl
                << std::endl;
    }
    std::cout << "El grafo no es conexo\nEl número de componentes conexas "
                 "encontradas es: "
              << componentes_conexas << std::endl
              << "El peso total de los MST encontrados es " << peso_mst
              << std::endl;
  }
}

void GRAFO::MostrarCamino(unsigned s, unsigned i, std::vector<unsigned> pred) {
  if (i != s) {
    MostrarCamino(s, pred[i], pred);
    std::cout << pred[i] + 1 << " - ";
  }
}

void GRAFO::Dijkstra_(double &comparaciones, unsigned s) {
  // Inicialmente no hay ningun nodo permanentemente etiquetado
  std::vector<bool> PermanentementeEtiquetado(n_, false);
  // Inicialmente todas las etiquetas distancias son infinito
  std::vector<int> d(n_, maxint);
  // Inicialmente el pred es null
  std::vector<unsigned> pred(n_, UERROR);
  int min{maxint};
  unsigned candidato;
  // La etiqueta distancia del nodo origen es 0, y es su propio pred
  d[--s] = 0;
  pred[s] = s;
  comparaciones = 0;
  do {
    /*
    -Buscamos un nodo candidato a ser permanentemente etiquetado
        : aquel de entre los no permanentemente etiquetados,
        es decir,
        en el almacén con menor etiqueta distancia no infinita.-
            Si existe ese candidato,
    */
    min = maxint;
    for (int i{0}; i < n_; ++i) {
      if (PermanentementeEtiquetado[i] == false) {
        if (min > d[i]) {
          min = d[i];
          candidato = i;
        }
      }
    }
    /*
         lo etiquetamos permanentemente y usamos los arcos de la lista de
             sucesores para buscar atajos.Por cada comparación realizada para
                 buscar atajos,
         incrementamos el contador de comparaciones.-
             Esto lo hacemos mientras haya candidatos
     */
    if (min != maxint) {  // mientras hayan candidatos
      PermanentementeEtiquetado[candidato] = true;
      for (int i{0}; i < LS_[candidato].size(); ++i) {
        if (d[candidato] + LS_[candidato][i].c < d[LS_[candidato][i].j]) {
          d[LS_[candidato][i].j] = d[candidato] + LS_[candidato][i].c;
          pred[LS_[candidato][i].j] = candidato;
        }
        comparaciones++;
      }
    }
  } while (min != maxint);

  std::cout << "Soluciones:" << std::endl;
  /*
  En esta parte del código,
      mostramos los caminos mínimos para cada nodo si los hay.
  */
  for (int i{1}; i < n_; ++i) {
    if (d[i] != maxint) {
      std::cout << "El camino desde " << (s + 1) << " al nodo " << (i + 1)
                << " es : ";
      MostrarCamino(s, i, pred);
      std::cout << (i + 1) << " de longitud " << d[i] << std::endl;
    } else {
      std::cout << "No hay camino mínimo desde " << (s + 1) << " al nodo "
                << (i + 1) << std::endl;
    }
  }
}

void GRAFO::BellmanFordEnd_(double &comparaciones, unsigned s) {
  std::vector<int> d(n_, maxint);
  std::vector<unsigned> pred(n_, UERROR);
  unsigned numeromejoras = 0;
  bool mejora;
  d[--s] = 0;
  pred[s] = s;
  comparaciones = 0;
  do {
    mejora = false;
    /*
    // recorremos todos los arcos, y para cada (i, j), buscamos si d[j] > d[i]
    +cij, y actualizamos d y pred,
        incrementando el contador comparaciones cuando comparamos,
        independientemente de si mejoramos o no.
        // si al menos en una ocasion ha mejorado una etiqueta distancia, no
        // hemos terminado;
    contabilizamos los bucles en los que ha habido mejoras
    */
    for (int i{0}; i < n_; ++i) {
      for (int j{0}; j < LS_[i].size(); ++j) {
        if (d[LS_[i][j].j] >
            (d[i] + LS_[i][j].c)) {  // Si el coste de su sucesor es mayor que
                                     // el de su predecesor + el camino
          d[LS_[i][j].j] = (d[i] + LS_[i][j].c);
          pred[LS_[i][j].j] = i;
          mejora = true;
        }
        comparaciones++;
      }
    }
    if (mejora) numeromejoras++;
  } while ((numeromejoras < n_) && (mejora == true));
  /*
  // para salir del bucle, si mejora es true, pues hay un ciclo, pues hemos
  realizado n + 1 veces la relajacion con mejora;
  si mejora es false,
      pues tenemos solucion
          // Mostramos los caminos mínimos que se puedan haber encontrado, o
          advertimos de la existencia de un ciclo de coste negativo.
  */
  if (numeromejoras >= n_) {
    std::cout << "Hay un ciclo de coste negativo" << std::endl;
  }
  std::cout << "Soluciones:" << std::endl;
  for (int i{1}; i < n_; ++i) {
    if (d[i] != maxint) {
      std::cout << "El camino desde " << (s + 1) << " al nodo " << (i + 1)
                << " es : ";
      MostrarCamino(s, i, pred);
      std::cout << (i + 1) << " de longitud " << d[i] << std::endl;
    } else {
      std::cout << "No hay camino mínimo desde " << (s + 1) << " al nodo "
                << (i + 1) << std::endl;
    }
  }
}

void GRAFO::ComparativaCM() {
  unsigned nodo_partida;
  double comparaciones_dijsktra{0.0}, comparaciones_bfe{0.0};
  std::cout << "Camino mínimo de un nodo al resto:" << std::endl
            << "Nodo de partida? [1-6]: ";
  std::cin >> nodo_partida;
  std::cout << "Resultados del algoritmo de Dijkstra." << std::endl;
  Dijkstra_(comparaciones_dijsktra, nodo_partida);
  std::cout << "Dijsktra ha realizado un total de : " << comparaciones_dijsktra
            << " comparaciones" << std::endl
            << std::endl;
  std::cout << "Resultados del algoritmo de Bellman Ford End." << std::endl;
  BellmanFordEnd_(comparaciones_bfe, nodo_partida);
  std::cout << "Ha realizado un total de : " << comparaciones_bfe
            << " comparaciones" << std::endl
            << std::endl;
  std::cout
      << "El algoritmo general basado en la condición de optimalidad, Bellman, "
         "Ford, End, ha realizado "
      << (comparaciones_bfe - comparaciones_dijsktra)
      << " comparaciones más para mejorar la etiqueta distancia que Dijkstra"
      << std::endl
      << std::endl;
}

void GRAFO::Dijkstra(unsigned s) {
  // Inicialmente no hay ningun nodo permanentemente etiquetado
  std::vector<bool> PermanentementeEtiquetado(n_, false);
  // Inicialmente todas las etiquetas distancias son infinito
  std::vector<int> d(n_, maxint);
  // Inicialmente el pred es null
  std::vector<unsigned> pred(n_, UERROR);
  int min{maxint};
  unsigned candidato;
  // La etiqueta distancia del nodo origen es 0, y es su propio pred
  d[--s] = 0;
  pred[s] = s;
  do {
    /*
    -Buscamos un nodo candidato a ser permanentemente etiquetado
        : aquel de entre los no permanentemente etiquetados,
        es decir,
        en el almacén con menor etiqueta distancia no infinita.-
            Si existe ese candidato,
    */
    min = maxint;
    for (int i{0}; i < n_; ++i) {
      if (PermanentementeEtiquetado[i] == false) {
        if (min > d[i]) {
          min = d[i];
          candidato = i;
        }
      }
    }
    /*
         lo etiquetamos permanentemente y usamos los arcos de la lista de
             sucesores para buscar atajos.Por cada comparación realizada para
                 buscar atajos,
         incrementamos el contador de comparaciones.-
             Esto lo hacemos mientras haya candidatos
     */
    if (min != maxint) {  // mientras hayan candidatos
      PermanentementeEtiquetado[candidato] = true;
      for (int i{0}; i < LS_[candidato].size(); ++i) {
        if (d[candidato] + LS_[candidato][i].c < d[LS_[candidato][i].j]) {
          d[LS_[candidato][i].j] = d[candidato] + LS_[candidato][i].c;
          pred[LS_[candidato][i].j] = candidato;
        }
      }
    }
  } while (min != maxint);

  std::cout << "Soluciones:" << std::endl;
  /*
  En esta parte del código,
      mostramos los caminos mínimos para cada nodo si los hay.
  */
  for (int i{1}; i < n_; ++i) {
    if (d[i] != maxint) {
      std::cout << "El camino desde " << (s + 1) << " al nodo " << (i + 1)
                << " es : ";
      MostrarCamino(s, i, pred);
      std::cout << (i + 1) << " de longitud " << d[i] << std::endl;
    } else {
      std::cout << "No hay camino mínimo desde " << (s + 1) << " al nodo "
                << (i + 1) << std::endl;
    }
  }
}
