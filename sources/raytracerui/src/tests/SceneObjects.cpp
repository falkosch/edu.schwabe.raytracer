#include "tests/SceneObjects.h"
#include "../stdafx.h"

namespace raytracerui
{
    namespace
    {
        void place(SceneObject& object, const SceneMaterial& material, const Placement& placement)
        {
            object.scale(placement.scale);
            object.rotate(placement.rotation);
            object.translate(placement.translation);
            material.applyTo(object);
        }
    }

    std::unique_ptr<SceneObject> createSceneObject(
        std::string id, std::unique_ptr<const Form> form, const SceneMaterial& material, const Placement& placement
    )
    {
        auto object = std::make_unique<SceneObject>(std::move(id));
        object->setForm(std::move(form));
        place(*object, material, placement);
        return object;
    }

    std::unique_ptr<SceneObject> createSceneObject(
        std::string id, const Form* form, const SceneMaterial& material, const Placement& placement
    )
    {
        auto object = std::make_unique<SceneObject>(std::move(id));
        object->setForm(form);
        place(*object, material, placement);
        return object;
    }

    Float fitMeshScale(const Form& mesh, const Float size)
    {
        return size / max3(extents(mesh.getBounding()));
    }

    void addSphereLight(
        Scene& scene, std::string id, const Float4& center, const Float radius, const Float4& radiance
    )
    {
        auto emitter = createSceneObject(
            std::move(id), std::make_unique<Sphere>(), SceneMaterial::emissive(radiance),
            {Float3(radius, radius, radius), Float3(0.f, 0.f, 0.f), Float3(x(center), y(center), z(center))}
        );
        emitter->setLightEmitter(true);
        scene.getSceneObjects().push_back(std::move(emitter));

        auto light = std::make_unique<LightInfo>();
        light->position = oneW(center);
        light->emittance = RGBS(zeroW(radiance) * Float4(Pi<Float>() * radius * radius));
        light->attenuationFactors = Float4(0.f, 0.f, 1.f, 0.f);
        light->radius = radius;
        scene.getLights().push_back(std::move(light));
    }
}
