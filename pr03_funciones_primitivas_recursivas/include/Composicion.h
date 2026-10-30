#ifndef COMPOSICION_H
#define COMPOSICION_H

#include <vector>

#include "FuncionPrimitivaRecursiva.h"

/**
 * @brief Construye una función mediante composición de funciones.
 *
 * Evalúa cada función interior sobre los argumentos recibidos y aplica la
 * función exterior a los resultados obtenidos.
 */
class Composicion final : public FuncionPrimitivaRecursiva {
 public:
  /// @brief Crea una composición con referencias no propietarias a sus funciones.
  Composicion(const FuncionPrimitivaRecursiva* exterior,
              std::vector<const FuncionPrimitivaRecursiva*> interiores,
              ContadorLlamadas& contador);

 protected:
  /// @brief Evalúa las funciones interiores y luego la función exterior.
  Natural calcular(const std::vector<Natural>& argumentos) const override;

 private:
  const FuncionPrimitivaRecursiva* exterior_;  ///< Función aplicada al final.
  std::vector<const FuncionPrimitivaRecursiva*> interiores_;  ///< Funciones previas.
};

#endif
