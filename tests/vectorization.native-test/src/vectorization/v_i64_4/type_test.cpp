#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i64_4_TypeTest) {
  public:
    TEST_METHOD(definesArchitecture) {
      Assert::AreEqual(VectorSizes::W, v_i64_4::SIZE, L"Size mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<Int_64, v_i64_4::ValueType>, L"Value type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<Int_64, v_i64_4::BoolType>, L"Bool type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_i64_4, v_i64_4::VectorType>, L"Vector type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_i64_4, v_i64_4::VectorBoolType>, L"Vector bool type mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsZero) {
      const v_i64_4 actual;
      Assert::IsTrue(allTrue(Zero<v_i64_4>() == actual));
    }

    TEST_METHOD(constructsFromXYZW) {
      const v_i64_4 actual{10LL, 20LL, 30LL, 40LL};

      Assert::AreEqual(Int_64{10}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{20}, y(actual), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{30}, z(actual), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{40}, w(actual), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsBroadcastFromValueType) {
      const v_i64_4 actual{Int_64{42}};

      Assert::AreEqual(Int_64{42}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{42}, y(actual), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{42}, z(actual), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{42}, w(actual), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsFromValueTypePtr) {
      alignas(32) std::array<Int_64, 4> given{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 expected{
          given.at(VectorIndices::X),
          given.at(VectorIndices::Y),
          given.at(VectorIndices::Z),
          given.at(VectorIndices::W),
      };
      const v_i64_4 actual{given.data()};
      Assert::IsTrue(allTrue(expected == actual));
    }

    TEST_METHOD(constructsFromPackedType) {
      const v_i64_4 given{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 actual{given.components};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(implicitlyConvertsFromPackedType) {
      const v_i64_4 given{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 actual = given.components;
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(readsAtIndex) {
      const v_i64_4 given{10LL, 20LL, 30LL, 40LL};
      Assert::AreEqual(Int_64{10}, given[VectorIndices::X], L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{20}, given[VectorIndices::Y], L"Y mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{30}, given[VectorIndices::Z], L"Z mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{40}, given[VectorIndices::W], L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(storesValues) {
      const v_i64_4 given{10LL, 20LL, 30LL, 40LL};
      alignas(32) std::array<Int_64, 4> dst{};
      store(given, dst.data());
      Assert::AreEqual(Int_64{10}, dst[0], L"store X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{20}, dst[1], L"store Y mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{30}, dst[2], L"store Z mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{40}, dst[3], L"store W mismatch", LINE_INFO());
    }
  };
}
