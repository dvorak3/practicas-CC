// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 1: Programar un simulador de un autómata con pila
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 25/09/2026
// Archivo AutomataConPilaBuilder.cc
// Contiene la implementación de la clase AutomataConPilaBuilder,
// encargada de construir objetos de tipo AutomataConPila
// Referencias:
// Enlaces de interés
// https://refactoring.guru/es/design-patterns/builder
// Historial de revisiones

#include "../include/AutomataConPilaBuilder.h"

#include <fstream> // para la lectura de archivos
#include <iostream>
#include <memory>
#include <sstream> // para tener un buffer extra
#include <limits>

//**
// void AutomataConPilaBuilder::saltarComentarios() {
// }
//  */

std::vector<std::vector<SimboloCadena>> AutomataConPilaBuilder::leerCadenasDesdeArchivo(
    const std::string& nombre_fichero) const {
  std::ifstream inf{nombre_fichero};
  if (!inf) {
    throw std::runtime_error("Error: No se pudo abrir el fichero de cadenas: " + nombre_fichero);
  }

  std::vector<std::vector<SimboloCadena>> cadenas;
  std::string linea;
  while (std::getline(inf, linea)) {
    if (!linea.empty() && linea.back() == '\r') linea.pop_back();
    if (linea.empty()) continue;
    if (linea == ".") {
      cadenas.emplace_back();
    } else {
      cadenas.emplace_back(linea.begin(), linea.end());
    }
  }

  if (inf.bad()) {
    throw std::runtime_error("Error: Fallo al leer el fichero de cadenas: " + nombre_fichero);
  }
  return cadenas;
}

std::map<identificadorEstado, Estado*> AutomataConPilaBuilder::construirEstados() {
  // ahora se supone que empezamos a leer estados
  std::map<identificadorEstado, Estado*> estados;

  try {
    std::string linea;
    std::getline(inf_, linea);
        
    // comprobamos que hayamos leído algo
    if (linea.empty()) throw std::runtime_error("Error: No se han definido estados."); 

    // 2. Metemos la línea dentro de un flujo de texto
    std::stringstream ss(linea); 
    std::string estado;

    // 3. Extraemos los elementos de ESA línea individualmente con >>
    while (ss >> estado) {
      std::cout << "Estado procesado: " << estado << '\n';
      // if (estado[0] != 'q') throw std::runtime_error("Error: no se ha definido un estado con el formato adecuado: " + estado);
      auto nuevo_estado = std::make_unique<Estado>(estado);
      const auto [it, insertado] = estados.emplace(estado, nuevo_estado.get());
      (void)it;
      if (insertado) nuevo_estado.release();
    }
  } catch (...) {
    borrarEstados(estados);
    throw;
  }
  return estados;
}

/**
 * @brief función que construye el aflbeto de la cadena o de la pila y además AGREGA EL SÍMBOLO VACÍO
 * @param bool que indica que si se trata de un alfabeto de cadena (true) o de pila (false) 
 * @return alfabeto para comprobar la construcción del AP
 */
std::set<SimboloCadena> AutomataConPilaBuilder::construirAlfabeto(bool alfabeto_de_cinta) {
  // ahora se supone que empezamos a leer estados
  std::set<SimboloCadena> alfabeto;

  std::string linea;
  std::getline(inf_, linea);
        
  // comprobamos que hayamos leído algo
  if (linea.empty()) throw std::runtime_error("Error: No se ha definido el alfabeto de la cadena."); 

  // 2. Metemos la línea dentro de un flujo de texto
  std::stringstream ss(linea); 
  std::string simbolo;

  // 3. Extraemos los elementos de ESA línea individualmente con >>
  // Este bucle terminará automáticamente cuando se acaben los estados de la línea
  while (ss >> simbolo) {
      if (alfabeto_de_cinta) {
        std::cout << "Símbolo del alfabeto de la cadena procesado: " << simbolo << '\n';
        if (simbolo.size() != 1) // && simbolo[0] != 'q') 
          throw std::runtime_error("Error: no se ha definido un símbolo válido para el alfabeto: " + simbolo);

      } else {
        std::cout << "Símbolo del alfabeto de la pila procesado: " << simbolo << '\n';
        if (simbolo.size() != 1 && ((simbolo[0] >= 'A' && simbolo[0] <= 'Z') || (simbolo[0] >= '1' && simbolo[0] <= '9')))
          throw std::runtime_error("Error: no se ha definido un símbolo válido para el alfabeto: " + simbolo);
      }
      alfabeto.insert(simbolo[0]);
  }
  // INSERTAMOS EL SÍMBOLO VACÍO EN EL ALFABETO
  if (alfabeto_de_cinta) alfabeto.insert(SIMBOLO_CADENA_VACIO);
  else                   alfabeto.insert(SIMBOLO_PILA_VACIO);

  return alfabeto;
}

