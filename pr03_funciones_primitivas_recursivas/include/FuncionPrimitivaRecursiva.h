#ifndef FUNCION_PRIMITIVA_RECURSIVA_H
#define FUNCION_PRIMITIVA_RECURSIVA_H

#include <cstddef>
#include <string>

#include "ContadorLlamadas.h"
#include "Tipos.h"

class FuncionPrimitivaRecursiva {
 public:
  FuncionPrimitivaRecursiva(std::size_t aridad, std::string nombre,
                            ContadorLlamadas& contador);
  virtual ~FuncionPrimitivaRecursiva() = default;

  Natural operator()(const Argumentos& argumentos) const;
  std::size_t aridad() const;

 protected:
  virtual Natural calcular(const Argumentos& argumentos) const = 0;
  ContadorLlamadas& contador_;

 private:
  std::size_t aridad_;
  std::string nombre_;
};

#endif
