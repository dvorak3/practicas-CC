#ifndef FACTORY_FUNCIONES_H
#define FACTORY_FUNCIONES_H

#include <cstddef>
#include <memory>
#include <vector>

#include "FuncionPrimitivaRecursiva.h"

class FactoryFunciones {
 public:
  explicit FactoryFunciones(ContadorLlamadas& contador);

  std::shared_ptr<const FuncionPrimitivaRecursiva> crearCero(
      std::size_t aridad) const;
  std::shared_ptr<const FuncionPrimitivaRecursiva> crearSucesor() const;
  std::shared_ptr<const FuncionPrimitivaRecursiva> crearProyeccion(
      std::size_t aridad, std::size_t posicion) const;
  std::shared_ptr<const FuncionPrimitivaRecursiva> crearComposicion(
      std::shared_ptr<const FuncionPrimitivaRecursiva> exterior,
      std::vector<std::shared_ptr<const FuncionPrimitivaRecursiva>> interiores)
      const;
  std::shared_ptr<const FuncionPrimitivaRecursiva> crearRecursion(
      std::shared_ptr<const FuncionPrimitivaRecursiva> casoBase,
      std::shared_ptr<const FuncionPrimitivaRecursiva> pasoRecursivo) const;
  std::shared_ptr<const FuncionPrimitivaRecursiva> crearPotencia() const;

 private:
  ContadorLlamadas& contador_;
};

#endif
