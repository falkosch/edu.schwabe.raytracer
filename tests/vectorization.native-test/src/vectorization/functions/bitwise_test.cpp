#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(BitwiseTest) {
  public:
    // bitwiseNot — float types

    TEST_METHOD(bitwiseNotPackedFloat4_128) {
      const auto zero = _mm_setzero_ps();
      const auto notZero = bitwiseNot(zero);
      Assert::AreEqual(0xF, _mm_movemask_ps(notZero), L"~0 has all sign bits set", LINE_INFO());
      std::array<Float_32, VectorSizes::W> roundTrip{};
      _mm_store_ps(roundTrip.data(), bitwiseNot(notZero));
      for (ASizeT i = 0; i < roundTrip.size(); ++i) {
        Assert::AreEqual(0.0f, roundTrip[i], L"~~0 should be zero", LINE_INFO());
      }
    }

    TEST_METHOD(bitwiseNotPackedFloat2_128) {
      const auto zero = _mm_setzero_pd();
      const auto notZero = bitwiseNot(zero);
      Assert::AreEqual(0x3, _mm_movemask_pd(notZero), L"~0 has all sign bits set", LINE_INFO());
      std::array<Float_64, VectorSizes::X2> roundTrip{};
      _mm_store_pd(roundTrip.data(), bitwiseNot(notZero));
      for (ASizeT i = 0; i < roundTrip.size(); ++i) {
        Assert::AreEqual(0.0, roundTrip[i], L"~~0 should be zero", LINE_INFO());
      }
    }

    TEST_METHOD(bitwiseNotPackedFloat8_256) {
      const auto zero = _mm256_setzero_ps();
      const auto notZero = bitwiseNot(zero);
      Assert::AreEqual(0xFF, _mm256_movemask_ps(notZero), L"~0 has all sign bits set", LINE_INFO());
      alignas(YMM_ALIGNMENT) std::array<Float_32, VectorSizes::X8> roundTrip{};
      _mm256_store_ps(roundTrip.data(), bitwiseNot(notZero));
      for (ASizeT i = 0; i < roundTrip.size(); ++i) {
        Assert::AreEqual(0.0f, roundTrip[i], L"~~0 should be zero", LINE_INFO());
      }
    }

    TEST_METHOD(bitwiseNotPackedFloat4_256) {
      const auto zero = _mm256_setzero_pd();
      const auto notZero = bitwiseNot(zero);
      Assert::AreEqual(0xF, _mm256_movemask_pd(notZero), L"~0 has all sign bits set", LINE_INFO());
      alignas(YMM_ALIGNMENT) std::array<Float_64, VectorSizes::W> roundTrip{};
      _mm256_store_pd(roundTrip.data(), bitwiseNot(notZero));
      for (ASizeT i = 0; i < roundTrip.size(); ++i) {
        Assert::AreEqual(0.0, roundTrip[i], L"~~0 should be zero", LINE_INFO());
      }
    }

    TEST_METHOD(bitwiseNotIsInvolution) {
      const auto v = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), bitwiseNot(bitwiseNot(v)));
      Assert::AreEqual(1.0f, actual[0], L"double not restores x", LINE_INFO());
      Assert::AreEqual(2.0f, actual[1], L"double not restores y", LINE_INFO());
      Assert::AreEqual(3.0f, actual[2], L"double not restores z", LINE_INFO());
      Assert::AreEqual(4.0f, actual[3], L"double not restores w", LINE_INFO());
    }

    // bitwiseNot — integer types

    TEST_METHOD(bitwiseNotPackedInts128) {
      const auto zero = _mm_setzero_si128();
      const auto notZero = bitwiseNot(zero);
      std::array<Int_32, VectorSizes::W> actual{};
      _mm_storeu_si128(reinterpret_cast<__m128i *>(actual.data()), notZero);
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(Int_32{-1}, actual[i], L"~0 should be all-ones", LINE_INFO());
      }
    }

    TEST_METHOD(bitwiseNotPackedInts256) {
      const auto zero = _mm256_setzero_si256();
      const auto notZero = bitwiseNot(zero);
      alignas(YMM_ALIGNMENT) std::array<Int_32, VectorSizes::X8> actual{};
      _mm256_store_si256(reinterpret_cast<__m256i *>(actual.data()), notZero);
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(Int_32{-1}, actual[i], L"~0 should be all-ones", LINE_INFO());
      }
    }

    // bitwiseAnd — float types

    TEST_METHOD(bitwiseAndPackedFloat4_128) {
      const auto a = MaskAll<PackedFloat4_128>();
      const auto b = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), bitwiseAnd(a, b));
      Assert::AreEqual(1.0f, actual[0], L"all-ones AND x preserves x", LINE_INFO());
      Assert::AreEqual(2.0f, actual[1], L"all-ones AND y preserves y", LINE_INFO());
    }

    TEST_METHOD(bitwiseAndWithZeroIsZero) {
      const auto v = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      const auto zero = _mm_setzero_ps();
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), bitwiseAnd(v, zero));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(0.0f, actual[i], L"x AND 0 should be 0", LINE_INFO());
      }
    }

    TEST_METHOD(bitwiseAndPackedInts128) {
      const auto a = _mm_set_epi32(0xFF00, 0x00FF, 0xF0F0, 0x0F0F);
      const auto b = _mm_set_epi32(0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF);
      std::array<Int_32, VectorSizes::W> actual{};
      _mm_storeu_si128(reinterpret_cast<__m128i *>(actual.data()), bitwiseAnd(a, b));
      Assert::AreEqual(Int_32{0x0F0F}, actual[0], L"lane 0", LINE_INFO());
      Assert::AreEqual(static_cast<Int_32>(0xF0F0), actual[1], L"lane 1", LINE_INFO());
      Assert::AreEqual(Int_32{0x00FF}, actual[2], L"lane 2", LINE_INFO());
      Assert::AreEqual(static_cast<Int_32>(0xFF00), actual[3], L"lane 3", LINE_INFO());
    }

    TEST_METHOD(bitwiseAndPackedInts256) {
      const auto a = _mm256_set1_epi32(0x0F0F0F0F);
      const auto b = _mm256_set1_epi32(static_cast<Int_32>(0xF0F0F0F0));
      alignas(YMM_ALIGNMENT) std::array<Int_32, VectorSizes::X8> actual{};
      _mm256_store_si256(reinterpret_cast<__m256i *>(actual.data()), bitwiseAnd(a, b));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(Int_32{0}, actual[i], L"disjoint bits AND to zero", LINE_INFO());
      }
    }

    // bitwiseOr

    TEST_METHOD(bitwiseOrPackedFloat4_128WithZero) {
      const auto v = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      const auto zero = _mm_setzero_ps();
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), bitwiseOr(v, zero));
      Assert::AreEqual(1.0f, actual[0], L"x OR 0 preserves x", LINE_INFO());
      Assert::AreEqual(2.0f, actual[1], L"y OR 0 preserves y", LINE_INFO());
    }

    TEST_METHOD(bitwiseOrPackedInts128) {
      const auto a = _mm_set1_epi32(0x0F0F0F0F);
      const auto b = _mm_set1_epi32(static_cast<Int_32>(0xF0F0F0F0));
      std::array<Int_32, VectorSizes::W> actual{};
      _mm_storeu_si128(reinterpret_cast<__m128i *>(actual.data()), bitwiseOr(a, b));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(Int_32{-1}, actual[i], L"complementary bits OR to all-ones", LINE_INFO());
      }
    }

    TEST_METHOD(bitwiseOrPackedInts256) {
      const auto a = _mm256_set1_epi32(0x0F0F0F0F);
      const auto b = _mm256_set1_epi32(static_cast<Int_32>(0xF0F0F0F0));
      alignas(YMM_ALIGNMENT) std::array<Int_32, VectorSizes::X8> actual{};
      _mm256_store_si256(reinterpret_cast<__m256i *>(actual.data()), bitwiseOr(a, b));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(Int_32{-1}, actual[i], L"complementary bits OR to all-ones", LINE_INFO());
      }
    }

    // bitwiseXor

    TEST_METHOD(bitwiseXorWithSelfIsZero) {
      const auto v = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), bitwiseXor(v, v));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(0.0f, actual[i], L"x XOR x should be 0", LINE_INFO());
      }
    }

    TEST_METHOD(bitwiseXorPackedInts128) {
      const auto a = _mm_set1_epi32(0x0F0F0F0F);
      const auto b = _mm_set1_epi32(static_cast<Int_32>(0xF0F0F0F0));
      std::array<Int_32, VectorSizes::W> actual{};
      _mm_storeu_si128(reinterpret_cast<__m128i *>(actual.data()), bitwiseXor(a, b));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(Int_32{-1}, actual[i], L"complementary bits XOR to all-ones", LINE_INFO());
      }
    }

    TEST_METHOD(bitwiseXorPackedInts256WithSelf) {
      const auto v = _mm256_set_epi32(8, 7, 6, 5, 4, 3, 2, 1);
      alignas(YMM_ALIGNMENT) std::array<Int_32, VectorSizes::X8> actual{};
      _mm256_store_si256(reinterpret_cast<__m256i *>(actual.data()), bitwiseXor(v, v));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(Int_32{0}, actual[i], L"x XOR x should be 0", LINE_INFO());
      }
    }

    // bitwiseAndNot: (~a) & b

    TEST_METHOD(bitwiseAndNotPackedFloat4_128) {
      const auto zero = _mm_setzero_ps();
      const auto v = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), bitwiseAndNot(zero, v));
      Assert::AreEqual(1.0f, actual[0], L"(~0) & x preserves x", LINE_INFO());
      Assert::AreEqual(2.0f, actual[1], L"(~0) & y preserves y", LINE_INFO());
      Assert::AreEqual(3.0f, actual[2], L"(~0) & z preserves z", LINE_INFO());
      Assert::AreEqual(4.0f, actual[3], L"(~0) & w preserves w", LINE_INFO());
    }

    TEST_METHOD(bitwiseAndNotPackedInts128) {
      const auto mask = _mm_set1_epi32(0x0F0F0F0F);
      const auto value = _mm_set1_epi32(static_cast<Int_32>(0xFFFFFFFF));
      std::array<Int_32, VectorSizes::W> actual{};
      _mm_storeu_si128(reinterpret_cast<__m128i *>(actual.data()), bitwiseAndNot(mask, value));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(static_cast<Int_32>(0xF0F0F0F0), actual[i], L"(~0x0F) & 0xFF = 0xF0", LINE_INFO());
      }
    }

    TEST_METHOD(bitwiseAndNotPackedInts256) {
      const auto mask = _mm256_set1_epi32(0x0F0F0F0F);
      const auto value = _mm256_set1_epi32(static_cast<Int_32>(0xFFFFFFFF));
      alignas(YMM_ALIGNMENT) std::array<Int_32, VectorSizes::X8> actual{};
      _mm256_store_si256(reinterpret_cast<__m256i *>(actual.data()), bitwiseAndNot(mask, value));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(static_cast<Int_32>(0xF0F0F0F0), actual[i], L"(~0x0F) & 0xFF = 0xF0", LINE_INFO());
      }
    }

    // 256-bit float types

    TEST_METHOD(bitwiseAndPackedFloat8_256WithZero) {
      const auto v = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
      const auto zero = _mm256_setzero_ps();
      alignas(YMM_ALIGNMENT) std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), bitwiseAnd(v, zero));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(0.0f, actual[i], L"x AND 0 should be 0", LINE_INFO());
      }
    }

    TEST_METHOD(bitwiseOrPackedFloat4_256WithZero) {
      const auto v = _mm256_set_pd(4.0, 3.0, 2.0, 1.0);
      const auto zero = _mm256_setzero_pd();
      alignas(YMM_ALIGNMENT) std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), bitwiseOr(v, zero));
      Assert::AreEqual(1.0, actual[0], L"x OR 0 preserves x", LINE_INFO());
      Assert::AreEqual(2.0, actual[1], L"y OR 0 preserves y", LINE_INFO());
      Assert::AreEqual(3.0, actual[2], L"z OR 0 preserves z", LINE_INFO());
      Assert::AreEqual(4.0, actual[3], L"w OR 0 preserves w", LINE_INFO());
    }

    TEST_METHOD(bitwiseXorPackedFloat8_256WithSelf) {
      const auto v = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
      alignas(YMM_ALIGNMENT) std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), bitwiseXor(v, v));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(0.0f, actual[i], L"x XOR x should be 0", LINE_INFO());
      }
    }

    TEST_METHOD(bitwiseXorPackedFloat4_256WithSelf) {
      const auto v = _mm256_set_pd(4.0, 3.0, 2.0, 1.0);
      alignas(YMM_ALIGNMENT) std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), bitwiseXor(v, v));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(0.0, actual[i], L"x XOR x should be 0", LINE_INFO());
      }
    }
  };
}
