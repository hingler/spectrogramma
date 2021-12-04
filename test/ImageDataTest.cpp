#include <gtest/gtest.h>
#include <ImageData.hpp>

#include <iostream>

#define EPS 0.00001

TEST(ImageDataTest, NearestSample) {
  ImageData temp("./res/test/4x4test.png");
  for (int i = 0; i < 16; i++) {
    int x = (i % 4);
    int y = (i / 4) % 4;
    glm::vec4 sample = temp.NearestSample(x + 0.5f, y + 0.5f);
    float contrast = ((x + y) % 2) == 0 ? 0.0f : 1.0f;
    for (int j = 0; j < 3; j++) {
      ASSERT_NEAR(contrast, sample[j], EPS);
    }

    ASSERT_NEAR(1.0, sample[3], EPS);

    std::cout << i << std::endl;
    sample = temp.NearestSample(x + 0.15f, y + 0.85f);
    for (int j = 0; j < 3; j++) {
      ASSERT_NEAR(contrast, sample[j], EPS);
    }

    ASSERT_NEAR(1.0, sample[3], EPS);
  }
}

TEST(ImageDataTest, WrapTest) {
  ImageData temp("./res/test/4x4test.png");
  temp.mode = WrapMode::REPEAT;
  for (int i = 0; i < 16; i++) {
    glm::vec4 sample = temp.NearestSample(0.5, static_cast<float>(i) + 0.5f);
    for (int j = 0; j < 3; j++) {
      ASSERT_NEAR((i % 2), sample[j], EPS);
    }

    ASSERT_NEAR(1.0, sample[3], EPS);
  }

  for (int i = 0; i < 16; i++) {
    glm::vec4 sample = temp.NearestSample(static_cast<float>(i) + 0.5f, 0.5f);
    for (int j = 0; j < 3; j++) {
      ASSERT_NEAR((i % 2), sample[j], EPS);
    }

    ASSERT_NEAR(1.0, sample[3], EPS);
  }
}

TEST(ImageDataTest, LinearSampleOnPixel) {
  ImageData temp("./res/test/4x4test.png");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      glm::vec2 coord(static_cast<float>(i) + 0.5f, static_cast<float>(j) + 0.5f);
      glm::vec4 linear = temp.LinearSample(coord);
      glm::vec4 nearest = temp.NearestSample(coord);
      for (int k = 0; k < 4; k++) {
        ASSERT_NEAR(linear[k], nearest[k], EPS);
      }
    }
  }
}

TEST(ImageDataTest, LinearSampleWrapping) {
  ImageData temp("./res/test/4x4test.png");

  for (int i = 0; i < 32; i++) {
    glm::vec2 coord(0.5 + static_cast<float>(i) * 0.5f, 0.5);
    glm::vec4 sample = temp.LinearSample(coord);
    float desired_color = (i % 2 == 1 ? 0.5f : (i % 4 == 0) ? 0.0f : 1.0f);
    for (int k = 0; k < 3; k++) {
      ASSERT_NEAR(sample[k], desired_color, EPS);
    }

    ASSERT_NEAR(sample[3], 1.0, EPS);

    // x and y dirs
    coord = glm::vec2(0.5f, 0.5f + static_cast<float>(i) * 0.5f);
    sample = temp.LinearSample(coord);
    for (int k = 0; k < 3; k++) {
      ASSERT_NEAR(sample[k], desired_color, EPS);
    }

    ASSERT_NEAR(sample[3], 1.0, EPS);
  }
}

TEST(ImageDataTest, SubPixelSampling) {
  ImageData temp("./res/test/4x4test.png");

  for (int i = 0; i < 32; i++) {
    glm::vec2 coord(0.5, 0.5 + (static_cast<float>(i) / 32.0f));
    glm::vec4 sample = temp.LinearSample(coord);
    for (int k = 0; k < 3; k++) {
      ASSERT_NEAR(sample[k], static_cast<float>(i) / 32.0f, EPS);
    }

    ASSERT_NEAR(sample[3], 1.0, EPS);
  }
}