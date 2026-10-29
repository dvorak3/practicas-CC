#ifndef COMPOSICION_H
#define COMPOSICION_H

#include <memory>
#include <vector>

#include "FuncionPrimitivaRecursiva.h"

class Composicion final : public FuncionPrimitivaRecursiva {
 public:
  Composicion(std::shared_ptr<const FuncionPrimitivaRecursiva> exterior,
              std::vector<std::shared_ptr<const FuncionPrimitivaRecursiva>>
                  interiores,
              ContadorLlamadas& contador);

 protected:
  Natural calcular(const Argumentos& argumentos) const override;

 private:
  std::shared_ptr<const FuncionPrimitivaRecursiva> exterior_;
  std::vector<std::shared_ptr<const FuncionPrimitivaRecursiva>> interiores_;
};

#endif
