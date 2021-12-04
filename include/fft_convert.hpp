#ifndef FFT_CONVERT_H_
#define FFT_CONVERT_H_

#include <ImageData.hpp>
#include <vector>

namespace fft_convert {
  
  /**
   * @brief Inverse transforms an input image's columns and returns a new image
   *        for which each row represents a column's inverse transformed luminance.
   * 
   * @param input the input image :D
   * @param samples_per_column number of samples to take from each column for fourier transform
   * @return ImageData the inverse transformed image data
   */
  ImageData ImageFFT(const ImageData& input, int samples_per_column);

  /**
   * @brief Turns FFT'd image data into audio samples.
   * 
   * @param input - FFT transformed image output.
   * @param output_size - number of samples desired for output.
   * @return vector containing sample data.
   */
  std::vector<float> FFTToSamples(const ImageData& input, uint32_t output_size);
}

#endif // FFT_CONVERT_H_