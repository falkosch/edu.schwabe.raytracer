#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(ShiftTest) {
  public:
    // -- shiftLeft32 --

    TEST_METHOD(shiftLeft32PerLane128) {
      const auto a = _mm_set_epi32(8, 4, 2, 1);
      const auto b = _mm_set_epi32(3, 2, 1, 0);
      alignas(XMM_ALIGNMENT) std::array<Int_32, VectorSizes::W> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftLeft32(a, b));
      Assert::AreEqual(Int_32{1}, actual[0], L"1<<0", LINE_INFO());
      Assert::AreEqual(Int_32{4}, actual[1], L"2<<1", LINE_INFO());
      Assert::AreEqual(Int_32{16}, actual[2], L"4<<2", LINE_INFO());
      Assert::AreEqual(Int_32{64}, actual[3], L"8<<3", LINE_INFO());
    }

    TEST_METHOD(shiftLeft32PerLane256) {
      const auto a = _mm256_set_epi32(8, 7, 6, 5, 4, 3, 2, 1);
      const auto b = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);
      alignas(YMM_ALIGNMENT) std::array<Int_32, VectorSizes::X8> actual{};
      _mm256_store_si256(reinterpret_cast<__m256i *>(actual.data()), shiftLeft32(a, b));
      Assert::AreEqual(Int_32{1 << 7}, actual[0], L"1<<7", LINE_INFO());
      Assert::AreEqual(Int_32{2 << 6}, actual[1], L"2<<6", LINE_INFO());
      Assert::AreEqual(Int_32{3 << 5}, actual[2], L"3<<5", LINE_INFO());
      Assert::AreEqual(Int_32{4 << 4}, actual[3], L"4<<4", LINE_INFO());
      Assert::AreEqual(Int_32{5 << 3}, actual[4], L"5<<3", LINE_INFO());
      Assert::AreEqual(Int_32{6 << 2}, actual[5], L"6<<2", LINE_INFO());
      Assert::AreEqual(Int_32{7 << 1}, actual[6], L"7<<1", LINE_INFO());
      Assert::AreEqual(Int_32{8}, actual[7], L"8<<0", LINE_INFO());
    }

    TEST_METHOD(shiftLeft32ScalarUniform128) {
      const auto a = _mm_set_epi32(4, 3, 2, 1);
      alignas(XMM_ALIGNMENT) std::array<Int_32, VectorSizes::W> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftLeft32(a, Int_32{2}));
      Assert::AreEqual(Int_32{4}, actual[0], L"1<<2", LINE_INFO());
      Assert::AreEqual(Int_32{8}, actual[1], L"2<<2", LINE_INFO());
      Assert::AreEqual(Int_32{12}, actual[2], L"3<<2", LINE_INFO());
      Assert::AreEqual(Int_32{16}, actual[3], L"4<<2", LINE_INFO());
    }

    TEST_METHOD(shiftLeft32ScalarUniform128Unsigned) {
      const auto a = _mm_set_epi32(4, 3, 2, 1);
      alignas(XMM_ALIGNMENT) std::array<Int_32, VectorSizes::W> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftLeft32(a, UInt_32{3}));
      Assert::AreEqual(Int_32{8}, actual[0], L"1<<3", LINE_INFO());
      Assert::AreEqual(Int_32{16}, actual[1], L"2<<3", LINE_INFO());
      Assert::AreEqual(Int_32{24}, actual[2], L"3<<3", LINE_INFO());
      Assert::AreEqual(Int_32{32}, actual[3], L"4<<3", LINE_INFO());
    }

    TEST_METHOD(shiftLeft32ScalarUniform256) {
      const auto a = _mm256_set_epi32(8, 7, 6, 5, 4, 3, 2, 1);
      alignas(YMM_ALIGNMENT) std::array<Int_32, VectorSizes::X8> actual{};
      _mm256_store_si256(reinterpret_cast<__m256i *>(actual.data()), shiftLeft32(a, Int_32{1}));
      Assert::AreEqual(Int_32{2}, actual[0], L"1<<1", LINE_INFO());
      Assert::AreEqual(Int_32{4}, actual[1], L"2<<1", LINE_INFO());
      Assert::AreEqual(Int_32{16}, actual[7], L"8<<1", LINE_INFO());
    }

    TEST_METHOD(shiftLeft32ByZero) {
      const auto a = _mm_set_epi32(4, 3, 2, 1);
      const auto zero = _mm_setzero_si128();
      alignas(XMM_ALIGNMENT) std::array<Int_32, VectorSizes::W> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftLeft32(a, zero));
      Assert::AreEqual(Int_32{1}, actual[0], L"unchanged", LINE_INFO());
      Assert::AreEqual(Int_32{2}, actual[1], L"unchanged", LINE_INFO());
      Assert::AreEqual(Int_32{3}, actual[2], L"unchanged", LINE_INFO());
      Assert::AreEqual(Int_32{4}, actual[3], L"unchanged", LINE_INFO());
    }

    // -- shiftRightLogical32 --

    TEST_METHOD(shiftRightLogical32PerLane128) {
      const auto a = _mm_set_epi32(64, 16, 4, 1);
      const auto b = _mm_set_epi32(3, 2, 1, 0);
      alignas(XMM_ALIGNMENT) std::array<UInt_32, VectorSizes::W> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftRightLogical32(a, b));
      Assert::AreEqual(UInt_32{1}, actual[0], L"1>>0", LINE_INFO());
      Assert::AreEqual(UInt_32{2}, actual[1], L"4>>1", LINE_INFO());
      Assert::AreEqual(UInt_32{4}, actual[2], L"16>>2", LINE_INFO());
      Assert::AreEqual(UInt_32{8}, actual[3], L"64>>3", LINE_INFO());
    }

    TEST_METHOD(shiftRightLogical32ZeroFillsOnHighBit) {
      const auto a = _mm_set1_epi32(static_cast<Int_32>(0x80000000u));
      const auto b = _mm_set_epi32(4, 3, 2, 1);
      alignas(XMM_ALIGNMENT) std::array<UInt_32, VectorSizes::W> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftRightLogical32(a, b));
      Assert::AreEqual(UInt_32{0x40000000u}, actual[0], L"0x80000000>>1 zero-fills", LINE_INFO());
      Assert::AreEqual(UInt_32{0x20000000u}, actual[1], L"0x80000000>>2 zero-fills", LINE_INFO());
      Assert::AreEqual(UInt_32{0x10000000u}, actual[2], L"0x80000000>>3 zero-fills", LINE_INFO());
      Assert::AreEqual(UInt_32{0x08000000u}, actual[3], L"0x80000000>>4 zero-fills", LINE_INFO());
    }

    TEST_METHOD(shiftRightLogical32PerLane256) {
      const auto a = _mm256_set_epi32(256, 128, 64, 32, 16, 8, 4, 2);
      const auto b = _mm256_set_epi32(1, 1, 1, 1, 1, 1, 1, 1);
      alignas(YMM_ALIGNMENT) std::array<UInt_32, VectorSizes::X8> actual{};
      _mm256_store_si256(reinterpret_cast<__m256i *>(actual.data()), shiftRightLogical32(a, b));
      Assert::AreEqual(UInt_32{1}, actual[0], L"2>>1", LINE_INFO());
      Assert::AreEqual(UInt_32{128}, actual[7], L"256>>1", LINE_INFO());
    }

    TEST_METHOD(shiftRightLogical32ScalarUniform128) {
      const auto a = _mm_set_epi32(16, 8, 4, 2);
      alignas(XMM_ALIGNMENT) std::array<UInt_32, VectorSizes::W> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftRightLogical32(a, Int_32{1}));
      Assert::AreEqual(UInt_32{1}, actual[0], L"2>>1", LINE_INFO());
      Assert::AreEqual(UInt_32{2}, actual[1], L"4>>1", LINE_INFO());
      Assert::AreEqual(UInt_32{4}, actual[2], L"8>>1", LINE_INFO());
      Assert::AreEqual(UInt_32{8}, actual[3], L"16>>1", LINE_INFO());
    }

    // -- shiftRightArithmetic32 --

    TEST_METHOD(shiftRightArithmetic32PerLane128SignExtends) {
      const auto a = _mm_set_epi32(-64, -32, -16, -8);
      const auto b = _mm_set_epi32(4, 3, 2, 1);
      alignas(XMM_ALIGNMENT) std::array<Int_32, VectorSizes::W> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftRightArithmetic32(a, b));
      Assert::AreEqual(Int_32{-4}, actual[0], L"-8>>1 sign-extends", LINE_INFO());
      Assert::AreEqual(Int_32{-4}, actual[1], L"-16>>2 sign-extends", LINE_INFO());
      Assert::AreEqual(Int_32{-4}, actual[2], L"-32>>3 sign-extends", LINE_INFO());
      Assert::AreEqual(Int_32{-4}, actual[3], L"-64>>4 sign-extends", LINE_INFO());
    }

    TEST_METHOD(shiftRightArithmetic32PerLane128Positive) {
      const auto a = _mm_set_epi32(64, 32, 16, 8);
      const auto b = _mm_set_epi32(4, 3, 2, 1);
      alignas(XMM_ALIGNMENT) std::array<Int_32, VectorSizes::W> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftRightArithmetic32(a, b));
      Assert::AreEqual(Int_32{4}, actual[0], L"8>>1", LINE_INFO());
      Assert::AreEqual(Int_32{4}, actual[1], L"16>>2", LINE_INFO());
      Assert::AreEqual(Int_32{4}, actual[2], L"32>>3", LINE_INFO());
      Assert::AreEqual(Int_32{4}, actual[3], L"64>>4", LINE_INFO());
    }

    TEST_METHOD(shiftRightArithmetic32PerLane256SignExtends) {
      const auto a = _mm256_set1_epi32(-128);
      const auto b = _mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0);
      alignas(YMM_ALIGNMENT) std::array<Int_32, VectorSizes::X8> actual{};
      _mm256_store_si256(reinterpret_cast<__m256i *>(actual.data()), shiftRightArithmetic32(a, b));
      Assert::AreEqual(Int_32{-128}, actual[0], L"-128>>0", LINE_INFO());
      Assert::AreEqual(Int_32{-64}, actual[1], L"-128>>1", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, actual[7], L"-128>>7", LINE_INFO());
    }

    TEST_METHOD(shiftRightArithmetic32ScalarUniform128) {
      const auto a = _mm_set_epi32(-16, -8, 8, 16);
      alignas(XMM_ALIGNMENT) std::array<Int_32, VectorSizes::W> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftRightArithmetic32(a, Int_32{1}));
      Assert::AreEqual(Int_32{8}, actual[0], L"16>>1", LINE_INFO());
      Assert::AreEqual(Int_32{4}, actual[1], L"8>>1", LINE_INFO());
      Assert::AreEqual(Int_32{-4}, actual[2], L"-8>>1 sign-extends", LINE_INFO());
      Assert::AreEqual(Int_32{-8}, actual[3], L"-16>>1 sign-extends", LINE_INFO());
    }

    // -- shiftLeft64 --

    TEST_METHOD(shiftLeft64PerLane128) {
      const auto a = _mm_set_epi64x(Int_64{3}, Int_64{1});
      const auto b = _mm_set_epi64x(Int_64{2}, Int_64{4});
      alignas(XMM_ALIGNMENT) std::array<Int_64, VectorSizes::X2> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftLeft64(a, b));
      Assert::AreEqual(Int_64{16}, actual[0], L"1<<4", LINE_INFO());
      Assert::AreEqual(Int_64{12}, actual[1], L"3<<2", LINE_INFO());
    }

    TEST_METHOD(shiftLeft64PerLane256) {
      const auto a = _mm256_set_epi64x(Int_64{1}, Int_64{2}, Int_64{3}, Int_64{4});
      const auto b = _mm256_set_epi64x(Int_64{3}, Int_64{2}, Int_64{1}, Int_64{0});
      alignas(YMM_ALIGNMENT) std::array<Int_64, VectorSizes::W> actual{};
      _mm256_store_si256(reinterpret_cast<__m256i *>(actual.data()), shiftLeft64(a, b));
      Assert::AreEqual(Int_64{4}, actual[0], L"4<<0", LINE_INFO());
      Assert::AreEqual(Int_64{6}, actual[1], L"3<<1", LINE_INFO());
      Assert::AreEqual(Int_64{8}, actual[2], L"2<<2", LINE_INFO());
      Assert::AreEqual(Int_64{8}, actual[3], L"1<<3", LINE_INFO());
    }

    TEST_METHOD(shiftLeft64ScalarUniform128) {
      const auto a = _mm_set_epi64x(Int_64{5}, Int_64{3});
      alignas(XMM_ALIGNMENT) std::array<Int_64, VectorSizes::X2> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftLeft64(a, Int_64{2}));
      Assert::AreEqual(Int_64{12}, actual[0], L"3<<2", LINE_INFO());
      Assert::AreEqual(Int_64{20}, actual[1], L"5<<2", LINE_INFO());
    }

    TEST_METHOD(shiftLeft64ScalarUniform256) {
      const auto a = _mm256_set_epi64x(Int_64{4}, Int_64{3}, Int_64{2}, Int_64{1});
      alignas(YMM_ALIGNMENT) std::array<Int_64, VectorSizes::W> actual{};
      _mm256_store_si256(reinterpret_cast<__m256i *>(actual.data()), shiftLeft64(a, Int_64{1}));
      Assert::AreEqual(Int_64{2}, actual[0], L"1<<1", LINE_INFO());
      Assert::AreEqual(Int_64{4}, actual[1], L"2<<1", LINE_INFO());
      Assert::AreEqual(Int_64{6}, actual[2], L"3<<1", LINE_INFO());
      Assert::AreEqual(Int_64{8}, actual[3], L"4<<1", LINE_INFO());
    }

    // -- shiftRightLogical64 --

    TEST_METHOD(shiftRightLogical64PerLane128) {
      const auto a = _mm_set_epi64x(Int_64{16}, Int_64{8});
      const auto b = _mm_set_epi64x(Int_64{2}, Int_64{1});
      alignas(XMM_ALIGNMENT) std::array<UInt_64, VectorSizes::X2> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftRightLogical64(a, b));
      Assert::AreEqual(UInt_64{4}, actual[0], L"8>>1", LINE_INFO());
      Assert::AreEqual(UInt_64{4}, actual[1], L"16>>2", LINE_INFO());
    }

    TEST_METHOD(shiftRightLogical64ZeroFillsOnHighBit) {
      const auto a = _mm_set1_epi64x(static_cast<Int_64>(0x8000000000000000ull));
      const auto b = _mm_set_epi64x(Int_64{2}, Int_64{1});
      alignas(XMM_ALIGNMENT) std::array<UInt_64, VectorSizes::X2> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftRightLogical64(a, b));
      Assert::AreEqual(UInt_64{0x4000000000000000ull}, actual[0], L"high-bit>>1 zero-fills", LINE_INFO());
      Assert::AreEqual(UInt_64{0x2000000000000000ull}, actual[1], L"high-bit>>2 zero-fills", LINE_INFO());
    }

    TEST_METHOD(shiftRightLogical64ScalarUniform128) {
      const auto a = _mm_set_epi64x(Int_64{32}, Int_64{16});
      alignas(XMM_ALIGNMENT) std::array<UInt_64, VectorSizes::X2> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftRightLogical64(a, Int_64{2}));
      Assert::AreEqual(UInt_64{4}, actual[0], L"16>>2", LINE_INFO());
      Assert::AreEqual(UInt_64{8}, actual[1], L"32>>2", LINE_INFO());
    }

    // -- shiftRightArithmetic64 --

    TEST_METHOD(shiftRightArithmetic64PerLane128SignExtends) {
      const auto a = _mm_set_epi64x(Int_64{-32}, Int_64{-8});
      const auto b = _mm_set_epi64x(Int_64{2}, Int_64{1});
      alignas(XMM_ALIGNMENT) std::array<Int_64, VectorSizes::X2> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftRightArithmetic64(a, b));
      Assert::AreEqual(Int_64{-4}, actual[0], L"-8>>1 sign-extends", LINE_INFO());
      Assert::AreEqual(Int_64{-8}, actual[1], L"-32>>2 sign-extends", LINE_INFO());
    }

    TEST_METHOD(shiftRightArithmetic64PerLane128Positive) {
      const auto a = _mm_set_epi64x(Int_64{32}, Int_64{8});
      const auto b = _mm_set_epi64x(Int_64{2}, Int_64{1});
      alignas(XMM_ALIGNMENT) std::array<Int_64, VectorSizes::X2> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftRightArithmetic64(a, b));
      Assert::AreEqual(Int_64{4}, actual[0], L"8>>1", LINE_INFO());
      Assert::AreEqual(Int_64{8}, actual[1], L"32>>2", LINE_INFO());
    }

    TEST_METHOD(shiftRightArithmetic64PerLane256SignExtends) {
      const auto a = _mm256_set_epi64x(Int_64{-64}, Int_64{-32}, Int_64{-16}, Int_64{-8});
      const auto b = _mm256_set_epi64x(Int_64{3}, Int_64{2}, Int_64{1}, Int_64{0});
      alignas(YMM_ALIGNMENT) std::array<Int_64, VectorSizes::W> actual{};
      _mm256_store_si256(reinterpret_cast<__m256i *>(actual.data()), shiftRightArithmetic64(a, b));
      Assert::AreEqual(Int_64{-8}, actual[0], L"-8>>0", LINE_INFO());
      Assert::AreEqual(Int_64{-8}, actual[1], L"-16>>1", LINE_INFO());
      Assert::AreEqual(Int_64{-8}, actual[2], L"-32>>2", LINE_INFO());
      Assert::AreEqual(Int_64{-8}, actual[3], L"-64>>3", LINE_INFO());
    }

    TEST_METHOD(shiftRightArithmetic64ScalarUniform128) {
      const auto a = _mm_set_epi64x(Int_64{-16}, Int_64{16});
      alignas(XMM_ALIGNMENT) std::array<Int_64, VectorSizes::X2> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftRightArithmetic64(a, Int_64{2}));
      Assert::AreEqual(Int_64{4}, actual[0], L"16>>2", LINE_INFO());
      Assert::AreEqual(Int_64{-4}, actual[1], L"-16>>2 sign-extends", LINE_INFO());
    }

    TEST_METHOD(shiftRightArithmetic64ScalarUniform256) {
      const auto a = _mm256_set_epi64x(Int_64{-128}, Int_64{-64}, Int_64{64}, Int_64{128});
      alignas(YMM_ALIGNMENT) std::array<Int_64, VectorSizes::W> actual{};
      _mm256_store_si256(reinterpret_cast<__m256i *>(actual.data()), shiftRightArithmetic64(a, Int_64{1}));
      Assert::AreEqual(Int_64{64}, actual[0], L"128>>1", LINE_INFO());
      Assert::AreEqual(Int_64{32}, actual[1], L"64>>1", LINE_INFO());
      Assert::AreEqual(Int_64{-32}, actual[2], L"-64>>1 sign-extends", LINE_INFO());
      Assert::AreEqual(Int_64{-64}, actual[3], L"-128>>1 sign-extends", LINE_INFO());
    }

    // -- over-width shifts --

    TEST_METHOD(shiftLeft32OverWidthZeros) {
      const auto a = _mm_set1_epi32(static_cast<Int_32>(0xFFFFFFFFu));
      const auto b = _mm_set_epi32(32, 33, 64, 100);
      alignas(XMM_ALIGNMENT) std::array<Int_32, VectorSizes::W> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftLeft32(a, b));
      for (int i = 0; i < 4; ++i)
        Assert::AreEqual(Int_32{0}, actual[i], L"left shift >= 32 must zero", LINE_INFO());
    }

    TEST_METHOD(shiftRightLogical32OverWidthZeros) {
      const auto a = _mm_set1_epi32(static_cast<Int_32>(0xFFFFFFFFu));
      const auto b = _mm_set_epi32(32, 33, 64, 100);
      alignas(XMM_ALIGNMENT) std::array<UInt_32, VectorSizes::W> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftRightLogical32(a, b));
      for (int i = 0; i < 4; ++i)
        Assert::AreEqual(UInt_32{0}, actual[i], L"logical right shift >= 32 must zero", LINE_INFO());
    }

    TEST_METHOD(shiftRightArithmetic32OverWidthFillsSignBit) {
      const auto neg = _mm_set1_epi32(-1);
      const auto pos = _mm_set1_epi32(42);
      const auto b = _mm_set_epi32(32, 33, 64, 100);
      alignas(XMM_ALIGNMENT) std::array<Int_32, VectorSizes::W> actualNeg{}, actualPos{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actualNeg.data()), shiftRightArithmetic32(neg, b));
      _mm_store_si128(reinterpret_cast<__m128i *>(actualPos.data()), shiftRightArithmetic32(pos, b));
      for (int i = 0; i < 4; ++i) {
        Assert::AreEqual(Int_32{-1}, actualNeg[i], L"arithmetic >> on negative fills sign bits", LINE_INFO());
        Assert::AreEqual(Int_32{0}, actualPos[i], L"arithmetic >> on positive fills zero", LINE_INFO());
      }
    }

    TEST_METHOD(shiftLeft64OverWidthZeros) {
      const auto a = _mm_set1_epi64x(static_cast<Int_64>(0xFFFFFFFFFFFFFFFFull));
      const auto b = _mm_set_epi64x(Int_64{64}, Int_64{100});
      alignas(XMM_ALIGNMENT) std::array<Int_64, VectorSizes::X2> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftLeft64(a, b));
      Assert::AreEqual(Int_64{0}, actual[0], L"left shift >= 64 must zero", LINE_INFO());
      Assert::AreEqual(Int_64{0}, actual[1], L"left shift >= 64 must zero", LINE_INFO());
    }

    TEST_METHOD(shiftRightLogical64OverWidthZeros) {
      const auto a = _mm_set1_epi64x(static_cast<Int_64>(0xFFFFFFFFFFFFFFFFull));
      const auto b = _mm_set_epi64x(Int_64{64}, Int_64{100});
      alignas(XMM_ALIGNMENT) std::array<UInt_64, VectorSizes::X2> actual{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actual.data()), shiftRightLogical64(a, b));
      Assert::AreEqual(UInt_64{0}, actual[0], L"logical right shift >= 64 must zero", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, actual[1], L"logical right shift >= 64 must zero", LINE_INFO());
    }

    TEST_METHOD(shiftRightArithmetic64OverWidthFillsSignBit) {
      const auto neg = _mm_set1_epi64x(Int_64{-1});
      const auto pos = _mm_set1_epi64x(Int_64{42});
      const auto b = _mm_set_epi64x(Int_64{64}, Int_64{100});
      alignas(XMM_ALIGNMENT) std::array<Int_64, VectorSizes::X2> actualNeg{}, actualPos{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actualNeg.data()), shiftRightArithmetic64(neg, b));
      _mm_store_si128(reinterpret_cast<__m128i *>(actualPos.data()), shiftRightArithmetic64(pos, b));
      Assert::AreEqual(Int_64{-1}, actualNeg[0], L"arithmetic >> on negative fills sign bits", LINE_INFO());
      Assert::AreEqual(Int_64{-1}, actualNeg[1], L"arithmetic >> on negative fills sign bits", LINE_INFO());
      Assert::AreEqual(Int_64{0}, actualPos[0], L"arithmetic >> on positive fills zero", LINE_INFO());
      Assert::AreEqual(Int_64{0}, actualPos[1], L"arithmetic >> on positive fills zero", LINE_INFO());
    }

    TEST_METHOD(shiftRightArithmetic64ScalarUniformOverWidth) {
      const auto neg = _mm_set1_epi64x(Int_64{-42});
      const auto pos = _mm_set1_epi64x(Int_64{42});
      alignas(XMM_ALIGNMENT) std::array<Int_64, VectorSizes::X2> actualNeg{}, actualPos{};
      _mm_store_si128(reinterpret_cast<__m128i *>(actualNeg.data()), shiftRightArithmetic64(neg, Int_64{64}));
      _mm_store_si128(reinterpret_cast<__m128i *>(actualPos.data()), shiftRightArithmetic64(pos, Int_64{64}));
      Assert::AreEqual(Int_64{-1}, actualNeg[0], L"scalar uniform >> 64 on negative fills sign bits", LINE_INFO());
      Assert::AreEqual(Int_64{0}, actualPos[0], L"scalar uniform >> 64 on positive fills zero", LINE_INFO());
    }
  };
}
