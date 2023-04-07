/*
 *  Programa principal.
 *
 *
 *               Autores : Antonio Sedeno Noda, Sergio Alonso.
 *               Cursos  : 2012-2021
 */

#include <string.h>

#include "grafo.hpp"

void menu(unsigned dirigido, char &opcion) {
  // Expresion del menu de opciones segun sea un grafo dirigido o no dirigido
  std::cout << "==============================================================\n";
  std::cout << "Optimiza!cion en Grafos, 2022-2023 ERIC BERMÚDEZ HERNÁNDEZ"
            << std::endl;
  std::cout << "c. [c]argar grafo desde fichero" << std::endl;
  if (dirigido == 0)  // Grafo no dirigido
  {
    std::cout << "i. Mostrar [i]nformacion basica del grafo" << std::endl;
    std::cout << "a. Mostrar la lista de [a]dyacencia del grafo" << std::endl;
    std::cout << "m. Realizar un recorrido en a[m]plitud del grafo desde un nodo por sucesores" << std::endl;
    std::cout << "r. Realizar un recorrido en p[r]ofundidad del grafo desde un nodo por sucesores" << std::endl;
    // Aquí se añaden más opciones al menú del grafo no dirigido
  } else {
    std::cout << "i. Mostrar [i]nformacion basica del grafo" << std::endl;
    std::cout << "s. Mostrar la lista de [s]ucesores del grafo" << std::endl;
    std::cout << "p. Mostrar la lista de [p]redecesores del grafo" << std::endl;
    std::cout << "m. Realizar un recorrido en a[m]plitud del grafo desde un nodo por sucesores" << std::endl;
    std::cout << "r. Realizar un recorrido en p[r]ofundidad del grafo desde un nodo por sucesores" << std::endl;
    // Aquí se añaden más opciones al menú del grafo dirigido
  };
  std::cout << "q. Finalizar el programa" << std::endl;
  std::cout << "Introduce la letra de la accion a ejecutar  > ";
  std::cin >> opcion;
};

// El principal es simplemente un gestor de menu, diferenciando acciones para
// dirigido y para no dirigido, y un menú inicial si no hay un grafo cargado
int main(int argc, char *argv[]) {
  int error_apertura;
  char nombrefichero[85], opcion;

  // Si tenemos el nombre del primer fichero por argumento, es una excepcion, y
  // lo intentamos cargar, si no, lo pedimos desde teclado
  if (argc > 1) {
    std::cout << "Cargando datos desde el fichero dado como argumento"
              << std::endl;
    strcpy(nombrefichero, argv[1]);
  } else {
    std::cout << "Introduce el nombre completo del fichero de datos"
              << std::endl;
    std::cin >> nombrefichero;
  };
  GRAFO G(nombrefichero, error_apertura);
  if (error_apertura == 1) {
    std::cout
        << "Error en la apertura del fichero desde argumento: revisa nombre y "
           "formato"
        << std::endl;

  } else {
    std::cout << "Grafo cargado desde el fichero " << nombrefichero
              << std::endl;

    do {
      menu(G.Es_dirigido(), opcion);
      switch (opcion) {
        case 'c':

          std::cout << "Introduce el nombre completo del fichero de datos"
                    << std::endl;
          std::cin >> nombrefichero;
          G.actualizar(nombrefichero, error_apertura);
          if (error_apertura == 1) {
            std::cout
                << "Error en la apertura del fichero: revisa nombre y formato "
                   ": puedes volver a intentarlo"
                << std::endl;
          } else {
            std::cout << "Fichero cargado correctamente desde " << nombrefichero
                      << std::endl;
          };

          break;

        case 'i':
          std::cout << "Grafo cargado desde " << nombrefichero << std::endl;
          G.Info_Grafo();

          break;
        
        case 's': // Muestra lista de sucesores
          std::cout << "Nodos de la lista de sucesores" << std::endl;
          G.Mostrar_Listas(1);
          break;

        case 'e': // Muestra la lisa de predecesores
          std::cout << "Nodos de la lista de predecesores" << std::endl;
          G.Mostrar_Listas(-1);
          break;

        case 'a': // Muestra la lista de adyacencia
          std::cout << "Nodos de la lista de adyacentes" << std::endl;
          G.Mostrar_Listas(0);
          break;
        
        case 'm':
          G.RecorridoAmplitud();
          break;

        case 'r':
          G.RecorridoProfundidad();
          break;
          // Situar aquí el resto de opciones del menú
      }
    } while (opcion != 'q');
  }
  std::cout << "Fin del programa" << std::endl;
  G.~GRAFO();
  return (0);
}
