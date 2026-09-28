// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Lenguajes y Paradigmas de Programación
// Curso: 4º
// Práctica 1: Programación de Autómata con Pila
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 25/09/2026
// Archivo: Estado.h
// Contiene la declaración de la clase Estado, encargada de representar
// un estado del autómata y gestionar sus posibles transiciones.
// Referencias:
// Enlaces de interés:
// Historial de revisiones:

#ifndef ESTADO_H
#define ESTADO_H

#include "Tipos.h"
#include "Transicion.h"

#include <vector>
#include <set>
#include <map>
#include <string>
#include <utility>

class Estado {
 public:
  explicit Estado(identificadorEstado identificador)
      : identificador_(std::move(identificador)) {}

  const identificadorEstado& identificador() const {
    return identificador_;
  }

  //ResultadoTransicion obtenerTransiciones(Transicion);

  // método que añade una transición con origen en el Estado actual
  inline void añadirTransicion(EntradaTransicion entrada_transicion, ResultadoTransicion resultado_transicion) {
    transiciones_.emplace(entrada_transicion, resultado_transicion);
  }

  // método que obtiene las transiciones NO vacías para el estado actual
  std::vector<ResultadoTransicion> obtenerTransiciones(const EntradaTransicion& transicion);

 private:
  identificadorEstado identificador_;
  std::multimap<EntradaTransicion, ResultadoTransicion> transiciones_;
};


#endif