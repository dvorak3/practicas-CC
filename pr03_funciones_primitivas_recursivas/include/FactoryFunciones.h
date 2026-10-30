#ifndef FACTORY_FUNCIONES_H
#define FACTORY_FUNCIONES_H

#include <cstddef>
#include <vector>

#include "FuncionPrimitivaRecursiva.h"

/**
 * @brief Crea y administra las funciones del grafo de la práctica.
 *
 * La fábrica es propietaria de las funciones dinámicas que crea. Los punteros
 * devueltos no transfieren propiedad y dejan de ser válidos al destruirse la
 * fábrica.
 */
class FactoryFunciones {
 public:
  /// @brief Crea una fábrica asociada al contador compartido.
  explicit FactoryFunciones(ContadorLlamadas& contador);
  /// @brief Libera todas las funciones dinámicas creadas por esta fábrica.
  ~FactoryFunciones();

  /// @brief Impide copiar la fábrica y duplicar la propiedad de sus funciones.
  FactoryFunciones(const FactoryFunciones&) = delete;
  /// @brief Impide asignar una fábrica y duplicar la propiedad de sus funciones.
  FactoryFunciones& operator=(const FactoryFunciones&) = delete;

  /// @brief Crea la función cero con la aridad indicada.
  const FuncionPrimitivaRecursiva* crearCero(std::size_t aridad);
  /// @brief Crea la función sucesor unaria.
  const FuncionPrimitivaRecursiva* crearSucesor();
  /// @brief Crea la proyección de una posición entre 1 y la aridad.
  const FuncionPrimitivaRecursiva* crearProyeccion(std::size_t aridad,
                                                   std::size_t posicion);
  /// @brief Crea la composición de una función exterior e interiores.
  const FuncionPrimitivaRecursiva* crearComposicion(
      const FuncionPrimitivaRecursiva* exterior,
      std::vector<const FuncionPrimitivaRecursiva*> interiores);
  /// @brief Crea una función con el esquema de recursión primitiva.
  const FuncionPrimitivaRecursiva* crearRecursion(
      const FuncionPrimitivaRecursiva* casoBase,
      const FuncionPrimitivaRecursiva* pasoRecursivo);
  /// @brief Construye la potencia a partir de las funciones primitivas.
  /// Combina cero, sucesor, proyección, composición y recursión primitiva.
  const FuncionPrimitivaRecursiva* crearPotencia();

 private:
  /// @brief Registra la función para liberarla al destruir la fábrica.
  /// Si falla el registro, libera el objeto y propaga la excepción.
  template <typename T>
  T* registrarFuncion(T* funcion) {
    try {
      funciones_.push_back(funcion);
    } catch (...) {
      delete funcion;
      throw;
    }
    return funcion;
  }

  ContadorLlamadas& contador_;  ///< Contador compartido por las funciones.
  std::vector<FuncionPrimitivaRecursiva*>
      funciones_;  ///< Funciones propiedad de la fábrica.
};

#endif
