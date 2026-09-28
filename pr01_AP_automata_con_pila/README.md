# Simulador de autómata con pila

Práctica 1 de Complejidad Computacional, curso 2026/27. Implementación en C++17 de un simulador de autómata con pila.

## Tipo de autómata implementado

Este proyecto implementa **APf: autómata con pila con aceptación por estado final**. Una cadena se acepta cuando la entrada se ha consumido por completo y la ejecución alcanza un estado final.

## Requisitos

- Compilador compatible con C++17, por ejemplo `g++`.
- `make`.

## Compilación

Desde el directorio `build`:

```sh
make
```

Para recompilar desde cero:

```sh
make clean && make
```

## Ejecución

Desde `build`, ejecuta el programa indicando el fichero de definición del autómata, si se activa la traza y el fichero de cadenas:

```sh
./automata_pila -config APf/APf-1.txt -trace n -in APf/cadenas.txt
```

Opciones disponibles:

- `-config <fichero>`: definición del autómata; obligatorio.
- `-trace <y|n>`: activa o desactiva la traza por pantalla; obligatorio.
- `-in <fichero>`: fichero de cadenas de entrada, con una cadena por línea; obligatorio en esta versión.
- `-h` o `--help`: muestra la ayuda.

`-out` todavía no está implementada. Cuando se activa `-trace y`, la traza se muestra por pantalla.

### Fichero de cadenas

Cada línea no vacía contiene una cadena que se comprobará. El punto (`.`) representa la cadena vacía. Por ejemplo:

```text
ab
aabb
aab
.
```

El programa informa si cada cadena es aceptada o rechazada. La ruta de `-config` se interpreta desde el directorio de trabajo actual; con los ejemplos incluidos, ejecuta el binario desde `build`.

### Fichero de configuración

El builder lee la definición del autómata desde el fichero indicado con `-config`. El formato sigue el guion de la práctica: conjuntos de estados, alfabeto de entrada, alfabeto de pila, estado inicial, símbolo inicial de pila, estados finales y las transiciones, una por línea. El punto (`.`) representa epsilon en la entrada y una cadena de reemplazo vacía en la pila. Los símbolos de ambos alfabetos son caracteres individuales; los símbolos del alfabeto de pila deben ser mayúsculas, según las restricciones de este proyecto.

## Diseño e implementación

- **Builder (`AutomataConPilaBuilder`)**: encapsula la lectura de la definición del autómata y del fichero de cadenas, y construye el objeto `AutomataConPila` a partir de la configuración.
- **Separación de responsabilidades**: `main` interpreta las opciones y coordina la lectura y la simulación; `AutomataConPila` ejecuta el autómata; `Estado` almacena las transiciones de cada estado; `Pila` encapsula las operaciones y la representación de la pila.
- **Traza mediante flujo opcional**: `leerCadena` recibe un `std::ostream*` opcional. Si se proporciona, informa de la configuración explorada, la entrada restante, la pila y las transiciones posibles; si es `nullptr`, no genera traza.
- **Exploración en profundidad**: las configuraciones pendientes se almacenan en una pila LIFO (`std::stack`). Se expande una rama y, al encontrar alternativas, se explora primero la que queda en la cima de esa pila.

## Estructura del proyecto

- `include/`: interfaces de las clases.
- `src/`: implementación y programa principal.
- `build/APf/`: ejemplos de definiciones de autómatas.
- `doc/`: documentación de la práctica.
