#ifndef FFT_DATA_H_
#define FFT_DATA_H_

#include <memory>

struct FFT_data {
  std::shared_ptr<float> real;
  std::shared_ptr<float> imag;
};

#endif // FFT_DATA_H_