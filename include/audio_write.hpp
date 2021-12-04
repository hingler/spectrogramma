#ifndef AUDIO_WRITE_H_
#define AUDIO_WRITE_H_

#include <string>
#include <vector>


namespace audio_write {
  /**
   * @brief Normalize sample data to a range of (-1, 1)
   * 
   * @param samples - sample data 
   */
  template <typename T>
  void Normalize(std::vector<T>& samples) {
    T max = abs(samples[0]);
    for (const auto& sample : samples) {
      max = std::max(max, abs(sample));
    }

    double fac = 1.0 / static_cast<double>(max);

    for (int i = 0; i < samples.size(); i++) {
      samples[i] *= static_cast<T>(fac);
    }
  }

  /**
   * @brief Writes audio samples to dest file
   * 
   * @param dest file name
   * @param samples vector of samples to write
   */
  void WriteSamplesToFile(const std::string& dest, const std::vector<float>& samples);
}

#endif // AUDIO_WRITE_H_