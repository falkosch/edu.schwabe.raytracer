#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(Swizzle256dTest) {
  public:
    TEST_METHOD(genericallySwizzlesIdentityAndItsReverse) {
      const auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
      const auto givenPack = _mm256_load_pd(given.data());
      std::array<Float_64, VectorSizes::W> actual{};

      {
        const std::array expected{
            given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::Z),
            given.at(VectorIndices::W)
        };
        _mm256_store_pd(
            actual.data(), swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::W), given.at(VectorIndices::Z), given.at(VectorIndices::Y),
            given.at(VectorIndices::X)
        };
        _mm256_store_pd(
            actual.data(), swizzle<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallySwizzlesBroadcast) {
      const auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
      const auto givenPack = _mm256_load_pd(given.data());
      std::array<Float_64, VectorSizes::W> actual{};

      {
        const std::array expected{
            given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::X),
            given.at(VectorIndices::X)
        };
        _mm256_store_pd(
            actual.data(), swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::Y),
            given.at(VectorIndices::Y)
        };
        _mm256_store_pd(
            actual.data(), swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Z),
            given.at(VectorIndices::Z)
        };
        _mm256_store_pd(
            actual.data(), swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::W), given.at(VectorIndices::W), given.at(VectorIndices::W),
            given.at(VectorIndices::W)
        };
        _mm256_store_pd(
            actual.data(), swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallySwizzlesDuplicateForms) {
      const auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
      const auto givenPack = _mm256_load_pd(given.data());
      std::array<Float_64, VectorSizes::W> actual{};

      {
        const std::array expected{
            given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Y),
            given.at(VectorIndices::Y)
        };
        _mm256_store_pd(
            actual.data(), swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Z),
            given.at(VectorIndices::Z)
        };
        _mm256_store_pd(
            actual.data(), swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Z, VectorIndices::Z>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::X),
            given.at(VectorIndices::Y)
        };
        _mm256_store_pd(
            actual.data(), swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::W),
            given.at(VectorIndices::W)
        };
        _mm256_store_pd(
            actual.data(), swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::W),
            given.at(VectorIndices::W)
        };
        _mm256_store_pd(
            actual.data(), swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::Z),
            given.at(VectorIndices::W)
        };
        _mm256_store_pd(
            actual.data(), swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(givenPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForSwizzlesOfXFirst) {
      const auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
      const auto givenPack = _mm256_load_pd(given.data());
      std::array<Float_64, VectorSizes::W> actual{};

      {
        const std::array expected{
            given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::X),
            given.at(VectorIndices::X)
        };
        _mm256_store_pd(actual.data(), xxxx(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::X),
            given.at(VectorIndices::Z)
        };
        _mm256_store_pd(actual.data(), xxxz(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Y),
            given.at(VectorIndices::Y)
        };
        _mm256_store_pd(actual.data(), xxyy(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::X), given.at(VectorIndices::X), given.at(VectorIndices::Z),
            given.at(VectorIndices::Z)
        };
        _mm256_store_pd(actual.data(), xxzz(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::X), given.at(VectorIndices::Y), given.at(VectorIndices::X),
            given.at(VectorIndices::Y)
        };
        _mm256_store_pd(actual.data(), xyxy(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::X), given.at(VectorIndices::Z), given.at(VectorIndices::Z),
            given.at(VectorIndices::Z)
        };
        _mm256_store_pd(actual.data(), xzzz(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForSwizzlesOfYFirst) {
      const auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
      const auto givenPack = _mm256_load_pd(given.data());
      std::array<Float_64, VectorSizes::W> actual{};

      {
        const std::array expected{
            given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::Y),
            given.at(VectorIndices::Y)
        };
        _mm256_store_pd(actual.data(), yyyy(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::Y), given.at(VectorIndices::X), given.at(VectorIndices::X),
            given.at(VectorIndices::Y)
        };
        _mm256_store_pd(actual.data(), yxxy(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::Y), given.at(VectorIndices::X), given.at(VectorIndices::W),
            given.at(VectorIndices::Z)
        };
        _mm256_store_pd(actual.data(), yxwz(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::Y), given.at(VectorIndices::Y), given.at(VectorIndices::W),
            given.at(VectorIndices::W)
        };
        _mm256_store_pd(actual.data(), yyww(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::Y), given.at(VectorIndices::Z), given.at(VectorIndices::X),
            given.at(VectorIndices::W)
        };
        _mm256_store_pd(actual.data(), yzxw(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::Y), given.at(VectorIndices::Z), given.at(VectorIndices::W),
            given.at(VectorIndices::X)
        };
        _mm256_store_pd(actual.data(), yzwx(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForSwizzlesOfZFirst) {
      const auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
      const auto givenPack = _mm256_load_pd(given.data());
      std::array<Float_64, VectorSizes::W> actual{};

      {
        const std::array expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Z),
            given.at(VectorIndices::Z)
        };
        _mm256_store_pd(actual.data(), zzzz(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::X), given.at(VectorIndices::Y),
            given.at(VectorIndices::W)
        };
        _mm256_store_pd(actual.data(), zxyw(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::Y),
            given.at(VectorIndices::Y)
        };
        _mm256_store_pd(actual.data(), zzyy(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::Z), given.at(VectorIndices::W),
            given.at(VectorIndices::W)
        };
        _mm256_store_pd(actual.data(), zzww(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::X),
            given.at(VectorIndices::Y)
        };
        _mm256_store_pd(actual.data(), zwxy(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::Z), given.at(VectorIndices::W), given.at(VectorIndices::Z),
            given.at(VectorIndices::W)
        };
        _mm256_store_pd(actual.data(), zwzw(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForSwizzlesOfWFirst) {
      const auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
      const auto givenPack = _mm256_load_pd(given.data());
      std::array<Float_64, VectorSizes::W> actual{};

      {
        const std::array expected{
            given.at(VectorIndices::W), given.at(VectorIndices::W), given.at(VectorIndices::W),
            given.at(VectorIndices::W)
        };
        _mm256_store_pd(actual.data(), wwww(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::W), given.at(VectorIndices::X), given.at(VectorIndices::Y),
            given.at(VectorIndices::Z)
        };
        _mm256_store_pd(actual.data(), wxyz(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }

      {
        const std::array expected{
            given.at(VectorIndices::W), given.at(VectorIndices::Z), given.at(VectorIndices::Y),
            given.at(VectorIndices::X)
        };
        _mm256_store_pd(actual.data(), wzyx(givenPack));
        Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
      }
    }
  };
}
