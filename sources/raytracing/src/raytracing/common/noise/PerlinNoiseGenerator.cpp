#include "raytracing/common/noise/PerlinNoiseGenerator.h"
#include "../../../stdafx.h"

#include "raytracing/common/Tools.h"

namespace raytracer {
  PerlinNoiseGenerator::PerlinNoiseGenerator(const UInt_64 seed)
      : gradients1(), gradients2(), gradients3(), gradients4(), permutations(),
        permutationsCount(DefaultPermutationsCount), iPermutationsCount(static_cast<Int>(DefaultPermutationsCount)),
        iPermutationsCountS1(static_cast<Int>(DefaultPermutationsCount - One<ASizeT>())),
        fPermutationsCount(static_cast<Float>(DefaultPermutationsCount)) {
    srand(static_cast<UInt_32>(seed));
    const Int4 permutationsOffset = Int4(iPermutationsCount);
    const Int4 permutationsScale = permutationsOffset + permutationsOffset;
    const Float4 permutationsNormalization = Float4(reciprocal(fPermutationsCount));

    // Initialize memory blocks
    const ASizeT permutationsCountExtended = permutationsCount + permutationsCount + Two<ASizeT>();
    permutations = AlignedAllocator<Int>::allocElements(permutationsCountExtended);
    gradients1 = new Float4[permutationsCountExtended];
    gradients2 = new Float4[permutationsCountExtended];
    gradients3 = new Float4[permutationsCountExtended];
    gradients4 = new Float4[permutationsCountExtended];

    // Initialize permutation and gradients tables
    for (ASizeT i = Zero<ASizeT>(); i < permutationsCount; ++i) {
      const Float4 randomPick = randomGradient(permutationsScale, permutationsOffset, permutationsNormalization);
      permutations[i] = static_cast<Int>(i);
      gradients1[i] = randomPick;
      // for multidimensional gradients, the gradients are normalized to still nicely fit into the [-1,1] range
      gradients2[i] = normalize(randomPick * Float4(One<Float>(), One<Float>()));
      gradients3[i] = normalize3(randomPick);
      gradients4[i] = normalize(randomPick);
    }

    // make the random permutations
    for (ASizeT i = Zero<ASizeT>(); i < permutationsCount; ++i)
      std::swap(permutations[i], permutations[rand() % iPermutationsCount]);

    // replicate values for upper table-part
    for (ASizeT i = Zero<ASizeT>(); i < permutationsCountExtended - permutationsCount; ++i) {
      const ASizeT j = permutationsCount + i;
      permutations[j] = permutations[i];
      gradients1[j] = gradients1[i];
      gradients2[j] = gradients2[i];
      gradients3[j] = gradients3[i];
      gradients4[j] = gradients4[i];
    }
  }

  const Float4
  PerlinNoiseGenerator::randomGradient(const Int4 &scale, const Int4 &offset, const Float4 &normalization) {
    const Int4 t = Int4(rand(), rand(), rand(), rand());
    return convert<Float4>(t % scale - offset) * normalization;
  }

  void PerlinNoiseGenerator::setup(const Float &v, Int4 &b, Float4 &r) const {
    const Int bOffset = static_cast<Int>(v);
    const Float t = fract(v);
    b = Int4(bOffset, bOffset + One<Int>()) & Int4(iPermutationsCount - One<Int>());
    r = Float4(t, t - One<Float>());
  }

  const Float4 PerlinNoiseGenerator::splineCurve(const Float4 &t) {
    return t * t * (Float4(3) - t - t);
  }

  const Float PerlinNoiseGenerator::noise(const Float4 &v) const {
    return this->noise4(v);
  }

  const Float PerlinNoiseGenerator::noise(const Float v) const {
    Int4 bx;
    Float4 rx;
    setup(v, bx, rx);
    const Float4 uv = rx * Float4(x(gradients1[permutations[x(bx)]]), x(gradients1[permutations[y(bx)]]));
    return mix(x(uv), y(uv), x(splineCurve(rx)));
  }

