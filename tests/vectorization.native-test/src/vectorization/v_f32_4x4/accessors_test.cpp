#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_4x4_AccessorsTest) {
  public:
    TEST_METHOD(readsEverySoaSlice) {
      const v_f32_4 soaX{1.0f, 5.0f, 9.0f, 13.0f};
      const v_f32_4 soaY{2.0f, 6.0f, 10.0f, 14.0f};
      const v_f32_4 soaZ{3.0f, 7.0f, 11.0f, 15.0f};
      const v_f32_4 soaW{4.0f, 8.0f, 12.0f, 16.0f};
      const v_f32_4x4 m{soaX, soaY, soaZ, soaW};

      Assert::IsTrue(allTrue(soa<VectorIndices::X>(m) == soaX), L"soa<X> mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(soa<VectorIndices::Y>(m) == soaY), L"soa<Y> mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(soa<VectorIndices::Z>(m) == soaZ), L"soa<Z> mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(soa<VectorIndices::W>(m) == soaW), L"soa<W> mismatch", LINE_INFO());
    }

    TEST_METHOD(writesEverySoaSlice) {
      v_f32_4x4 m{};
      const v_f32_4 newX{10.0f, 20.0f, 30.0f, 40.0f};
      const v_f32_4 newY{50.0f, 60.0f, 70.0f, 80.0f};
      const v_f32_4 newZ{90.0f, 100.0f, 110.0f, 120.0f};
      const v_f32_4 newW{130.0f, 140.0f, 150.0f, 160.0f};

      soa<VectorIndices::X>(m, newX);
      soa<VectorIndices::Y>(m, newY);
      soa<VectorIndices::Z>(m, newZ);
      soa<VectorIndices::W>(m, newW);

      Assert::IsTrue(allTrue(m.soaX == newX), L"soa<X> write mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(m.soaY == newY), L"soa<Y> write mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(m.soaZ == newZ), L"soa<Z> write mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(m.soaW == newW), L"soa<W> write mismatch", LINE_INFO());
    }

    TEST_METHOD(readsEveryAosVector) {
      const v_f32_4x4 m{
          v_f32_4{1.0f, 5.0f, 9.0f, 13.0f}, v_f32_4{2.0f, 6.0f, 10.0f, 14.0f}, v_f32_4{3.0f, 7.0f, 11.0f, 15.0f},
          v_f32_4{4.0f, 8.0f, 12.0f, 16.0f}
      };

      Assert::IsTrue(allTrue(aos<0>(m) == v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}), L"aos<0> mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(aos<1>(m) == v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}), L"aos<1> mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(aos<2>(m) == v_f32_4{9.0f, 10.0f, 11.0f, 12.0f}), L"aos<2> mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(aos<3>(m) == v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}), L"aos<3> mismatch", LINE_INFO());
    }

    TEST_METHOD(writesEveryAosVector) {
      v_f32_4x4 m{};

      aos<0>(m, v_f32_4{1.0f, 2.0f, 3.0f, 4.0f});
      aos<1>(m, v_f32_4{5.0f, 6.0f, 7.0f, 8.0f});
      aos<2>(m, v_f32_4{9.0f, 10.0f, 11.0f, 12.0f});
      aos<3>(m, v_f32_4{13.0f, 14.0f, 15.0f, 16.0f});

      Assert::IsTrue(
          allTrue(m.soaX == v_f32_4{1.0f, 5.0f, 9.0f, 13.0f}), L"soaX after aos writes mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(m.soaY == v_f32_4{2.0f, 6.0f, 10.0f, 14.0f}), L"soaY after aos writes mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(m.soaZ == v_f32_4{3.0f, 7.0f, 11.0f, 15.0f}), L"soaZ after aos writes mismatch", LINE_INFO()
      );
      Assert::IsTrue(
          allTrue(m.soaW == v_f32_4{4.0f, 8.0f, 12.0f, 16.0f}), L"soaW after aos writes mismatch", LINE_INFO()
      );
    }

    TEST_METHOD(aosWriteLeavesOtherIndicesUntouched) {
      v_f32_4x4 m{
          v_f32_4{1.0f, 5.0f, 9.0f, 13.0f}, v_f32_4{2.0f, 6.0f, 10.0f, 14.0f}, v_f32_4{3.0f, 7.0f, 11.0f, 15.0f},
          v_f32_4{4.0f, 8.0f, 12.0f, 16.0f}
      };

      aos<2>(m, v_f32_4{99.0f, 98.0f, 97.0f, 96.0f});

      Assert::IsTrue(allTrue(aos<0>(m) == v_f32_4{1.0f, 2.0f, 3.0f, 4.0f}), L"aos<0> changed", LINE_INFO());
      Assert::IsTrue(allTrue(aos<1>(m) == v_f32_4{5.0f, 6.0f, 7.0f, 8.0f}), L"aos<1> changed", LINE_INFO());
      Assert::IsTrue(allTrue(aos<2>(m) == v_f32_4{99.0f, 98.0f, 97.0f, 96.0f}), L"aos<2> write mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(aos<3>(m) == v_f32_4{13.0f, 14.0f, 15.0f, 16.0f}), L"aos<3> changed", LINE_INFO());
    }
  };
}
