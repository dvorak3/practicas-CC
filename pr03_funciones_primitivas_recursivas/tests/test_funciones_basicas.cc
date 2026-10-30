#include <iostream>
#include <limits>
#include <stdexcept>

#include "FactoryFunciones.h"

/**
 * @brief Comprueba los límites de la función sucesor.
 *
 * Verifica que el mayor Natural representable se obtiene desde su predecesor y
 * que incrementar ese máximo informa del desbordamiento.
 *
 * @return Cero si ambos límites se comportan correctamente; uno en caso contrario.
 */
int main() {
  ContadorLlamadas contador;
  FactoryFunciones factory(contador);
  const FuncionPrimitivaRecursiva* sucesor = factory.crearSucesor();
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
