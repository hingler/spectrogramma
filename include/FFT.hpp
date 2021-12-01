#ifndef FFT_H_
#define FFT_H_

#include <cinttypes>

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
 public:
  /**
   * @brief Construct a new FFT object
   * @param data - real data to transform
   * @param len - length of result
   * 
   * @throws invalid arg exception if length is not a power of 2.
   */
  FFT(float* data, int len);

  /**
   * @brief Construct a new FFT object, using both real and imaginary components.
   * 
   * @param real - real component of input data.
   * @param imag - imaginary component of input data.
   * @param len -  length of data fields.
   * 
   * @throws exception if length is not a power of two.
   */
  FFT(float* real, float* imag, int len);


  static uint32_t BitReverse(uint32_t input);

  /**
   * @brief Performs inverse FFT on data.
   * 
   * @return float* inverse convolved result allocated in heap memory. Must be freed by caller.
   */
  float* InverseTransform();


  /**
   * @brief Performs FFT on data.
   * 
   * @return float** convolved result in heap mem, index0 is real, index1 is imag results.
   */
  float** Transform();


  // rule of 5
  FFT(const FFT& other);
  FFT(FFT&& other);

  FFT& operator=(const FFT& other);
  FFT& operator=(FFT&& other);

  ~FFT();
};

#endif // FFT_H_