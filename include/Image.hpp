#ifndef IMAGE_H_
#define IMAGE_H_

#include <glm/glm.hpp>

/**
 * @brief Abstract class for images.
 * 
 */
struct Image {

  /**
   * @brief Get the Image Pointer object
   * 
   * @return const float* a pointer to this image's internal data.
   *                      Images provided in <height> scanlines of <width>, one float per color channel per pixel
   */
  virtual const float* GetImagePointer() = 0;
  virtual glm::ivec2 GetImageDimensions() = 0;
  virtual int GetChannelCount() = 0;
  virtual ~Image() {};
};

#endif // IMAGE_H_