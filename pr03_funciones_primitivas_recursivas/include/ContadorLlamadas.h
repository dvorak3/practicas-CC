#ifndef CONTADOR_LLAMADAS_H
#define CONTADOR_LLAMADAS_H

#include <cstdint>
#include <limits>
#include <stdexcept>

class ContadorLlamadas {
 public:
  void incrementar() {
    if (llamadas_ == std::numeric_limits<std::uint64_t>::max()) {
      throw std::overflow_error("El contador de llamadas ha desbordado.");
    }
    ++llamadas_;
  }

  std::uint64_t llamadas() const { return llamadas_; }

 private:
  std::uint64_t llamadas_{0};
};

#endif
