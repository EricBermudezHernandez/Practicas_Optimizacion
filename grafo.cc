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
    //A[i].clear();
    if (dirigido_ == 1) {
      LP_[i].clear();
    }
  }
  LS_.clear();
  LP_.clear();
  //A.clear();
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
        LP_[dummy.j].emplace_back(aux);  // Actualizamos la lista de predecesores del nodo j, diciendo
                                        // que el predecesor es el nodo i
      } else {     // El grafo es no dirigido
        if (i - 1 != dummy.j) {  // Comprobamos que no es un bucle, por que si
                                 // no estaríamos insertando dos veces lo mismo
          aux.j = (i - 1);  // El nodo adyacente es el propio nodo i en el que estábamos
          aux.c = dummy.c;  // El coste es el mismo al tratarse del mismo arco
          LS_[dummy.j].emplace_back(aux);  // Actualizamos la lista de adyacencia del nodo sucesor de
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
    std::cout << "Grafo no dirigido  | nodos " << n_ << " | arcos " << m_
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
    for (unsigned j{0}; j < L[k].size(); ++j) {  // Recorremos todos los nodos u adyacentes al nodo k+1

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
    for(int j{0}; j < d.size(); ++j) {
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
        std::cout << i << " - " << (pred[j] + 1) << " - " << (j + 1) << std::endl;
      }
    }
  }
}
