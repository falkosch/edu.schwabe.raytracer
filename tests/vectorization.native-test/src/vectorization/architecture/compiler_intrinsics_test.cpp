#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(CompilerIntrinsicsTest) {
  public:
    TEST_METHOD(hasFixedSizeTypes) {
      Assert::AreEqual(
          ASizeT{8}, sizeof(Int_8) * ASizeT{std::numeric_limits<UInt_8>::digits}, L"wrong fixed size", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{8}, sizeof(UInt_8) * ASizeT{std::numeric_limits<UInt_8>::digits}, L"wrong fixed size", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{16}, sizeof(Int_16) * ASizeT{std::numeric_limits<UInt_8>::digits}, L"wrong fixed size", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{16}, sizeof(UInt_16) * ASizeT{std::numeric_limits<UInt_8>::digits}, L"wrong fixed size", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{32}, sizeof(Int_32) * ASizeT{std::numeric_limits<UInt_8>::digits}, L"wrong fixed size", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{32}, sizeof(UInt_32) * ASizeT{std::numeric_limits<UInt_8>::digits}, L"wrong fixed size", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{64}, sizeof(Int_64) * ASizeT{std::numeric_limits<UInt_8>::digits}, L"wrong fixed size", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{64}, sizeof(UInt_64) * ASizeT{std::numeric_limits<UInt_8>::digits}, L"wrong fixed size", LINE_INFO()
      );

      Assert::AreEqual(
          ASizeT{32}, sizeof(Float_32) * ASizeT{std::numeric_limits<UInt_8>::digits}, L"wrong fixed size", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{64}, sizeof(Float_64) * ASizeT{std::numeric_limits<UInt_8>::digits}, L"wrong fixed size", LINE_INFO()
      );

      Assert::AreEqual(
          ASizeT{128}, sizeof(PackedInts_128) * ASizeT{std::numeric_limits<UInt_8>::digits}, L"wrong fixed size",
          LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{128}, sizeof(PackedFloat4_128) * ASizeT{std::numeric_limits<UInt_8>::digits}, L"wrong fixed size",
          LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{128}, sizeof(PackedFloat2_128) * ASizeT{std::numeric_limits<UInt_8>::digits}, L"wrong fixed size",
          LINE_INFO()
      );

      Assert::AreEqual(
          ASizeT{256}, sizeof(PackedInts_256) * ASizeT{std::numeric_limits<UInt_8>::digits}, L"wrong fixed size",
          LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{256}, sizeof(PackedFloat8_256) * ASizeT{std::numeric_limits<UInt_8>::digits}, L"wrong fixed size",
          LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{256}, sizeof(PackedFloat4_256) * ASizeT{std::numeric_limits<UInt_8>::digits}, L"wrong fixed size",
          LINE_INFO()
      );
    }

    TEST_METHOD(hasSignedTypes) {
      Assert::IsTrue(std::is_signed_v<Int_8>, L"type should be signed", LINE_INFO());
      Assert::IsTrue(std::is_signed_v<Int_16>, L"type should be signed", LINE_INFO());
      Assert::IsTrue(std::is_signed_v<Int_32>, L"type should be signed", LINE_INFO());
      Assert::IsTrue(std::is_signed_v<Int_64>, L"type should be signed", LINE_INFO());

      Assert::IsTrue(std::is_signed_v<Float_32>, L"type should be signed", LINE_INFO());
      Assert::IsTrue(std::is_signed_v<Float_64>, L"type should be signed", LINE_INFO());

      Assert::IsTrue(std::is_signed_v<Bool_8>, L"type should be signed", LINE_INFO());
      Assert::IsTrue(std::is_signed_v<Bool_16>, L"type should be signed", LINE_INFO());
      Assert::IsTrue(std::is_signed_v<Bool_32>, L"type should be signed", LINE_INFO());
      Assert::IsTrue(std::is_signed_v<Bool_64>, L"type should be signed", LINE_INFO());
    }

    TEST_METHOD(hasUnsignedTypes) {
      Assert::IsFalse(std::is_signed_v<ASizeT>, L"type shouldn't be signed", LINE_INFO());

      Assert::IsFalse(std::is_signed_v<UInt_8>, L"type shouldn't be signed", LINE_INFO());
      Assert::IsFalse(std::is_signed_v<UInt_16>, L"type shouldn't be signed", LINE_INFO());
      Assert::IsFalse(std::is_signed_v<UInt_32>, L"type shouldn't be signed", LINE_INFO());
      Assert::IsFalse(std::is_signed_v<UInt_64>, L"type shouldn't be signed", LINE_INFO());
    }

    TEST_METHOD(generatesMasksForInt4_32) {
      Assert::AreEqual(
          0 + 0 + 0 + 0 + 0 + 0 + 0 + 0, BLEND_MASK_INT4_32(0, 0, 0, 0), L"  0 != MASK(0,0,0,0)", LINE_INFO()
      );

      Assert::AreEqual(
          1 + 2 + 0 + 0 + 16 + 32 + 0 + 0, BLEND_MASK_INT4_32(1, 0, 1, 0), L" 51 != MASK(1,0,1,0)", LINE_INFO()
      );

      Assert::AreEqual(
          0 + 0 + 4 + 8 + 0 + 0 + 64 + 128, BLEND_MASK_INT4_32(0, 1, 0, 1), L"204 != MASK(0,1,0,1)", LINE_INFO()
      );

      Assert::AreEqual(
          1 + 2 + 4 + 8 + 16 + 32 + 64 + 128, BLEND_MASK_INT4_32(1, 1, 1, 1), L"255 != MASK(1,1,1,1)", LINE_INFO()
      );
    }

    TEST_METHOD(convertsPackedUInt_32ToFloat32) {
      constexpr std::array<UInt_32, VectorSizes::W> given{0, 1, 2, 3};
      const auto givenPacked = _mm_load_si128(given.data());
      constexpr std::array expected{0.0f, 1.0f, 2.0f, 3.0f};

      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), vectorization::_mm_cvtepu32_ps(givenPacked));

      Assert::AreEqual(expected, actual, L"conversion mismatch", LINE_INFO());
    }
  };
}
