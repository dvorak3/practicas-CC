#include "RecursionPrimitiva.h"

#include <stdexcept>
#include <utility>

namespace {
std::size_t obtenerAridad(
    const std::shared_ptr<const FuncionPrimitivaRecursiva>& casoBase,
    const std::shared_ptr<const FuncionPrimitivaRecursiva>& pasoRecursivo) {
  if (!casoBase || !pasoRecursivo ||
      pasoRecursivo->aridad() != casoBase->aridad() + 2) {
    throw std::invalid_argument(
        "La recursividad primitiva requiere una funcion base de aridad k y "
        "un paso de aridad k + 2.");
  }
  return casoBase->aridad() + 1;
}
}  // namespace

RecursionPrimitiva::RecursionPrimitiva(
    std::shared_ptr<const FuncionPrimitivaRecursiva> casoBase,
    std::shared_ptr<const FuncionPrimitivaRecursiva> pasoRecursivo,
    ContadorLlamadas& contador)
    : FuncionPrimitivaRecursiva(
          obtenerAridad(casoBase, pasoRecursivo), "recursion primitiva",
          contador),
      caso_base_(std::move(casoBase)),
      paso_recursivo_(std::move(pasoRecursivo)) {}

Natural RecursionPrimitiva::calcular(const Argumentos& argumentos) const {
  const Natural iteraciones = argumentos.front();
  Argumentos parametros(argumentos.begin() + 1, argumentos.end());
  Natural resultado = (*caso_base_)(parametros);

  for (Natural indice = 0; indice < iteraciones; ++indice) {
    // Cada vuelta representa una llamada recursiva de esta funcion.
    contador_.incrementar();
    Argumentos argumentosPaso;
    argumentosPaso.reserve(argumentos.size() + 1);
    argumentosPaso.push_back(indice);
    argumentosPaso.push_back(resultado);
    argumentosPaso.insert(argumentosPaso.end(), parametros.begin(),
                          parametros.end());
    resultado = (*paso_recursivo_)(argumentosPaso);
  }
  return resultado;
}
