// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 2: Programar un simulador de una máquina de Turing
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 16/10/2026
// Archivo: Estado.h
// Contiene la declaración de la clase Estado, que representa un estado
// de la máquina de Turing y gestiona sus transiciones deterministas.
// Referencias:
// Enlaces de interés:
// Historial de revisiones:

#ifndef ESTADO_H
#define ESTADO_H

#include "Transicion.h"

#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

/**
 * @brief Representa un estado y las transiciones que parten de él.
 *
 * Las transiciones se indexan mediante el vector de símbolos leídos en las
 * cintas. Al ser determinista, cada vector de lectura identifica como máximo
 * una transición de salida.
 */
class Estado {
 public:
  /// @brief Construye un estado con su identificador único.
  explicit Estado(std::string identificador)
      : identificador_(std::move(identificador)) {}

  /// @brief Devuelve el identificador del estado.
  const std::string& identificador() const noexcept {
    return identificador_;
  }

  /// @brief Añade una transición indexada por los símbolos leídos en las cintas.
  void anadirTransicion(const std::vector<Simbolo>& lectura,
                        Transicion transicion) {
    if (!transiciones_.emplace(lectura, std::move(transicion)).second) {
      throw std::runtime_error("Transicion no determinista en el estado " +
                               identificador_ + ".");
    }
  }

  /// @brief Busca la transición asociada a la lectura actual.
  const Transicion* buscarTransicion(
      const std::vector<Simbolo>& lectura) const noexcept {
    const auto encontrada = transiciones_.find(lectura);
    return encontrada == transiciones_.end() ? nullptr : &encontrada->second;
  }

 private:
  std::string identificador_;  ///< Identificador único del estado.
  std::map<std::vector<Simbolo>, Transicion> transiciones_;  ///< Transiciones indexadas por lectura.
};

#endif
