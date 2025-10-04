#pragma once

#include "../Shader.h"

namespace raytracer {
  template <typename ContainmentType, typename IntersectionType, typename OutputType>
  class ConstShader final : public Shader<ContainmentType, IntersectionType, OutputType> {
    OutputType constant;

  public:
    explicit ConstShader(const OutputType &constantIn) : constant(constantIn) {
    }

    ~ConstShader() override {
    }

    OutputType sample(const ContainmentType &, const IntersectionType &) const override {
      return constant;
    }

    OutputType operator()(const ContainmentType &, const IntersectionType &) const override {
      return constant;
    }

    OutputType &getConstant() const {
      return constant;
    }
  };
}
