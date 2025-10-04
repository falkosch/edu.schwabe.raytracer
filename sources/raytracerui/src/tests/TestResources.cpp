#include "tests/TestResources.h"
#include "../stdafx.h"

namespace raytracerui {
  void TestResources::setup(Resources & /*resources*/) {
    // resources.textures["simplexNoise"] = new NoiseGeneratorMap<4>(*new SimplexNoiseGenerator(Tools::seed()));
    // resources.textures["perlinNoise"] = new NoiseGeneratorMap<8>(*new PerlinNoiseGenerator(Tools::seed()));
  }
}