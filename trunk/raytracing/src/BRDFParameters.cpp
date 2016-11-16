#include "stdafx.h"

#include "BRDFParameters.h"

namespace raytracer
{

    BRDFParameters::BRDFParameters()
        :
        intersection(),
        surface(),
        lighting(),
        transmissionDirection(),
        reflectanceCoefficient(One<Float4>()),
        absorbtionCoefficient(),
        viewDistance()
    { }

}
