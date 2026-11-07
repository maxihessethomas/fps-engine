#pragma once

#include "Math/Math.h"

namespace EngineLN 
{

    struct Light {
        Vec3 position;
        Vec3 ambient;
        Vec3 diffuse;
        Vec3 specular;
    };

}