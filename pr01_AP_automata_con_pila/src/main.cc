#include "../include/AutomataConPilaBuilder.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

struct Opciones {
  std::string fichero_configuracion;
  std::string fichero_cadenas;
  bool traza{false};
  bool config_indicada{false};
  bool entrada_indicada{false};
  bool traza_indicada{false};
};

void imprimirAyuda(const char* programa) {
  std::cout << "Uso: " << programa
            << " -config <fichero> -trace <y|n> -in <fichero>\n"
            << "  -config <fichero>  Fichero con la definición del autómata\n"
            << "  -trace <y|n>       Activar o desactivar la traza (pendiente de implementar)\n"
            << "  -in <fichero>      Fichero con una cadena de entrada por línea\n"
            << "Una línea con '.' representa la cadena vacía.\n";
}

Opciones analizarArgumentos(int argc, char* argv[]) {
  Opciones opciones;

  for (int i = 1; i < argc; ++i) {
    const std::string opcion = argv[i];
    if (opcion == "-h" || opcion == "--help") {
      imprimirAyuda(argv[0]);
      std::exit(0);
    }
    if (opcion == "-out") {
      throw std::runtime_error("La opción -out queda fuera de esta versión.");
    }
    if (opcion != "-config" && opcion != "-trace" && opcion != "-in") {
      throw std::runtime_error("Opción desconocida: " + opcion);
    }
    if (i + 1 >= argc) {
      throw std::runtime_error("Falta el valor para la opción " + opcion + ".");
    }

    const std::string valor = argv[++i];
    if (valor.empty() || valor[0] == '-') {
      throw std::runtime_error("Valor no válido para la opción " + opcion + ".");
    }

    if (opcion == "-config") {
      if (opciones.config_indicada) throw std::runtime_error("La opción -config está repetida.");
      opciones.fichero_configuracion = valor;
      opciones.config_indicada = true;
    } else if (opcion == "-in") {
      if (opciones.entrada_indicada) throw std::runtime_error("La opción -in está repetida.");
      opciones.fichero_cadenas = valor;
      opciones.entrada_indicada = true;
    } else {
      if (opciones.traza_indicada) throw std::runtime_error("La opción -trace está repetida.");
      if (valor != "y" && valor != "n") {
        throw std::runtime_error("-trace debe recibir 'y' o 'n'.");
      }
      opciones.traza = (valor == "y");
      opciones.traza_indicada = true;
    }
  }

  if (!opciones.config_indicada) throw std::runtime_error("Falta la opción obligatoria -config.");
  if (!opciones.traza_indicada) throw std::runtime_error("Falta la opción obligatoria -trace.");
  if (!opciones.entrada_indicada) throw std::runtime_error("Falta la opción -in en esta versión.");
  return opciones;
}

}  // namespace

int main(int argc, char* argv[]) {
  try {
    const Opciones opciones = analizarArgumentos(argc, argv);
    AutomataConPilaBuilder builder;

    // El builder abre y valida internamente el fichero de configuración.
    AutomataConPila automata = builder.construirDesdeArchivo(opciones.fichero_configuracion);
    const auto cadenas = builder.leerCadenasDesdeArchivo(opciones.fichero_cadenas);

    if (cadenas.empty()) {
      throw std::runtime_error("El fichero de entrada no contiene ninguna cadena.");
    }

    if (opciones.traza) {
      std::cerr << "Aviso: -trace y está seleccionado, pero la traza aún no está implementada.\n";
    }

    for (std::size_t i = 0; i < cadenas.size(); ++i) {
      const bool aceptada = automata.leerCadena(cadenas[i]);
      std::cout << "Cadena " << (i + 1) << ": "
                << (aceptada ? "ACEPTADA" : "RECHAZADA") << '\n';
    }
  } catch (const std::exception& error) {
    std::cerr << "Error: " << error.what() << '\n';
    std::cerr << "Use -h para ver las opciones disponibles.\n";
    return 2;
  }

  return 0;
}