// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 2: Programar un simulador de una máquina de Turing
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 16/10/2026
// Archivo: MTBuilder.cc
// Contiene la implementación de MTBuilder, que construye y valida la máquina,
// y de MTBuilderDesdeArchivo, que lee la configuración desde un fichero.
// Referencias:
// Enlaces de interés:
// https://refactoring.guru/design-patterns/builder
// Historial de revisiones:

#include "../include/MTBuilder.h"

#include <sstream>
#include <stdexcept>
#include <utility>

/**
 * @brief Obtiene la siguiente línea no vacía de la fuente concreta.
 * @param campo Nombre del campo que debe leerse.
 * @return Tokens del campo requerido.
 * @throws std::runtime_error Si no quedan líneas disponibles.
 */
std::vector<std::string> MTBuilder::lineaObligatoria(
    const std::string& campo) {
  std::vector<std::string> tokens = leerTokens();
  if (tokens.empty()) {
    throw std::runtime_error("Falta " + campo + " en la configuracion.");
  }
  return tokens;
}

/**
 * @brief Valida los tokens de un alfabeto y los convierte a símbolos.
 * @param tokens Tokens de símbolos leídos de la definición.
 * @param campo Nombre del alfabeto usado en los mensajes de error.
 * @return Conjunto de símbolos únicos.
 * @throws std::runtime_error Si un token no es un símbolo individual o está
 * duplicado.
 */
std::set<Simbolo> MTBuilder::leerAlfabeto(
    const std::vector<std::string>& tokens, const std::string& campo) {
  std::set<Simbolo> alfabeto;
  for (const std::string& token : tokens) {
    if (token.size() != 1) {
      throw std::runtime_error("Cada simbolo de " + campo +
                               " debe ocupar un unico caracter: " + token);
    }
    if (!alfabeto.insert(token[0]).second) {
      throw std::runtime_error("Elemento repetido en " + campo + ": " + token);
    }
  }
  return alfabeto;
}

/**
 * @brief Valida una lista de estados y la convierte en un conjunto.
 * @param tokens Identificadores de estado.
 * @return Conjunto de estados únicos.
 * @throws std::runtime_error Si hay identificadores repetidos.
 */
std::set<IdentificadorEstado> MTBuilder::leerEstados(
    const std::vector<std::string>& tokens) {
  std::set<IdentificadorEstado> estados(tokens.begin(), tokens.end());
  if (estados.size() != tokens.size()) {
    throw std::runtime_error("Hay identificadores de estado repetidos.");
  }
  return estados;
}

/**
 * @brief Traduce la notación textual de movimiento al tipo enumerado.
 * @param token Movimiento leído del fichero, `L`, `R` o `S`.
 * @return Valor enumerado asociado al movimiento.
 * @throws std::runtime_error Si el token no representa un movimiento válido.
 */
Movimiento MTBuilder::leerMovimiento(const std::string& token) {
  if (token == "L") return Movimiento::L;
  if (token == "R") return Movimiento::R;
  if (token == "S") return Movimiento::S;
  throw std::runtime_error("Movimiento no valido: " + token);
}

/**
 * @brief Valida que una línea contenga un único identificador.
 * @param tokens Tokens que forman la línea.
 * @param campo Nombre del campo para el mensaje de error.
 * @return Identificador de la línea.
 * @throws std::runtime_error Si no hay exactamente un token.
 */
std::string MTBuilder::leerIdentificador(
    const std::vector<std::string>& tokens, const std::string& campo) {
  if (tokens.size() != 1) {
    throw std::runtime_error(campo + " debe contener exactamente un valor.");
  }
  return tokens.front();
}

/**
 * @brief Convierte un token de un solo carácter en un símbolo.
 * @param token Texto que representa el símbolo.
 * @param campo Nombre descriptivo del campo para el mensaje de error.
 * @return Símbolo correspondiente al token.
 * @throws std::runtime_error Si el token no contiene un único carácter.
 */
