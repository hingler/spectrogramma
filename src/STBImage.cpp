#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <STBImage.hpp>

#include <iostream>

STBImage::STBImage(const std::string& path) {
  stb_image_data_ = stbi_loadf(path.c_str(), &x_, &y_, &channel_count_, 0);
  if (stb_image_data_ == NULL) {
    std::cerr << "BAD PATH " << path << std::endl;
    throw std::invalid_argument("Invalid argument `path` to ImageData constructor");
  }
}

glm::ivec2 STBImage::GetImageDimensions() {
  return glm::ivec2(x_, y_);
}

int STBImage::GetChannelCount() {
  return channel_count_;
}

STBImage::~STBImage() {
  if (stb_image_data_ != NULL) {
    stbi_image_free(stb_image_data_);
  }
}

STBImage::STBImage(STBImage&& other) {
  stb_image_data_ = other.stb_image_data_;
  other.stb_image_data_ = NULL;

  x_ = other.x_;
  y_ = other.y_;
  channel_count_ = other.channel_count_;
}

STBImage& STBImage::operator=(STBImage&& other) {
  if (stb_image_data_ != NULL) {
    stbi_image_free(stb_image_data_);
  }

  stb_image_data_ = other.stb_image_data_;
  other.stb_image_data_ = NULL;

  x_ = other.x_;
  y_ = other.y_;
  channel_count_ = other.channel_count_;
  return *this;
}