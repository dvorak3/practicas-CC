#include "Composicion.h"

#include <stdexcept>
#include <utility>

namespace {
/**
 * @brief Valida la estructura de la composición y obtiene su aridad.
 * @param exterior Función que recibe los resultados de las funciones interiores.
 * @param interiores Funciones aplicadas a los argumentos originales.
 * @return Aridad común de las funciones interiores.
 * @throws std::invalid_argument Si falta alguna función o las aridades no son
 * compatibles con la composición.
 */
std::size_t obtenerAridad(
    const FuncionPrimitivaRecursiva* exterior,
    const std::vector<const FuncionPrimitivaRecursiva*>& interiores) {
  if (!exterior || interiores.empty()) {
    throw std::invalid_argument(
        "Una composicion necesita una funcion exterior y al menos una interior.");
  }
  for (const FuncionPrimitivaRecursiva* interior : interiores) {
    if (!interior || interior->aridad() != interiores.front()->aridad()) {
      throw std::invalid_argument(
          "Las funciones interiores deben tener la misma aridad.");
    }
  }
  if (exterior->aridad() != interiores.size()) {
    throw std::invalid_argument(
        "El numero de funciones interiores debe coincidir con la aridad "
        "de la funcion exterior.");
  }
  return interiores.front()->aridad();
}
}  // namespace

/**
 * @brief Inicializa la composición después de validar sus funciones.
 * @param exterior Función aplicada a los resultados interiores.
 * @param interiores Funciones evaluadas sobre los argumentos de entrada.
 * @param contador Contador compartido por la composición y sus componentes.
 * @throws std::invalid_argument Si la estructura no forma una composición válida.
 */
Composicion::Composicion(
    const FuncionPrimitivaRecursiva* exterior,
    std::vector<const FuncionPrimitivaRecursiva*> interiores,
    ContadorLlamadas& contador)
    : FuncionPrimitivaRecursiva(obtenerAridad(exterior, interiores),
                                "composicion", contador),
      exterior_(exterior),
    interiores_(std::move(interiores)) {}

/**
 * @brief Evalúa cada función interior y compone sus resultados.
 * @param argumentos Argumentos que se pasan a todas las funciones interiores.
 * @return Resultado de aplicar la función exterior a los valores interiores.
 */
Natural Composicion::calcular(
    const std::vector<Natural>& argumentos) const {
  std::vector<Natural> resultados;
  resultados.reserve(interiores_.size());
  for (const FuncionPrimitivaRecursiva* interior : interiores_) {
    resultados.push_back((*interior)(argumentos));
  }
  return (*exterior_)(resultados);
}