Simbolo MTBuilder::leerSimbolo(const std::string& token,
                               const std::string& campo) {
  if (token.size() != 1) {
    throw std::runtime_error(campo + " debe ser un unico caracter.");
  }
  return token[0];
}

/**
 * @brief Comprueba que un identificador pertenezca al conjunto de estados.
 * @param estados Estados declarados en la definición.
 * @param identificador Estado que se quiere validar.
 * @param campo Campo de la definición al que pertenece el estado.
 * @throws std::runtime_error Si el identificador no pertenece a Q.
 */
void MTBuilder::validarEstado(
    const std::set<IdentificadorEstado>& estados,
    const IdentificadorEstado& identificador,
    const std::string& campo) {
  if (estados.count(identificador) == 0) {
    throw std::runtime_error(campo + " no pertenece a Q: " + identificador);
  }
}

/**
 * @brief Construye y valida una máquina leyendo la definición desde la fuente.
 * @return Máquina de Turing con estados y transiciones ya cargados.
 * @throws std::runtime_error Si algún elemento de la definición incumple el
 * formato o las restricciones de una MT determinista.
 */
MaquinaDeTuring MTBuilder::construirMT() {
  const auto estados = leerEstados(lineaObligatoria("Q"));
  if (estados.empty()) throw std::runtime_error("Q no puede estar vacio.");

  const auto alfabeto_entrada =
      leerAlfabeto(lineaObligatoria("el alfabeto de entrada"),
                   "el alfabeto de entrada");
  const auto alfabeto_cinta =
      leerAlfabeto(lineaObligatoria("el alfabeto de cinta"),
                   "el alfabeto de cinta");
  if (alfabeto_entrada.empty() || alfabeto_cinta.empty()) {
    throw std::runtime_error("Los alfabetos no pueden estar vacios.");
  }
  if (alfabeto_cinta.count(SIMBOLO_BLANCO) == 0) {
    throw std::runtime_error("El alfabeto de cinta debe incluir el blanco '.'.");
  }
  if (alfabeto_entrada.count(SIMBOLO_BLANCO) != 0) {
    throw std::runtime_error(
        "El blanco no puede pertenecer al alfabeto de entrada.");
  }
  for (const Simbolo simbolo : alfabeto_entrada) {
    if (alfabeto_cinta.count(simbolo) == 0) {
      throw std::runtime_error("El simbolo de entrada no pertenece al alfabeto "
                               "de cinta.");
    }
  }

  const std::string inicial =
      leerIdentificador(lineaObligatoria("el estado inicial"),
                        "El estado inicial");
  validarEstado(estados, inicial, "El estado inicial");

  const auto blanco_tokens = lineaObligatoria("el simbolo blanco");
  if (blanco_tokens.size() != 1 ||
      leerSimbolo(blanco_tokens.front(), "El simbolo blanco") !=
          SIMBOLO_BLANCO) {
    throw std::runtime_error("Esta implementacion utiliza '.' como blanco.");
  }

  const auto finales = leerEstados(lineaObligatoria("los estados finales"));
  for (const auto& final : finales) {
    validarEstado(estados, final, "El estado final");
  }

  const auto cintas_tokens = lineaObligatoria("el numero de cintas");
  if (cintas_tokens.size() != 1) {
    throw std::runtime_error("El numero de cintas debe ser un entero.");
  }
  std::size_t numero_cintas = 0;
  try {
    std::size_t consumidos = 0;
    numero_cintas = std::stoul(cintas_tokens.front(), &consumidos);
    if (consumidos != cintas_tokens.front().size() || numero_cintas == 0) {
      throw std::runtime_error("El numero de cintas debe ser positivo.");
    }
  } catch (const std::invalid_argument&) {
    throw std::runtime_error("El numero de cintas no es valido.");
  } catch (const std::out_of_range&) {
    throw std::runtime_error("El numero de cintas es demasiado grande.");
  }

  MaquinaDeTuring maquina(estados, alfabeto_entrada, alfabeto_cinta, inicial,
                          finales, numero_cintas);
  while (true) {
    const auto tokens = leerTokens();
    if (tokens.empty()) break;
    const std::size_t esperados = 2 + 3 * numero_cintas;
    if (tokens.size() != esperados) {
      throw std::runtime_error(
          "Transicion mal formada en la linea " +
          std::to_string(numeroLineaActual()) + ": se esperaban " +
          std::to_string(esperados) + " elementos.");
    }

    const std::string& origen = tokens[0];
    validarEstado(estados, origen, "El estado de origen");
    std::vector<Simbolo> lectura;
    lectura.reserve(numero_cintas);
    for (std::size_t i = 0; i < numero_cintas; ++i) {
      const Simbolo simbolo =
          leerSimbolo(tokens[1 + i], "El simbolo leido en la transicion");
      if (alfabeto_cinta.count(simbolo) == 0) {
        throw std::runtime_error("Simbolo leido fuera de Gamma: " +
                                 tokens[1 + i]);
      }
      lectura.push_back(simbolo);
    }

    const std::size_t indice_destino = 1 + numero_cintas;
    const std::string& destino = tokens[indice_destino];
    validarEstado(estados, destino, "El estado de destino");

    std::vector<Simbolo> escritura;
    escritura.reserve(numero_cintas);
    for (std::size_t i = 0; i < numero_cintas; ++i) {
      const Simbolo simbolo =
          leerSimbolo(tokens[indice_destino + 1 + i],
                      "El simbolo escrito en la transicion");
      if (alfabeto_cinta.count(simbolo) == 0) {
        throw std::runtime_error("Simbolo escrito fuera de Gamma: " +
                                 tokens[indice_destino + 1 + i]);
      }
      escritura.push_back(simbolo);
    }

    std::vector<Movimiento> movimientos;
    movimientos.reserve(numero_cintas);
    for (std::size_t i = 0; i < numero_cintas; ++i) {
      const std::string& movimiento =
          tokens[indice_destino + 1 + numero_cintas + i];
      movimientos.push_back(leerMovimiento(movimiento));
    }

    maquina.anadirTransicion(
        origen, lectura, Transicion{destino, std::move(escritura),
                                    std::move(movimientos)});
  }
  return maquina;
}

