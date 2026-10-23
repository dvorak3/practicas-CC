// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 2: Programar un simulador de una máquina de Turing
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 16/10/2026
// Archivo: Cinta.cc
// Contiene la implementación de la clase Cinta y la sobrecarga del
// operador de salida para mostrar el contenido y la cabeza de lectura.
// Referencias:
// Enlaces de interés:
// Historial de revisiones:

#include "../include/Cinta.h"

#include <algorithm>

/**
 * @brief Lee el símbolo de la celda señalada por la cabeza.
 * @return El símbolo almacenado o SIMBOLO_BLANCO si la celda no está escrita.
 */
Simbolo Cinta::leer() const {
  const auto celda = celdas_.find(cabeza_);
  return celda == celdas_.end() ? SIMBOLO_BLANCO : celda->second;
}

/**
 * @brief Escribe un símbolo en la celda actualmente señalada.
 * @param simbolo Símbolo que se escribirá; si es blanco, elimina la celda
 * explícita del almacenamiento disperso.
 */
void Cinta::escribir(Simbolo simbolo) {
  if (simbolo == SIMBOLO_BLANCO) {
    celdas_.erase(cabeza_);
  } else {
    celdas_[cabeza_] = simbolo;
  }
}

/**
 * @brief Mueve la cabeza una celda según la dirección indicada.
 * @param movimiento Movimiento::L, Movimiento::R o Movimiento::S.
 */
void Cinta::mover(Movimiento movimiento) {
  switch (movimiento) {
    case Movimiento::L:
      --cabeza_;
      break;
    case Movimiento::R:
      ++cabeza_;
      break;
    case Movimiento::S:
      break;
  }
}

/**
 * @brief Inicializa la cinta con una cadena de entrada.
 * @param entrada Cadena que se copiará desde la posición cero.
 *
 * La cabeza se restablece a la primera posición y las celdas que contienen
 * el símbolo blanco no se almacenan explícitamente.
 */
void Cinta::cargarEntrada(const std::string& entrada) {
  celdas_.clear();
  cabeza_ = 0;
  for (std::size_t i = 0; i < entrada.size(); ++i) {
    if (entrada[i] != SIMBOLO_BLANCO) {
      celdas_.emplace(static_cast<long long>(i), entrada[i]);
    }
  }
}

/**
 * @brief Escribe una representación visible de una cinta en un flujo.
 * @param salida Flujo de salida donde se inserta la representación.
 * @param cinta Cinta que se representará.
 * @return El flujo recibido, permitiendo encadenar operaciones de salida.
 *
 * Se muestran las celdas desde una posición antes del primer símbolo
 * almacenado hasta una posición después del último; la celda bajo la cabeza
 * queda delimitada por corchetes.
 */
std::ostream& operator<<(std::ostream& salida, const Cinta& cinta) {
  if (cinta.celdas_.empty()) return salida << "[.]";

  long long minimo = cinta.celdas_.begin()->first;
  long long maximo = cinta.celdas_.rbegin()->first;
  minimo = std::min(minimo - 1, cinta.cabeza_);
  maximo = std::max(maximo + 1, cinta.cabeza_);

  for (long long posicion = minimo; posicion <= maximo; ++posicion) {
    const auto celda = cinta.celdas_.find(posicion);
    const Simbolo simbolo =
        celda == cinta.celdas_.end() ? SIMBOLO_BLANCO : celda->second;
    if (posicion == cinta.cabeza_) salida << '[';
    salida << simbolo;
    if (posicion == cinta.cabeza_) salida << ']';
  }
  return salida;
}
