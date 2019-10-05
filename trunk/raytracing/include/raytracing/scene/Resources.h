#pragma once

#include "../common/HDRImage.h"
#include "../shading/shaders/ConstShader.h"
#include "../shading/shaders/MultiplyByConstShader.h"
#include "../shading/ObjectShader.h"
#include "../geometry/forms/meshes/Mesh.h"

#include <map>

namespace raytracer
{
    using namespace vectorization;

    class Resources
    {
    public:

        typedef ConstShader<ObjectShader, FacetIntersection, Float4> ConstMaterialShader;

        typedef MultiplyByConstShader <
            ObjectShader,
            FacetIntersection,
            Float4,
            ObjectShader::MaterialShader
        > MultiplyByConstMaterialShader;

        ALIGNED_ALLOCATORS(__alignof(Resources));

        Resources();

        virtual ~Resources();

        std::map<std::string, Mesh *> meshes;

        std::map<std::string, HDRImage *> textures;

        Mesh * const getMesh(const std::string & identifier);

        HDRImage * const getPPM(const std::string & identifier);

    protected:

        std::vector<std::string, AlignedAllocator<std::string>> revertedNormalsCheckList;

        const bool hasRevertedNormals(const std::string & identifier) const;
    };
}
