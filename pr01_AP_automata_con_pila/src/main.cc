#include "../include/AutomataConPilaBuilder.h"

#include <iostream>

int main(int argc, char* argv[]) {
  AutomataConPilaBuilder lector;
  AutomataConPila AP = lector.construirDesdeArchivo(argv[1]);
  std::cout << "La cadena es: " << AP.leerCadena({'a', 'b'}) << std::endl;
}