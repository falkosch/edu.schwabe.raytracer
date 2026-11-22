#pragma once

namespace raytracer
{
    // How HDRImage::sampleBilinear treats texture coordinates outside [0, 1) and the texels at the edges. The values
    // are bit sets of the clamped axes (1: u, 2: v), which sampleBilinear turns into lane masks.
    enum class TextureWrap
    {
        // Both axes wrap around (tiled textures): the last row and column blend with the first ones
        Repeat = 0,

        // u wraps around, v is clamped to the bottom and the top row: images that span a sphere once, whose rows end
        // at the poles (spherical texture coordinates, latitude-longitude environment maps)
        ClampV = 2,

        // Both axes are clamped to the edge texels: images that end at their borders, like the discs of fisheye
        // domes, mirror balls and angular maps
        Clamp = 3,
    };
}
