#include "primitives/Ray.h"

using namespace vectorization;

namespace primitives
{

    Ray::Ray()
        :
        origin(),
        direction(),
        reciprocalDirection()
    { }

    Ray::Ray(const Float4 & originIn)
        :
        origin(originIn),
        direction(),
        reciprocalDirection()
    { }

    Ray::Ray(
        const Float4 & originIn,
        const Float4 & directionIn)
        :
        origin(originIn),
        direction(directionIn),
        reciprocalDirection(zeroW(reciprocal(directionIn)))
    { }

    void Ray::setDirection(const Float4 & directionIn)
    {
        direction = directionIn;
        reciprocalDirection = zeroW(reciprocal(directionIn));
    }

    const Ray replaceDirection(const Ray & r, const Float4 & value)
    {
        return Ray(r.origin, value);
    }

    const Float4 point(const Ray & r, const Float4 & v)
    {
        return r.origin + r.direction * v;
    }

    const Float4 point(const Ray & r, const Float t)
    {
        return point(r, Float4(t));
    }

}
