#include <exception>
#include <stdexcept>

#include <STBImage.hpp>
#include <ImagePtr.hpp>
#include <ImageData.hpp>

#include <Macro.hpp>

#include <iostream>

#define CLAMP(mn, mx, t) std::min(std::max(t, mn), mx)

ImageData::ImageData(const std::string& path) {
  image_data_ = std::make_shared<STBImage>(path);

  auto dim = image_data_->GetImageDimensions();
  x_ = dim.x;
  y_ = dim.y;
  channel_count_ = image_data_->GetChannelCount();

  mode = WrapMode::REPEAT;
}

ImageData::ImageData(const float* ptr, int x, int y, int channel_count) {
  image_data_ = std::make_shared<ImagePtr>(ptr, x, y, channel_count);

  // todo: redundant imo
  x_ = x;
  y_ = y;
  channel_count_ = channel_count;

  mode = WrapMode::REPEAT;
}

ImageData::ImageData(float*&& ptr, int x, int y, int channel_count) {
  image_data_ = std::make_shared<ImagePtr>(std::move(ptr), x, y, channel_count);

  // todo: redundant imo lol
  x_ = x;
  y_ = y;
  channel_count_ = channel_count;

  mode = WrapMode::REPEAT;
}

glm::vec4 ImageData::NearestSample(glm::vec2 coord) const {
  return NearestSample(coord.x, coord.y);
}

glm::vec4 ImageData::NearestSample(float x, float y) const {
  // 0.5 should be center of a pixel
  // ie 0.5, 0.5 should sample (0, 0)
  // 1.001, 1.001 should sample (1, 1)
  // round down :D
  int xf = static_cast<int>(x);
  int yf = static_cast<int>(y);

  if (mode == WrapMode::CLAMP) {
    xf = CLAMP(0, x_ - 1, xf);
    yf = CLAMP(0, y_ - 1, yf);
  } else if (mode == WrapMode::REPEAT) {
    double round_xf = floor(static_cast<double>(xf) / x_);
    double round_yf = floor(static_cast<double>(yf) / y_);

    xf -= (static_cast<int>(round_xf) * x_);
    yf -= (static_cast<int>(round_yf) * y_);

    // redundant
    xf = CLAMP(0, x_ - 1, xf);
    yf = CLAMP(0, y_ - 1, yf);
  }

  if (xf < 0 || xf >= x_ || yf < 0 || yf >= y_) {
    // oob
    return glm::vec4(0);
  }

  const float* coldata = (image_data_->GetImagePointer()) + ((x_ * yf + xf) * channel_count_);

  if (channel_count_ == 1) {
    return glm::vec4(*coldata);
  } else if (channel_count_ == 2) {
    return glm::vec4(coldata[0] * coldata[1]);
  } else if (channel_count_ == 3) {
    return glm::vec4(*coldata, *(coldata + 1), *(coldata + 2), 1.0);
  } else {// channel_count_ == 4
    return glm::vec4(coldata[0], coldata[1], coldata[2], coldata[3]);
  }
}

glm::vec4 ImageData::LinearSample(glm::vec2 coord) const {
  float xt = glm::fract(coord.x - 0.5f);
  float yt = glm::fract(coord.y - 0.5f);

  // 0, 0 refers to top left corner
  glm::vec2 cf = glm::floor(coord - glm::vec2(0.5));

  glm::vec4 res(0.0);
  res += (static_cast<float>((1.0f - xt) * (1.0f - yt)) * NearestSample(cf.x, cf.y));
  res += static_cast<float>(xt * (1.0f - yt)) * NearestSample(cf.x + 1.0f, cf.y);
  res += static_cast<float>((1.0f - xt) * yt) * NearestSample(cf.x, cf.y + 1.0f);
  res += static_cast<float>(xt * yt) * NearestSample(cf.x + 1.0f, cf.y + 1.0f);

  return res;
}

glm::vec4 ImageData::LinearSample(float x, float y) const {
  return LinearSample(glm::vec2(x, y));
}

glm::vec2 ImageData::GetDimensions() const {
  return glm::vec2(x_, y_);
}

// define a width for our final sound
// define a resolution -- how many samples to read per column
// for each column:
//  - inverseFFT linear sampled pixel data to get samples
//  - patch the samples on the ends so they join up nicely when looped (this might actually not pose an issue)
//  - turn the inverseFFT sample data into an image (?)
//  - linear sample the image, moving left to right (per sample) and top to bottom (over time)

// need an image data ctor which accepts raw data ptr (smart ptr?)