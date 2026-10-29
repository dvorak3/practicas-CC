CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2 -I../include
SOURCES = ../src/main.cc ../src/FuncionPrimitivaRecursiva.cc \
	../src/Composicion.cc ../src/RecursionPrimitiva.cc \
	../src/FactoryFunciones.cc
TARGET = funciones_primitivas_recursivas

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@

.PHONY: all clean
all: $(TARGET)

clean:
	rm -f $(TARGET)
