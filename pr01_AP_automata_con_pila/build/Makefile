vpath %.cc ../src/
CXX = g++
CXXFLAGS = -std=c++17 -Wall
files := main.cc AutomataConPilaBuilder.cc

automata_pila: $(files)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -fv automata_pila