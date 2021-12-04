#include <ImagePtr.hpp>
#include <Macro.hpp>

#include <stdexcept>

ImagePtr::ImagePtr(const float* ptr, int x, int y, int channel_count) {
  image_ptr_ = new float[x * y * channel_count];
  if (image_ptr_ == NULL) {
    throw std::runtime_error("Not enough memory could be allocated for ImagePtr");
  }

  DEBUG_LOG("COPY FLOAT DATA CTOR IMAGEPTR");

  x_ = x;
  y_ = y;
  channel_count_ = channel_count;

  DEBUG_LOG(x * y * channel_count);

  memcpy(image_ptr_, ptr, x * y * channel_count * sizeof(float));
}

ImagePtr::ImagePtr(float*&& data_move, int x, int y, int channel_count) {
  DEBUG_LOG("MOVE FLOAT DATA CTOR IMAGEPTR");

  image_ptr_ = data_move;
  x_ = x;
  y_ = y;
  channel_count_ = channel_count;

  DEBUG_LOG(x * y * channel_count);
}

glm::ivec2 ImagePtr::GetImageDimensions() {
  return glm::ivec2(x_, y_);
}

int ImagePtr::GetChannelCount() {
  return channel_count_;
}

ImagePtr::~ImagePtr() {
  FREE_IF_NONNULL_A(image_ptr_);
}