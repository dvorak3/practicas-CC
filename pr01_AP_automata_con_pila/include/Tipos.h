// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 1: Programar un simulador de un autómata con pila
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 26/09/2026
// Archivo: Tipos.h
// Contiene la definición de los tipos utilizados para representar
// los símbolos del autómata con pila.
// Referencias:
// Enlaces de interés:
// Historial de revisiones:

#ifndef TIPOS_H
#define TIPOS_H

#include <string>

using SimboloCadena = char;
using SimboloPila = char;
using identificadorEstado = std::string;

// definimos el símbolo vacío para las cadenas
const SimboloCadena SIMBOLO_CADENA_VACIO = '.';
const SimboloPila   SIMBOLO_PILA_VACIO = '.';

#endif