#include <audio_write.hpp>
#include <fft_convert.hpp>
#include <ImageData.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <iostream>

int main(int argc, char** argv) {
  if (argc < 4) {
    std::cout << "USAGE: ./spectrogramma <input image> <output path> <num-samples> {<sample-count>}" << std::endl;
    return 1;
  }

  int sample_count = 256;

  if (argc > 4) {
    sample_count = atoi(argv[4]);
  }

  if (sample_count < 1 || (sample_count & (sample_count - 1)) != 0) {
    std::cout << "Invalid sample count: " << argv[4] <<  std::endl;
    return 1;
  }

  int num_samples = atoi(argv[3]);
  if (num_samples < 1) {
    std::cout << "Invalid sample length: " << argv[3] << std::endl;
    return 1;
  }
  
  ImageData src(argv[1]);
  ImageData fftImage = fft_convert::ImageFFT(src, sample_count);
  std::vector<float> samples = fft_convert::FFTToSamples(fftImage, static_cast<uint32_t>(num_samples));
  audio_write::Normalize(samples);
  audio_write::WriteSamplesToFile(std::string(argv[2]), samples);
}
