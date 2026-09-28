// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 1: Programar un simulador de un autómata con pila
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 26/09/2026
// Archivo: Pila.h
// Contiene la declaración de la clase Pila, encargada de representar
// y gestionar la pila utilizada por el autómata con pila.
// Referencias:
// Enlaces de interés:
// Historial de revisiones:

#ifndef PILA_H
#define PILA_H

#include <ostream>

class Pila {
 public:
  // constructor - una pila requiere sí o sí un símbolo inicial
  Pila(SimboloPila simbolo_inicial) {
    apilar(simbolo_inicial);
  }

  // ============= métodos ============= 

  // crea una nueva CIMA de la pila
  void apilar(SimboloPila simbolo) {
   pila_.push_back(simbolo);
  }

  // inserta varios elementos de golpe 
  void apilar(const std::vector<SimboloPila>& simbolos) {
    for (std::size_t i{simbolos.size()}; i > 0; --i)
      apilar(simbolos[i - 1]);
  }

  // borra el elemento TOP de la pila
  void desapilar() {
    pila_.pop_back();
  }

  // muestra el elemento de la CIMA 
  const SimboloPila& cima() const {
    return pila_.back();
  }

  // comprueba si la pila está vacía
  bool estaVacia() const {
    return pila_.empty();
  }

  friend std::ostream& operator<<(std::ostream& salida, const Pila& pila) {
    salida << '[';
    for (auto it = pila.pila_.rbegin(); it != pila.pila_.rend(); ++it) {
      if (it != pila.pila_.rbegin()) salida << ' ';
      salida << *it;
    }
    return salida << ']';
  }
  
 private:
  std::vector<SimboloPila> pila_;
};

#endif