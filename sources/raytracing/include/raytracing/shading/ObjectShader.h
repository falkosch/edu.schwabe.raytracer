#pragma once

#include "../geometry/SceneGeometry.h"

#include "SceneShader.h"
#include "SurfaceShading.h"
#include "TextureTransformInfo.h"

namespace raytracer {
  using namespace vectorization;

  class ObjectShader : public ObjectGeometry, public Shader<SceneShader, SceneIntersection, SurfaceShading> {
  public:
    typedef Shader<ObjectShader, FacetIntersection, Float4> MaterialShader;

    ObjectShader();

    ~ObjectShader() override;

    const MaterialShader *getDiffusionShader() const;
    void setDiffusionShader(const MaterialShader *value);

    const MaterialShader *getReflectanceShader() const;
    void setReflectanceShader(const MaterialShader *value);

    const MaterialShader *getSpecularShader() const;
    void setSpecularShader(const MaterialShader *value);

    const MaterialShader *getShininessShader() const;
    void setShininessShader(const MaterialShader *value);

    const MaterialShader *getTransmittanceShader() const;
    void setTransmittanceShader(const MaterialShader *value);

    const MaterialShader *getRefractionEtaShader() const;
    void setRefractionEtaShader(const MaterialShader *value);

    const MaterialShader *getEmittanceShader() const;
    void setEmittanceShader(const MaterialShader *value);

    void resetTextureMatrix();

    void rotateTexture(Float angle);

    void translateTexture(const Float2 &translation);

    void scaleTexture(const Float2 &scale);

    SurfaceShading sample(const SceneShader &sceneShader, const SceneIntersection &intersection) const override;

  protected:
    Float44 textureMatrix;

    TextureTransformInfo textureTransformInfo;

    const MaterialShader *diffusionShader;
    const MaterialShader *reflectanceShader;
    const MaterialShader *specularShader;
    const MaterialShader *shininessShader;
    const MaterialShader *transmittanceShader;
    const MaterialShader *refractionEtaShader;
    const MaterialShader *emittanceShader;

    void updateTextureMatrix();
  };
}
