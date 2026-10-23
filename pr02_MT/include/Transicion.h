// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 2: Programar un simulador de una máquina de Turing
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 16/10/2026
// Archivo: Transicion.h
// Contiene la estructura Transicion, que almacena el estado de destino,
// los símbolos que se escriben y los movimientos de las cabezas.
// Referencias:
// Enlaces de interés:
// Historial de revisiones:

#ifndef TRANSICION_H
#define TRANSICION_H

#include "Tipos.h"

#include <string>
#include <vector>

/**
 * @brief Efecto de una transición de una máquina de Turing multicinta.
 *
 * Define el estado al que se transita, el símbolo que se escribe y el
 * movimiento que realiza cada cabeza.
 */
struct Transicion {
  IdentificadorEstado estado_destino;  ///< Estado al que pasa la máquina.
  std::vector<Simbolo> simbolos_escritos;  ///< Símbolo escrito en cada cinta.
  std::vector<Movimiento> movimientos;  ///< Movimiento de cada cabeza.
};

#endif
