#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_4_ConstantsTest) {
  public:
    TEST_METHOD(hasMaskAll) {
      Assert::AreEqual(ASizeT{128}, bitCount(MaskAll<v_f32_4>().components));
    }

    TEST_METHOD(hasMaskNone) {
      Assert::AreEqual(ASizeT{0}, bitCount(MaskNone<v_f32_4>().components));
    }

    TEST_METHOD(hasMaskX) {
      auto actual = MaskX<v_f32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskXY) {
      auto actual = MaskXY<v_f32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskXYZ) {
      auto actual = MaskXYZ<v_f32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::Z>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskXYZW) {
      Assert::AreEqual(ASizeT{128}, bitCount(MaskXYZW<v_f32_4>().components));
    }

    TEST_METHOD(hasMaskY) {
      auto actual = MaskY<v_f32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskYZ) {
      auto actual = MaskYZ<v_f32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::Y, VectorIndices::Z>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::W, VectorIndices::W>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskYZW) {
      auto actual = MaskYZW<v_f32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskZ) {
      auto actual = MaskZ<v_f32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskZW) {
      auto actual = MaskZW<v_f32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasMaskW) {
      auto actual = MaskW<v_f32_4>().components;
      Assert::AreEqual(
          ASizeT{128},
          bitCount(swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
      Assert::AreEqual(
          ASizeT{0}, bitCount(swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::Z>(actual)),
          L"wrong mask constant value", LINE_INFO()
      );
    }

    TEST_METHOD(hasDegreeToRadian) {
      Assert::IsTrue(allTrue(v_f32_4(DegreeToRadian<v_f32_4::ValueType>()) == DegreeToRadian<v_f32_4>()));
    }

    TEST_METHOD(hasEpsilon) {
      Assert::IsTrue(allTrue(v_f32_4(Epsilon<v_f32_4::ValueType>()) == Epsilon<v_f32_4>()));
    }

    TEST_METHOD(hasHalf) {
      Assert::IsTrue(allTrue(v_f32_4(Half<v_f32_4::ValueType>()) == Half<v_f32_4>()));
    }

    TEST_METHOD(hasInfinity) {
      Assert::IsTrue(allTrue(Zero<v_f32_4>() != Infinity<v_f32_4>()));
    }

    TEST_METHOD(hasNegativeInfinity) {
      Assert::IsTrue(allTrue(Zero<v_f32_4>() != NegativeInfinity<v_f32_4>()));
    }

    TEST_METHOD(hasNegativeOne) {
      Assert::IsTrue(allTrue(v_f32_4(NegativeOne<v_f32_4::ValueType>()) == NegativeOne<v_f32_4>()));
    }

    TEST_METHOD(hasNegativeTwo) {
      Assert::IsTrue(allTrue(v_f32_4(NegativeTwo<v_f32_4::ValueType>()) == NegativeTwo<v_f32_4>()));
    }

    TEST_METHOD(hasNegativeZero) {
      Assert::IsTrue(allTrue(Zero<v_f32_4>() == NegativeZero<v_f32_4>()), L"wrong constant value", LINE_INFO());

      std::array<v_f32_4::ValueType, v_f32_4::SIZE> actual{};
      _mm_store_ps(actual.data(), NegativeZero<v_f32_4>().components);
      for (auto &&v : actual) {
        Assert::IsTrue(std::signbit(v), L"wrong constant value", LINE_INFO());
      }
    }

    TEST_METHOD(hasNotANumber) {
      Assert::IsTrue(allTrue(Zero<v_f32_4>() != NotANumber<v_f32_4>()));
    }

    TEST_METHOD(hasOne) {
      Assert::IsTrue(allTrue(v_f32_4(One<v_f32_4::ValueType>()) == One<v_f32_4>()));
    }

    TEST_METHOD(hasOneHalf) {
      Assert::IsTrue(allTrue(v_f32_4(OneHalf<v_f32_4::ValueType>()) == OneHalf<v_f32_4>()));
    }

    TEST_METHOD(hasOneX) {
      v_f32_4 expected{
          One<v_f32_4::ValueType>(), Zero<v_f32_4::ValueType>(), Zero<v_f32_4::ValueType>(), Zero<v_f32_4::ValueType>()
      };
      Assert::IsTrue(allTrue(expected == OneX<v_f32_4>()));
    }

    TEST_METHOD(hasOneY) {
      v_f32_4 expected{
          Zero<v_f32_4::ValueType>(), One<v_f32_4::ValueType>(), Zero<v_f32_4::ValueType>(), Zero<v_f32_4::ValueType>()
      };
      Assert::IsTrue(allTrue(expected == OneY<v_f32_4>()));
    }

    TEST_METHOD(hasOneZ) {
      v_f32_4 expected{
          Zero<v_f32_4::ValueType>(), Zero<v_f32_4::ValueType>(), One<v_f32_4::ValueType>(), Zero<v_f32_4::ValueType>()
      };
      Assert::IsTrue(allTrue(expected == OneZ<v_f32_4>()));
    }

    TEST_METHOD(hasOneW) {
      v_f32_4 expected{
          Zero<v_f32_4::ValueType>(), Zero<v_f32_4::ValueType>(), Zero<v_f32_4::ValueType>(), One<v_f32_4::ValueType>()
      };
      Assert::IsTrue(allTrue(expected == OneW<v_f32_4>()));
    }

    TEST_METHOD(hasPi) {
      Assert::IsTrue(allTrue(v_f32_4(Pi<v_f32_4::ValueType>()) == Pi<v_f32_4>()));
    }

    TEST_METHOD(hasRadianToDegree) {
      Assert::IsTrue(allTrue(v_f32_4(RadianToDegree<v_f32_4::ValueType>()) == RadianToDegree<v_f32_4>()));
    }

    TEST_METHOD(hasRadianToUniform) {
      Assert::IsTrue(allTrue(v_f32_4(RadianToUniform<v_f32_4::ValueType>()) == RadianToUniform<v_f32_4>()));
    }

    TEST_METHOD(hasReciprocalPi) {
      Assert::IsTrue(allTrue(v_f32_4(ReciprocalPi<v_f32_4::ValueType>()) == ReciprocalPi<v_f32_4>()));
    }

    TEST_METHOD(hasSelfOcclusionEpsilon) {
      Assert::IsTrue(allTrue(v_f32_4(SelfOcclusionEpsilon<v_f32_4::ValueType>()) == SelfOcclusionEpsilon<v_f32_4>()));
    }

    TEST_METHOD(hasSin45) {
      Assert::IsTrue(allTrue(v_f32_4(Sin45<v_f32_4::ValueType>()) == Sin45<v_f32_4>()));
    }

    TEST_METHOD(hasTwo) {
      Assert::IsTrue(allTrue(v_f32_4(Two<v_f32_4::ValueType>()) == Two<v_f32_4>()));
    }

    TEST_METHOD(hasZero) {
      Assert::AreEqual(ASizeT{0}, bitCount(Zero<v_f32_4>().components));
    }
  };
}
