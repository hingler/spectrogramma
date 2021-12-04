#include <fft_convert.hpp>

#include <FFT.hpp>
#include <FFT_data.hpp>

#include <vector>

#include <Macro.hpp>
#include <audio_write.hpp>

#define _USE_MATH_DEFINES
#include <cmath>

#define LUM(v) v.a * (0.2126 * v.r + 0.7152 * v.g + 0.0722 * v.b)

namespace fft_convert {
  ImageData ImageFFT(const ImageData& input, int samples_per_column) {
    auto dim = input.GetDimensions();
    DEBUG_LOG("dims: " << dim.x << ", " << dim.y);
    float* res = new float[static_cast<size_t>(dim.x * static_cast<float>(samples_per_column * 2))];
    DEBUG_LOG(static_cast<size_t>(dim.x * static_cast<float>(samples_per_column * 2)));
    
    float* fft_data = new float[2 * samples_per_column];
    FFT_data<float> data;

    // normalization squashes the pattern due to phase syncs afaik
    float* imag = new float[2 * samples_per_column];


    FFT transform(2 * samples_per_column);

    glm::dvec2 coord;
    glm::vec4 sample;
    double res_step = (dim.y - 1.0) / samples_per_column;
    double scale;

    for (int i = 0; i < dim.x; i++) {
      coord.x = static_cast<double>(i) + 0.5;
      fft_data[0] = 0.4f;
      for (int j = 0; j < samples_per_column; j++) {
        // might flip image? sample in reverse
        // reduce value as we increase y
        coord.y = res_step * static_cast<double>(samples_per_column - j - 1) + 0.5;
        sample = input.LinearSample(coord);
        // scale = pow(0.99, static_cast<double>(j) * (dim.y / 1024.0));
        scale = 1.0;
        // scale higher ordinal values to avoid high freq samples
        fft_data[j + 1] = static_cast<float>(LUM(sample) * static_cast<double>(2 * samples_per_column) * scale);
      }

      // perform a rotation!
      for (int j = 0; j <= samples_per_column; j++) {
        double theta = 3.14159265358979 * (static_cast<double>(j) / samples_per_column);
        // DEBUG_LOG(theta);
        imag[j] = static_cast<float>(sin(theta)) * fft_data[j];
        fft_data[j] *= static_cast<float>(cos(theta));
      }

      for (int j = 1; j < samples_per_column; j++) {
        fft_data[samples_per_column + j] = fft_data[samples_per_column - j];
        imag[samples_per_column + j] = -imag[samples_per_column - j];
      }


      transform.SetSampleData(fft_data, imag);
      // should be an object containing two vectors instead
      data = transform.InverseTransform_T();
      audio_write::Normalize(data.real);

      for (int j = 0; j < 2 * samples_per_column; j++) {
        res[i * 2 * samples_per_column + j] = data.real[j];
        // assert(abs(data.imag[j]) < 0.0001);
      }
    }

    delete[] fft_data;
    return ImageData(std::move(res), 2 * samples_per_column, static_cast<int>(dim.x), 1);
  }

  // floating point! negatives are no problem :D
  std::vector<float> FFTToSamples(const ImageData& input, uint32_t output_size) {
    std::vector<float> res;
    res.reserve(output_size);

    glm::dvec2 coord;
    coord.x = 0.5;
    coord.y = 0.5;

    double res_step = (static_cast<double>(input.GetDimensions().y) - 1.0) / static_cast<double>(output_size);

    for (uint32_t i = 0; i < output_size; i++) {
      coord.x += 1.0;
      coord.y += res_step;

      res.push_back(input.LinearSample(coord).r);
    }

    return res;
  }
}