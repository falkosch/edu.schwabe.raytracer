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
}
