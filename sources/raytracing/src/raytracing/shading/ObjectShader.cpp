#include "raytracing/shading/ObjectShader.h"
#include "../../stdafx.h"

#include "raytracing/shading/spectral/spectrum.h"
#include "raytracing/shading/spectral/wavelengths.h"

namespace raytracer
{
    ObjectShader::ObjectShader()
        : ObjectGeometry(), textureMatrix(), textureTransformInfo(), diffusionShader(), reflectanceShader(),
          specularShader(), roughnessShader(), transmittanceShader(), refractionEtaShader(), emittanceShader()
    {
        resetTextureMatrix();
    }

    ObjectShader::~ObjectShader() = default;

    const ObjectShader::MaterialShader* ObjectShader::getDiffusionShader() const
    {
        return diffusionShader.get();
    }

    void ObjectShader::setDiffusionShader(std::unique_ptr<const MaterialShader> value)
    {
        diffusionShader = std::move(value);
    }

    const ObjectShader::MaterialShader* ObjectShader::getReflectanceShader() const
    {
        return reflectanceShader.get();
    }

    void ObjectShader::setReflectanceShader(std::unique_ptr<const MaterialShader> value)
    {
        reflectanceShader = std::move(value);
    }

    const ObjectShader::MaterialShader* ObjectShader::getSpecularShader() const
    {
        return specularShader.get();
    }

    void ObjectShader::setSpecularShader(std::unique_ptr<const MaterialShader> value)
    {
        specularShader = std::move(value);
    }

    const ObjectShader::MaterialShader* ObjectShader::getRoughnessShader() const
    {
        return roughnessShader.get();
    }

    void ObjectShader::setRoughnessShader(std::unique_ptr<const MaterialShader> value)
    {
        roughnessShader = std::move(value);
    }

    const ObjectShader::MaterialShader* ObjectShader::getTransmittanceShader() const
    {
        return transmittanceShader.get();
    }

    void ObjectShader::setTransmittanceShader(std::unique_ptr<const MaterialShader> value)
    {
        transmittanceShader = std::move(value);
    }

    const ObjectShader::MaterialShader* ObjectShader::getRefractionEtaShader() const
    {
        return refractionEtaShader.get();
    }

    void ObjectShader::setRefractionEtaShader(std::unique_ptr<const MaterialShader> value)
    {
        refractionEtaShader = std::move(value);
    }

    const ObjectShader::MaterialShader* ObjectShader::getEmittanceShader() const
    {
        return emittanceShader.get();
    }

    void ObjectShader::setEmittanceShader(std::unique_ptr<const MaterialShader> value)
    {
        emittanceShader = std::move(value);
    }

    void ObjectShader::resetTextureMatrix()
    {
        textureTransformInfo = TextureTransformInfo();
        updateTextureMatrix();
    }

    void ObjectShader::rotateTexture(const Float angle)
    {
        textureTransformInfo.rotation += angle;
        updateTextureMatrix();
    }

    void ObjectShader::translateTexture(const Float2& translationIn)
    {
        textureTransformInfo.translation += translationIn;
        updateTextureMatrix();
    }

    void ObjectShader::scaleTexture(const Float2& scaleIn)
    {
        textureTransformInfo.scale *= scaleIn;
        updateTextureMatrix();
    }

    void ObjectShader::updateTextureMatrix()
    {
        // transforms are applied in reverse order, like in OGL!
        textureMatrix = vectorization::scale(
            vectorization::rotate(
                vectorization::translate(Identity<MFloat4x4>(), convert<Float4>(textureTransformInfo.translation)),
                textureTransformInfo.rotation, OneZ<Float4>()
            ),
            xy_xy(convert<Float4>(textureTransformInfo.scale), One<Float4>())
        );
    }

    SurfaceShading ObjectShader::sample(const SceneShader&, const SceneIntersection& intersection) const
    {
        return shade(intersection, spectral::LAMBDA_MIN + spectral::LAMBDA_RANGE * Half<Float>());
    }

    SurfaceShading ObjectShader::shade(const SceneIntersection& intersection, const Float heroLambda) const
    {
        const auto wavelengths = spectral::wavelengthsAt(heroLambda);
        return {
            spectral::fromRGB(wavelengths, diffusionShader->sample(*this, intersection)),
            spectral::fromRGB(wavelengths, reflectanceShader->sample(*this, intersection)),
            spectral::fromRGB(wavelengths, specularShader->sample(*this, intersection)),
            roughnessShader->sample(*this, intersection),
            spectral::fromRGB(wavelengths, transmittanceShader->sample(*this, intersection)),
            refractionEtaShader->sample(*this, intersection),
            spectral::fromRGB(wavelengths, emittanceShader->sample(*this, intersection))
        };
    }
}
