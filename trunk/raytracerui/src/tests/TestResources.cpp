#include "../stdafx.h"

#include "tests/TestResources.h"

#include <shading/shaders/NoiseGeneratorMap.h>
#include <common/noise/PerlinNoiseGenerator.h>
#include <common/Tools.h>

namespace raytracer
{

    void TestResources::setup(Resources & /*resources*/)
    {
        //resources.textures["simplexNoise"] = new NoiseGeneratorMap<4>(*new SimplexNoiseGenerator(Tools::seed()));
        //resources.textures["perlinNoise"] = new NoiseGeneratorMap<8>(*new PerlinNoiseGenerator(Tools::seed()));
    }

}
