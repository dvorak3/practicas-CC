#include "../include/AutomataConPilaBuilder.h"

#include <iostream>

int main(int argc, char* argv[]) {
  AutomataConPilaBuilder lector;
  lector.construirDesdeArchivo(argv[1]);
}