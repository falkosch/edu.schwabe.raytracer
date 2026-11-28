#include "raytracing/common/noise/SimplexNoiseGenerator.h"
#include "../../../stdafx.h"

#include <vectorization.h>

namespace raytracer {
  SimplexNoiseGenerator::SimplexNoiseGenerator(const unsigned int seed) : simplexGrid() {
    srand(seed);
    for (ASizeT i{0}; i < N; ++i) {
      simplexGrid[i] = rand() % static_cast<Int>(B);
    }
  }

  Float SimplexNoiseGenerator::noise(const Float v) const {
    return noise3(Float4(v));
  }

  Float SimplexNoiseGenerator::noise(const Float4 &v) const {
    return noise3(v);
  }

  Float SimplexNoiseGenerator::noise2(const Float4 &v) const {
    return noise3(v);
  }

  Float SimplexNoiseGenerator::noise3(const Float4 &v) const {
    const auto ijk = floor(v + staticAverage<3, Float4::ValueType>(&v[0]));
    const auto uvw = v - ijk + Half<Float>() * staticAverage<3>(&ijk[0]);
    const auto hi1 = select(x(uvw) >= y(uvw), Zero<Int>(), One<Int>());
    const auto hi2 = select(y(uvw) >= z(uvw), One<Int>(), Two<Int>());
    const auto hi = select(x(uvw) >= z(uvw), hi1, hi2);
    const auto lo1 = select(x(uvw) < y(uvw), Zero<Int>(), One<Int>());
    const auto lo2 = select(y(uvw) < z(uvw), One<Int>(), Two<Int>());
    const auto lo = select(x(uvw) < z(uvw), lo1, lo2);
    // K() walks the simplex's corners by advancing A, so the calls must run in this order
    auto A = Zero<Float3>();
    auto sum = K(hi, ijk, uvw, A);
    sum += K(3 - hi - lo, ijk, uvw, A);
    sum += K(lo, ijk, uvw, A);
    return sum + K(Zero<Int>(), ijk, uvw, A);
  }

  Float SimplexNoiseGenerator::noise4(const Float4 &v) const {
    return noise3(v);
  }

  Float SimplexNoiseGenerator::deviation() const {
    // measured over 200000 random points of noise3; the extremes reach about 3 times as far
    return static_cast<Float>(0.11);
  }

  Int SimplexNoiseGenerator::shuffle(const Int3 &ijk) const {
    const auto jki = Int3(y(ijk), z(ijk), x(ijk));
    const auto kij = Int3(z(ijk), x(ijk), y(ijk));
    return simplex(ijk, Zero<Int>()) + simplex(jki, One<Int>()) + simplex(kij, Two<Int>()) + simplex(ijk, 3)
           + simplex(jki, 4) + simplex(kij, 5) + simplex(ijk, 6) + simplex(jki, 7);
  }

  Int SimplexNoiseGenerator::simplex(const Int3 &ijk, const Int b) const {
    const auto skew = ((ijk >> Int3(b)) & One<Int3>()) << Int3(Two<Int>(), One<Int>(), Zero<Int>());
    return simplexGrid[x(skew) | y(skew) | z(skew)];
  }

  Float SimplexNoiseGenerator::K(const Int a, const Float4 &ijk, const Float4 &uvw, Float4 &A) const {
    // the corner A of the simplex, hashed before A advances to the next corner (Perlin's reference order)
    const auto xyz = uvw - A + Half<Float>() * staticAverage<3, Float4::ValueType>(&A[0]);
    const auto h = shuffle(convert<Int3>(ijk + A));
    A = replaceComponent(A, A + One<Float4>(), a);

    // only x, y and z: the w lanes of uvw and A hold leftovers of the skewing. The squared kernel radius 0.6 is the
    // reference's; it reaches slightly beyond the simplex (0.5 would end at its faces), which leaves steps of up to
    // about 1e-3 at the faces, small against the output range of about +-0.3.
    auto t = static_cast<Float>(0.6) - dot3(xyz, xyz);
    if (t < Zero<Float>()) {
      return Zero<Float>();
    }

    const auto b5 = (h >> 5) & One<Int>();
    const auto b4 = (h >> 4) & One<Int>();
    const auto b3 = (h >> 3) & One<Int>();
    const auto b2 = h & 4;
    const auto b1 = h & 3;

    const auto b1eq0 = b1 == Zero<Int>();
    const auto b1eq1 = b1 == One<Int>();
    const auto b1eq2 = b1 == Two<Int>();
    auto p = select(b1eq1, x(xyz), select(b1eq2, y(xyz), z(xyz)));
    auto q = select(b1eq1, y(xyz), select(b1eq2, z(xyz), x(xyz)));
    auto r = select(b1eq1, z(xyz), select(b1eq2, x(xyz), y(xyz)));
    p = select(b5 == b3, -p, p);
    q = select(b5 == b4, -q, q);
    r = select(b5 != (b4 ^ b3), -r, r);
    t *= t;
    return static_cast<Float>(8) * t * t * (p + select(b1eq0, q + r, select(b2 == Zero<Int>(), q, r)));
  }
}
