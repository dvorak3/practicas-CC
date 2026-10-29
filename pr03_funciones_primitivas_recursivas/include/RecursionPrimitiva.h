#ifndef RECURSION_PRIMITIVA_H
#define RECURSION_PRIMITIVA_H

#include <memory>

#include "FuncionPrimitivaRecursiva.h"

class RecursionPrimitiva final : public FuncionPrimitivaRecursiva {
 public:
  RecursionPrimitiva(
      std::shared_ptr<const FuncionPrimitivaRecursiva> casoBase,
      std::shared_ptr<const FuncionPrimitivaRecursiva> pasoRecursivo,
      ContadorLlamadas& contador);

 protected:
  Natural calcular(const Argumentos& argumentos) const override;

 private:
  std::shared_ptr<const FuncionPrimitivaRecursiva> caso_base_;
  std::shared_ptr<const FuncionPrimitivaRecursiva> paso_recursivo_;
};

#endif