identificadorEstado AutomataConPilaBuilder::construirEstadoInicial() {
  // ahora toca leer cúal es el estado inicial
  std::string linea;
  std::getline(inf_, linea);
        
  // comprobamos que hayamos leído algo
  if (linea.empty()) throw std::runtime_error("Error: No se ha definido ningún estado inicial."); 

  // 2. Metemos la línea dentro de un flujo de texto
  std::stringstream ss(linea);
  identificadorEstado estado_inicial;
  ss >> estado_inicial;
  std::cout << "Estado inicial procesado: " << estado_inicial << std::endl;

  // 3. Ahora... si leemos algo más significa que ha habido un error y se ha definido algo más.
  std::string basura_temporal;
  if (ss >> basura_temporal) throw std::runtime_error("Error: se han definido más de un estado inicial.");

  return estado_inicial;
}

SimboloPila AutomataConPilaBuilder::construirSimboloInicialPila() {
  // ahora toca leer cúal es el símbolo inicial
  std::string linea;
  std::getline(inf_, linea);
        
  // comprobamos que hayamos leído algo
  if (linea.empty()) throw std::runtime_error("Error: No se ha definido ningún símbolo inicial para la pila."); 

  // 2. Metemos la línea dentro de un flujo de texto
  std::stringstream ss(linea);
  SimboloPila simbolo_inicial;
  ss >> simbolo_inicial;
  std::cout << "Simbolo inicial procesado: " << simbolo_inicial << std::endl;

  // 3. Ahora... si leemos algo más significa que ha habido un error y se ha definido algo más.
  std::string basura_temporal;
  if (ss >> basura_temporal) throw std::runtime_error("Error: se han definido más de un simbolo inicial para la pila.");

  return simbolo_inicial;
}

/**
 * @brief método similar a construirEstados salvo que este NO genera nuevos estados en memoria dinámica, solo devuelve
          la lista con los identificadores de dichos estados que se comprobará por el método comprobarExistenciaEstado
          (en caso de existir este método retornará la dirección de memoria del elemento en memoria dinámica)
  @return vector de identificadores 
 */
std::vector<identificadorEstado> AutomataConPilaBuilder::construirListaEstadosFinales() {
  // ahora se supone que empezamos a leer estados
  std::vector<identificadorEstado> identificadores_estados_finales;
  
  std::string linea;
  std::getline(inf_, linea);
  
  // comprobamos que hayamos leído algo
  if (linea.empty()) throw std::runtime_error("Error: No se han definido estados de aceptación."); 
  
  // 2. Metemos la línea dentro de un flujo de texto
  std::stringstream ss(linea); 
  identificadorEstado identificador_estado_final;
  
  // 3. Extraemos los elementos de ESA línea individualmente con >>
  while (ss >> identificador_estado_final) {
    std::cout << "Estado final procesado: " << identificador_estado_final << '\n';
    /**
    if (identificador_estado_final[0] != 'q')  
      throw std::runtime_error("Error: no se ha definido un estado final con el formato adecuado: " + identificador_estado_final);
    */
    identificadores_estados_finales.push_back(identificador_estado_final);
  }
  return identificadores_estados_finales;
}

/**
 * @brief método que comprueba que el estado específicado existe, y en caso de existir devuelve un puntero que apunta a él
 * @param estados la lista de estados sin modificar
 * @param identificadorEstado el identificador del estado que se quiere comprobar
 * @return lanza un excepción si el archivo de construcción está mal (apunta a un estado inexistente)
           o devuelve un puntero que apunta al estado en cuestión
 */
