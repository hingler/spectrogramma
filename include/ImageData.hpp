#ifndef IMAGE_DATA_H_
#define IMAGE_DATA_H_

#include <string>
#include <glm/glm.hpp>
#include <Image.hpp>

#include <memory>

enum WrapMode {
  CLAMP = 0,
  CLIP = 1,
  REPEAT = 2
};

class ImageData {
 private:
  // raw image data
  std::shared_ptr<Image> image_data_;
  int x_;
  int y_;
  int channel_count_;

 public:
  /**
   * @brief Construct a new Image Data object
   * 
   * @param path - path to desired file :D
   */
  ImageData(const std::string& path);

  /**
   * @brief Construct a new Image Data object from memory
   * 
   * @param ptr - pointer to raw image data
   * @param x - x dimension
   * @param y - y dimension
   * @param channel_count - number of channels per pixel
   */
  ImageData(const float* ptr, int x, int y, int channel_count);

  /**
   * @brief Construct a new Image Data object from memory, taking ownership of passed data arg.
   * 
   * @param data_move - pointer to raw image data
   * @param x - x dim
   * @param y - y dim
   * @param channel_count - num channels
   */
  ImageData(float*&& data_move, int x, int y, int channel_count);

  /**
   * @brief Perform linear sampling on image data.
   * 
   * @param x - x coordinate, relative to top left.
   * @param y - y coordinate, relative to top left.
   * @return glm::vec4 - RGBA color data.
   */
  glm::vec4 LinearSample(float x, float y) const;

  /**
   * @brief Perform linear sampling on image data.
   * 
   * @param coord - XY pixel coordinates, relative to top left.
   * @return glm::vec4 - RGBA color data.
   */
  glm::vec4 LinearSample(glm::vec2 coord) const;

  /**
   * @brief Perform nearest neighbor sampling on image data.
   * 
   * @param x - x pixel coord
   * @param y - y pixel coord
   * @return glm::vec4 - RGBA color data.
   */
  glm::vec4 NearestSample(float x, float y) const;

  /**
   * @brief Perform nearest neighbor sampling on image data.
   * 
   * @param coord - XY pixel coordinates, relative to top left.
   * @return glm::vec4 - RGBA color data.
   */
  glm::vec4 NearestSample(glm::vec2 coord) const;

  /**
   * @return glm::vec2 - image dimensions
   */
  glm::vec2 GetDimensions() const;

  const float* GetImageData() const {
    return image_data_->GetImagePointer();
  }

  // mode for sample wrap. defaults to WrapMode::REPEAT
  WrapMode mode;
};

#endif // IMAGE_DATA_H_