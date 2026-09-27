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

class Estado {
 public:
  //ResultadoTransicion obtenerTransiciones(Transicion);

  void añadirTransicion(EntradaTransicion entrada_transicion, ResultadoTransicion resultado_transicion) {
    transiciones_.emplace(entrada_transicion, resultado_transicion);
  }

 private:
  std::multimap<EntradaTransicion, ResultadoTransicion> transiciones_;
};

#endif