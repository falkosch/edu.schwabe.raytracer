#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(Swizzle128sTest) {
  public:
    TEST_METHOD(genericallySwizzlesIdentityAndItsReverse) {
      auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
      auto givenPack = _mm_load_ps(given.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::Z),
            given.at(VectorIndices::W)
        };
        _mm_store_ps(
            actual.data(), swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::W), given.at(VectorIndices::Z), given.at(VectorIndices::Y),
            given.at(VectorIndices::X)
        };
        _mm_store_ps(
            actual.data(), swizzle<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallySwizzlesBroadcast) {
      auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
      auto givenPack = _mm_load_ps(given.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::X),
            given.at(VectorIndices::X)
        };
        _mm_store_ps(
            actual.data(), swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::Y),
            given.at(VectorIndices::Y)
        };
        _mm_store_ps(
            actual.data(), swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Z),
            given.at(VectorIndices::Z)
        };
        _mm_store_ps(
            actual.data(), swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::W), given.at(VectorIndices::W), given.at(VectorIndices::W),
            given.at(VectorIndices::W)
        };
        _mm_store_ps(
            actual.data(), swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallySwizzlesDuplicateForms) {
      auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
      auto givenPack = _mm_load_ps(given.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Y),
            given.at(VectorIndices::Y)
        };
        _mm_store_ps(
            actual.data(), swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Z),
            given.at(VectorIndices::Z)
        };
        _mm_store_ps(
            actual.data(), swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::X),
            given.at(VectorIndices::Y)
        };
        _mm_store_ps(
            actual.data(), swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::W),
            given.at(VectorIndices::W)
        };
        _mm_store_ps(
            actual.data(), swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::W),
            given.at(VectorIndices::W)
        };
        _mm_store_ps(
            actual.data(), swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::Z),
            given.at(VectorIndices::W)
        };
        _mm_store_ps(
            actual.data(), swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForSwizzlesOfXFirst) {
      auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
      auto givenPack = _mm_load_ps(given.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::X),
            given.at(VectorIndices::X)
        };
        _mm_store_ps(actual.data(), xxxx(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::X),
            given.at(VectorIndices::Z)
        };
        _mm_store_ps(actual.data(), xxxz(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Y),
            given.at(VectorIndices::Y)
        };
        _mm_store_ps(actual.data(), xxyy(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Z),
            given.at(VectorIndices::Z)
        };
        _mm_store_ps(actual.data(), xxzz(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::X),
            given.at(VectorIndices::Y)
        };
        _mm_store_ps(actual.data(), xyxy(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::X), given.at(VectorIndices::Z), given.at(VectorIndices::Z),
            given.at(VectorIndices::Z)
        };
        _mm_store_ps(actual.data(), xzzz(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForSwizzlesOfYFirst) {
      auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
      auto givenPack = _mm_load_ps(given.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::Y),
            given.at(VectorIndices::Y)
        };
        _mm_store_ps(actual.data(), yyyy(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Y), given.at(VectorIndices::X), given.at(VectorIndices::X),
            given.at(VectorIndices::Y)
        };
        _mm_store_ps(actual.data(), yxxy(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Y), given.at(VectorIndices::X), given.at(VectorIndices::W),
            given.at(VectorIndices::Z)
        };
        _mm_store_ps(actual.data(), yxwz(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::W),
            given.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), yyww(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Y), given.at(VectorIndices::Z), given.at(VectorIndices::X),
            given.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), yzxw(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Y), given.at(VectorIndices::Z), given.at(VectorIndices::W),
            given.at(VectorIndices::X)
        };
        _mm_store_ps(actual.data(), yzwx(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForSwizzlesOfZFirst) {
      auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
      auto givenPack = _mm_load_ps(given.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Z),
            given.at(VectorIndices::Z)
        };
        _mm_store_ps(actual.data(), zzzz(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::X), given.at(VectorIndices::Y),
            given.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), zxyw(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Y),
            given.at(VectorIndices::Y)
        };
        _mm_store_ps(actual.data(), zzyy(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::W),
            given.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), zzww(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::X),
            given.at(VectorIndices::Y)
        };
        _mm_store_ps(actual.data(), zwxy(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::Z),
            given.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), zwzw(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForSwizzlesOfWFirst) {
      auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
      auto givenPack = _mm_load_ps(given.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::W), given.at(VectorIndices::W), given.at(VectorIndices::W),
            given.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), wwww(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::W), given.at(VectorIndices::X), given.at(VectorIndices::Y),
            given.at(VectorIndices::Z)
        };
        _mm_store_ps(actual.data(), wxyz(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::W> expected{
            given.at(VectorIndices::W), given.at(VectorIndices::Z), given.at(VectorIndices::Y),
            given.at(VectorIndices::X)
        };
        _mm_store_ps(actual.data(), wzyx(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }
  };
}
