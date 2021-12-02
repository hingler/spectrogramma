#include <gtest/gtest.h>
#include <FFT.hpp>

#define EPS 0.00001

const static float real_in[8] = {0, 1, 0, -1, 0, 1, 0, -1};
// imag_in  is 0
// real_out is 0
const static float imag_out[8] = {0, 0, -4, 0, 0, 0, 4, 0};

TEST(FFTTest, ForwardFFT) {
  FFT test(real_in, 8);
  float** res = test.Transform();
  for (int i = 0; i < 8; i++) {
    EXPECT_NEAR(res[0][i], 0, EPS);
    EXPECT_NEAR(res[1][i], imag_out[i], EPS);
  }
}

TEST(FFTTest, InverseFFT) {
  FFT test(NULL, imag_out, 8);
  float** res = test.InverseTransform();
  for (int i = 0; i < 8; i++) {
    EXPECT_NEAR(res[0][i], real_in[i], EPS);
    EXPECT_NEAR(res[1][i], 0, EPS);
  }
}