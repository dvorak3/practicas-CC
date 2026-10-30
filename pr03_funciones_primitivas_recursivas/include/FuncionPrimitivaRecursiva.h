#ifndef FUNCION_PRIMITIVA_RECURSIVA_H
#define FUNCION_PRIMITIVA_RECURSIVA_H

#include <cstddef>
#include <string>
#include <vector>

#include "ContadorLlamadas.h"
#include "Tipos.h"

/**
 * @brief Interfaz común para las funciones primitivas recursivas.
 *
 * Valida la aridad de cada evaluación y registra las llamadas antes de delegar
 * el cálculo concreto en las clases derivadas.
 */
class FuncionPrimitivaRecursiva {
 public:
  /// @brief Construye una función con aridad, nombre y contador compartido.
  FuncionPrimitivaRecursiva(std::size_t aridad, std::string nombre,
                            ContadorLlamadas& contador);
  /// @brief Permite destruir objetos derivados mediante la interfaz base.
  virtual ~FuncionPrimitivaRecursiva() = default;

  /// @brief Evalúa la función tras comprobar el número de argumentos.
  Natural operator()(const std::vector<Natural>& argumentos) const;
  /// @brief Devuelve la cantidad de argumentos que requiere la función.
  std::size_t aridad() const;

 protected:
  /// @brief Calcula el resultado para argumentos cuya aridad ya fue validada.
  virtual Natural calcular(const std::vector<Natural>& argumentos) const = 0;
  ContadorLlamadas& contador_;  ///< Contador compartido por el grafo de funciones.

 private:
  std::size_t aridad_;  ///< Número de argumentos requeridos.
  std::string nombre_;  ///< Nombre usado al informar errores de aridad.
};

#endif
