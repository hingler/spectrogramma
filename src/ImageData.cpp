#define STB_IMAGE_IMPLEMENTATION

#include <exception>
#include <stdexcept>

#include <ImageData.hpp>
#include <stb_image.h>

#include <iostream>

#define CLAMP(mn, mx, t) std::min(std::max(t, mn), mx)

ImageData::ImageData(const std::string& path) {
  image_data_ = stbi_loadf(path.c_str(), &x_, &y_, &channel_count_, 0);
  if (image_data_ == NULL) {
    std::cerr << "BAD PATH " << path << std::endl;
    throw std::invalid_argument("Invalid argument `path` to ImageData constructor");
  }

  mode = WrapMode::REPEAT;
}

glm::vec4 ImageData::NearestSample(glm::vec2 coord) {
  return NearestSample(coord.x, coord.y);
}

glm::vec4 ImageData::NearestSample(float x, float y) {
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

  float* coldata = image_data_ + ((y_ * yf + xf) * channel_count_);

  if (channel_count_ == 1) {
    return glm::vec4(*coldata);
  } else if (channel_count_ == 2) {
    return glm::vec4(coldata[0] * coldata[1]);
  } else if (channel_count_ == 3) {
    return glm::vec4(coldata[0], coldata[1], coldata[2], 1.0);
  } else {// channel_count_ == 4
    return glm::vec4(coldata[0], coldata[1], coldata[2], coldata[3]);
  }
}

glm::vec4 ImageData::LinearSample(glm::vec2 coord) {
  float xt = glm::fract(coord.x - 0.5f);
  float yt = glm::fract(coord.y - 0.5f);

  // 0, 0 refers to top left corner
  glm::vec2 cf = glm::floor(coord - glm::vec2(0.5));

  glm::vec4 res(0.0);
  res += static_cast<float>((1.0f - xt) * (1.0f - yt)) * NearestSample(cf.x, cf.y);
  res += static_cast<float>(xt * (1.0f - yt)) * NearestSample(cf.x + 1.0f, cf.y);
  res += static_cast<float>((1.0f - xt) * yt) * NearestSample(cf.x, cf.y + 1.0f);
  res += static_cast<float>(xt * yt) * NearestSample(cf.x + 1.0f, cf.y + 1.0f);

  return res;
}

glm::vec4 ImageData::LinearSample(float x, float y) {
  return LinearSample(glm::vec2(x, y));
}

glm::vec2 ImageData::GetDimensions() {
  return glm::vec2(x_, y_);
}

ImageData::ImageData(ImageData&& other) {
  x_ = other.x_;
  y_ = other.y_;
  channel_count_ = other.channel_count_;
  image_data_ = other.image_data_;
  other.image_data_ = NULL;
}

ImageData& ImageData::operator=(ImageData&& other) {
  x_ = other.x_;
  y_ = other.y_;
  channel_count_ = other.channel_count_;
  image_data_ = other.image_data_;
  return *this;
}

ImageData::~ImageData() {
  if (image_data_ != NULL) {
    stbi_image_free(image_data_);
  }
}