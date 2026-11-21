#include "primitives_tests.h"

#include <array>
#include <cmath>
#include <random>

namespace primitives::test {
  namespace {
    constexpr ASizeT Lanes{FacetLanes::SIZE};
    constexpr unsigned AllLanes{(1u << FacetLanes::SIZE) - 1u};
    constexpr auto MaxDistance = 100.0f;

    // plane normals of the facet (v0, v1, v2) as the mesh computes them for nearestIntersectionHavel
    FacetNormals planeNormalsOf(const Float4 &v0, const Float4 &v1, const Float4 &v2) {
      const auto edge1 = v1 - v0;
      const auto edge2 = v2 - v0;
      const auto planeNormal0 = cross3(edge1, edge2);
      const auto planeNormal1 = cross3(edge2, planeNormal0);
      const auto planeNormal2 = cross3(planeNormal0, edge1);
      auto planeNormals = FacetNormals{
          replaceW(planeNormal0, -dot3(v0, planeNormal0)), replaceW(planeNormal1, -dot3(v0, planeNormal1)),
          replaceW(planeNormal2, -dot3(v0, planeNormal2))
      };
      planeNormals.v1 /= dotv(planeNormals.v1, v1);
      planeNormals.v2 /= dotv(planeNormals.v2, v2);
      return planeNormals;
    }

    FacetNormalsBlock blockOf(const std::array<FacetNormals, Lanes> &facets) {
      auto block = FacetNormalsBlock{};
      for (auto lane = Zero<ASizeT>(); lane < Lanes; ++lane) {
        for (auto c = Zero<ASizeT>(); c < 4; ++c) {
          block.v0[c][lane] = facets[lane].v0[c];
          block.v1[c][lane] = facets[lane].v1[c];
          block.v2[c][lane] = facets[lane].v2[c];
        }
      }
      return block;
    }

    Ray randomRay(std::mt19937 &generator) {
      auto uniform = std::uniform_real_distribution<Float>(-1.0f, 1.0f);
      const auto origin = Float4{2.0f * uniform(generator), 2.0f * uniform(generator), 2.0f * uniform(generator), 1.0f};
      auto direction = Zero<Float4>();
      while (length3(direction) < 0.1f) {
        direction = Float4{uniform(generator), uniform(generator), uniform(generator), 0.0f};
      }
      return Ray{origin, normalize3(direction)};
    }

    // Barycentric coordinates { u, v } of a point clear of the facet's edges: inside if hit, outside otherwise.
    Float4 randomTexCoords(const bool hit, std::mt19937 &generator) {
      auto uniform = std::uniform_real_distribution<Float>(0.0f, 1.0f);
      if (hit) {
        const auto u = 0.1f + 0.7f * uniform(generator);
        const auto v = 0.1f + (0.8f - u) * uniform(generator);
        return Float4{u, v, 0.0f, 0.0f};
      }
      while (true) {
        const auto u = -1.0f + 3.0f * uniform(generator);
        const auto v = -1.0f + 3.0f * uniform(generator);
        if (u < -0.1f || v < -0.1f || u + v > 1.1f) {
          return Float4{u, v, 0.0f, 0.0f};
        }
      }
    }

    // A facet of random shape and orientation, never close to parallel to the ray, that the ray crosses at distance t
    // and at the barycentric coordinates texCoords.
    FacetNormals facetCrossing(const Ray &ray, const Float t, const Float4 &texCoords, std::mt19937 &generator) {
      auto uniform = std::uniform_real_distribution<Float>(-1.0f, 1.0f);
      while (true) {
        const auto edge1 = Float4{uniform(generator), uniform(generator), uniform(generator), 0.0f};
        const auto edge2 = Float4{uniform(generator), uniform(generator), uniform(generator), 0.0f};
        const auto normal = cross3(edge1, edge2);
        if (length3(normal) < 0.2f || std::abs(dot3(normalize3(normal), ray.direction)) < 0.3f) {
          continue;
        }
        const auto point = ray.origin + ray.direction * Float4{t};
        const auto v0 = point - edge1 * xxxx(texCoords) - edge2 * yyyy(texCoords);
        return planeNormalsOf(v0, v0 + edge1, v0 + edge2);
      }
    }

    // lane i crosses its facet at distance i + 1
    Float crossingDistance(const ASizeT lane) {
      return static_cast<Float>(lane + One<ASizeT>());
    }

    // culled() of the mesh's single-facet loop
    bool culled(const FacetNormals &planeNormals, const RayCast &rayCast) {
      const auto frontfaced = dot3(planeNormals.v0, rayCast.ray.direction) < Zero<Float>();
      return (!frontfaced && backfaceCulled(rayCast)) || (frontfaced && frontfaceCulled(rayCast));
    }

    FacetBlockTest testAll(const FacetNormalsBlock &block, const RayCast &rayCast, const Float maxDistance) {
      return intersectionsHavel(
          block, FacetBlockRay{rayCast.ray}, FacetLanes{maxDistance}, AllLanes, backfaceCulled(rayCast),
          frontfaceCulled(rayCast)
      );
    }
  }

