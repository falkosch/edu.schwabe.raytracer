#include "raytracing_tests.h"

#include <raytracing/AdaptiveRounds.h>

namespace raytracer::test {
  TEST_CLASS(AdaptiveRoundsTest) {
    static constexpr ASizeT MaxRounds = 3;
    static constexpr Float Threshold = 0.01f;

    // A 5 x 4 image whose pixels' first rounds all have the error
    static AdaptiveRounds uniform(const Float error) {
      auto rounds = AdaptiveRounds(Size2(5, 4));
      for (auto i = Zero<ASizeT>(); i < 20; ++i) {
        rounds.setFirstRoundError(i, error);
      }
      return rounds;
    }

    static ASizeT roundsOf(const AdaptiveRounds &rounds, const Size2 &pixel) {
      return rounds.roundsOf(pixel, MaxRounds, Threshold);
    }

  public:
    TEST_METHOD(roundsBringTheNeighboursErrorBelowTheThreshold) {
      // k rounds divide the error by sqrt(k + 1): 1.5 times the threshold needs 2 (2.25 / 3 < 1 < 2.25 / 2), 1.9
      // times 3, below it none
      const auto center = Size2(2, 1);
      Assert::AreEqual(Zero<ASizeT>(), roundsOf(uniform(0.9f * Threshold), center), L"below", LINE_INFO());
      Assert::AreEqual(One<ASizeT>(), roundsOf(uniform(1.2f * Threshold), center), L"1.2 times", LINE_INFO());
      Assert::AreEqual(Two<ASizeT>(), roundsOf(uniform(1.5f * Threshold), center), L"1.5 times", LINE_INFO());
      Assert::AreEqual(MaxRounds, roundsOf(uniform(1.9f * Threshold), center), L"1.9 times", LINE_INFO());
      Assert::AreEqual(MaxRounds, roundsOf(uniform(10.0f * Threshold), center), L"at most", LINE_INFO());
    }

    TEST_METHOD(aPixelsOwnErrorDoesntCount) {
      // a spreading pixel among quiet ones takes no further rounds, its neighbours do
      auto rounds = uniform(0.0f);
      rounds.setFirstRoundError(1 * 5 + 2, 1.0f);
      Assert::AreEqual(Zero<ASizeT>(), roundsOf(rounds, Size2(2, 1)), L"the pixel", LINE_INFO());
      Assert::AreEqual(MaxRounds, roundsOf(rounds, Size2(1, 0)), L"a neighbour", LINE_INFO());
      Assert::AreEqual(MaxRounds, roundsOf(rounds, Size2(3, 2)), L"a diagonal neighbour", LINE_INFO());
      Assert::AreEqual(Zero<ASizeT>(), roundsOf(rounds, Size2(4, 3)), L"further away", LINE_INFO());
    }

    TEST_METHOD(neighboursPoolTheirSquaredErrors) {
      // one of the corner's three neighbours spreads: the root mean square of (0.025, 0, 0) is 0.025 / sqrt(3), its
      // square about 2.1 times the threshold's, so 2 rounds
      auto rounds = uniform(0.0f);
      rounds.setFirstRoundError(1, 0.025f);
      Assert::AreEqual(Two<ASizeT>(), roundsOf(rounds, Size2(0, 0)), L"corner", LINE_INFO());
    }

    TEST_METHOD(aSinglePixelHasNoNeighbours) {
      auto rounds = AdaptiveRounds(Size2(1, 1));
      rounds.setFirstRoundError(0, 1.0f);
      Assert::AreEqual(Zero<ASizeT>(), roundsOf(rounds, Size2(0, 0)), L"no neighbours", LINE_INFO());
    }
  };
}
