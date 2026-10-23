// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 2: Programar un simulador de una máquina de Turing
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 16/10/2026
// Archivo: Tipos.h
// Contiene los alias, el tipo de movimiento y la constante del símbolo
// blanco utilizados al representar la máquina de Turing.
// Referencias:
// Enlaces de interés:
// Historial de revisiones:

#ifndef TIPOS_H
#define TIPOS_H

#include <string>

/// @brief Tipo usado para representar un símbolo de entrada o de cinta.
using Simbolo = char;

/// @brief Tipo usado para identificar de forma única los estados de la MT.
using IdentificadorEstado = std::string;

/**
 * @brief Movimientos permitidos para las cabezas de las cintas.
 */
enum class Movimiento {
  L,  ///< Desplaza la cabeza una posición a la izquierda.
  R,  ///< Desplaza la cabeza una posición a la derecha.
  S   ///< Mantiene la cabeza en la posición actual.
};

/// @brief Símbolo que representa las celdas en blanco de las cintas.
inline constexpr Simbolo SIMBOLO_BLANCO = '.';

#endif
