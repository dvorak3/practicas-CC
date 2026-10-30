#include <charconv>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <string_view>
#include <system_error>

#include "FactoryFunciones.h"
#include "Tipos.h"

namespace {
/**
 * @brief Muestra la sintaxis de uso y las opciones de la aplicación.
 * @param programa Nombre del ejecutable proporcionado por el sistema.
 */
void mostrarAyuda(const char* programa) {
  std::cout << "Uso: " << programa << " -x <natural> -y <natural>\n"
            << "  -x <n>  Base de la potencia.\n"
            << "  -y <n>  Exponente de la potencia.\n"
            << "  -h      Muestra esta ayuda.\n";
}

/**
 * @brief Convierte texto decimal a un número natural representable.
 * @param texto Texto que se va a interpretar.
 * @param opcion Nombre de la opción asociada, para el mensaje de error.
 * @return Valor natural convertido.
 * @throws std::invalid_argument Si el texto es negativo, está vacío, no es
 * decimal válido o excede el rango de Natural.
 */
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

/**
 * @brief Agrupa la base y el exponente recibidos por línea de comandos.
 */
struct Opciones {
  Natural base{0};  ///< Base de la potencia.
  Natural exponente{0};  ///< Exponente de la potencia.
  bool tiene_base{false};  ///< Indica si se proporcionó `-x`.
  bool tiene_exponente{false};  ///< Indica si se proporcionó `-y`.
};

/**
 * @brief Analiza las opciones y convierte sus valores a números naturales.
 * @param argc Cantidad de argumentos recibidos por el programa.
 * @param argv Vector de argumentos de la línea de comandos.
 * @return Opciones validadas para calcular la potencia.
 * @throws std::invalid_argument Si una opción es desconocida, se repite, no
 * tiene valor o falta la base o el exponente.
 */
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

/**
 * @brief Calcula y muestra la potencia indicada en la línea de comandos.
 * @param argc Cantidad de argumentos recibidos por el programa.
 * @param argv Valores de los argumentos.
 * @return Cero si el cálculo termina correctamente; uno si se produce un error.
 */
int main(int argc, char* argv[]) {
  try {
    const Opciones opciones = analizarOpciones(argc, argv);
    ContadorLlamadas contador;
    FactoryFunciones factory(contador);
    const FuncionPrimitivaRecursiva* potencia = factory.crearPotencia();
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
