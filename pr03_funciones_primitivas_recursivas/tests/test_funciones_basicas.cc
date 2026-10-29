#include <iostream>
#include <limits>
#include <stdexcept>

#include "FactoryFunciones.h"

int main() {
  ContadorLlamadas contador;
  const FactoryFunciones factory(contador);
  const auto sucesor = factory.crearSucesor();
  const Natural maximo = std::numeric_limits<Natural>::max();

  if ((*sucesor)({maximo - 1}) != maximo) {
    std::cerr << "El sucesor debe aceptar el mayor natural representable.\n";
    return 1;
  }
  try {
    (*sucesor)({maximo});
  } catch (const std::overflow_error&) {
    return contador.llamadas() == 2 ? 0 : 1;
  }

  std::cerr << "El sucesor no detecto el desbordamiento.\n";
  return 1;
}
