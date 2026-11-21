#include "primitives_tests.h"

namespace primitives::test {
  TEST_CLASS(FacetBlockTestTest) {
  public:
    TEST_METHOD(dividesTheLanesDeterminantsByItsDeterminant) {
      // determinants are powers of two, so the divisions are exact however the compiler evaluates them
      const FacetBlockTest test{
          FacetLanes{1.0f, 2.0f, 4.0f, 8.0f, -2.0f, 0.5f, 32.0f, 16.0f},
          FacetLanes{3.0f, 6.0f, 12.0f, 24.0f, -6.0f, 1.5f, 96.0f, 48.0f},
          FacetLanes{0.5f, 0.5f, 1.0f, 2.0f, -1.0f, 0.25f, 16.0f, 4.0f},
          FacetLanes{0.25f, 1.0f, 2.0f, 4.0f, -0.5f, 0.125f, 8.0f, 8.0f}, Zero<unsigned>()
      };
      for (auto lane = Zero<int>(); lane < static_cast<int>(FacetLanes::SIZE); ++lane) {
        const auto determinant = test.determinant[lane];
        const auto result = texCoordsAndDistance(test, lane);
        Assert::AreEqual(test.detU[lane] / determinant, x(result), L"u", LINE_INFO());
        Assert::AreEqual(test.detV[lane] / determinant, y(result), L"v", LINE_INFO());
        Assert::AreEqual(test.detT[lane] / determinant, z(result), L"distance in z", LINE_INFO());
        Assert::AreEqual(test.detT[lane] / determinant, w(result), L"distance in w", LINE_INFO());
      }
    }
  };
}
