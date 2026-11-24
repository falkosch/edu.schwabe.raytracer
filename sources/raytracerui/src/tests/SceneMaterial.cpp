#include "tests/SceneMaterial.h"
#include "../stdafx.h"

namespace raytracerui
{
    namespace
    {
        const Float4 VACUUM_TO_VACUUM = RefractionIndices::refractionEta(RefractionIndices::Vacuum,
                                                                        RefractionIndices::Vacuum);
    }

    SceneMaterial SceneMaterial::matte(const Float4& color)
    {
        return {
            zeroW(color), Zero<Float4>(), Zero<Float4>(), Float4{1.f, 1.f, 1.f, 0.f}, Zero<Float4>(), VACUUM_TO_VACUUM,
            Zero<Float4>()
        };
    }

    SceneMaterial SceneMaterial::plastic(const Float4& color, const Float specular, const Float roughness)
    {
        return {
            zeroW(color), Float4{1.f, 1.f, 1.f, 0.f}, zeroW(Float4{specular}), zeroW(Float4{roughness}),
            Zero<Float4>(), VACUUM_TO_VACUUM, Zero<Float4>()
        };
    }

    SceneMaterial SceneMaterial::metal(const Float4& color, const Float roughness)
    {
        return {
            Zero<Float4>(), Float4{1.f, 1.f, 1.f, 0.f}, zeroW(color), zeroW(Float4{roughness}), Zero<Float4>(),
            VACUUM_TO_VACUUM, Zero<Float4>()
        };
    }

    SceneMaterial SceneMaterial::dielectric(
        const Float refractionIndex, const Float4& transmittance, const Float roughness
    )
    {
        // the specular lobe takes its Fresnel reflectance from the indices; F0 for where the medium is opaque (no
        // transmittance)
        const auto f0 = sqr((refractionIndex - One<Float>()) / (refractionIndex + One<Float>()));
        return {
            Zero<Float4>(), Float4{1.f, 1.f, 1.f, 0.f}, zeroW(Float4{f0}), zeroW(Float4{roughness}),
            zeroW(transmittance), RefractionIndices::refractionEta(RefractionIndices::Vacuum, refractionIndex),
            Zero<Float4>()
        };
    }

    SceneMaterial SceneMaterial::emissive(const Float4& color)
    {
        return {
            Zero<Float4>(), Zero<Float4>(), Zero<Float4>(), Float4{1.f, 1.f, 1.f, 0.f}, Zero<Float4>(),
            VACUUM_TO_VACUUM, zeroW(color)
        };
    }

    void SceneMaterial::applyTo(SceneObject& object) const
    {
        object.setDiffusionShader(std::make_unique<Resources::ConstMaterialShader>(diffusion));
        object.setReflectanceShader(std::make_unique<Resources::ConstMaterialShader>(reflectance));
        object.setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(specular));
        object.setRoughnessShader(std::make_unique<Resources::ConstMaterialShader>(roughness));
        object.setTransmittanceShader(std::make_unique<Resources::ConstMaterialShader>(transmittance));
        object.setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(refractionEta));
        object.setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(emittance));
    }
}
