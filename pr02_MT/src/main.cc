// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Complejidad Computacional
// Curso: 4º
// Práctica 2: Programar un simulador de una máquina de Turing
// Autor: Iván Hernández Rodríguez
// Correo: alu0101562100@ull.edu.es
// Fecha: 16/10/2026
// Archivo: main.cc
// Contiene el punto de entrada del programa, analiza las opciones de
// ejecución y coordina la simulación de las entradas.
// Referencias:
// Enlaces de interés:
// Historial de revisiones:

#include "../include/MTBuilder.h"

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace {

/**
 * @brief Agrupa las opciones de ejecución de la línea de comandos.
 */
struct Opciones {
  std::string configuracion;  ///< Ruta de la definición de la máquina.
  std::string entradas;  ///< Ruta opcional del fichero de entradas.
  bool configuracion_indicada{false};  ///< Indica si se recibió `-config`.
  bool entradas_indicadas{false};  ///< Indica si se recibió `-in`.
  bool traza{false};  ///< Activa la impresión de configuraciones intermedias.
};

/**
 * @brief Muestra la sintaxis y las opciones disponibles del programa.
 * @param programa Nombre del ejecutable proporcionado por el sistema.
 */
void imprimirAyuda(const char* programa) {
  std::cout << "Uso: " << programa << " -config <fichero> [-in <fichero>] "
               "[-trace <y|n>]\n"
            << "  -config <fichero>  Definicion de la maquina de Turing\n"
            << "  -in <fichero>      Entradas, una por linea; si se omite, se "
               "leen por teclado\n"
            << "  -trace <y|n>       Mostrar la traza (por defecto: n)\n"
            << "Una linea con '.' representa la entrada vacia.\n";
}

/**
 * @brief Analiza y valida los argumentos de la línea de comandos.
 * @param argc Número de argumentos.
 * @param argv Valores de los argumentos.
 * @return Opciones interpretadas para la ejecución.
 * @throws std::runtime_error Si hay opciones desconocidas, incompletas o
 * incompatibles con el formato esperado.
 */
Opciones analizarArgumentos(int argc, char* argv[]) {
  Opciones opciones;
  for (int i = 1; i < argc; ++i) {
    const std::string opcion = argv[i];
    if (opcion == "-h" || opcion == "--help") {
      imprimirAyuda(argv[0]);
      std::exit(0);
    }
    if (opcion != "-config" && opcion != "-in" && opcion != "-trace") {
      throw std::runtime_error("Opcion desconocida: " + opcion);
    }
    if (i + 1 >= argc) {
      throw std::runtime_error("Falta el valor de " + opcion + ".");
    }
    const std::string valor = argv[++i];
    if (valor.empty()) throw std::runtime_error("Valor vacio para " + opcion + ".");

    if (opcion == "-config") {
      if (opciones.configuracion_indicada) {
        throw std::runtime_error("La opcion -config esta repetida.");
      }
      opciones.configuracion = valor;
      opciones.configuracion_indicada = true;
    } else if (opcion == "-in") {
      if (opciones.entradas_indicadas) {
        throw std::runtime_error("La opcion -in esta repetida.");
      }
      opciones.entradas = valor;
      opciones.entradas_indicadas = true;
    } else {
      if (valor != "y" && valor != "n") {
        throw std::runtime_error("-trace debe recibir 'y' o 'n'.");
      }
      opciones.traza = valor == "y";
    }
  }
  if (!opciones.configuracion_indicada) {
    throw std::runtime_error("La opcion -config es obligatoria.");
  }
  return opciones;
}

/**
 * @brief Normaliza una línea de entrada y decodifica el marcador de vacío.
 * @param entrada Línea leída del fichero o del teclado.
 * @return La entrada sin CR final; cadena vacía si la línea era `.`.
 */
std::string normalizarEntrada(std::string entrada) {
  if (!entrada.empty() && entrada.back() == '\r') entrada.pop_back();
  if (entrada == ".") return {};
  return entrada;
}

/**
 * @brief Ejecuta una entrada e imprime aceptación y cintas finales.
 * @param maquina Máquina de Turing que procesará la entrada.
 * @param entrada Cadena que se quiere simular.
 * @param numero Número ordinal de la entrada para la salida.
 * @param traza Indica si se imprimen las configuraciones intermedias.
 * @throws std::runtime_error Si la máquina no puede procesar la entrada.
 */
void ejecutarEntrada(const MaquinaDeTuring& maquina,
                     const std::string& entrada,
                     std::size_t numero,
                     bool traza) {
  const ResultadoEjecucion resultado = maquina.ejecutar(entrada, traza);
  std::cout << "Entrada " << numero << ": "
            << (entrada.empty() ? "." : entrada) << '\n'
            << "Resultado: " << (resultado.aceptada ? "ACEPTADA" : "RECHAZADA")
            << " (parada por ausencia de transicion, " << resultado.pasos
            << " pasos)\n";
  for (std::size_t i = 0; i < resultado.cintas.size(); ++i) {
    std::cout << "Cinta " << (i + 1) << ": " << resultado.cintas[i] << '\n';
  }
}

/**
 * @brief Lee y ejecuta las entradas almacenadas en un fichero.
 * @param maquina Máquina que se utilizará para procesar las cadenas.
 * @param opciones Opciones con la ruta del fichero y el estado de la traza.
 * @throws std::runtime_error Si no se puede abrir o leer el fichero, o si no
 * contiene entradas.
 */
void ejecutarDesdeArchivo(const MaquinaDeTuring& maquina,
                          const Opciones& opciones) {
  std::ifstream archivo(opciones.entradas);
  if (!archivo) {
    throw std::runtime_error("No se pudo abrir el fichero de entradas: " +
                             opciones.entradas);
  }
  std::string linea;
  std::size_t numero = 0;
  while (std::getline(archivo, linea)) {
    if (!linea.empty() && linea.back() == '\r') linea.pop_back();
    if (!linea.empty() && linea.front() == '#') continue;
    ejecutarEntrada(maquina, normalizarEntrada(linea), ++numero, opciones.traza);
  }
  if (archivo.bad()) {
    throw std::runtime_error("Error al leer el fichero de entradas: " +
                             opciones.entradas);
  }
  if (numero == 0) {
    throw std::runtime_error("El fichero no contiene entradas.");
  }
}

/**
 * @brief Lee entradas desde la entrada estándar hasta alcanzar EOF.
 * @param maquina Máquina que procesará cada cadena introducida.
 * @param traza Indica si se imprimen las configuraciones intermedias.
 * @throws std::runtime_error Si una entrada contiene símbolos inválidos.
 */
void ejecutarDesdeTeclado(const MaquinaDeTuring& maquina, bool traza) {
  std::cout << "Introduce una entrada por linea (Ctrl+D para terminar; '.' "
               "representa la cadena vacia).\n";
  std::string linea;
  std::size_t numero = 0;
  while (std::getline(std::cin, linea)) {
    ejecutarEntrada(maquina, normalizarEntrada(linea), ++numero, traza);
  }
}

}  // namespace

/**
 * @brief Punto de entrada: construye la máquina y coordina su ejecución.
 * @param argc Número de argumentos de la línea de comandos.
 * @param argv Valores de los argumentos.
 * @return Código de salida cero si la ejecución termina correctamente; dos si
 * se produce un error de argumentos, configuración o entrada.
 */
int main(int argc, char* argv[]) {
  try {
    const Opciones opciones = analizarArgumentos(argc, argv);
    const MaquinaDeTuring maquina =
        MTBuilderDesdeArchivo(opciones.configuracion).construirMT();
    if (opciones.entradas_indicadas) {
      ejecutarDesdeArchivo(maquina, opciones);
    } else {
      ejecutarDesdeTeclado(maquina, opciones.traza);
    }
  } catch (const std::exception& error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 2;
  }
  return 0;
}
