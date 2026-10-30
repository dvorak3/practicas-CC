#include "RecursionPrimitiva.h"

#include <stdexcept>

namespace {
/**
 * @brief Valida el caso base y el paso recursivo y calcula la aridad.
 * @param casoBase Función aplicada cuando el índice de recursión es cero.
 * @param pasoRecursivo Función aplicada en cada despliegue recursivo.
 * @return Aridad de la función recursiva, igual a la del caso base más uno.
 * @throws std::invalid_argument Si faltan funciones o sus aridades no cumplen
 * el esquema de recursión primitiva.
 */
std::size_t obtenerAridad(
    const FuncionPrimitivaRecursiva* casoBase,
    const FuncionPrimitivaRecursiva* pasoRecursivo) {
  if (!casoBase || !pasoRecursivo ||
      pasoRecursivo->aridad() != casoBase->aridad() + 2) {
    throw std::invalid_argument(
        "La recursividad primitiva requiere una funcion base de aridad k y "
        "un paso de aridad k + 2.");
  }
  return casoBase->aridad() + 1;
}
}  // namespace

/**
 * @brief Construye una función recursiva a partir de sus dos componentes.
 * @param casoBase Función base, de aridad k.
 * @param pasoRecursivo Paso recursivo, de aridad k + 2.
 * @param contador Contador compartido por la función y sus componentes.
 * @throws std::invalid_argument Si las funciones no cumplen las aridades
 * requeridas.
 */
RecursionPrimitiva::RecursionPrimitiva(
    const FuncionPrimitivaRecursiva* casoBase,
    const FuncionPrimitivaRecursiva* pasoRecursivo,
    ContadorLlamadas& contador)
    : FuncionPrimitivaRecursiva(
          obtenerAridad(casoBase, pasoRecursivo), "recursion primitiva",
          contador),
      caso_base_(casoBase),
      paso_recursivo_(pasoRecursivo) {}

/**
 * @brief Despliega iterativamente el esquema de recursión primitiva.
 *
 * Evalúa primero el caso base con los parámetros adicionales y, para cada
 * índice anterior al solicitado, construye los argumentos del paso como
 * (índice, resultado previo, parámetros). Cada despliegue se suma al contador.
 *
 * @param argumentos Índice de recursión seguido de los parámetros adicionales.
 * @return Valor obtenido tras completar las iteraciones solicitadas.
 * @throws std::overflow_error Si el contador se desborda durante un despliegue.
 */
Natural RecursionPrimitiva::calcular(
    const std::vector<Natural>& argumentos) const {
  const Natural iteraciones = argumentos.front();
  std::vector<Natural> parametros(argumentos.begin() + 1, argumentos.end());
  Natural resultado = (*caso_base_)(parametros);

  for (Natural indice = 0; indice < iteraciones; ++indice) {
    // Cada vuelta representa una llamada recursiva de esta funcion.
    contador_.incrementar();
    std::vector<Natural> argumentosPaso;
    argumentosPaso.reserve(argumentos.size() + 1);
    argumentosPaso.push_back(indice);
    argumentosPaso.push_back(resultado);
    argumentosPaso.insert(argumentosPaso.end(), parametros.begin(),
                          parametros.end());
    resultado = (*paso_recursivo_)(argumentosPaso);
  }
  return resultado;
}
