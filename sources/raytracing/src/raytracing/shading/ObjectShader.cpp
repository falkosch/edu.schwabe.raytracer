#include "raytracing/shading/ObjectShader.h"
#include "../../stdafx.h"

namespace raytracer {
  ObjectShader::ObjectShader()
      : ObjectGeometry(), textureMatrix(), textureTransformInfo(), diffusionShader(), reflectanceShader(),
        specularShader(), shininessShader(), transmittanceShader(), refractionEtaShader(), emittanceShader() {
    resetTextureMatrix();
  }

  ObjectShader::~ObjectShader() = default;

  const ObjectShader::MaterialShader *const ObjectShader::getDiffusionShader() const {
    return diffusionShader;
  }

  void ObjectShader::setDiffusionShader(const ObjectShader::MaterialShader *const value) {
    diffusionShader = value;
  }

  const ObjectShader::MaterialShader *const ObjectShader::getReflectanceShader() const {
    return reflectanceShader;
  }

  void ObjectShader::setReflectanceShader(const ObjectShader::MaterialShader *const value) {
    reflectanceShader = value;
  }

  const ObjectShader::MaterialShader *const ObjectShader::getSpecularShader() const {
    return specularShader;
  }

  void ObjectShader::setSpecularShader(const ObjectShader::MaterialShader *const value) {
    specularShader = value;
  }

  const ObjectShader::MaterialShader *const ObjectShader::getShininessShader() const {
    return shininessShader;
  }

  void ObjectShader::setShininessShader(const ObjectShader::MaterialShader *const value) {
    shininessShader = value;
  }

  const ObjectShader::MaterialShader *const ObjectShader::getTransmittanceShader() const {
    return transmittanceShader;
  }

  void ObjectShader::setTransmittanceShader(const ObjectShader::MaterialShader *const value) {
    transmittanceShader = value;
  }

  const ObjectShader::MaterialShader *const ObjectShader::getRefractionEtaShader() const {
    return refractionEtaShader;
  }

  void ObjectShader::setRefractionEtaShader(const ObjectShader::MaterialShader *const value) {
    refractionEtaShader = value;
  }

  const ObjectShader::MaterialShader *const ObjectShader::getEmittanceShader() const {
    return emittanceShader;
  }

  void ObjectShader::setEmittanceShader(const ObjectShader::MaterialShader *const value) {
    emittanceShader = value;
  }

  void ObjectShader::resetTextureMatrix() {
    textureTransformInfo = TextureTransformInfo();
    updateTextureMatrix();
  }

  void ObjectShader::rotateTexture(const Float angle) {
    textureTransformInfo.rotation += angle;
    updateTextureMatrix();
  }

  void ObjectShader::translateTexture(const Float2 &translationIn) {
    textureTransformInfo.translation += translationIn;
    updateTextureMatrix();
  }

  void ObjectShader::scaleTexture(const Float2 &scaleIn) {
    textureTransformInfo.scale *= scaleIn;
    updateTextureMatrix();
  }

  void ObjectShader::updateTextureMatrix() {
    // transforms are applied in reverse order, like in OGL!
    textureMatrix = vectorization::scale(
        vectorization::rotate(
            vectorization::translate(Identity<Float44>(), convert<Float4>(textureTransformInfo.translation)),
            textureTransformInfo.rotation, OneZ<Float4>()
        ),
        xy_xy(convert<Float4>(textureTransformInfo.scale), One<Float4>())
    );
  }

  const Float4 sampleRaw(
      const ObjectShader &objectShader, const ObjectShader::MaterialShader &shader,
      const SceneIntersection &intersection
  ) {
    return shader(objectShader, intersection);
  }

  const Float4 sampleMultiplyW(
      const ObjectShader &objectShader, const ObjectShader::MaterialShader &shader,
      const SceneIntersection &intersection
  ) {
    const Float4 t = sampleRaw(objectShader, shader, intersection);
    return t * wwww(t);
  }

  const SurfaceShading ObjectShader::sample(const SceneShader &, const SceneIntersection &intersection) const {
    return SurfaceShading(
        sampleMultiplyW(*this, *diffusionShader, intersection),
        sampleMultiplyW(*this, *reflectanceShader, intersection), sampleMultiplyW(*this, *specularShader, intersection),
        sampleMultiplyW(*this, *shininessShader, intersection),
        sampleMultiplyW(*this, *transmittanceShader, intersection),
        sampleRaw(*this, *refractionEtaShader, intersection), sampleMultiplyW(*this, *emittanceShader, intersection)
    );
  }
}
