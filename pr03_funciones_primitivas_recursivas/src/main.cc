#include <charconv>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <system_error>

#include "FactoryFunciones.h"
#include "Tipos.h"

namespace {
void mostrarAyuda(const char* programa) {
  std::cout << "Uso: " << programa << " -x <natural> -y <natural>\n"
            << "  -x <n>  Base de la potencia.\n"
            << "  -y <n>  Exponente de la potencia.\n"
            << "  -h      Muestra esta ayuda.\n";
}

Natural convertirNatural(std::string_view texto, const char* opcion) {
  if (texto.empty() || texto.front() == '-') {
    throw std::invalid_argument(std::string("Valor natural no valido para ") +
                                opcion + ": " + std::string(texto));
  }
  Natural valor = 0;
  const auto resultado =
      std::from_chars(texto.data(), texto.data() + texto.size(), valor);
  if (resultado.ec != std::errc{} ||
      resultado.ptr != texto.data() + texto.size()) {
    throw std::invalid_argument(std::string("Valor natural no valido para ") +
                                opcion + ": " + std::string(texto));
  }
  return valor;
}

struct Opciones {
  Natural base{0};
  Natural exponente{0};
  bool tiene_base{false};
  bool tiene_exponente{false};
};

Opciones analizarOpciones(int argc, char* argv[]) {
  Opciones opciones;
  for (int indice = 1; indice < argc; ++indice) {
    const std::string opcion = argv[indice];
    if (opcion == "-h" || opcion == "--help") {
      mostrarAyuda(argv[0]);
      std::exit(0);
    }
    if (opcion != "-x" && opcion != "-y") {
      throw std::invalid_argument("Opcion desconocida: " + opcion);
    }
    if (indice + 1 >= argc) {
      throw std::invalid_argument("Falta el valor para " + opcion + ".");
    }
    if (opcion == "-x") {
      if (opciones.tiene_base) {
        throw std::invalid_argument("La opcion -x no puede repetirse.");
      }
      opciones.base = convertirNatural(argv[++indice], "-x");
      opciones.tiene_base = true;
    } else {
      if (opciones.tiene_exponente) {
        throw std::invalid_argument("La opcion -y no puede repetirse.");
      }
      opciones.exponente = convertirNatural(argv[++indice], "-y");
      opciones.tiene_exponente = true;
    }
  }
  if (!opciones.tiene_base || !opciones.tiene_exponente) {
    throw std::invalid_argument("Es necesario indicar -x y -y.");
  }
  return opciones;
}
}  // namespace

int main(int argc, char* argv[]) {
  try {
    const Opciones opciones = analizarOpciones(argc, argv);
    ContadorLlamadas contador;
    const FactoryFunciones factory(contador);
    const auto potencia = factory.crearPotencia();
    const Natural resultado =
        (*potencia)({opciones.exponente, opciones.base});

    std::cout << "Resultado: " << opciones.base << "^" << opciones.exponente
              << " = " << resultado << '\n'
              << "Numero de llamadas a funciones: " << contador.llamadas()
              << '\n';
  } catch (const std::exception& error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }
  return 0;
}
