#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(HighestLaneTest) {
  public:
    TEST_METHOD(returnsZeroWithoutSetBits) {
      Assert::AreEqual(ASizeT{0}, highestLane(Int_32{0}), L"no lane set", LINE_INFO());
    }

    TEST_METHOD(returnsSingleSetLane) {
      Assert::AreEqual(ASizeT{0}, highestLane(Int_32{0b0001}), L"lane 0", LINE_INFO());
      Assert::AreEqual(ASizeT{2}, highestLane(Int_32{0b0100}), L"lane 2", LINE_INFO());
      Assert::AreEqual(ASizeT{7}, highestLane(Int_32{0b1000'0000}), L"lane 7", LINE_INFO());
    }

    TEST_METHOD(returnsHighestOfSeveralSetLanes) {
      Assert::AreEqual(ASizeT{3}, highestLane(Int_32{0b1011}), L"lanes 0, 1, 3", LINE_INFO());
      Assert::AreEqual(ASizeT{5}, highestLane(Int_32{0b0010'0110}), L"lanes 1, 2, 5", LINE_INFO());
    }
  };
}
