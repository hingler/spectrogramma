#include <FFT.hpp>

#define _USE_MATH_DEFINES

#include <cassert>
#include <cmath>
#include <exception>
#include <stdexcept>

#ifndef NDEBUG
  #include <iostream>
#endif

#define NEGATE(n, b) b ? -n : n



uint32_t FFT::BitReverse(uint32_t input, uint32_t len) {
  uint32_t out = input;
  out = out << 16 | out >> 16;
  out = ((out << 8) & 0xFF00FF00) | ((out >> 8) & 0x00FF00FF);
  out = ((out << 4) & 0xF0F0F0F0) | ((out >> 4) & 0x0F0F0F0F);
  out = ((out << 2) & 0xCCCCCCCC) | ((out >> 2) & 0x33333333);
  out = ((out << 1) & 0xAAAAAAAA) | ((out >> 1) & 0x55555555);
  return out >> (32 - len);
}

FFT::FFT(int len) : FFT(NULL, NULL, len) {}
FFT::FFT(const float* data, int len) : FFT(data, NULL, len) {}
FFT::FFT(const float* real, const float* imag, int len) {
  if ((len & len - 1) != 0) {
    throw std::invalid_argument("FFT length was not power of 2");
  }

  len_ = len;

  // cache 2 pi case
  sin_cache_ = new float[len + 1];
  cos_cache_ = new float[len + 1];

  reverse_bits_table_ = new uint32_t[len];

  real_ = new float[len];
  imag_ = new float[len];

  if (sin_cache_ == NULL || cos_cache_ == NULL || reverse_bits_table_ == NULL || real_ == NULL || imag_ == NULL) {
    throw std::runtime_error("Not enough memory could be allocated for FFT");
  }

  CacheTrigFunctions();
  GenerateReverseBitTable();
  SetSampleData(real, imag);
}

void FFT::SetSampleData(const float* real, const float* imag) {
  for (int i = 0; i < len_; i++) {
    if (real != NULL) {
      real_[i] = real[i];
    } else {
      real_[i] = 0.0F;
    }
    if (imag != NULL) {
      imag_[i] = imag[i];
    } else {
      imag_[i] = 0.0F;
    }
  }
}

void FFT::CacheTrigFunctions() {
  double res_step = (-2.0 * M_PI) / static_cast<double>(len_);
  double theta;
  for (int i = 0; i <= len_; i++) {
    theta = res_step * static_cast<double>(i);
    sin_cache_[i] = static_cast<float>(sin(theta));
    cos_cache_[i] = static_cast<float>(cos(theta));
  }
}

void FFT::GenerateReverseBitTable() {
  int len = 0;
  uint32_t temp = static_cast<uint32_t>(len_ - 1);

  while (temp > 0) {
    len++;
    temp >>= 1;
  }

  #ifndef NDEBUG
    std::cout << "BIT LEN EQUAL TO " << len << std::endl;
  #endif

  for (uint32_t i = 0; i < len_; i++) {
    reverse_bits_table_[i] = BitReverse(i, len);
  }
}

float** FFT::Transform() {
  return GenericTransform(false);
}

// template?
float** FFT::InverseTransform() {
  // scale down when inverting
  float** res = GenericTransform(true);
  for (int i = 0; i < len_; i++) {
    res[0][i] /= len_;
    res[1][i] /= len_;
  }

  return res;
}

float** FFT::GenericTransform(bool invert) {
  double even_imag, even_real, odd_imag, odd_real;
  double sin_res, cos_res;

  uint32_t even_ind, odd_ind, trig_ind;

  float** res = new float*[2];

  res[0] = new float[len_]; // real
  res[1] = new float[len_]; // imag

  for (uint32_t i = 0; i < len_; i++) {
    res[0][i] = real_[reverse_bits_table_[i]];
    res[1][i] = imag_[reverse_bits_table_[i]];
  }

  // res is convolved

  int mul = (invert ? -1 : 1);
  int len = (invert ? len_ : 0);

  for (uint32_t size = 1; size < len_; size <<= 1) {
    uint32_t runs = (len_ / size) / 2;
    for (uint32_t i = 0; i < runs; i++) {
      for (uint32_t k = 0; k < size; k++) {
        even_ind = i * 2 * size + k;
        odd_ind = even_ind + size;

        // this is literally the only difference, everything else should be the same
        // todo: factor redundancy out

        // write an inline func
        trig_ind = (len + mul * (k * len_ / (2 * size)));

        #ifndef NDEBUG
          // in case if stmt isn't factored out
          assert(trig_ind <= len_);
          assert(trig_ind >= 0);

          if (invert) {
            assert(trig_ind >= (len_ / 2));
          } else {
            assert(trig_ind < (len_ / 2));
          }
        #endif
        
        sin_res = sin_cache_[trig_ind];
        cos_res = cos_cache_[trig_ind];

        even_real = res[0][even_ind];
        even_imag = res[1][even_ind];

        odd_real = res[0][odd_ind] * cos_res - res[1][odd_ind] * sin_res;
        odd_imag = res[1][odd_ind] * cos_res + res[0][odd_ind] * sin_res;

        res[0][even_ind] = static_cast<float>(even_real + odd_real);
        res[1][even_ind] = static_cast<float>(even_imag + odd_imag);
        res[0][odd_ind] = static_cast<float>(even_real - odd_real);
        res[1][odd_ind] = static_cast<float>(even_imag - odd_imag);
      }
    }
  }

  return res;
}

// inverse transform is equiv to transform, only we flip our thetas

FFT::~FFT() {
  if (sin_cache_ != NULL) {
    delete sin_cache_;
  }

  if (cos_cache_ != NULL) {
    delete cos_cache_;
  }

  if (reverse_bits_table_ != NULL) {
    delete reverse_bits_table_;
  }

  if (real_ != NULL) {
    delete real_;
  }

  if (imag_ != NULL) {
    delete imag_;
  }
}