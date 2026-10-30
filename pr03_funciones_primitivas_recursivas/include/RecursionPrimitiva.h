#ifndef RECURSION_PRIMITIVA_H
#define RECURSION_PRIMITIVA_H

#include <vector>

#include "FuncionPrimitivaRecursiva.h"

/**
 * @brief Construye una función mediante el esquema de recursión primitiva.
 *
 * El primer argumento es el índice de recursión; los restantes se mantienen
 * como parámetros durante todas las iteraciones.
 */
class RecursionPrimitiva final : public FuncionPrimitivaRecursiva {
 public:
  /// @brief Crea la recursión a partir del caso base y el paso recursivo.
  RecursionPrimitiva(
      const FuncionPrimitivaRecursiva* casoBase,
      const FuncionPrimitivaRecursiva* pasoRecursivo,
      ContadorLlamadas& contador);

 protected:
  /// @brief Evalúa el caso base y despliega iterativamente el paso recursivo.
  Natural calcular(const std::vector<Natural>& argumentos) const override;

 private:
  const FuncionPrimitivaRecursiva* caso_base_;  ///< Función para índice cero.
  const FuncionPrimitivaRecursiva*
      paso_recursivo_;  ///< Función para cada sucesor.
};

#endif
