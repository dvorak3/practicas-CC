// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 1: Programar un simulador de un autómata con pila
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 26/09/2026
// Archivo: Transicion.h
// Contiene la definición de las estructuras necesarias para representar
// las condiciones de entrada y el resultado de una transición del autómata.
// Referencias:
// Enlaces de interés:
// Historial de revisiones:

#ifndef TRANSICION_H
#define TRANSICION_H

#include "Tipos.h"

#include <vector>

// Declaración adelantada de la clase Estado
class Estado;

struct EntradaTransicion {
  SimboloCadena   simbolo_entrada_cadena;
  SimboloPila     simbolo_entrada_pila;

  bool operator<(const EntradaTransicion& otra) const {
    // ordenamos por el orden alfabético del Símbolo de la Cadena
    if (simbolo_entrada_cadena != otra.simbolo_entrada_cadena) {
      return simbolo_entrada_cadena < otra.simbolo_entrada_cadena;
    }

    // y si son iguales entonces ordenamos por el orden alfabético del Símbolo de la Pila
    return simbolo_entrada_pila < otra.simbolo_entrada_pila;
  }
};

struct ResultadoTransicion {
  Estado* proximo_estado_;
  std::vector<SimboloPila> cadena_a_escribir_en_pila_;
};

#endif