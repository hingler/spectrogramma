#include <gtest/gtest.h>
#include <FFT.hpp>

TEST(BitReverseTest, VerifyBitReversal) {
  uint32_t reverseVerify;
  uint32_t temp;
  uint32_t res;
  for (uint32_t i = 0; i < 1048576; i++) {
    temp = i;
    reverseVerify = 0;
    
    for (int j = 0; j < 32; j++) {
      reverseVerify <<= 1;
      reverseVerify |= (temp & 1);
      temp >>= 1;
    }

    res = FFT::BitReverse(i, 32);
    EXPECT_EQ(res, reverseVerify);
  }
}

static void testLen(uint32_t len) {
  uint32_t reverseVerify;
  uint32_t temp;
  uint32_t res;

  for (uint32_t i = 0; i < (1U << (len - 1)); i++) {
    temp = i;
    reverseVerify = 0;

    for (uint32_t j = 0; j < len; j++) {
      reverseVerify <<= 1;
      reverseVerify |= (temp & 1);
      temp >>= 1;
    }

    res = FFT::BitReverse(i, len);
    EXPECT_EQ(res, reverseVerify);
  }
}

TEST(BitReverseTest, VerifyBitReversalWithLen) {
  const uint32_t len = 18;
  for (int i = 2; i <= len; i++) {
    testLen(i);
  }
}