#include "FactoryFunciones.h"

#include <limits>
#include <stdexcept>
#include <utility>

#include "Composicion.h"
#include "RecursionPrimitiva.h"

namespace {
class FuncionCero final : public FuncionPrimitivaRecursiva {
 public:
  FuncionCero(std::size_t aridad, ContadorLlamadas& contador)
      : FuncionPrimitivaRecursiva(aridad, "cero", contador) {}

 protected:
  Natural calcular(const Argumentos&) const override { return 0; }
};

class FuncionSucesor final : public FuncionPrimitivaRecursiva {
 public:
  explicit FuncionSucesor(ContadorLlamadas& contador)
      : FuncionPrimitivaRecursiva(1, "sucesor", contador) {}

 protected:
  Natural calcular(const Argumentos& argumentos) const override {
    if (argumentos.front() == std::numeric_limits<Natural>::max()) {
      throw std::overflow_error("El resultado excede el rango de Natural.");
    }
    return argumentos.front() + 1;
  }
};

class FuncionProyeccion final : public FuncionPrimitivaRecursiva {
 public:
  FuncionProyeccion(std::size_t aridad, std::size_t posicion,
                    ContadorLlamadas& contador)
      : FuncionPrimitivaRecursiva(aridad, "proyeccion", contador),
        posicion_(posicion) {}

 protected:
  Natural calcular(const Argumentos& argumentos) const override {
    return argumentos[posicion_ - 1];
  }

 private:
  std::size_t posicion_;
};
}  // namespace

FactoryFunciones::FactoryFunciones(ContadorLlamadas& contador)
    : contador_(contador) {}

std::shared_ptr<const FuncionPrimitivaRecursiva> FactoryFunciones::crearCero(
    std::size_t aridad) const {
  if (aridad == 0) {
    throw std::invalid_argument(
        "La funcion cero debe tener al menos un argumento.");
  }
  return std::make_shared<FuncionCero>(aridad, contador_);
}

std::shared_ptr<const FuncionPrimitivaRecursiva>
FactoryFunciones::crearSucesor() const {
  return std::make_shared<FuncionSucesor>(contador_);
}

std::shared_ptr<const FuncionPrimitivaRecursiva>
FactoryFunciones::crearProyeccion(std::size_t aridad,
                                  std::size_t posicion) const {
  if (aridad == 0 || posicion == 0 || posicion > aridad) {
    throw std::invalid_argument(
        "La posicion de una proyeccion debe estar entre 1 y su aridad.");
  }
  return std::make_shared<FuncionProyeccion>(aridad, posicion, contador_);
}

std::shared_ptr<const FuncionPrimitivaRecursiva>
FactoryFunciones::crearComposicion(
    std::shared_ptr<const FuncionPrimitivaRecursiva> exterior,
    std::vector<std::shared_ptr<const FuncionPrimitivaRecursiva>> interiores)
    const {
  return std::make_shared<Composicion>(std::move(exterior),
                                       std::move(interiores), contador_);
}

std::shared_ptr<const FuncionPrimitivaRecursiva>
FactoryFunciones::crearRecursion(
    std::shared_ptr<const FuncionPrimitivaRecursiva> casoBase,
    std::shared_ptr<const FuncionPrimitivaRecursiva> pasoRecursivo) const {
  return std::make_shared<RecursionPrimitiva>(
      std::move(casoBase), std::move(pasoRecursivo), contador_);
}

std::shared_ptr<const FuncionPrimitivaRecursiva>
FactoryFunciones::crearPotencia() const {
  const auto sucesor = crearSucesor();
  const auto ceroUnaria = crearCero(1);
  const auto uno = crearComposicion(sucesor, {ceroUnaria});

  const auto casoBaseSuma = crearProyeccion(1, 1);
  const auto valorPrevioSuma = crearProyeccion(3, 2);
  const auto pasoSuma =
      crearComposicion(sucesor, {valorPrevioSuma});
  const auto suma = crearRecursion(casoBaseSuma, pasoSuma);

  const auto casoBaseProducto = crearCero(1);
  const auto previoProducto = crearProyeccion(3, 2);
  const auto factorProducto = crearProyeccion(3, 3);
  const auto pasoProducto =
      crearComposicion(suma, {factorProducto, previoProducto});
  const auto producto = crearRecursion(casoBaseProducto, pasoProducto);

  const auto previoPotencia = crearProyeccion(3, 2);
  const auto basePotencia = crearProyeccion(3, 3);
  const auto pasoPotencia =
      crearComposicion(producto, {basePotencia, previoPotencia});
  return crearRecursion(uno, pasoPotencia);
}
