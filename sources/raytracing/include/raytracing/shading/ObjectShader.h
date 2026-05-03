#pragma once

#include "../geometry/SceneGeometry.h"

#include "SceneShader.h"
#include "SurfaceShading.h"
#include "TextureTransformInfo.h"

#include <memory>

namespace raytracer
{
  using namespace vectorization;

  class ObjectShader : public ObjectGeometry, public Shader<SceneShader, SceneIntersection, SurfaceShading>
  {
  public:
    typedef Shader<ObjectShader, FacetIntersection, Float4> MaterialShader;

    ObjectShader();

    ~ObjectShader() override;

    const MaterialShader* getDiffusionShader() const;
    void setDiffusionShader(std::unique_ptr<const MaterialShader> value);

    const MaterialShader* getReflectanceShader() const;
    void setReflectanceShader(std::unique_ptr<const MaterialShader> value);

    const MaterialShader* getSpecularShader() const;
    void setSpecularShader(std::unique_ptr<const MaterialShader> value);

    const MaterialShader* getShininessShader() const;
    void setShininessShader(std::unique_ptr<const MaterialShader> value);

    const MaterialShader* getTransmittanceShader() const;
    void setTransmittanceShader(std::unique_ptr<const MaterialShader> value);

    const MaterialShader* getRefractionEtaShader() const;
    void setRefractionEtaShader(std::unique_ptr<const MaterialShader> value);

    const MaterialShader* getEmittanceShader() const;
    void setEmittanceShader(std::unique_ptr<const MaterialShader> value);

    void resetTextureMatrix();

    void rotateTexture(Float angle);

    void translateTexture(const Float2& translation);

    void scaleTexture(const Float2& scale);

    SurfaceShading sample(const SceneShader& sceneShader, const SceneIntersection& intersection) const override;

  protected:
    MFloat4x4 textureMatrix;

    TextureTransformInfo textureTransformInfo;

    std::unique_ptr<const MaterialShader> diffusionShader;
    std::unique_ptr<const MaterialShader> reflectanceShader;
    std::unique_ptr<const MaterialShader> specularShader;
    std::unique_ptr<const MaterialShader> shininessShader;
    std::unique_ptr<const MaterialShader> transmittanceShader;
    std::unique_ptr<const MaterialShader> refractionEtaShader;
    std::unique_ptr<const MaterialShader> emittanceShader;

    void updateTextureMatrix();
  };
}
