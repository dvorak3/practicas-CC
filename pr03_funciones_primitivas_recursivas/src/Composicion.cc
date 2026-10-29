#include "Composicion.h"

#include <stdexcept>

namespace {
std::size_t obtenerAridad(
    const std::shared_ptr<const FuncionPrimitivaRecursiva>& exterior,
    const std::vector<std::shared_ptr<const FuncionPrimitivaRecursiva>>&
        interiores) {
  if (!exterior || interiores.empty()) {
    throw std::invalid_argument(
        "Una composicion necesita una funcion exterior y al menos una interior.");
  }
  for (const auto& interior : interiores) {
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

Composicion::Composicion(
    std::shared_ptr<const FuncionPrimitivaRecursiva> exterior,
    std::vector<std::shared_ptr<const FuncionPrimitivaRecursiva>> interiores,
    ContadorLlamadas& contador)
    : FuncionPrimitivaRecursiva(obtenerAridad(exterior, interiores),
                                "composicion", contador),
      exterior_(std::move(exterior)),
      interiores_(std::move(interiores)) {}

Natural Composicion::calcular(const Argumentos& argumentos) const {
  Argumentos resultados;
  resultados.reserve(interiores_.size());
  for (const auto& interior : interiores_) {
    resultados.push_back((*interior)(argumentos));
  }
  return (*exterior_)(resultados);
}