Estado* AutomataConPilaBuilder::comprobarExistenciaEstado(const std::map<identificadorEstado, Estado*>& estados, const identificadorEstado& identificador_estado) {
  // empezamos creando un puntero
  Estado* estado{nullptr}; 
  // debemos de comprobar que el estado específicado exista
  try {
    estado = estados.at(identificador_estado);
  } catch (const std::out_of_range& e) {
    // si no existe en el map dicho estado no existe -> se lanza un error
    throw std::runtime_error("Error: El estado inicial no se corresponde con ningún estado existente.");
  } 
  return estado;
}

/**
 * @brief comprueba la existencia de múltiples estados llamando al método para 1 solo
 * @param estados la lista de estados sin modificar
 * @param identificadorEstado el identificador del estado que se quiere comprobar
 * @return lanza un excepción si el archivo de construcción está mal (apunta a un estado inexistente)
           o devuelve un puntero que apunta al estado en cuestión
*/
std::vector<Estado*> AutomataConPilaBuilder::comprobarExistenciaEstado(
      const std::map<identificadorEstado, Estado*>& estados, 
      const std::vector<identificadorEstado>& identificadores_estados) {
  std::vector<Estado*> lista_estados;
  for (auto identificador_estado : identificadores_estados)
    lista_estados.push_back(comprobarExistenciaEstado(estados, identificador_estado));
  
  return lista_estados;
}

/**
 * @brief método que añade las transiciones para un estado directamente desde el archivo
 */
void AutomataConPilaBuilder::construirTransiciones(std::map<identificadorEstado, Estado*> estados) {
  std::string linea;
  std::getline(inf_, linea);

  // Imprimimos la construcción
  std::cout << "+------ Procesando transición: " << linea << " -----+" << std::endl;

  // comprobamos que hayamos leído algo
  if (linea.empty()) throw std::runtime_error("Error: No se ha leído ninguna transición");
  
  // 2. Metemos la línea dentro de un flujo de texto
  std::stringstream ss(linea); 

  // 3. Definimos las variables que vamos a leer
  identificadorEstado        identificador_estado_inicial;
  Estado*                    estado_inicial;
  SimboloCadena              simbolo_entrada_cadena;
  SimboloPila                simbolo_entrada_pila;

  identificadorEstado        identificador_estado_final;
  Estado*                    estado_final;
  std::vector<SimboloPila>   cadena_escribir_en_pila;

  // 4. Extraemos los elementos de ESA línea individualmente con >>

  // → q1 -- empezamos leyendo el estado que contendrá la transición
  ss >> identificador_estado_inicial;
  estado_inicial = comprobarExistenciaEstado(estados, identificador_estado_inicial);
  std::cout << "|  Estado procesado: " << identificador_estado_inicial << std::endl;

  // → a -- ahora leyemos el símbolo de entrada de la cadena para poder transitar
  ss >> simbolo_entrada_cadena;
  comprobar_pertencia_simbolocadena_alfabeto(simbolo_entrada_cadena);
  std::cout << "|  Símbolo del alfabeto de la cadena procesado: " << simbolo_entrada_cadena << std::endl;

  // → A -- ahora leyemos el símbolo que debe tener la cima de la pila para poder transitar
  ss >> simbolo_entrada_pila;
  comprobar_pertencia_simbolopila_alfabeto(simbolo_entrada_pila);
  std::cout << "|  Símbolo del alfabeto de la pila procesado: " << simbolo_entrada_pila << std::endl;

  // → q1 -- ahora leemos el estado que final de la transición
  ss >> identificador_estado_final;
  estado_final = comprobarExistenciaEstado(estados, identificador_estado_final);
  std::cout << "|  Estado procesado: " << identificador_estado_final << std::endl;

  // → q1 -- ahora leemos el estado que final de la transición
  SimboloPila temporal;
  while (ss >> temporal) {
    comprobar_pertencia_simbolopila_alfabeto(temporal);
    if(temporal != '.') cadena_escribir_en_pila.push_back(temporal);
    std::cout << "|  Simbolo procesado: " << temporal << std::endl;
  }

  std::cout << "+-----------------------------------------------+" << std::endl;

  // 5. Si queda algo escrito en la pila significa que la transición está mal
  std::string basura_temporal;
  if (ss >> basura_temporal) 
    throw std::runtime_error("Error: se especificado erróneamente una de las transiciones. No debería de contener: " + basura_temporal);

  // 6. Ahora al final podemos introducir la transición en el Estado
  estado_inicial->añadirTransicion(EntradaTransicion{simbolo_entrada_cadena, simbolo_entrada_pila}, ResultadoTransicion{estado_final, cadena_escribir_en_pila});
}

