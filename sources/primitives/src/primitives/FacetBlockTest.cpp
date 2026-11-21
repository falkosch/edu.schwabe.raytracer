#include "primitives/FacetBlockTest.h"

using namespace vectorization;

namespace primitives {
  Float4 texCoordsAndDistance(const FacetBlockTest &test, const int lane) noexcept {
    return Float4{test.detU[lane], test.detV[lane], test.detT[lane], test.detT[lane]} / Float4{test.determinant[lane]};
  }
}
