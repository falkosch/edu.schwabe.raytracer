#pragma once

#include "../../common/noise/NoiseGenerator.h"
#include "../ObjectShader.h"

namespace raytracer {
  using namespace vectorization;

  template <ASizeT OctavesCount>
  class NoiseGeneratorMap : public Shader<ObjectShader, FacetIntersection, Float4> {
    const NoiseGenerator *generator;

  public:
    ALIGNED_ALLOCATORS(__alignof(NoiseGeneratorMap));

    NoiseGeneratorMap(const NoiseGenerator &generatorIn) : generator(&generatorIn) {
    }

    virtual ~NoiseGeneratorMap() {
    }

     Float4 sample(const ObjectShader &objectShader, const FacetIntersection &intersection) const {
      return (*this)(objectShader, intersection);
    }

     Float4 operator()(const ObjectShader & /*objectShader*/, const FacetIntersection &intersection) const {
      Float4 noise = Zero<Float4>();
      Float4 frequency = One<Float4>();

      for (ASizeT i = Zero<ASizeT>(); i < OctavesCount; ++i) {
        noise =
            noise * Half<Float4>() + Float4(vectorization::sin(generator->noise4(intersection.texCoords * frequency)));
        frequency += frequency;
      }

      return clamp(noise, Zero<Float4>(), One<Float4>());
    }
  };
}
