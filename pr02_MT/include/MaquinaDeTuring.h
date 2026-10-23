// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 2: Programar un simulador de una máquina de Turing
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 16/10/2026
// Archivo: MaquinaDeTuring.h
// Contiene la declaración de la clase MaquinaDeTuring, que almacena la
// definición de la máquina y coordina la ejecución de sus cintas.
// Referencias:
// Enlaces de interés:
// Historial de revisiones:

#ifndef MAQUINA_DE_TURING_H
#define MAQUINA_DE_TURING_H

#include "Estado.h"

#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

/**
 * @brief Resume el resultado de ejecutar una máquina sobre una entrada.
 *
 * Almacena la decisión de aceptación, el número de transiciones ejecutadas y
 * una representación textual de las cintas al detenerse la simulación.
 */
struct ResultadoEjecucion {
  bool aceptada{false};  ///< Indica si la máquina se detuvo en un estado final.
  std::size_t pasos{0};  ///< Número de transiciones ejecutadas.
  std::vector<std::string> cintas;  ///< Representación final de cada cinta.
};

/**
 * @brief Almacena una MT determinista multicinta y permite simularla.
 *
 * Mantiene la definición formal de la máquina y ejecuta sus transiciones hasta
 * que no exista ninguna aplicable. La aceptación depende de que el estado de
 * parada pertenezca al conjunto de estados finales.
 */
class MaquinaDeTuring {
 public:
  /// @brief Construye la máquina con sus estados, alfabetos y estados especiales.
  MaquinaDeTuring(std::set<IdentificadorEstado> estados,
                  std::set<Simbolo> alfabeto_entrada,
                  std::set<Simbolo> alfabeto_cinta,
                  IdentificadorEstado estado_inicial,
                  std::set<IdentificadorEstado> estados_finales,
                  std::size_t numero_cintas);

  /// @brief Añade una transición determinista a un estado de origen.
  void anadirTransicion(const IdentificadorEstado& origen,
                        const std::vector<Simbolo>& lectura,
                        Transicion transicion);

  /// @brief Ejecuta la máquina sobre una cadena de entrada.
  ResultadoEjecucion ejecutar(const std::string& entrada,
                             bool mostrar_traza = false) const;

  /// @brief Devuelve el número de cintas de la máquina.
  std::size_t numeroCintas() const noexcept {
    return numero_cintas_;
  }

 private:
  std::set<Simbolo> alfabeto_entrada_;  ///< Alfabeto Sigma.
  std::set<Simbolo> alfabeto_cinta_;  ///< Alfabeto Gamma, incluido el blanco.
  std::map<IdentificadorEstado, Estado> estados_;  ///< Estados definidos por la máquina.
  IdentificadorEstado estado_inicial_;  ///< Estado desde el que comienza cada simulación.
  std::set<IdentificadorEstado> estados_finales_;  ///< Estados de aceptación.
  std::size_t numero_cintas_;  ///< Cantidad de cintas y cabezas de la máquina.
};

#endif
