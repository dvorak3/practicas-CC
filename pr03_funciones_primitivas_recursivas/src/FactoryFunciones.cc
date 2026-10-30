#include "FactoryFunciones.h"

#include <limits>
#include <stdexcept>
#include <utility>

#include "Composicion.h"
#include "RecursionPrimitiva.h"

namespace {
/**
 * @brief Función constante cero con una aridad configurable.
 */
class FuncionCero final : public FuncionPrimitivaRecursiva {
 public:
  FuncionCero(std::size_t aridad, ContadorLlamadas& contador)
      : FuncionPrimitivaRecursiva(aridad, "cero", contador) {}

 protected:
  /**
   * @brief Devuelve cero para cualquier argumento de aridad válida.
   * @param argumentos Argumentos que no afectan al resultado.
   * @return Cero.
   */
  Natural calcular(const std::vector<Natural>&) const override { return 0; }
};

/**
 * @brief Función sucesor definida para números naturales representables.
 */
class FuncionSucesor final : public FuncionPrimitivaRecursiva {
 public:
  explicit FuncionSucesor(ContadorLlamadas& contador)
      : FuncionPrimitivaRecursiva(1, "sucesor", contador) {}

 protected:
  /**
   * @brief Incrementa el único argumento.
   * @param argumentos Vector unario validado por la interfaz base.
   * @return El argumento incrementado en uno.
   * @throws std::overflow_error Si el argumento ya es el máximo Natural.
   */
  Natural calcular(const std::vector<Natural>& argumentos) const override {
    if (argumentos.front() == std::numeric_limits<Natural>::max()) {
      throw std::overflow_error("El resultado excede el rango de Natural.");
    }
    return argumentos.front() + 1;
  }
};

/**
 * @brief Función que selecciona una posición de sus argumentos.
 */
class FuncionProyeccion final : public FuncionPrimitivaRecursiva {
 public:
  FuncionProyeccion(std::size_t aridad, std::size_t posicion,
                    ContadorLlamadas& contador)
      : FuncionPrimitivaRecursiva(aridad, "proyeccion", contador),
        posicion_(posicion) {}

 protected:
  /**
   * @brief Devuelve el argumento de la posición configurada.
   * @param argumentos Argumentos cuya aridad ya ha sido comprobada.
   * @return Valor de la posición seleccionada.
   */
  Natural calcular(const std::vector<Natural>& argumentos) const override {
    return argumentos[posicion_ - 1];
  }

 private:
  std::size_t posicion_;
};
}  // namespace

/**
 * @brief Asocia la fábrica al contador usado por todas sus funciones.
 * @param contador Contador que se comparte con las funciones creadas.
 */
FactoryFunciones::FactoryFunciones(ContadorLlamadas& contador)
    : contador_(contador) {}

/**
 * @brief Libera cada función creada y registrada por la fábrica.
 */
FactoryFunciones::~FactoryFunciones() {
  for (FuncionPrimitivaRecursiva* funcion : funciones_) {
    delete funcion;
  }
}

/**
 * @brief Crea y registra una función cero.
 * @param aridad Número de argumentos aceptados.
 * @return Puntero no propietario a la función creada.
 * @throws std::invalid_argument Si la aridad es cero.
 */
const FuncionPrimitivaRecursiva* FactoryFunciones::crearCero(
    std::size_t aridad) {
  if (aridad == 0) {
    throw std::invalid_argument(
        "La funcion cero debe tener al menos un argumento.");
  }
  return registrarFuncion(new FuncionCero(aridad, contador_));
}

/**
 * @brief Crea y registra la función sucesor.
 * @return Puntero no propietario a la función creada.
 */
const FuncionPrimitivaRecursiva* FactoryFunciones::crearSucesor() {
  return registrarFuncion(new FuncionSucesor(contador_));
}

/**
 * @brief Crea y registra una función de proyección.
 * @param aridad Número de argumentos de la función.
 * @param posicion Posición, comenzando en uno, que se desea seleccionar.
 * @return Puntero no propietario a la función creada.
 * @throws std::invalid_argument Si la posición no pertenece a la aridad.
 */
const FuncionPrimitivaRecursiva* FactoryFunciones::crearProyeccion(
    std::size_t aridad, std::size_t posicion) {
  if (aridad == 0 || posicion == 0 || posicion > aridad) {
    throw std::invalid_argument(
        "La posicion de una proyeccion debe estar entre 1 y su aridad.");
  }
  return registrarFuncion(new FuncionProyeccion(aridad, posicion, contador_));
}

/**
 * @brief Crea y registra una composición de funciones.
 * @param exterior Función aplicada a los resultados interiores.
 * @param interiores Funciones que reciben los argumentos originales.
 * @return Puntero no propietario a la composición creada.
 * @throws std::invalid_argument Si las funciones no forman una composición.
 */
const FuncionPrimitivaRecursiva* FactoryFunciones::crearComposicion(
    const FuncionPrimitivaRecursiva* exterior,
    std::vector<const FuncionPrimitivaRecursiva*> interiores) {
  return registrarFuncion(
      new Composicion(exterior, std::move(interiores), contador_));
}

/**
 * @brief Crea y registra una función de recursión primitiva.
 * @param casoBase Función que define el valor para índice cero.
 * @param pasoRecursivo Función que calcula cada paso posterior.
 * @return Puntero no propietario a la función creada.
 * @throws std::invalid_argument Si las aridades no satisfacen el esquema.
 */
const FuncionPrimitivaRecursiva* FactoryFunciones::crearRecursion(
    const FuncionPrimitivaRecursiva* casoBase,
    const FuncionPrimitivaRecursiva* pasoRecursivo) {
  return registrarFuncion(
      new RecursionPrimitiva(casoBase, pasoRecursivo, contador_));
}

/**
 * @brief Construye la potencia usando las funciones primitivas requeridas.
 *
 * Compone sucesor, cero y proyecciones para definir suma, producto y potencia
 * mediante recursión primitiva. La fábrica conserva la propiedad de todos los
 * nodos creados para que sus referencias sigan siendo válidas durante el uso.
 *
 * @return Puntero no propietario a la función potencia.
 */
const FuncionPrimitivaRecursiva* FactoryFunciones::crearPotencia() {
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
