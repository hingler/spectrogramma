#ifndef IMAGE_PTR_H_
#define IMAGE_PTR_H_

#include <Image.hpp>

class ImagePtr : public Image {
 private:
  float* image_ptr_;
  int x_;
  int y_;
  int channel_count_;

 public:
  ImagePtr(const float* ptr, int x, int y, int channel_count);
  ImagePtr(float*&& data_move, int x, int y, int channel_count);
  const float* GetImagePointer() { return image_ptr_; };
  glm::ivec2 GetImageDimensions();
  int GetChannelCount();
  ~ImagePtr();

  ImagePtr(const ImagePtr& other);
  ImagePtr& operator=(const ImagePtr& other);

  ImagePtr(ImagePtr&& other);
  ImagePtr& operator=(ImagePtr&& other);
};

#endif // IMAGE_PTR_H_