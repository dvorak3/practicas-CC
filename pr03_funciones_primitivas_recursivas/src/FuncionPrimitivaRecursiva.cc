#include "FuncionPrimitivaRecursiva.h"

#include <stdexcept>
#include <utility>

/**
 * @brief Inicializa la aridad, el nombre y el contador de la función.
 * @param aridad Número de argumentos aceptados por la función.
 * @param nombre Nombre empleado en los mensajes de error.
 * @param contador Contador compartido al que se registran las evaluaciones.
 */
FuncionPrimitivaRecursiva::FuncionPrimitivaRecursiva(
    std::size_t aridad, std::string nombre, ContadorLlamadas& contador)
    : contador_(contador), aridad_(aridad), nombre_(std::move(nombre)) {}

/**
 * @brief Valida los argumentos, registra la evaluación y calcula el resultado.
 * @param argumentos Valores recibidos por la función.
 * @return Resultado de la función derivada.
 * @throws std::invalid_argument Si el número de argumentos no coincide con la
 * aridad declarada.
 * @throws std::overflow_error Si el contador no puede incrementarse.
 */
Natural FuncionPrimitivaRecursiva::operator()(
    const std::vector<Natural>& argumentos) const {
  if (argumentos.size() != aridad_) {
    throw std::invalid_argument("La funcion " + nombre_ + " requiere " +
                                std::to_string(aridad_) + " argumentos.");
  }
  contador_.incrementar();
  return calcular(argumentos);
}

/**
 * @brief Devuelve la aridad declarada al construir la función.
 * @return Número de argumentos que requiere esta función.
 */
std::size_t FuncionPrimitivaRecursiva::aridad() const { return aridad_; }
