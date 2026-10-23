# Práctica 2: simulador de una máquina de Turing

Implementación en C++17 de un simulador determinista de una o varias cintas y
dos máquinas de Turing para los problemas del guion de la Práctica 2 de
Complejidad Computacional.

## Opciones seleccionadas

- Escritura y movimientos simultáneos: cada transición lee y escribe un símbolo
  por cinta, y define un movimiento para cada cabeza.
- Movimientos `L`, `R` y `S` (izquierda, derecha y sin movimiento).
- Cintas infinitas en ambas direcciones.

La ejecución termina cuando no existe una transición aplicable. Solo se acepta
si el estado en el que se detiene pertenece al conjunto de estados finales.
Cada entrada se carga inicialmente en la cinta 1, con la cabeza en el primer
carácter; las demás cintas comienzan vacías. La salida incluye la aceptación,
los pasos y el contenido de cada cinta con la cabeza entre corchetes.

## Requisitos y compilación

Se necesita un compilador C++17 (por ejemplo, `g++`) y GNU Make. Desde la raíz
de este directorio:

```sh
make -C build
```

Para limpiar y recompilar:

```sh
make -C build clean
make -C build
```

## Ejecución

Desde la raíz del proyecto:

```sh
build/maquina_turing -config tests/configuracion/MT_1_anbm.txt \
  -in tests/entradas/anbm.txt
build/maquina_turing -config tests/configuracion/MT_2_contar.txt \
  -in tests/entradas/contar.txt
```

Opciones disponibles:

- `-config <fichero>`: definición de la máquina; obligatorio.
- `-in <fichero>`: fichero opcional con una entrada por línea. Si se omite,
  se leen las entradas desde el teclado hasta EOF.
- `-trace y|n`: muestra la configuración de cada paso; opcional, por defecto
  `n`.
- `-h` o `--help`: muestra la ayuda.

Una línea que contenga `.` representa la entrada vacía. En los ficheros de
entrada, las líneas que comienzan por `#` son comentarios.

## Formato de configuración

El fichero consta de las siguientes líneas, en este orden:

1. Estados `Q`.
2. Alfabeto de entrada `Σ`.
3. Alfabeto de cinta `Γ` (debe incluir `.` como blanco).
4. Estado inicial.
5. Símbolo blanco (`.`).
6. Estados finales `F`.
7. Número de cintas.
8. Una transición por línea, hasta el final del fichero.

Para `k` cintas, una transición tiene `2 + 3k` campos:

```text
estado_origen lectura_1 ... lectura_k estado_destino escritura_1 ... escritura_k movimiento_1 ... movimiento_k
```

Los símbolos son caracteres individuales; los movimientos permitidos son `L`,
`R` y `S`, que el simulador representa internamente con el tipo `Movimiento`
de `include/Tipos.h`. Se permiten líneas vacías y comentarios que comienzan
por `#`. El builder comprueba alfabetos, estados, número de campos,
movimientos y determinismo antes de ejecutar la máquina.

## Diseño del builder

`MTBuilder` es una clase abstracta que contiene el algoritmo común de
construcción y validación de la máquina, junto con los métodos protegidos para
interpretar estados, alfabetos, símbolos y movimientos. La única operación
abstracta es `leerTokens()`, que adapta la fuente de la configuración al
algoritmo compartido. `MTBuilderDesdeArchivo` hereda de `MTBuilder` y se ocupa
de abrir el fichero, descartar comentarios y entregar sus líneas como tokens.
Así, una futura variante que obtenga la definición desde otra fuente puede
reutilizar las validaciones sin duplicar la construcción.

## Máquinas diseñadas

1. **`L = { a^n b^m | m >= n, n > 0 }`**
   - Configuración: `tests/configuracion/MT_1_anbm.txt`
   - Entradas de prueba: `tests/entradas/anbm.txt`
   - Grafo: `doc/grafos/MT_1_anbm.svg`
   - La máquina marca cada `a` y empareja una `b` posterior; al terminar las
     `a`, comprueba que no quede ninguna `a` en el sufijo.

2. **Recuento unario de símbolos**
   - Configuración: `tests/configuracion/MT_2_contar.txt`
   - Entradas de prueba: `tests/entradas/contar.txt`
   - Grafo: `doc/grafos/MT_2_contar.svg`
   - Usa dos cintas. En una pasada copia la entrada a la cinta 2 y borra la
     cinta 1; después recorre la copia para escribir en la cinta 1 el recuento
     de `b`, un blanco separador y el recuento de `a`. Los recuentos vacíos se
     representan con `0`. Al terminar, la cabeza queda al principio del
     resultado. El grafo muestra sus 12 estados y las 30 transiciones; las
     etiquetas `Tnn` remiten a la clave completa bajo el diagrama.

El resultado del ejemplo `abbabaabb` es `11111.1111`; para `aa` es `0.11` y
para `bb` es `11.0`.

## Estructura

- `include/`: tipos, cinta, estados, transiciones, builder e interfaz de la máquina.
- `src/`: lectura/validación, implementación de la cinta y simulación.
- `build/Makefile`: compilación.
- `tests/configuracion/`: definiciones de las dos máquinas y ejemplos de
  entradas.
- `tests/entradas/`: conjuntos de entradas para ejecutar las pruebas manuales.
- `doc/grafos/`: imágenes vectoriales de los grafos.

La clase `Cinta` encapsula las celdas y la posición de su cabeza; ofrece las
operaciones de lectura, escritura, movimiento y carga de entrada. Su
representación se escribe mediante la sobrecarga de `operator<<`.
