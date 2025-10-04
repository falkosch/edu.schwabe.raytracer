#pragma once

#include "../Shader.h"

#include <vectorization.h>

namespace raytracer {
  template <
      typename ContainmentType, typename IntersectionType, typename OutputType, typename ConcatenationFunctor,
      typename LeftShader, typename RightShader>
  class ConcatenatingShader : public Shader<ContainmentType, IntersectionType, OutputType> {
  public:
    ALIGNED_ALLOCATORS(__alignof(ConcatenatingShader));

    ConcatenatingShader(ConcatenationFunctor concat, const LeftShader &left, const RightShader &right)
        : concat(concat), left(&left), right(&right) {
    }

    virtual ~ConcatenatingShader() {
    }

    const OutputType sample(const ContainmentType &containment, const IntersectionType &intersection) const {
      return (*this)(containment, intersection);
    }

    const OutputType operator()(const ContainmentType &containment, const IntersectionType &intersection) const {
      return concat((*left)(containment, intersection), (*right)(containment, intersection));
    }

    const ConcatenationFunctor &getConcat() const {
      return concat;
    }

    const LeftShader &getLeft() const {
      return *left;
    }

    const RightShader &getRight() const {
      return *right;
    }

  protected:
    ConcatenationFunctor concat;
    const LeftShader *left;
    const RightShader *right;
  };
}
