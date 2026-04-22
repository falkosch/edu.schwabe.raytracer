#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(BroadcastTest) {
  public:
    TEST_METHOD(broadcastsFloat32ToAllLanes) {
      constexpr Float_32 given = 3.14f;
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), broadcast(given));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(given, actual[i], L"broadcast float32 lane mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(broadcastsFloat32Zero) {
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), broadcast(Zero<Float_32>()));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(Zero<Float_32>(), actual[i], L"broadcast zero lane mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(broadcastsFloat32NegativeValue) {
      constexpr Float_32 given = -2.5f;
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), broadcast(given));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(given, actual[i], L"broadcast negative float32 lane mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(broadcastsFloat32MatchesSetPs1) {
      constexpr Float_32 given = 42.0f;
      std::array<Float_32, VectorSizes::W> expected{};
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(expected.data(), _mm_set_ps1(given));
      _mm_store_ps(actual.data(), broadcast(given));
      Assert::AreEqual(expected, actual, L"broadcast must match _mm_set_ps1", LINE_INFO());
    }

    TEST_METHOD(broadcastsFloat64ToAllLanes) {
      constexpr Float_64 given = 2.718281828;
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), broadcast(given));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(given, actual[i], L"broadcast float64 lane mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(broadcastsFloat64Zero) {
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), broadcast(Zero<Float_64>()));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(Zero<Float_64>(), actual[i], L"broadcast zero float64 lane mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(broadcastsFloat64MatchesSet1Pd) {
      constexpr Float_64 given = -99.5;
      std::array<Float_64, VectorSizes::X2> expected{};
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(expected.data(), _mm_set1_pd(given));
      _mm_store_pd(actual.data(), broadcast(given));
      Assert::AreEqual(expected, actual, L"broadcast must match _mm_set1_pd", LINE_INFO());
    }

    TEST_METHOD(broadcastsInt32ToAllLanes) {
      constexpr Int_32 given = 7;
      alignas(XMM_ALIGNMENT) std::array<Int_32, VectorSizes::W> actual{};
      _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()), broadcast(given));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(given, actual[i], L"broadcast int32 lane mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(broadcastsInt32NegativeValue) {
      constexpr Int_32 given = -13;
      alignas(XMM_ALIGNMENT) std::array<Int_32, VectorSizes::W> actual{};
      _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()), broadcast(given));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(given, actual[i], L"broadcast negative int32 lane mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(broadcastsInt32Zero) {
      alignas(XMM_ALIGNMENT) std::array<Int_32, VectorSizes::W> actual{};
      _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()), broadcast(Zero<Int_32>()));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(Zero<Int_32>(), actual[i], L"broadcast zero int32 lane mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(broadcastsInt32MatchesSet1Epi32) {
      constexpr Int_32 given = 255;
      alignas(XMM_ALIGNMENT) std::array<Int_32, VectorSizes::W> expected{};
      alignas(XMM_ALIGNMENT) std::array<Int_32, VectorSizes::W> actual{};
      _mm_store_si128(reinterpret_cast<PackedInts_128 *>(expected.data()), _mm_set1_epi32(given));
      _mm_store_si128(reinterpret_cast<PackedInts_128 *>(actual.data()), broadcast(given));
      Assert::AreEqual(expected, actual, L"broadcast must match _mm_set1_epi32", LINE_INFO());
    }
  };
}
