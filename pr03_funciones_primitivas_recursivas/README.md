# Práctica 3: funciones primitivas recursivas

Implementación en C++17 de la función `potencia(x, y) = x^y` para números
naturales, construida a partir de las funciones básicas cero, sucesor y
proyección, utilizando composición y recursión primitiva.

## Requisitos

- Compilador compatible con C++17, por ejemplo `g++`.
- GNU Make.

## Compilación

Desde este directorio:

```sh
make -C build
```

Para limpiar y recompilar:

```sh
make -C build clean
make -C build
```

## Ejecución

El programa recibe base y exponente como opciones de línea de comandos:

```sh
build/funciones_primitivas_recursivas -x 3 -y 4
```

La salida contiene el resultado y el número total de llamadas a funciones.
Los argumentos deben ser valores `Natural` representables con `uint64_t`.
Si el resultado no cabe en ese tipo se informa del desbordamiento. Se considera
`0^0 = 1`, de acuerdo con el caso base de la definición recursiva usada aquí.

## Construcción de la función

Se emplea la convención estándar de recursión primitiva:

```text
h(0, a)     = f(a)
h(n + 1, a) = g(n, h(n, a), a)
```

La suma se obtiene con sucesor y proyección; el producto se obtiene por
recursión aplicando la suma; y la potencia se obtiene por recursión aplicando
el producto. Internamente, `potencia` recibe `(y, x)`, pues el primer argumento
de una función recursiva es el índice de recursión; la interfaz CLI mantiene
la notación habitual `-x` para la base y `-y` para el exponente.

## Diseño

- `FuncionPrimitivaRecursiva` define la interfaz común, valida la aridad y
  registra cada evaluación.
- `Composicion` y `RecursionPrimitiva` implementan las dos operaciones de
  construcción solicitadas.
- `FactoryFunciones` crea las funciones básicas y construye la suma, el
  producto y la potencia reutilizando los componentes. Es propietaria de las
  funciones dinámicas y las libera en su destructor; los punteros que devuelve
  son no propietarios y solo son válidos mientras viva la fábrica.
- `ContadorLlamadas` se comparte entre todas las funciones. Cuenta las
  evaluaciones de las funciones y cada despliegue recursivo de la recursión
  primitiva. **Cada evaluación de una composición cuenta como una llamada, igual
  que las evaluaciones de sus funciones interiores y de la función exterior.
  Por tanto, el total incluye funciones básicas, composiciones y recursiones,
  no solo llamadas a `potencia`.**

La recursión se evalúa iterativamente para evitar consumir la pila del programa
en cada nivel; cada iteración representa y contabiliza un despliegue de la
definición recursiva primitiva.

## Pruebas

```sh
bash tests/test_practica.sh
```

Las pruebas cubren exponentes cero, base cero, un resultado ordinario,
argumentos inválidos o fuera del rango admitido y desbordamiento del sucesor.

## Estructura

- `include/`: tipos e interfaces de las funciones y del contador.
- `src/`: funciones básicas, composición, recursión, fábrica y programa principal.
- `build/Makefile`: compilación desde consola.
- `tests/`: pruebas automáticas de ejecución.
- `doc/CC_2627_Practica3.pdf`: guion de la práctica.
