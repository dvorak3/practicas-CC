// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 1: Programación de Autómata con Pila
// Autor: Iván Hernández Rodríguez 
// Correo: alu0101562100@ull.edu.es
// Fecha: 25/09/2026
// Archivo AutomataConPila.h
// Contiene la declaración de la clase AutomataConPila
// Referencias:
// Enlaces de interés
// https://en.wikipedia.org/wiki/Standard_Template_Library#Containers//
// Historial de revisiones

#ifndef AUTOMATA_CON_PILA_H
#define AUTOMATA_CON_PILA_H

#include "Estado.h"
#include "Pila.h"

#include <vector>

class AutomataConPila {
 public:
  /**
   * @brief método que construye el AP, el cual es responsable de la eliminación de los estados en su destrucción
   */
  AutomataConPila(std::vector<Estado*> estados,
                  Estado* estado_actual,
                  std::vector<Estado*> estados_finales,
                  SimboloPila simbolo_inicial_pila)
    : estados_(std::move(estados)),
      estado_actual_(estado_actual),
      estados_finales_(std::move(estados_finales)),
      pila_(simbolo_inicial_pila) {}

  // constructor que inicializa el Estado inicial...
  //Automata();

  // método que comienza la ejecución de todo el Autómata
  // leerCadena();

 private:
  std::vector<Estado*>  estados_;
  Estado*               estado_actual_;
  std::vector<Estado*>  estados_finales_;
  Pila                  pila_;
};

#endif