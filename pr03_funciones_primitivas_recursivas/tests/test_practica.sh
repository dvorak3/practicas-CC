#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
make -C "$ROOT/build"
PROGRAMA="$ROOT/build/funciones_primitivas_recursivas"
PRUEBA_FUNCIONES="$(mktemp)"
trap 'rm -f "$PRUEBA_FUNCIONES"' EXIT

g++ -std=c++17 -Wall -Wextra -Wpedantic -I"$ROOT/include" \
  "$ROOT/tests/test_funciones_basicas.cc" \
  "$ROOT/src/FuncionPrimitivaRecursiva.cc" "$ROOT/src/Composicion.cc" \
  "$ROOT/src/RecursionPrimitiva.cc" "$ROOT/src/FactoryFunciones.cc" \
  -o "$PRUEBA_FUNCIONES"
"$PRUEBA_FUNCIONES"

comprobar_resultado() {
  local base="$1"
  local exponente="$2"
  local esperado="$3"
  local salida
  salida="$("$PROGRAMA" -x "$base" -y "$exponente")"
  grep -Fq "Resultado: $base^$exponente = $esperado" <<<"$salida"
  grep -Eq '^Numero de llamadas a funciones: [1-9][0-9]*$' <<<"$salida"
}

comprobar_error() {
  if "$PROGRAMA" "$@" >/dev/null 2>&1; then
    echo "Se esperaba un error para: $*" >&2
    exit 1
  fi
}

comprobar_resultado 2 0 1
comprobar_resultado 0 5 0
comprobar_resultado 3 4 81
comprobar_error -x -1 -y 2
comprobar_error -x 2
comprobar_error -x 18446744073709551616 -y 1
comprobar_error -x 2 -y abc

echo "Todas las pruebas de la practica 3 han pasado."
