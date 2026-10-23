CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2
SOURCES = ../src/main.cc ../src/Cinta.cc ../src/MaquinaDeTuring.cc ../src/MTBuilder.cc

maquina_turing: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@

.PHONY: clean
clean:
	rm -f maquina_turing
