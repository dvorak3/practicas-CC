// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 1: Programación de Autómata con Pila
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 25/09/2026
// Archivo: AutomataConPila.cc
// Contiene la implementación de la clase AutomataConPila.
// Referencias:
// Enlaces de interés:
// https://en.wikipedia.org/wiki/Standard_Template_Library#Containers
// Historial de revisiones:

#include "../include/AutomataConPila.h"

#include <stack>

/**
* @brief método que comienza la ejecución de todo el Autómata
* @param std::vector<SimboloCadena>  un vector de simbolos de la cadena a procesar
* @return si la cadena es aceptada o no 
*/ 
/**
bool AutomataConPila::leerCadena(const std::vector<SimboloCadena>& cadena,
                                 std::ostream* salida_traza) {
  std::cout << pila_.cima() << std::endl;
  // 0. Necesitaremos un sitio dónde guardar todas nuestras posibles transiciones pendientes → las guardamos en una pila
  std::stack<Configuracion> configuraciones;

  // 1. iremos consumiendo cada Simbolo de la Cadena hasta llegar al final de una rama
  for (size_t i{0}; i < cadena.size();) {
    // 2. obtenemos las posibles transiciones actuales para la configuración actual
    std::vector<ResultadoTransicion> posibles_transiciones = estado_actual_->obtenerTransiciones({cadena[i], pila_.cima()});

    // ======= si no hemos obtenido transiciones debemos comprobar si nuestro Estado actual es uno de ACEPTACIÓN =======
    if (posibles_transiciones.size() == 0 && estados_finales_.count(estado_actual_)) return 1;
    // =================================================================================================================

    // 4. de lo contrario... nuestro método de ordenación de las transiciones es alfabético / númerico, así que... 
    //    como vamos a transicionar en profundidad, sacaremos la primera transición y el resto las guardaremos
    for (size_t j{1}; j < posibles_transiciones.size(); ++j)
      configuraciones.push_back(Configuracion{posibles_transiciones[j].proximo_estado_, i, pila_});

    // 5. transicionamos
    estado_actual_ = posibles_transiciones[0].proximo_estado_;
    pila_.apilar(std::vector<SimboloPila>(cadena.begin() + i, cadena.end()));
    // si nuestra transición 
  }
  return true;
}
*/

/**
* @brief método que comienza la ejecución de todo el Autómata
* @param std::vector<SimboloCadena>  un vector de simbolos de la cadena a procesar
* @return si la cadena es aceptada o no 
*/ 
bool AutomataConPila::leerCadena(const std::vector<SimboloCadena>& cadena,
                                 std::ostream* salida_traza) {
  // reiniciamos para una nueva cadena
  estado_actual_ = estado_inicial_;
  pila_ = Pila(simbolo_inicial_pila_);
  // -----
  // 0. Necesitaremos un sitio dónde guardar todas nuestras posibles transiciones pendientes → las guardamos en una pila
  // aquí guardamos, para esta configuración X existe esta posible salida
  std::stack<ConfiguracionPendiente> configuraciones_pendientes_;
  size_t                             posicion_sobre_cadena{0};
  size_t                             numero_paso{0};

  do {
    // ================= CONDICIÓN DE PARADA ====================
    if (posicion_sobre_cadena == cadena.size() && estados_finales_.count(estado_actual_)) {
      if (salida_traza != nullptr) {
        *salida_traza << "Paso " << numero_paso << ": estado " << estado_actual_->identificador()
                      << ", entrada restante: ., pila (cima primero): " << pila_
                      << " -> ACEPTADA\n";
      }
      return true;
    }
    // ==========================================================

    // metemos las transiciones iniciales VACIAS luego las NO VACIAS
    std::vector<SimboloCadena> simbolos_a_consumir{SIMBOLO_CADENA_VACIO};
    if(posicion_sobre_cadena != cadena.size()) simbolos_a_consumir.push_back(cadena[posicion_sobre_cadena]); 

    if (salida_traza != nullptr) {
      const std::string entrada_restante(cadena.begin() + posicion_sobre_cadena, cadena.end());
      *salida_traza << "Paso " << numero_paso << ": estado " << estado_actual_->identificador()
                    << ", entrada restante: "
                    << (entrada_restante.empty() ? "." : entrada_restante)
                    << ", pila (cima primero): " << pila_ << '\n'
                    << "  Transiciones posibles:\n";
    }

    bool hay_transiciones = false;

    for (size_t j{0}; j < simbolos_a_consumir.size(); ++j) {
      std::vector<ResultadoTransicion> posibles_transiciones;
      if (!pila_.estaVacia()) posibles_transiciones = estado_actual_->obtenerTransiciones(EntradaTransicion{simbolos_a_consumir[j], pila_.cima()});
      for (const auto& transicion : posibles_transiciones) {
        hay_transiciones = true;
        if (salida_traza != nullptr) {
          *salida_traza << "    δ(" << estado_actual_->identificador() << ", "
                        << (simbolos_a_consumir[j] == SIMBOLO_CADENA_VACIO
                                ? "." : std::string(1, simbolos_a_consumir[j]))
                        << ", " << pila_.cima() << ") -> ("
                        << transicion.proximo_estado_->identificador() << ", ";
          if (transicion.cadena_a_escribir_en_pila_.empty()) {
            *salida_traza << '.';
          } else {
            for (SimboloPila simbolo : transicion.cadena_a_escribir_en_pila_) {
              *salida_traza << simbolo;
            }
          }
          *salida_traza << ")\n";
        }
      }
      for (size_t i{posibles_transiciones.size()}; i > 0; --i) {
        Estado* proximo_estado =  posibles_transiciones[i - 1].proximo_estado_;
        Pila    pila_ya_escrita = pila_;
        if (!pila_ya_escrita.estaVacia()) pila_ya_escrita.desapilar();
        pila_ya_escrita.apilar(posibles_transiciones[i - 1].cadena_a_escribir_en_pila_);
        
        configuraciones_pendientes_.push(ConfiguracionPendiente{proximo_estado, posicion_sobre_cadena + j, pila_ya_escrita});
      }
    }

    if (salida_traza != nullptr && !hay_transiciones) {
      *salida_traza << "    (ninguna)\n";
    }

    // Si no hay sucesoras ni alternativas, la ejecución ha fallado.
    if (configuraciones_pendientes_.empty()) {
      if (salida_traza != nullptr) *salida_traza << "  Rama sin continuación -> RECHAZADA\n";
      return false;
    }

    // ahora transicionamos a la transición en lo alto de la cima
    ConfiguracionPendiente nueva_configuracion = configuraciones_pendientes_.top();
    estado_actual_        = nueva_configuracion.estado;
    posicion_sobre_cadena = nueva_configuracion.posicion_cadena; 
    pila_                 = nueva_configuracion.pila;

    configuraciones_pendientes_.pop();
    ++numero_paso;


  } while(true);
}