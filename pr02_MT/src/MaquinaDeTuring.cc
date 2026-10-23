// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 2: Programar un simulador de una máquina de Turing
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 16/10/2026
// Archivo: MaquinaDeTuring.cc
// Contiene la implementación de la clase MaquinaDeTuring, que ejecuta las
// transiciones y genera el resultado de cada simulación.
// Referencias:
// Enlaces de interés:
// Historial de revisiones:

#include "../include/MaquinaDeTuring.h"
#include "../include/Cinta.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <utility>

namespace {

/**
 * @brief Convierte las cintas a sus representaciones textuales finales.
 * @param cintas Cintas de la configuración actual.
 * @return Una cadena representativa por cada cinta.
 */
std::vector<std::string> serializarCintas(const std::vector<Cinta>& cintas) {
  std::vector<std::string> salida;
  salida.reserve(cintas.size());
  for (const Cinta& cinta : cintas) {
    std::ostringstream flujo;
    flujo << cinta;
    salida.push_back(flujo.str());
  }
  return salida;
}

/**
 * @brief Imprime el número de paso, estado actual y contenido de las cintas.
 * @param salida Flujo donde se escribirá la configuración.
 * @param paso Número de transición ejecutada hasta el momento.
 * @param estado Identificador del estado actual.
 * @param cintas Cintas de la configuración actual.
 */
void imprimirConfiguracion(std::ostream& salida,
                           std::size_t paso,
                           const IdentificadorEstado& estado,
                           const std::vector<Cinta>& cintas) {
  salida << "Paso " << paso << " - estado " << estado << '\n';
  for (std::size_t i = 0; i < cintas.size(); ++i) {
    salida << "  Cinta " << (i + 1) << ": " << cintas[i] << '\n';
  }
}

}  // namespace

/**
 * @brief Construye una máquina de Turing con su definición formal.
 * @param estados Conjunto de identificadores de estado.
 * @param alfabeto_entrada Símbolos permitidos como entrada.
 * @param alfabeto_cinta Símbolos permitidos en las cintas, incluido el blanco.
 * @param estado_inicial Identificador del estado inicial.
 * @param estados_finales Estados en los que una parada es aceptación.
 * @param numero_cintas Número de cintas de la máquina.
 */
MaquinaDeTuring::MaquinaDeTuring(
    std::set<IdentificadorEstado> estados,
    std::set<Simbolo> alfabeto_entrada,
    std::set<Simbolo> alfabeto_cinta,
    IdentificadorEstado estado_inicial,
    std::set<IdentificadorEstado> estados_finales,
    std::size_t numero_cintas)
    : alfabeto_entrada_(std::move(alfabeto_entrada)),
      alfabeto_cinta_(std::move(alfabeto_cinta)),
      estado_inicial_(std::move(estado_inicial)),
      estados_finales_(std::move(estados_finales)),
      numero_cintas_(numero_cintas) {
  for (const auto& identificador : estados) {
    estados_.emplace(std::piecewise_construct,
                     std::forward_as_tuple(identificador),
                     std::forward_as_tuple(identificador));
  }
}

/**
 * @brief Registra una transición en el estado de origen indicado.
 * @param origen Identificador del estado desde el que parte la transición.
 * @param lectura Símbolos que deben estar bajo las cabezas para activarla.
 * @param transicion Nuevo estado, escritura y movimiento de las cintas.
 * @throws std::runtime_error Si el estado de origen no pertenece a la máquina
 * o ya existe una transición para esa lectura.
 */
void MaquinaDeTuring::anadirTransicion(
    const IdentificadorEstado& origen,
    const std::vector<Simbolo>& lectura,
    Transicion transicion) {
  const auto estado = estados_.find(origen);
  if (estado == estados_.end()) {
    throw std::runtime_error("Estado de origen desconocido: " + origen + ".");
  }
  estado->second.anadirTransicion(lectura, std::move(transicion));
}

/**
 * @brief Simula la máquina hasta que no exista una transición aplicable.
 * @param entrada Cadena que se coloca inicialmente en la primera cinta.
 * @param mostrar_traza Indica si se debe imprimir cada configuración.
 * @return Resultado con aceptación, pasos y contenido final de las cintas.
 * @throws std::runtime_error Si la entrada contiene un símbolo ajeno a Sigma.
 */
ResultadoEjecucion MaquinaDeTuring::ejecutar(const std::string& entrada,
                                              bool mostrar_traza) const {
  for (const Simbolo simbolo : entrada) {
    if (alfabeto_entrada_.count(simbolo) == 0) {
      throw std::runtime_error(std::string("Simbolo de entrada no permitido: '") +
                               simbolo + "'.");
    }
  }

  std::vector<Cinta> cintas(numero_cintas_);
  cintas.front().cargarEntrada(entrada);

  IdentificadorEstado estado_actual = estado_inicial_;
  std::size_t pasos = 0;
  if (mostrar_traza) imprimirConfiguracion(std::cout, pasos, estado_actual, cintas);

  while (true) {
    const Estado& estado = estados_.at(estado_actual);
    std::vector<Simbolo> lectura;
    lectura.reserve(cintas.size());
    for (const Cinta& cinta : cintas) lectura.push_back(cinta.leer());

    const Transicion* transicion = estado.buscarTransicion(lectura);
    if (transicion == nullptr) break;

    for (std::size_t i = 0; i < cintas.size(); ++i) {
      cintas[i].escribir(transicion->simbolos_escritos[i]);
    }
    estado_actual = transicion->estado_destino;
    for (std::size_t i = 0; i < cintas.size(); ++i) {
      cintas[i].mover(transicion->movimientos[i]);
    }
    ++pasos;
    if (mostrar_traza) imprimirConfiguracion(std::cout, pasos, estado_actual, cintas);
  }

  return ResultadoEjecucion{estados_finales_.count(estado_actual) != 0,
                            pasos,
                            serializarCintas(cintas)};
}
