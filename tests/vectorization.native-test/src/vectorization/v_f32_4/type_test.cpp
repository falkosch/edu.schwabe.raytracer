#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_4_TypeTest) {
  public:
    TEST_METHOD(definesArchitecture) {
      Assert::AreEqual(VectorSizes::W, v_f32_4::SIZE, L"Size mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<Float_32, v_f32_4::ValueType>, L"Value type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<Int_32, v_f32_4::BoolType>, L"Bool type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_f32_4, v_f32_4::VectorType>, L"Vector type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_i32_4, v_f32_4::VectorBoolType>, L"Vector bool type mismatch", LINE_INFO());
      Assert::IsTrue(
          std::is_same_v<PackedTypes<Float_32, VectorSizes::W>::Type, v_f32_4::PackedType>, L"Packed type mismatch",
          LINE_INFO()
      );
    }

    TEST_METHOD(constructsZero) {
      Assert::IsTrue(allTrue(Zero<v_f32_4>() == v_f32_4()));
    }

    TEST_METHOD(constructsFromXYZW) {
      constexpr std::array given{1.0f, 2.0f, 3.0f, 4.0f};

      const v_f32_4 actualVector{
          given.at(VectorIndices::X),
          given.at(VectorIndices::Y),
          given.at(VectorIndices::Z),
          given.at(VectorIndices::W),
      };
      std::array<v_f32_4::ValueType, v_f32_4::SIZE> actual{};
      _mm_store_ps(actual.data(), actualVector.components);

      Assert::AreEqual(given, actual);
    }

    TEST_METHOD(constructsCopy) {
      const v_f32_4 given{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 actual{given};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsFromPackedType) {
      const v_f32_4 given{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 actual{given.components};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsBroadcastFromValueType) {
      const v_f32_4 given{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 actual{given.components};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsFromXY00) {
      const v_f32_4 given{1.0f, 2.0f, 0.0f, 0.0f};
      const v_f32_4 actual{1.0f, 2.0f};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsFromXYZ0) {
      const v_f32_4 given{1.0f, 2.0f, 3.0f, 0.0f};
      const v_f32_4 actual{1.0f, 2.0f, 3.0f};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsFromVectorTypePtr) {
      v_f32_4 given{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 actual{&given};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsFromValueTypePtr) {
      std::array given{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 expected{
          given.at(VectorIndices::X),
          given.at(VectorIndices::Y),
          given.at(VectorIndices::Z),
          given.at(VectorIndices::W),
      };
      const v_f32_4 actual{given.data()};
      Assert::IsTrue(allTrue(expected == actual));
    }

    TEST_METHOD(constructsFromPackedTypePtr) {
      const v_f32_4 given{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 actual{given.components};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(implicitlyConvertsFromComponentsPackedType) {
      const v_f32_4 given{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 actual = given.components;
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(implicitlyConvertsFromAnyPackedType) {
      const v_f32_4 given{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4::PackedType actual = given.components;
      const v_f32_4 actual2 = actual;
      Assert::IsTrue(allTrue(given == actual2));
    }

    TEST_METHOD(implicitlyConvertsFromAnyConstPackedType) {
      const v_f32_4 given{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4::PackedType actual = given.components;
      const v_f32_4 actual2 = actual;
      Assert::IsTrue(allTrue(given == actual2));
    }

    TEST_METHOD(readsAtIndex) {
      const v_f32_4 given{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 actual{
          given[VectorIndices::X], given[VectorIndices::Y], given[VectorIndices::Z], given[VectorIndices::W]
      };
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(storesToVectorType) {
      const v_f32_4 given{1.0f, 2.0f, 3.0f, 4.0f};
      v_f32_4 actual;
      store(given, &actual);
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(storesToPackedType) {
      const v_f32_4 given{1.0f, 2.0f, 3.0f, 4.0f};
      v_f32_4::PackedType actualPack;
      store(given, &actualPack);
      const v_f32_4 actual{actualPack};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(storesToValueTypeArray) {
      const v_f32_4 given{1.0f, 2.0f, 3.0f, 4.0f};
      std::array<v_f32_4::ValueType, v_f32_4::SIZE> actualArray{};
      store(given, actualArray.data());
      const v_f32_4 actual{actualArray.data()};
      Assert::IsTrue(allTrue(given == actual));
    }
  };
}
