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

#include <set>
#include <utility>
#include <vector>

/**
 * @brief una Configuración es un struct auxiliar que nos va a ayudar a guardar para un estado X, que cadena le quedaba
          y que tenía escrito en la pila
 */
class AutomataConPila {
 public:
  /**
   * @brief método que construye el AP, el cual es responsable de la eliminación de los estados en su destrucción
   *        además inicializa el estado inicial del autómata
   */
  AutomataConPila(std::vector<Estado*> estados,
                  Estado* estado_actual,
                  std::vector<Estado*> estados_finales,
                  SimboloPila simbolo_inicial_pila)
    : estados_(std::move(estados)),
      estado_inicial_(estado_actual),
      estado_actual_(estado_actual),
      estados_finales_(estados_finales.begin(), estados_finales.end()),
      pila_(simbolo_inicial_pila),
      simbolo_inicial_pila_(simbolo_inicial_pila) {}
  
  ~AutomataConPila() {
    for (Estado* estado : estados_)
      delete estado;
  }
      
  // método que comienza la ejecución de todo el Autómata
  bool leerCadena(const std::vector<SimboloCadena>& cadena);

 protected:
  struct ConfiguracionPendiente {
    Estado* estado; // estado = el estado al que transitar desde estado_actual_
    size_t  posicion_cadena;
    Pila pila;
  };

 private:
  std::vector<Estado*>  estados_;
  Estado*               estado_inicial_;
  Estado*               estado_actual_;
  std::set<Estado*>     estados_finales_;
  Pila                  pila_;
  SimboloPila           simbolo_inicial_pila_;
};

#endif