#pragma once

#include "io/xml_schemas/checkpoint/checkpoint_schema.h"
#include "particles/Particle.h"

class InternalToXSDTypeAdapter {
   public:
    static ParticleType convertToParticle(const Particle& particle);

    static DoubleVec3Type convertToVector(const std::array<double, 3>& vector);
};