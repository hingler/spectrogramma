#ifndef STB_IMAGE_H_
#define STB_IMAGE_H_

#include <memory>
#include <string>
#include <Image.hpp>

class STBImage : public Image {
 private:
  float* stb_image_data_;

  int x_;
  int y_;
  int channel_count_;

 public:
  STBImage(const std::string& path);
  const float* GetImagePointer() { return stb_image_data_; };
  glm::ivec2 GetImageDimensions();
  int GetChannelCount();
  ~STBImage();

  STBImage(const STBImage& other) = delete;
  STBImage& operator=(const STBImage& other) = delete;

  STBImage(STBImage&& other);
  STBImage& operator=(STBImage&& other);
};

#endif // STB_IMAGE_H_