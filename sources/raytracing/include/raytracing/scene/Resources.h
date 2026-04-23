#pragma once

#include "../common/HDRImage.h"
#include "../geometry/forms/meshes/Mesh.h"
#include "../shading/ObjectShader.h"
#include "../shading/shaders/ConstShader.h"
#include "../shading/shaders/MultiplyByConstShader.h"

#include <map>
#include <memory>

namespace raytracer {
  using namespace vectorization;

  class Resources {
  public:
    typedef ConstShader<ObjectShader, FacetIntersection, Float4> ConstMaterialShader;

    typedef MultiplyByConstShader<ObjectShader, FacetIntersection, Float4, ObjectShader::MaterialShader>
        MultiplyByConstMaterialShader;

    Resources();

    virtual ~Resources();

    std::map<std::string, std::unique_ptr<Mesh>> meshes;

    std::map<std::string, std::unique_ptr<HDRImage>> textures;

    Mesh *getMesh(const std::string &identifier);

    HDRImage *getPPM(const std::string &identifier);

  protected:
    std::vector<std::string, AlignedAllocator<std::string>> revertedNormalsCheckList;

    bool hasRevertedNormals(const std::string& identifier) const;
  };
}
