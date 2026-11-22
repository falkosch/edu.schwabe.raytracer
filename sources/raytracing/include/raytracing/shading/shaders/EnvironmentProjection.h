#pragma once

namespace raytracer
{
    // How an environment image covers the sphere of directions (environment_coordinates.h). The start camera looks
    // along -z with +y up; the projections with a center put the start camera's view direction there. The image's
    // top row is up (v = 1).
    enum class EnvironmentProjection
    {
        // An all-sky dome seen from below: u, v = 0.5 + 0.5 * (x, z) of the direction, so the image's radius is the sine
        // of the angle to the zenith. The lower hemisphere mirrors the upper one.
        FisheyeDome,

        // A photo of a mirror ball (OpenGL's sphere map): the image's center shows -z and its rim +z, behind the start
        // camera. The ball's radius grows with the sine of half the angle to -z, so the back is squeezed into the rim.
        MirrorBall,

        // Debevec's angular map (his light probes): like the mirror ball, but the radius grows linearly with the angle to
        // -z, so the back is as sharp as the front.
        AngularMap,

        // Equirectangular: u is the azimuth (0.5 towards -z, growing towards +x, the seam at +z), v the elevation (1 at
        // +y, 0 at -y).
        LatitudeLongitude,
    };
}