/**
 * @brief Abre el fichero que contiene la definición de la máquina.
 * @param nombre_fichero Ruta del fichero de configuración.
 * @throws std::runtime_error Si el fichero no se puede abrir.
 */
MTBuilderDesdeArchivo::MTBuilderDesdeArchivo(
    const std::string& nombre_fichero)
    : archivo_(nombre_fichero) {
  if (!archivo_) {
    throw std::runtime_error("No se pudo abrir la configuracion: " +
                             nombre_fichero);
  }
}

/**
 * @brief Lee la siguiente línea útil del fichero y la divide en tokens.
 * @return Tokens de la línea; un vector vacío indica el final del fichero.
 * @throws std::runtime_error Si se produce un error de lectura.
 *
 * Se eliminan los comentarios iniciados por `#`, los espacios separadores y
 * las líneas que quedan vacías.
 */
std::vector<std::string> MTBuilderDesdeArchivo::leerTokens() {
  std::string linea;
  while (std::getline(archivo_, linea)) {
    ++numero_linea_;
    if (!linea.empty() && linea.back() == '\r') linea.pop_back();
    const std::size_t comentario = linea.find('#');
    if (comentario != std::string::npos) linea.erase(comentario);

    std::istringstream flujo(linea);
    std::vector<std::string> tokens;
    std::string token;
    while (flujo >> token) tokens.push_back(token);
    if (!tokens.empty()) return tokens;
  }
  if (archivo_.bad()) {
    throw std::runtime_error("Error al leer el fichero de configuracion.");
  }
  return {};
}
