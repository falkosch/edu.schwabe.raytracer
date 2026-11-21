#include "primitives_tests.h"

namespace primitives::test {
  TEST_CLASS(FacetBlockRayTest) {
  public:
    TEST_METHOD(broadcastsEachComponentToAllLanes) {
      const Ray ray{Float4{1.0f, -2.0f, 3.0f, 1.0f}, Float4{0.0f, 0.6f, -0.8f, 0.0f}};
      const FacetBlockRay blockRay{ray};
      for (auto c = Zero<int>(); c < 4; ++c) {
        for (auto lane = Zero<int>(); lane < static_cast<int>(FacetLanes::SIZE); ++lane) {
          Assert::AreEqual(ray.origin[c], blockRay.origin[c][lane], L"origin component in every lane", LINE_INFO());
          Assert::AreEqual(
              ray.direction[c], blockRay.direction[c][lane], L"direction component in every lane", LINE_INFO()
          );
        }
      }
    }
  };
}
