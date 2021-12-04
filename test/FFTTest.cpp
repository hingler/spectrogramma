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

TEST(FFTTest, ValuePreservation) {
  FFT test(512);

  float* data_real = new float[512];
  float the = 0.0f;
  for (int i = 0; i < 512; i++) {
    the = 0.0f;
    for (int j = 0; j < 32; j++) {
      the += sinf(static_cast<float>(j * j * i) * 3.141592653589f * 2.0f / 512.0f);
    }

    data_real[i] = the;
  }
  
  test.SetSampleData(data_real, NULL);
  FFT_data<float> result = test.InverseTransform_T();

  ASSERT_EQ(result.imag.size(), 512);
  ASSERT_EQ(result.real.size(), 512);

  test.SetSampleData(result.real.data(), result.imag.data());
  result = test.Transform_T();

  ASSERT_EQ(result.imag.size(), 512);
  ASSERT_EQ(result.real.size(), 512);

  for (int i = 0; i < 512; i++) {
    ASSERT_NEAR(data_real[i], result.real[i], 0.0001);
    ASSERT_NEAR(0.0, result.imag[i], 0.0001);
  }
}