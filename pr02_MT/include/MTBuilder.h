// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 2: Programar un simulador de una máquina de Turing
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 16/10/2026
// Archivo: MTBuilder.h
// Contiene la clase abstracta MTBuilder, que reúne la construcción y
// validación común, y MTBuilderDesdeArchivo, que obtiene los datos de un fichero.
// Referencias:
// Enlaces de interés:
// https://refactoring.guru/design-patterns/builder
// Historial de revisiones:

#ifndef MT_BUILDER_H
#define MT_BUILDER_H

#include "MaquinaDeTuring.h"

#include <fstream>
#include <set>
#include <string>
#include <utility>
#include <vector>

/**
 * @brief Define el algoritmo de construcción y validación común de una MT.
 *
 * Las subclases proporcionan los tokens de la fuente de configuración;
 * el proceso de interpretación y construcción se comparte en esta clase.
 */
class MTBuilder {
 public:
  /// @brief Destructor virtual para permitir la destrucción polimórfica.
  virtual ~MTBuilder() = default;

  /// @brief Construye una máquina a partir de los tokens de la fuente concreta.
  MaquinaDeTuring construirMT();

 protected:
  /// @brief Obtiene la siguiente línea no vacía de la fuente como tokens.
  virtual std::vector<std::string> leerTokens() = 0;

  /// @brief Devuelve el número de línea actual de la fuente.
  virtual std::size_t numeroLineaActual() const noexcept = 0;

  /// @brief Lee una línea obligatoria y comunica qué campo falta si no existe.
  std::vector<std::string> lineaObligatoria(const std::string& campo);

  /// @brief Valida y convierte los tokens de un alfabeto en un conjunto.
  static std::set<Simbolo> leerAlfabeto(
      const std::vector<std::string>& tokens, const std::string& campo);

  /// @brief Convierte una lista de identificadores en un conjunto de estados.
  static std::set<IdentificadorEstado> leerEstados(
      const std::vector<std::string>& tokens);

  /// @brief Convierte la notación textual de movimiento al enum Movimiento.
  static Movimiento leerMovimiento(const std::string& token);

  /// @brief Exige que una línea contenga un único identificador.
  static std::string leerIdentificador(
      const std::vector<std::string>& tokens, const std::string& campo);

  /// @brief Convierte un token de un carácter en un símbolo.
  static Simbolo leerSimbolo(const std::string& token,
                             const std::string& campo);

  /// @brief Comprueba que un identificador pertenezca al conjunto de estados.
  static void validarEstado(const std::set<IdentificadorEstado>& estados,
                            const IdentificadorEstado& identificador,
                            const std::string& campo);
};

/**
 * @brief Construye una máquina leyendo la configuración desde un fichero.
 *
 * Se encarga de abrir el archivo y proporcionar al algoritmo común del builder
 * las líneas útiles, descartando comentarios y líneas vacías.
 */
class MTBuilderDesdeArchivo : public MTBuilder {
 public:
  /// @brief Abre el fichero desde el que se construirá la máquina.
  explicit MTBuilderDesdeArchivo(const std::string& nombre_fichero);

 protected:
  /// @brief Lee y tokeniza la siguiente línea no vacía, ignorando comentarios.
  std::vector<std::string> leerTokens() override;

  /// @brief Devuelve el número de la última línea leída del fichero.
  std::size_t numeroLineaActual() const noexcept override {
    return numero_linea_;
  }

 private:
  std::ifstream archivo_;  ///< Flujo asociado al fichero de configuración.
  std::size_t numero_linea_{0};  ///< Número de la última línea leída.
};

#endif
