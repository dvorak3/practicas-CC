// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 1: Programar un simulador de un autómata con pila
// Autor: Iván Hernández Rodríguez 
// Correo: alu0101562100@ull.edu.es
// Fecha: 25/09/2026
// Archivo AutomataConPilaBuilder.h
// Contiene la declaración de la clase AutomataConPilaBuilder,
// encargada de construir objetos de tipo AutomataConPila
// Referencias:
// Enlaces de interés
// https://refactoring.guru/es/design-patterns/builder
// Historial de revisiones

#ifndef AUTOMATA_CON_PILA_BUILDER_H
#define AUTOMATA_CON_PILA_BUILDER_H

#include "AutomataConPila.h"
#include "Estado.h"
#include "Pila.h"
#include "Tipos.h"
#include "Transicion.h"

#include <fstream>
#include <map>
#include <string>
#include <limits>

class AutomataConPilaBuilder {
 public:
  // constructor encargado de inicializar el proceso de construcción del autómata
  //  virtual AutomataConPilaBuilder() = 0;

  // construir desde archivo
  AutomataConPila construirDesdeArchivo(const std::string& nombre_fichero);

  // vamos a asumir que solo podemos construir desde archivos
  // construirEstados();

 protected:
  // método propio de la construcción por archivo que se salta cualquier cantidad de comentarios del mismo
  inline void saltarComentarios(std::ifstream& inf) {
    while (inf.peek() == '#') inf.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  
  // ========================== métodos de alfabeto ========================== 

  // comprueba que el símbolo de la cadena pertenezca al alfabeto de la cadena y si no lanza una excepción
  inline void comprobar_pertencia_simbolocadena_alfabeto(const SimboloCadena& simbolo_comprobar) {
    if (!alfabeto_cadena_.count(simbolo_comprobar))
      throw std::runtime_error("Error: el símbolo específicado en una de las transiciones no existe en el alfabeto: " + simbolo_comprobar);
  } 

  inline void comprobar_pertencia_simbolopila_alfabeto(const SimboloPila& simbolo_comprobar) {
    if (!alfabeto_pila_.count(simbolo_comprobar)) 
      throw std::runtime_error("Error: El simbolo de la pila no corresponde con ningún simbolo del alfabeto de la pila:" + simbolo_comprobar);
  }

  // método que construye el alfabeto que se utilizará para validar durante la construcción de las transiciones 
  std::set<SimboloCadena> construirAlfabeto( std::ifstream& inf, bool alfabeto_de_cinta = true);

  // ========================== métodos de estados ========================== 
  
  // método que construye el atributo estados_ del AP
  std::map<identificadorEstado, Estado*> construirEstados(std::ifstream& inf);

  // método que construye el atributo estado_actual con su valor inicial
  identificadorEstado construirEstadoInicial(std::ifstream& inf);

  // método que construye la lista de estados finales
  std::vector<identificadorEstado> construirListaEstadosFinales(std::ifstream& inf);

  // método que comprueba que un estado especificado en alguna de las reglas exista
  Estado* comprobarExistenciaEstado(const std::map<identificadorEstado, Estado*>& estados, const identificadorEstado& identificador_estado);
  
  // sobrecarga de método que llama a la comprobación invidiual de una lista de identificadores
  std::vector<Estado*> comprobarExistenciaEstado(const std::map<identificadorEstado, Estado*>& estados, const std::vector<identificadorEstado>& identificadores_estados);


  // ========================== métodos de transiciones ========================== 

  // método que añade las transiciones para un estado directamente desde el archivo
  void construirTransiciones(std::ifstream& inf, std::map<identificadorEstado, Estado*> estados);


  // ========================== métodos de pila ========================== 

  //Pila construirPila(std::ifstream& inf);
  
  // método que construye el primer símbolo de la pila (obligatorio para el comienzo del funcionamiento)
  SimboloPila construirSimboloInicialPila(std::ifstream& inf);

 private:
  std::set<SimboloCadena> alfabeto_cadena_;
  std::set<SimboloPila> alfabeto_pila_;
};

#endif