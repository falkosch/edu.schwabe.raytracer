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

        const MaterialShader* getRoughnessShader() const;
        void setRoughnessShader(std::unique_ptr<const MaterialShader> value);

        const MaterialShader* getTransmittanceShader() const;
        void setTransmittanceShader(std::unique_ptr<const MaterialShader> value);

        const MaterialShader* getRefractionEtaShader() const;
        void setRefractionEtaShader(std::unique_ptr<const MaterialShader> value);

        const MaterialShader* getEmittanceShader() const;
        void setEmittanceShader(std::unique_ptr<const MaterialShader> value);

        // The emissive object of an area light (a sphere light's sphere, LightInfo::radius, or a box light's box,
        // LightInfo::halfExtents), or nullptr: glossy reflections get the light by its highlight and see the object
        // only weighed against the light's shadow rays (multiple importance sampling)
        bool isLightEmitter() const;
        const LightInfo* getEmittedLight() const;
        void setLightEmitter(const LightInfo* light);

        void resetTextureMatrix();

        void rotateTexture(Float angle);

        void translateTexture(const Float2& translation);

        void scaleTexture(const Float2& scale);

        SurfaceShading sample(const SceneShader& sceneShader, const SceneIntersection& intersection) const override;

        SurfaceShading shade(const SceneIntersection& intersection, const SpectralContext& spectralContext) const;

        // The transmittance (RGB absorption lengths, zero for opaque surfaces), the refraction indices and the
        // reflectance (the specular lobe's weight) at the intersection alone, without the other channels of shade():
        // what a shadow ray needs to pass the object, with the diffusion below
        Float4 sampleTransmittance(const FacetIntersection& intersection) const;
        Float4 sampleRefractionEta(const FacetIntersection& intersection) const;
        Float4 sampleReflectance(const FacetIntersection& intersection) const;

        // Whether the intersection emits light (an emittance above 0): a light's emitter is the light only there
        bool emitsAt(const FacetIntersection& intersection) const;

        // The diffusion's RGB at the intersection, before shade() takes it to a context's wavelengths: the denoiser's
        // albedo guide, the same for every hero wavelength
        Float4 sampleDiffusion(const FacetIntersection& intersection) const;

    protected:
        MFloat4x4 textureMatrix;

        TextureTransformInfo textureTransformInfo;

        std::unique_ptr<const MaterialShader> diffusionShader;
        std::unique_ptr<const MaterialShader> reflectanceShader;
        std::unique_ptr<const MaterialShader> specularShader;
        std::unique_ptr<const MaterialShader> roughnessShader;
        std::unique_ptr<const MaterialShader> transmittanceShader;
        std::unique_ptr<const MaterialShader> refractionEtaShader;
        std::unique_ptr<const MaterialShader> emittanceShader;

        const LightInfo* emittedLight{};

        void updateTextureMatrix();
    };
}
