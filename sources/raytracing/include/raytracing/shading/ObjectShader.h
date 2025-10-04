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

    virtual ~ObjectShader();

    const MaterialShader *const getDiffusionShader() const;

    void setDiffusionShader(const MaterialShader *const value);

    const MaterialShader *const getReflectanceShader() const;

    void setReflectanceShader(const MaterialShader *const value);

    const MaterialShader *const getSpecularShader() const;

    void setSpecularShader(const MaterialShader *const value);

    const MaterialShader *const getShininessShader() const;

    void setShininessShader(const MaterialShader *const value);

    const MaterialShader *const getTransmittanceShader() const;

    void setTransmittanceShader(const MaterialShader *const value);

    const MaterialShader *const getRefractionEtaShader() const;

    void setRefractionEtaShader(const MaterialShader *const value);

    const MaterialShader *const getEmittanceShader() const;

    void setEmittanceShader(const MaterialShader *const value);

    void resetTextureMatrix();

    void rotateTexture(const Float angle);

    void translateTexture(const Float2 &translation);

    void scaleTexture(const Float2 &scale);

    const SurfaceShading sample(const SceneShader &sceneShader, const SceneIntersection &intersection) const;

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
