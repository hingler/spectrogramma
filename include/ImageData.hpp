#ifndef IMAGE_DATA_H_
#define IMAGE_DATA_H_

#include <string>
#include <glm/glm.hpp>

enum WrapMode {
  CLAMP = 0,
  CLIP = 1,
  REPEAT = 2
};

class ImageData {
 private:
  // raw image data
  float* image_data_;
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
   * @brief Perform linear sampling on image data.
   * 
   * @param x - x coordinate, relative to top left.
   * @param y - y coordinate, relative to top left.
   * @return glm::vec4 - RGBA color data.
   */
  glm::vec4 LinearSample(float x, float y);

  /**
   * @brief Perform linear sampling on image data.
   * 
   * @param coord - XY pixel coordinates, relative to top left.
   * @return glm::vec4 - RGBA color data.
   */
  glm::vec4 LinearSample(glm::vec2 coord);

  /**
   * @brief Perform nearest neighbor sampling on image data.
   * 
   * @param x - x pixel coord
   * @param y - y pixel coord
   * @return glm::vec4 - RGBA color data.
   */
  glm::vec4 NearestSample(float x, float y);

  /**
   * @brief Perform nearest neighbor sampling on image data.
   * 
   * @param coord - XY pixel coordinates, relative to top left.
   * @return glm::vec4 - RGBA color data.
   */
  glm::vec4 NearestSample(glm::vec2 coord);

  /**
   * @return glm::vec2 - image dimensions
   */
  glm::vec2 GetDimensions();

  // mode for sample wrap. defaults to WrapMode::REPEAT
  WrapMode mode;

  ImageData(const ImageData& other) = delete;
  ImageData(ImageData&& other);

  ImageData& operator=(const ImageData& other) = delete;
  ImageData& operator=(ImageData&& other);

  // todo: fetch dimensions

  ~ImageData();
};

#endif // IMAGE_DATA_H_