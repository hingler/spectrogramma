#include <FFT.hpp>

#define _USE_MATH_DEFINES
#include <cmath>
#include <exception>
#include <stdexcept>

uint32_t FFT::BitReverse(uint32_t input) {
  uint32_t out = input;
  out = out << 16 | out >> 16;
  out = ((out << 8) & 0xFF00FF00) | ((out >> 8) & 0x00FF00FF);
  out = ((out << 4) & 0xF0F0F0F0) | ((out >> 4) & 0x0F0F0F0F);
  out = ((out << 2) & 0xCCCCCCCC) | ((out >> 2) & 0x33333333);
  out = ((out << 1) & 0xAAAAAAAA) | ((out >> 1) & 0x55555555);

  return out;
}

FFT::FFT(float* real, float* imag, int len) {
  if ((len & len - 1) != 0) {
    throw std::invalid_argument("FFT length was not power of 2");
  }

  len_ = len;

  sin_cache_ = new float[len];
  cos_cache_ = new float[len];

  reverse_bits_table_ = new uint32_t[len];

  real_ = new float[len];
  imag_ = new float[len];

  if (sin_cache_ == NULL || cos_cache_ == NULL || reverse_bits_table_ == NULL || real_ == NULL || imag_ == NULL) {
    throw std::runtime_error("Not enough memory could be allocated for FFT");
  }

  CacheTrigFunctions();
  GenerateReverseBitTable();

  for (int i = 0; i < len_; i++) {
    real_[i] = real[i];
    if (imag != NULL) {
      imag_[i] = imag[i];
    }
  }
}

void FFT::CacheTrigFunctions() {
  double res_step = (2.0 * M_PI) / static_cast<double>(len_);
  for (int i = 0; i < len_; i++) {
    sin_cache_[i] = static_cast<float>(sin(res_step * static_cast<double>(i)));
    cos_cache_[i] = static_cast<float>(cos(res_step * static_cast<double>(i)));
  }
}

void FFT::GenerateReverseBitTable() {
  for (uint32_t i = 0; i < len_; i++) {
    reverse_bits_table_[i] = BitReverse(i);
  }
}

float** FFT::Transform() {
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

  for (uint32_t size = 1; size < len_; size <<= 1) {
    uint32_t runs = (len_ / size) / 2;
    for (uint32_t i = 0; i < runs; i++) {
      for (uint32_t k = 0; k < size; k++) {
        // results are built from respective even and odd
        // we fetch the even and odd indices
        // size = 1, so we have 2 terms -- even and odd
        // 8 entries: 0 4 2 6 1 5 3 7
        // each level up we have (size) evens, and (size) odds
        even_ind = i * 2 * size + k;
        odd_ind = even_ind + size;

        trig_ind = k * len_ / (2 * size);
        sin_res = sin_cache_[trig_ind];
        cos_res = cos_cache_[trig_ind];

        even_real = res[0][even_ind];
        even_imag = res[1][even_ind];



        odd_real = res[0][odd_ind] * cos_res - res[1][odd_ind] * sin_res;
        odd_imag = res[1][odd_ind] * cos_res + res[0][odd_ind] * sin_res;

        res[0][even_ind] = even_real + odd_real;
        res[1][even_ind] = even_imag + odd_imag;
        res[0][odd_ind] = even_real - odd_real;
        res[1][odd_ind] = even_imag - odd_imag;
      }
    }
  }

  return res;
}

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