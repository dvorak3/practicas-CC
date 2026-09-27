// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Lenguajes y Paradigmas de Programación
// Curso: 4º
// Práctica 1: Programación de Autómata con Pila
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 25/09/2026
// Archivo: Estado.cc
// Contiene la definición de la clase Estado, encargada de representar
// un estado del autómata y gestionar sus posibles transiciones.
// Referencias:
// Enlaces de interés:
// Historial de revisiones:

#include "../include/Estado.h"
#include "../include/Transicion.h"

/** 
 * @brief método que (para un estado dado), devuelve el resultado de dicha transición entre todas las existentes 
 * @param Transicion transicion deseada
 * @return ResultadoTransicion el resultado de dicha transición
 ResultadoTransicion Estado::obtenerTransicion(Transicion transicion) {
  return transiciones_[transicion];
}
*/