  const Float PerlinNoiseGenerator::noise2(const Float4 &v) const {
    Int4 bx, by;
    Float4 rx, ry;
    setup(x(v), bx, rx);
    setup(y(v), by, ry);
    const Int4 ij = Int4(permutations[x(bx)], permutations[y(bx)]);
    const Int4 b0 = Int4(permutations[x(ij) + x(by)], permutations[x(ij) + y(by)]);
    const Int4 b1 = Int4(permutations[y(ij) + x(by)], permutations[y(ij) + y(by)]);
    const Float4 s = splineCurve(Float4(x(rx), x(ry)));
    const Float a =
        mix(dot3(Float4(x(rx), x(ry)), gradients2[x(b0)]), dot3(Float4(y(rx), x(ry)), gradients2[x(b1)]), x(s));
    const Float b =
        mix(dot3(Float4(x(rx), y(ry)), gradients2[y(b0)]), dot3(Float4(y(rx), y(ry)), gradients2[y(b1)]), x(s));
    return mix(a, b, y(s));
  }

  const Float PerlinNoiseGenerator::noise3(const Float4 &v) const {
    Int4 bx, by, bz;
    Float4 rx, ry, rz;
    setup(x(v), bx, rx);
    setup(y(v), by, ry);
    setup(z(v), bz, rz);
    const Int4 ij = Int4(permutations[x(bx)], permutations[y(bx)]);
    const Int4 b0 = Int4(permutations[x(ij) + x(by)], permutations[x(ij) + y(by)]);
    const Int4 b1 = Int4(permutations[y(ij) + x(by)], permutations[y(ij) + y(by)]);
    const Float4 s = splineCurve(Float4(x(rx), x(ry), x(rz)));
    const Float a =
        mix(dot3(Float4(x(rx), x(ry), x(rz)), gradients3[x(b0) + x(bz)]),
            dot3(Float4(y(rx), x(ry), x(rz)), gradients3[x(b1) + x(bz)]), x(s));
    const Float b =
        mix(dot3(Float4(x(rx), y(ry), x(rz)), gradients3[y(b0) + x(bz)]),
            dot3(Float4(y(rx), y(ry), x(rz)), gradients3[y(b1) + x(bz)]), x(s));
    const Float c =
        mix(dot3(Float4(x(rx), x(ry), y(rz)), gradients3[x(b0) + y(bz)]),
            dot3(Float4(y(rx), x(ry), y(rz)), gradients3[x(b1) + y(bz)]), x(s));
    const Float d =
        mix(dot3(Float4(x(rx), y(ry), y(rz)), gradients3[y(b0) + y(bz)]),
            dot3(Float4(y(rx), y(ry), y(rz)), gradients3[y(b1) + y(bz)]), x(s));
    return mix(mix(a, b, y(s)), mix(c, d, y(s)), z(s));
  }

  const Float PerlinNoiseGenerator::noise4(const Float4 &v) const {
    Int4 bx, by, bz, bw;
    Float4 rx, ry, rz, rw;
    setup(x(v), bx, rx);
    setup(y(v), by, ry);
    setup(z(v), bz, rz);
    setup(w(v), bz, rz);
    const Int4 ij = Int4(permutations[x(bx)], permutations[y(bx)]);
    const Int4 b0 = Int4(permutations[x(ij) + x(by)], permutations[x(ij) + y(by)]);
    const Int4 b1 = Int4(permutations[y(ij) + x(by)], permutations[y(ij) + y(by)]);
    const Float4 s = splineCurve(Float4(x(rx), x(ry), x(rz), x(rw)));
    const Float a =
        mix(dot(Float4(x(rx), x(ry), x(rz), One<Float>()), gradients4[x(b0) + x(bz)]),
            dot(Float4(y(rx), x(ry), x(rz), One<Float>()), gradients4[x(b1) + x(bz)]), x(s));
    const Float b =
        mix(dot(Float4(x(rx), y(ry), x(rz), One<Float>()), gradients4[y(b0) + x(bz)]),
            dot(Float4(y(rx), y(ry), x(rz), One<Float>()), gradients4[y(b1) + x(bz)]), x(s));
    const Float c =
        mix(dot(Float4(x(rx), x(ry), y(rz), One<Float>()), gradients4[x(b0) + y(bz)]),
            dot(Float4(y(rx), x(ry), y(rz), One<Float>()), gradients4[x(b1) + y(bz)]), x(s));
    const Float d =
        mix(dot(Float4(x(rx), y(ry), y(rz), One<Float>()), gradients4[y(b0) + y(bz)]),
            dot(Float4(y(rx), y(ry), y(rz), One<Float>()), gradients4[y(b1) + y(bz)]), x(s));
    return mix(mix(a, b, y(s)), mix(c, d, y(s)), z(s));
  }
}