#pragma once

#include <raytracing.h>

#include <sstream>

namespace raytracerui
{
    using namespace vectorization;
    using namespace raytracer;

    template <int LightsPerAxis = 3, int ObjectsPerAxis = 10>
    struct ProceduralScene
    {
        static void setupLightScene(Scene & scene, Resources & /*resources*/) {
            scene.setBackgroundShader(new ConstShader<SceneShader, Float4, Float4>(Float4(1.f, 1.f, 1.f, 0.f)));
            scene.setAmbientLight(Float4(0.f, 0.f, 0.f, 0.f));

            const Float4 EmissivityScale = reciprocal(Float4(LightsPerAxis * LightsPerAxis * LightsPerAxis + 1.f));
            for (int z = -LightsPerAxis; z <= LightsPerAxis; ++z) {
                for (int y = -LightsPerAxis; y <= LightsPerAxis; ++y) {
                    for (int x = -LightsPerAxis; x <= LightsPerAxis; ++x) {
                        LightInfo * light = new LightInfo();
                        const Float4 p = convert<Float4>(Int4(x, y, z));
                        light->position = oneW(p * Float4(4.f));
                        light->emissivity = oneW(abs(p * EmissivityScale) + EmissivityScale);
                        light->attenuationFactors = Float4(1.f, 1.f, 0.f, 1.f);
                        scene.getLights().push_back(light);
                    }
                }
            }
        }

        static void setup(Scene & scene, Resources & resources) {
            setupLightScene(scene, resources);

            for (int z = -ObjectsPerAxis; z < ObjectsPerAxis; ++z) {
                for (int y = -ObjectsPerAxis; y < ObjectsPerAxis; ++y) {
                    for (int x = -ObjectsPerAxis; x < ObjectsPerAxis; ++x) {
                        std::stringstream fmt;
                        fmt << z << " " << y << " " << x;

                        SceneObject * so = new SceneObject(fmt.str());
                        if ((x + y + z) & One<int>())
                            so->setForm(new Box());
                        else
                            so->setForm(new Sphere());
                        so->scale(Float3(.25f));
                        so->rotate(Float3(Float(z), Float(x), Float(y)) * Float(30.f));
                        so->translate(Float3(Float(x), Float(y), Float(z)) + Float(.5f));
                        so->scaleTexture(Float2(1.f, 1.f));
                        so->rotateTexture(0.f);
                        so->translateTexture(Float2(0.f, 0.f));
                        so->setEmissivityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
                        so->setDiffusionShader(new HDRImageShader(*resources.getPPM("marble")));
                        so->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .2f)));
                        so->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .2f)));
                        so->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 32.f)));
                        so->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
                        so->setRefractionEtaShader(new Resources::ConstMaterialShader(RefractionIndices::refractionEta(
                            RefractionIndices::Vacuum, RefractionIndices::Water)));
                        scene.getSceneObjects().push_back(so);
                    }
                }
            }
        }
    };
}
