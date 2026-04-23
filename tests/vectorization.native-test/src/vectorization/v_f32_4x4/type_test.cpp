#include "vectorization_tests.h"

#include <array>

namespace vectorization::test {
  TEST_CLASS(v_f32_4x4_TypeTest) {
  public:
    TEST_METHOD(definesArchitecture) {
      Assert::AreEqual(ASizeT{4}, v_f32_4x4::SIZE_SOA, L"SIZE_SOA mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{4}, v_f32_4x4::SIZE_AOS, L"SIZE_AOS mismatch", LINE_INFO());
      Assert::AreEqual(ASizeT{16}, v_f32_4x4::SIZE, L"SIZE mismatch", LINE_INFO());

      Assert::IsTrue(std::is_same_v<v_f32_4, v_f32_4x4::SOAVectorType>, L"SOA vector type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_f32_4, v_f32_4x4::AOSVectorType>, L"AOS vector type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_f32_4::PackedType, v_f32_4x4::PackedType>, L"Packed type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_f32_4::ValueType, v_f32_4x4::ValueType>, L"Value type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_f32_4::BoolType, v_f32_4x4::BoolType>, L"Bool type mismatch", LINE_INFO());
    }

    TEST_METHOD(defaultConstructsAllZero) {
      const v_f32_4x4 actual{};

      Assert::IsTrue(allTrue(actual.soaX == v_f32_4{0.0f}), L"soaX should be zero", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaY == v_f32_4{0.0f}), L"soaY should be zero", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaZ == v_f32_4{0.0f}), L"soaZ should be zero", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaW == v_f32_4{0.0f}), L"soaW should be zero", LINE_INFO());
    }

    TEST_METHOD(constructsFromFourPackedTypes) {
      const auto packedX = _mm_set_ps(13.0f, 9.0f, 5.0f, 1.0f);
      const auto packedY = _mm_set_ps(14.0f, 10.0f, 6.0f, 2.0f);
      const auto packedZ = _mm_set_ps(15.0f, 11.0f, 7.0f, 3.0f);
      const auto packedW = _mm_set_ps(16.0f, 12.0f, 8.0f, 4.0f);

      const v_f32_4x4 actual{packedX, packedY, packedZ, packedW};

      Assert::IsTrue(
          allTrue(actual.soaX == v_f32_4{1.0f, 5.0f, 9.0f, 13.0f}), L"soaX mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(actual.soaY == v_f32_4{2.0f, 6.0f, 10.0f, 14.0f}), L"soaY mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(actual.soaZ == v_f32_4{3.0f, 7.0f, 11.0f, 15.0f}), L"soaZ mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(actual.soaW == v_f32_4{4.0f, 8.0f, 12.0f, 16.0f}), L"soaW mismatch", LINE_INFO()
      );
    }

    TEST_METHOD(constructsFromFourSoaVectors) {
      const v_f32_4 soaX{1.0f, 5.0f, 9.0f, 13.0f};
      const v_f32_4 soaY{2.0f, 6.0f, 10.0f, 14.0f};
      const v_f32_4 soaZ{3.0f, 7.0f, 11.0f, 15.0f};
      const v_f32_4 soaW{4.0f, 8.0f, 12.0f, 16.0f};

      const v_f32_4x4 actual{soaX, soaY, soaZ, soaW};

      Assert::IsTrue(allTrue(actual.soaX == soaX), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaY == soaY), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaZ == soaZ), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaW == soaW), L"soaW mismatch", LINE_INFO());
    }

    TEST_METHOD(broadcastsSingleAosVectorIntoEachSoaSlot) {
      const v_f32_4 aos{1.0f, 2.0f, 3.0f, 4.0f};

      const v_f32_4x4 actual{aos};

      Assert::IsTrue(allTrue(actual.soaX == v_f32_4{1.0f}), L"soaX broadcast mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaY == v_f32_4{2.0f}), L"soaY broadcast mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaZ == v_f32_4{3.0f}), L"soaZ broadcast mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(actual.soaW == v_f32_4{4.0f}), L"soaW broadcast mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsFromValuePointer) {
      constexpr std::array<v_f32_4x4::ValueType, v_f32_4x4::SIZE> seed{
          1.0f, 5.0f, 9.0f, 13.0f,   // soaX
          2.0f, 6.0f, 10.0f, 14.0f,  // soaY
          3.0f, 7.0f, 11.0f, 15.0f,  // soaZ
          4.0f, 8.0f, 12.0f, 16.0f   // soaW
      };

      const v_f32_4x4 actual{seed.data()};

      Assert::IsTrue(
          allTrue(actual.soaX == v_f32_4{1.0f, 5.0f, 9.0f, 13.0f}), L"soaX mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(actual.soaY == v_f32_4{2.0f, 6.0f, 10.0f, 14.0f}), L"soaY mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(actual.soaZ == v_f32_4{3.0f, 7.0f, 11.0f, 15.0f}), L"soaZ mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(actual.soaW == v_f32_4{4.0f, 8.0f, 12.0f, 16.0f}), L"soaW mismatch", LINE_INFO()
      );
    }
  };
}