  TEST_CLASS(FacetNormalsBlockTest) {
  public:
    TEST_METHOD(matchesNearestIntersectionHavelPerLane) {
      // Each lane decides like nearestIntersectionHavel plus the mesh's culling. The rays cross the facets clear of
      // their edges, as the block test's FMA dot products may decide grazing rays differently.
      auto generator = std::mt19937{0x5eed1234u};
      auto coin = std::bernoulli_distribution(0.5);
      auto hits = Zero<int>();
      auto misses = Zero<int>();
      auto culledHits = Zero<int>();
      for (auto i = Zero<int>(); i < 64; ++i) {
        const auto ray = randomRay(generator);
        auto facets = std::array<FacetNormals, Lanes>();
        auto texCoords = std::array<Float4, Lanes>();
        for (auto lane = Zero<ASizeT>(); lane < Lanes; ++lane) {
          texCoords[lane] = randomTexCoords(coin(generator), generator);
          facets[lane] = facetCrossing(ray, crossingDistance(lane), texCoords[lane], generator);
        }
        const auto block = blockOf(facets);

        for (const auto culling : {-1, 0, 1}) {
          const RayCast rayCast{ray, cullingOrientationToMask(culling), Size2(0, 0), MaxDistance};
          const auto test = testAll(block, rayCast, MaxDistance);
          for (auto lane = Zero<ASizeT>(); lane < Lanes; ++lane) {
            const auto single = nearestIntersectionHavel(facets[lane], rayCast, Float4{MaxDistance});
            const auto singleHit = z(single) < MaxDistance;
            const auto expectedHit = singleHit && !culled(facets[lane], rayCast);
            const auto blockHit = (test.hits >> lane & One<unsigned>()) != Zero<unsigned>();
            Assert::AreEqual(expectedHit, blockHit, L"lane hit", LINE_INFO());
            if (!blockHit) {
              misses += singleHit ? Zero<int>() : One<int>();
              culledHits += singleHit ? One<int>() : Zero<int>();
              continue;
            }
            ++hits;
            const auto result = texCoordsAndDistance(test, static_cast<int>(lane));
            Assert::AreEqual(x(single), x(result), 1e-4f, L"u like nearestIntersectionHavel", LINE_INFO());
            Assert::AreEqual(y(single), y(result), 1e-4f, L"v like nearestIntersectionHavel", LINE_INFO());
            Assert::AreEqual(z(single), z(result), 1e-4f, L"distance like nearestIntersectionHavel", LINE_INFO());
            Assert::AreEqual(x(texCoords[lane]), x(result), 1e-3f, L"u of the crossing", LINE_INFO());
            Assert::AreEqual(y(texCoords[lane]), y(result), 1e-3f, L"v of the crossing", LINE_INFO());
            Assert::AreEqual(crossingDistance(lane), z(result), 1e-3f, L"distance of the crossing", LINE_INFO());
          }
        }
      }
      Assert::IsTrue(hits > 100, L"enough hits", LINE_INFO());
      Assert::IsTrue(misses > 100, L"enough misses", LINE_INFO());
      Assert::IsTrue(culledHits > 50, L"enough culled hits", LINE_INFO());
    }

    TEST_METHOD(testsOnlyCandidateLanes) {
      auto generator = std::mt19937{0x0ca1d1d5u};
      const auto ray = randomRay(generator);
      auto facets = std::array<FacetNormals, Lanes>();
      for (auto lane = Zero<ASizeT>(); lane < Lanes; ++lane) {
        facets[lane] = facetCrossing(ray, crossingDistance(lane), randomTexCoords(true, generator), generator);
      }
      const auto block = blockOf(facets);
      const RayCast rayCast{ray, cullingOrientationToMask(0), Size2(0, 0), MaxDistance};
      Assert::AreEqual(AllLanes, testAll(block, rayCast, MaxDistance).hits, L"every lane hits", LINE_INFO());

      for (const auto candidates : {0x00u, 0x01u, 0x80u, 0x5au, 0x3cu, 0xfeu}) {
        const auto test = intersectionsHavel(
            block, FacetBlockRay{ray}, FacetLanes{MaxDistance}, candidates, backfaceCulled(rayCast),
            frontfaceCulled(rayCast)
        );
        Assert::AreEqual(candidates, test.hits, L"hits are the candidates", LINE_INFO());
      }
    }

    TEST_METHOD(rejectsHitsBeyondMaxDistance) {
      auto generator = std::mt19937{0xd15ea5e5u};
      const auto ray = randomRay(generator);
      auto facets = std::array<FacetNormals, Lanes>();
      for (auto lane = Zero<ASizeT>(); lane < Lanes; ++lane) {
        facets[lane] = facetCrossing(ray, crossingDistance(lane), randomTexCoords(true, generator), generator);
      }
      const auto block = blockOf(facets);
      const RayCast rayCast{ray, cullingOrientationToMask(0), Size2(0, 0), MaxDistance};

      // lanes 0 to 3 cross at the distances 1 to 4
      Assert::AreEqual(0x0fu, testAll(block, rayCast, 4.5f).hits, L"hits up to maxDistance", LINE_INFO());
      Assert::AreEqual(0x00u, testAll(block, rayCast, 0.5f).hits, L"no hit before the first facet", LINE_INFO());
    }

    TEST_METHOD(rejectsFacetsBehindTheOrigin) {
      auto generator = std::mt19937{0xbe41d0u};
      const auto ray = randomRay(generator);
      auto facets = std::array<FacetNormals, Lanes>();
      for (auto lane = Zero<ASizeT>(); lane < Lanes; ++lane) {
        facets[lane] = facetCrossing(ray, crossingDistance(lane), randomTexCoords(true, generator), generator);
      }
      const auto block = blockOf(facets);
      const Ray reversed{ray.origin, -ray.direction};
      const RayCast rayCast{reversed, cullingOrientationToMask(0), Size2(0, 0), MaxDistance};
      Assert::AreEqual(0x00u, testAll(block, rayCast, MaxDistance).hits, L"no hit behind the origin", LINE_INFO());
    }
  };
}
