// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 2: Programar un simulador de una máquina de Turing
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 16/10/2026
// Archivo: Cinta.h
// Contiene la declaración de la clase Cinta, que representa una cinta
// infinita y proporciona operaciones para leer, escribir y mover la cabeza.
// Referencias:
// Enlaces de interés:
// Historial de revisiones:

#ifndef CINTA_H
#define CINTA_H

#include "Tipos.h"

#include <map>
#include <ostream>
#include <string>

/**
 * @brief Representa una cinta infinita con una cabeza de lectura y escritura.
 *
 * El almacenamiento es disperso: solo se conservan las celdas no blancas.
 * La clase encapsula la posición de la cabeza y ofrece las operaciones usadas
 * por la simulación de la máquina de Turing.
 */
class Cinta {
 public:
  /// @brief Lee el símbolo situado bajo la cabeza de lectura/escritura.
  Simbolo leer() const;

  /// @brief Escribe un símbolo en la celda actualmente apuntada.
  void escribir(Simbolo simbolo);

  /// @brief Desplaza la cabeza de acuerdo con el movimiento de la transición.
  void mover(Movimiento movimiento);

  /// @brief Inicializa la cinta con una entrada y coloca la cabeza al inicio.
  void cargarEntrada(const std::string& entrada);

  friend std::ostream& operator<<(std::ostream& salida, const Cinta& cinta);

 private:
  std::map<long long, Simbolo> celdas_;  ///< Celdas no blancas indexadas por posición.
  long long cabeza_{0};  ///< Posición actual de la cabeza en la cinta infinita.
};

/// @brief Inserta la cinta en un flujo y marca la celda bajo su cabeza.
std::ostream& operator<<(std::ostream& salida, const Cinta& cinta);

#endif
