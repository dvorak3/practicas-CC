#include "FuncionPrimitivaRecursiva.h"

#include <stdexcept>
#include <utility>

FuncionPrimitivaRecursiva::FuncionPrimitivaRecursiva(
    std::size_t aridad, std::string nombre, ContadorLlamadas& contador)
    : contador_(contador), aridad_(aridad), nombre_(std::move(nombre)) {}

Natural FuncionPrimitivaRecursiva::operator()(
    const Argumentos& argumentos) const {
  if (argumentos.size() != aridad_) {
    throw std::invalid_argument("La funcion " + nombre_ + " requiere " +
                                std::to_string(aridad_) + " argumentos.");
  }
  contador_.incrementar();
  return calcular(argumentos);
}

std::size_t FuncionPrimitivaRecursiva::aridad() const { return aridad_; }