AutomataConPila AutomataConPilaBuilder::construirDesdeArchivo(const std::string& nombre_fichero) {
  inf_.open(nombre_fichero);
  if (!inf_) throw std::runtime_error("Error: No se pudo abrir el archivo: " + nombre_fichero);

  saltarComentarios(); // saltamos posibles comentarios de más

  // 1. Leemos los estados
  std::map<identificadorEstado, Estado*> estados = construirEstados();

  try {
  // ---- ¡¡Hemos obtenido 1 de nuestros elementos del AP!! ---- (estados_)
  
  // 2. ahora toca leer el alfabeto de la cadena y la pila
  saltarComentarios(); // saltamos posibles comentarios de más
  alfabeto_cadena_ = construirAlfabeto();
  saltarComentarios(); // saltamos posibles comentarios de más
  alfabeto_pila_ = construirAlfabeto(false);
  saltarComentarios(); // saltamos posibles comentarios de más

  // ---- estos nos servirán para comprobar que en el resto del archivo no se especifican transiciones NO válidas ----
  
  // 3. Leemos el estado inicial que será estado_actual de nuestro autómata
  identificadorEstado identificador_estado_inicial = construirEstadoInicial();
  Estado* estado_inicial = comprobarExistenciaEstado(estados, identificador_estado_inicial); 
  
  // ---- ¡¡Hemos obtenido 1 de nuestros elementos del AP!! ---- (estado_actual_)
  
  saltarComentarios(); // saltamos posibles comentarios de más

  // 4. Ahora leemos el símbolo inicial de la pila (necesario para que hayan transiciones)
  SimboloPila simbolo_inicial_pila = construirSimboloInicialPila();
  comprobar_pertencia_simbolopila_alfabeto(simbolo_inicial_pila);

  // ---- ¡¡Hemos obtenido 1 de nuestros elementos de nuestra pila!! ---- (valor de entrada para el constructor)

  saltarComentarios(); // saltamos posibles comentarios de más

  // 5. Ahora debemos de leer la lista de Estados de aceptación
  std::vector<identificadorEstado> identificadores_estados_finales = construirListaEstadosFinales();
  // ahora deberíamos de comprobar que dichos estados existen y obtenerlos
  std::vector<Estado*> estados_finales = comprobarExistenciaEstado(estados, identificadores_estados_finales);

  // ---- ¡¡Hemos obtenido 1 de nuestros elementos de nuestra pila!! ---- (estados_finales_)

  saltarComentarios(); // saltamos posibles comentarios de más

  // 6. Ahora debemos de leer la lista de funciones de transición (cada línea será una función de transición)
  while (!inf_.eof() && inf_.peek() != ' ') {
    construirTransiciones(estados);
    saltarComentarios(); // saltamos posibles comentarios de más
  }
  // ---- ¡¡Hemos completado 1 de nuestros elementos del AP!! ---- (transiciones de cada Estado)

  // 7. Ahora ya tenemos todos los elementos necesarios para construir nuestro AP
  std::vector<Estado*> puntero_a_los_estados_sin_id;
  for(auto it = estados.begin(); it != estados.end(); ++it)
    puntero_a_los_estados_sin_id.push_back(it->second);
  return AutomataConPila(puntero_a_los_estados_sin_id, estado_inicial, estados_finales, simbolo_inicial_pila); 
  } catch (...) {
    borrarEstados(estados);
    throw;
  }
}