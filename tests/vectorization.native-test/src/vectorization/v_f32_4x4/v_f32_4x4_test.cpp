#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_4x4_Test) {
  public:
    TEST_METHOD(constructsZero) {
      const auto given = v_f32_4x4();

      Assert::AreEqual(Zero<Float_32>(), x(given.soaX), L"soaX.x mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), y(given.soaX), L"soaX.y mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), z(given.soaX), L"soaX.z mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), w(given.soaX), L"soaX.w mismatch", LINE_INFO());

      Assert::AreEqual(Zero<Float_32>(), x(given.soaY), L"soaY.x mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), y(given.soaY), L"soaY.y mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), z(given.soaY), L"soaY.z mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), w(given.soaY), L"soaY.w mismatch", LINE_INFO());

      Assert::AreEqual(Zero<Float_32>(), x(given.soaZ), L"soaZ.x mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), y(given.soaZ), L"soaZ.y mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), z(given.soaZ), L"soaZ.z mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), w(given.soaZ), L"soaZ.w mismatch", LINE_INFO());

      Assert::AreEqual(Zero<Float_32>(), x(given.soaW), L"soaW.x mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), y(given.soaW), L"soaW.y mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), z(given.soaW), L"soaW.z mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), w(given.soaW), L"soaW.w mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsFromSOAVectors) {
      const auto givenX = v_f32_4(1.0f, 5.0f, 9.0f, 13.0f);
      const auto givenY = v_f32_4(2.0f, 6.0f, 10.0f, 14.0f);
      const auto givenZ = v_f32_4(3.0f, 7.0f, 11.0f, 15.0f);
      const auto givenW = v_f32_4(4.0f, 8.0f, 12.0f, 16.0f);

      const auto actual = v_f32_4x4(givenX, givenY, givenZ, givenW);

      Assert::IsTrue(allTrue(givenX == actual.soaX), L"soaX mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(givenY == actual.soaY), L"soaY mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(givenZ == actual.soaZ), L"soaZ mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(givenW == actual.soaW), L"soaW mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsFromAOSVector) {
      // Broadcasting: a single AOS vector {1,2,3,4} should be spread so that
      // soaX = {1,1,1,1}, soaY = {2,2,2,2}, soaZ = {3,3,3,3}, soaW = {4,4,4,4}
      const auto aosVector = v_f32_4(1.0f, 2.0f, 3.0f, 4.0f);
      const auto actual = v_f32_4x4(aosVector);

      const auto expectedX = v_f32_4(1.0f);
      const auto expectedY = v_f32_4(2.0f);
      const auto expectedZ = v_f32_4(3.0f);
      const auto expectedW = v_f32_4(4.0f);

      Assert::IsTrue(allTrue(expectedX == actual.soaX), L"soaX broadcast mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(expectedY == actual.soaY), L"soaY broadcast mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(expectedZ == actual.soaZ), L"soaZ broadcast mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(expectedW == actual.soaW), L"soaW broadcast mismatch", LINE_INFO());
    }

    TEST_METHOD(readsSOAComponents) {
      const auto givenX = v_f32_4(1.0f, 5.0f, 9.0f, 13.0f);
      const auto givenY = v_f32_4(2.0f, 6.0f, 10.0f, 14.0f);
      const auto givenZ = v_f32_4(3.0f, 7.0f, 11.0f, 15.0f);
      const auto givenW = v_f32_4(4.0f, 8.0f, 12.0f, 16.0f);
      const auto m = v_f32_4x4(givenX, givenY, givenZ, givenW);

      Assert::IsTrue(allTrue(givenX == soa<VectorIndices::X>(m)), L"soa<X> mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(givenY == soa<VectorIndices::Y>(m)), L"soa<Y> mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(givenZ == soa<VectorIndices::Z>(m)), L"soa<Z> mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(givenW == soa<VectorIndices::W>(m)), L"soa<W> mismatch", LINE_INFO());
    }

    TEST_METHOD(writesSOAComponents) {
      auto m = v_f32_4x4();
      const auto newX = v_f32_4(10.0f, 20.0f, 30.0f, 40.0f);
      const auto newY = v_f32_4(50.0f, 60.0f, 70.0f, 80.0f);
      const auto newZ = v_f32_4(90.0f, 100.0f, 110.0f, 120.0f);
      const auto newW = v_f32_4(130.0f, 140.0f, 150.0f, 160.0f);

      soa<VectorIndices::X>(m, newX);
      soa<VectorIndices::Y>(m, newY);
      soa<VectorIndices::Z>(m, newZ);
      soa<VectorIndices::W>(m, newW);

      Assert::IsTrue(allTrue(newX == m.soaX), L"soa<X> write mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(newY == m.soaY), L"soa<Y> write mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(newZ == m.soaZ), L"soa<Z> write mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(newW == m.soaW), L"soa<W> write mismatch", LINE_INFO());
    }

    TEST_METHOD(readsAOSVectors) {
      // SOA layout: 4 AOS vectors {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16}
      // soaX = {1, 5, 9, 13}, soaY = {2, 6, 10, 14}, soaZ = {3, 7, 11, 15}, soaW = {4, 8, 12, 16}
      const auto m = v_f32_4x4(
          v_f32_4(1.0f, 5.0f, 9.0f, 13.0f), v_f32_4(2.0f, 6.0f, 10.0f, 14.0f), v_f32_4(3.0f, 7.0f, 11.0f, 15.0f),
          v_f32_4(4.0f, 8.0f, 12.0f, 16.0f)
      );

      // aos<0> gathers the 0th element from each SOA vector: {soaX[0], soaY[0], soaZ[0], soaW[0]} = {1,2,3,4}
      const auto aos0 = aos<0>(m);
      Assert::AreEqual(1.0f, x(aos0), L"aos<0>.x mismatch", LINE_INFO());
      Assert::AreEqual(2.0f, y(aos0), L"aos<0>.y mismatch", LINE_INFO());
      Assert::AreEqual(3.0f, z(aos0), L"aos<0>.z mismatch", LINE_INFO());
      Assert::AreEqual(4.0f, w(aos0), L"aos<0>.w mismatch", LINE_INFO());

      // aos<1> = {soaX[1], soaY[1], soaZ[1], soaW[1]} = {5,6,7,8}
      const auto aos1 = aos<1>(m);
      Assert::AreEqual(5.0f, x(aos1), L"aos<1>.x mismatch", LINE_INFO());
      Assert::AreEqual(6.0f, y(aos1), L"aos<1>.y mismatch", LINE_INFO());
      Assert::AreEqual(7.0f, z(aos1), L"aos<1>.z mismatch", LINE_INFO());
      Assert::AreEqual(8.0f, w(aos1), L"aos<1>.w mismatch", LINE_INFO());

      // aos<2> = {9,10,11,12}
      const auto aos2 = aos<2>(m);
      Assert::AreEqual(9.0f, x(aos2), L"aos<2>.x mismatch", LINE_INFO());
      Assert::AreEqual(10.0f, y(aos2), L"aos<2>.y mismatch", LINE_INFO());
      Assert::AreEqual(11.0f, z(aos2), L"aos<2>.z mismatch", LINE_INFO());
      Assert::AreEqual(12.0f, w(aos2), L"aos<2>.w mismatch", LINE_INFO());

      // aos<3> = {13,14,15,16}
      const auto aos3 = aos<3>(m);
      Assert::AreEqual(13.0f, x(aos3), L"aos<3>.x mismatch", LINE_INFO());
      Assert::AreEqual(14.0f, y(aos3), L"aos<3>.y mismatch", LINE_INFO());
      Assert::AreEqual(15.0f, z(aos3), L"aos<3>.z mismatch", LINE_INFO());
      Assert::AreEqual(16.0f, w(aos3), L"aos<3>.w mismatch", LINE_INFO());
    }

    TEST_METHOD(addsComponentWise) {
      const auto a = v_f32_4x4(
          v_f32_4(1.0f, 2.0f, 3.0f, 4.0f), v_f32_4(5.0f, 6.0f, 7.0f, 8.0f), v_f32_4(9.0f, 10.0f, 11.0f, 12.0f),
          v_f32_4(13.0f, 14.0f, 15.0f, 16.0f)
      );
      const auto b = v_f32_4x4(
          v_f32_4(10.0f, 20.0f, 30.0f, 40.0f), v_f32_4(50.0f, 60.0f, 70.0f, 80.0f),
          v_f32_4(90.0f, 100.0f, 110.0f, 120.0f), v_f32_4(130.0f, 140.0f, 150.0f, 160.0f)
      );

      const auto actual = a + b;

      Assert::IsTrue(
          allTrue(v_f32_4(11.0f, 22.0f, 33.0f, 44.0f) == actual.soaX), L"soaX add mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(v_f32_4(55.0f, 66.0f, 77.0f, 88.0f) == actual.soaY), L"soaY add mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(v_f32_4(99.0f, 110.0f, 121.0f, 132.0f) == actual.soaZ), L"soaZ add mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(v_f32_4(143.0f, 154.0f, 165.0f, 176.0f) == actual.soaW), L"soaW add mismatch", LINE_INFO()
      );
    }

    TEST_METHOD(subtractsComponentWise) {
      const auto a = v_f32_4x4(
          v_f32_4(10.0f, 20.0f, 30.0f, 40.0f), v_f32_4(50.0f, 60.0f, 70.0f, 80.0f),
          v_f32_4(90.0f, 100.0f, 110.0f, 120.0f), v_f32_4(130.0f, 140.0f, 150.0f, 160.0f)
      );
      const auto b = v_f32_4x4(
          v_f32_4(1.0f, 2.0f, 3.0f, 4.0f), v_f32_4(5.0f, 6.0f, 7.0f, 8.0f), v_f32_4(9.0f, 10.0f, 11.0f, 12.0f),
          v_f32_4(13.0f, 14.0f, 15.0f, 16.0f)
      );

      const auto actual = a - b;

      Assert::IsTrue(
          allTrue(v_f32_4(9.0f, 18.0f, 27.0f, 36.0f) == actual.soaX), L"soaX sub mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(v_f32_4(45.0f, 54.0f, 63.0f, 72.0f) == actual.soaY), L"soaY sub mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(v_f32_4(81.0f, 90.0f, 99.0f, 108.0f) == actual.soaZ), L"soaZ sub mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(v_f32_4(117.0f, 126.0f, 135.0f, 144.0f) == actual.soaW), L"soaW sub mismatch", LINE_INFO()
      );
    }

    TEST_METHOD(multipliesComponentWise) {
      const auto a = v_f32_4x4(
          v_f32_4(1.0f, 2.0f, 3.0f, 4.0f), v_f32_4(5.0f, 6.0f, 7.0f, 8.0f), v_f32_4(9.0f, 10.0f, 11.0f, 12.0f),
          v_f32_4(13.0f, 14.0f, 15.0f, 16.0f)
      );
      const auto b = v_f32_4x4(
          v_f32_4(2.0f, 3.0f, 4.0f, 5.0f), v_f32_4(2.0f, 3.0f, 4.0f, 5.0f), v_f32_4(2.0f, 3.0f, 4.0f, 5.0f),
          v_f32_4(2.0f, 3.0f, 4.0f, 5.0f)
      );

      const auto actual = a * b;

      Assert::IsTrue(
          allTrue(v_f32_4(2.0f, 6.0f, 12.0f, 20.0f) == actual.soaX), L"soaX mul mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(v_f32_4(10.0f, 18.0f, 28.0f, 40.0f) == actual.soaY), L"soaY mul mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(v_f32_4(18.0f, 30.0f, 44.0f, 60.0f) == actual.soaZ), L"soaZ mul mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(v_f32_4(26.0f, 42.0f, 60.0f, 80.0f) == actual.soaW), L"soaW mul mismatch", LINE_INFO()
      );
    }

    TEST_METHOD(dividesComponentWise) {
      const auto a = v_f32_4x4(
          v_f32_4(10.0f, 20.0f, 30.0f, 40.0f), v_f32_4(50.0f, 60.0f, 70.0f, 80.0f),
          v_f32_4(90.0f, 100.0f, 110.0f, 120.0f), v_f32_4(130.0f, 140.0f, 150.0f, 160.0f)
      );
      const auto b = v_f32_4x4(
          v_f32_4(2.0f, 4.0f, 5.0f, 8.0f), v_f32_4(10.0f, 12.0f, 14.0f, 16.0f), v_f32_4(9.0f, 10.0f, 11.0f, 12.0f),
          v_f32_4(13.0f, 14.0f, 15.0f, 16.0f)
      );

      const auto actual = a / b;

      Assert::AreEqual(5.0f, x(actual.soaX), L"soaX.x div mismatch", LINE_INFO());
      Assert::AreEqual(5.0f, y(actual.soaX), L"soaX.y div mismatch", LINE_INFO());
      Assert::AreEqual(6.0f, z(actual.soaX), L"soaX.z div mismatch", LINE_INFO());
      Assert::AreEqual(5.0f, w(actual.soaX), L"soaX.w div mismatch", LINE_INFO());

      Assert::AreEqual(5.0f, x(actual.soaY), L"soaY.x div mismatch", LINE_INFO());
      Assert::AreEqual(5.0f, y(actual.soaY), L"soaY.y div mismatch", LINE_INFO());
      Assert::AreEqual(5.0f, z(actual.soaY), L"soaY.z div mismatch", LINE_INFO());
      Assert::AreEqual(5.0f, w(actual.soaY), L"soaY.w div mismatch", LINE_INFO());

      Assert::AreEqual(10.0f, x(actual.soaZ), L"soaZ.x div mismatch", LINE_INFO());
      Assert::AreEqual(10.0f, y(actual.soaZ), L"soaZ.y div mismatch", LINE_INFO());
      Assert::AreEqual(10.0f, z(actual.soaZ), L"soaZ.z div mismatch", LINE_INFO());
      Assert::AreEqual(10.0f, w(actual.soaZ), L"soaZ.w div mismatch", LINE_INFO());

      Assert::AreEqual(10.0f, x(actual.soaW), L"soaW.x div mismatch", LINE_INFO());
      Assert::AreEqual(10.0f, y(actual.soaW), L"soaW.y div mismatch", LINE_INFO());
      Assert::AreEqual(10.0f, z(actual.soaW), L"soaW.z div mismatch", LINE_INFO());
      Assert::AreEqual(10.0f, w(actual.soaW), L"soaW.w div mismatch", LINE_INFO());
    }

    TEST_METHOD(transposesSOAToAOS) {
      // SOA layout representing 4 AOS vectors: {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16}
      const auto m = v_f32_4x4(
          v_f32_4(1.0f, 5.0f, 9.0f, 13.0f),  // soaX: all X components
          v_f32_4(2.0f, 6.0f, 10.0f, 14.0f),  // soaY: all Y components
          v_f32_4(3.0f, 7.0f, 11.0f, 15.0f),  // soaZ: all Z components
          v_f32_4(4.0f, 8.0f, 12.0f, 16.0f)   // soaW: all W components
      );

      const auto actual = transpose(m);

      // After transpose, each row should be one AOS vector:
      // soaX = {1, 2, 3, 4} (first AOS vector)
      Assert::AreEqual(1.0f, x(actual.soaX), L"transposed soaX.x mismatch", LINE_INFO());
      Assert::AreEqual(2.0f, y(actual.soaX), L"transposed soaX.y mismatch", LINE_INFO());
      Assert::AreEqual(3.0f, z(actual.soaX), L"transposed soaX.z mismatch", LINE_INFO());
      Assert::AreEqual(4.0f, w(actual.soaX), L"transposed soaX.w mismatch", LINE_INFO());

      // soaY = {5, 6, 7, 8} (second AOS vector)
      Assert::AreEqual(5.0f, x(actual.soaY), L"transposed soaY.x mismatch", LINE_INFO());
      Assert::AreEqual(6.0f, y(actual.soaY), L"transposed soaY.y mismatch", LINE_INFO());
      Assert::AreEqual(7.0f, z(actual.soaY), L"transposed soaY.z mismatch", LINE_INFO());
      Assert::AreEqual(8.0f, w(actual.soaY), L"transposed soaY.w mismatch", LINE_INFO());

      // soaZ = {9, 10, 11, 12} (third AOS vector)
      Assert::AreEqual(9.0f, x(actual.soaZ), L"transposed soaZ.x mismatch", LINE_INFO());
      Assert::AreEqual(10.0f, y(actual.soaZ), L"transposed soaZ.y mismatch", LINE_INFO());
      Assert::AreEqual(11.0f, z(actual.soaZ), L"transposed soaZ.z mismatch", LINE_INFO());
      Assert::AreEqual(12.0f, w(actual.soaZ), L"transposed soaZ.w mismatch", LINE_INFO());

      // soaW = {13, 14, 15, 16} (fourth AOS vector)
      Assert::AreEqual(13.0f, x(actual.soaW), L"transposed soaW.x mismatch", LINE_INFO());
      Assert::AreEqual(14.0f, y(actual.soaW), L"transposed soaW.y mismatch", LINE_INFO());
      Assert::AreEqual(15.0f, z(actual.soaW), L"transposed soaW.z mismatch", LINE_INFO());
      Assert::AreEqual(16.0f, w(actual.soaW), L"transposed soaW.w mismatch", LINE_INFO());
    }

    TEST_METHOD(hasZeroConstant) {
      const auto actual = Zero<v_f32_4x4>();

      Assert::AreEqual(Zero<Float_32>(), x(actual.soaX), L"Zero soaX.x mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), y(actual.soaX), L"Zero soaX.y mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), z(actual.soaX), L"Zero soaX.z mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), w(actual.soaX), L"Zero soaX.w mismatch", LINE_INFO());

      Assert::AreEqual(Zero<Float_32>(), x(actual.soaY), L"Zero soaY.x mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), y(actual.soaY), L"Zero soaY.y mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), z(actual.soaY), L"Zero soaY.z mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), w(actual.soaY), L"Zero soaY.w mismatch", LINE_INFO());

      Assert::AreEqual(Zero<Float_32>(), x(actual.soaZ), L"Zero soaZ.x mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), y(actual.soaZ), L"Zero soaZ.y mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), z(actual.soaZ), L"Zero soaZ.z mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), w(actual.soaZ), L"Zero soaZ.w mismatch", LINE_INFO());

      Assert::AreEqual(Zero<Float_32>(), x(actual.soaW), L"Zero soaW.x mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), y(actual.soaW), L"Zero soaW.y mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), z(actual.soaW), L"Zero soaW.z mismatch", LINE_INFO());
      Assert::AreEqual(Zero<Float_32>(), w(actual.soaW), L"Zero soaW.w mismatch", LINE_INFO());
    }
  };
}
