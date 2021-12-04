#ifndef FFT_DATA_H_
#define FFT_DATA_H_

#include <memory>
#include <vector>

template <typename T>
struct FFT_data {
  std::vector<T> real;
  std::vector<T> imag;
};

#endif // FFT_DATA_H_