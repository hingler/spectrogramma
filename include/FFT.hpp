#ifndef FFT_H_
#define FFT_H_

#include <cinttypes>
#include <cassert>
#include <FFT_data.hpp>


/**
 * @brief Class which performs fft operations on inputted data.
 */
class FFT {
 private:
  // cache for sine and cosine calculations
  float* sin_cache_;
  float* cos_cache_;

  // table for flipped bits
  uint32_t* reverse_bits_table_;

  // length of data
  int len_;

  float* real_;
  float* imag_;

  void CacheTrigFunctions();
  void GenerateReverseBitTable();
  float** GenericTransform(bool invert);

  FFT_data<float> GenericTransform_T(bool invert) {
    double even_imag, even_real, odd_imag, odd_real;
    double sin_res, cos_res;

    int even_ind, odd_ind, trig_ind;

    FFT_data<float> fft_res;
    fft_res.real.resize(len_, 0.0f);
    fft_res.imag.resize(len_, 0.0f);
    

    for (int i = 0; i < len_; i++) {
      fft_res.real[i] = real_[reverse_bits_table_[i]];
      fft_res.imag[i] = imag_[reverse_bits_table_[i]];
    }

    // res is convolved

    int mul = (invert ? -1 : 1);
    int len = (invert ? len_ : 0);

    for (int size = 1; size < len_; size <<= 1) {
      int runs = (len_ / size) / 2;
      for (int i = 0; i < runs; i++) {
        for (int k = 0; k < size; k++) {
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

          even_real = fft_res.real[even_ind];
          even_imag = fft_res.imag[even_ind];

          odd_real = fft_res.real[odd_ind] * cos_res - fft_res.imag[odd_ind] * sin_res;
          odd_imag = fft_res.imag[odd_ind] * cos_res + fft_res.real[odd_ind] * sin_res;

          fft_res.real[even_ind] = static_cast<float>(even_real + odd_real);
          fft_res.imag[even_ind] = static_cast<float>(even_imag + odd_imag);
          fft_res.real[odd_ind] = static_cast<float>(even_real - odd_real);
          fft_res.imag[odd_ind] = static_cast<float>(even_imag - odd_imag);
        }
      }
    }

    return fft_res;
  }
 public:
  /**
   * @brief Construct a new FFT object with no data.
   * 
   * @param len - length of FFT.
   */
  FFT(int len);
 
  /**
   * @brief Construct a new FFT object
   * @param data - real data to transform
   * @param len - length of result
   * 
   * @throws invalid arg exception if length is not a power of 2.
   */
  FFT(const float* data, int len);

  /**
   * @brief Construct a new FFT object, using both real and imaginary components.
   * 
   * @param real - real component of input data.
   * @param imag - imaginary component of input data.
   * @param len -  length of data fields.
   * 
   * @throws exception if length is not a power of two.
   */
  FFT(const float* real, const float* imag, int len);


  static uint32_t BitReverse(uint32_t input, uint32_t len);

  /**
   * @brief Sets the sample data stored in this FFT object.
   * 
   * @param real - real number data - must be at least length <len_>, or NULL.
   * @param imag - imaginary number data - must be at least length <len_>, or NULL.
   */
  void SetSampleData(const float* real, const float* imag);

  /**
   * @brief Performs inverse FFT on data.
   * 
   * @return float** inverse convolved result allocated in heap memory, [0] is real, [1] is imag. Must be freed by caller.
   */
  float** InverseTransform();

  FFT_data<float> InverseTransform_T() {
    auto res = GenericTransform_T(true);

    for (int i = 0; i < len_; i++) {
      res.real[i] /= len_;
      res.imag[i] /= len_;
    }

    return res;
  }


  /**
   * @brief Performs FFT on data.
   * 
   * @return float** convolved result in heap mem, index0 is real, index1 is imag results. Must be freed by caller.
   */
  float** Transform();

  FFT_data<float> Transform_T() {
    return GenericTransform_T(false);
  }


  // rule of 5
  FFT(const FFT& other);
  FFT(FFT&& other);

  FFT& operator=(const FFT& other);
  FFT& operator=(FFT&& other);

  ~FFT();
};

#endif // FFT_H_