#ifndef CONTADOR_LLAMADAS_H
#define CONTADOR_LLAMADAS_H

#include <limits>
#include <stdexcept>

#include "Tipos.h"

/**
 * @brief Cuenta las evaluaciones de funciones primitivas recursivas.
 *
 * El contador detecta el desbordamiento antes de incrementar su valor.
 */
class ContadorLlamadas {
 public:
  /// @brief Incrementa el contador o informa si ha alcanzado su máximo.
  void incrementar() {
    if (llamadas_ == std::numeric_limits<Natural>::max()) {
      throw std::overflow_error("El contador de llamadas ha desbordado.");
    }
    ++llamadas_;
  }

  /// @brief Devuelve el número de llamadas registradas.
  Natural llamadas() const { return llamadas_; }

 private:
  Natural llamadas_{0};  ///< Número acumulado de llamadas.
};

#endif
