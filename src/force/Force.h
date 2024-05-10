#pragma once

#include <container/Particle.h>

#include <array>

/* Interface for the force calculation
 */
class Force {
   public:
    virtual ~Force() = default;
    virtual std::array<double, 3> calculateForce(Particle &p1,
                                                 Particle &p2) const = 0;
    virtual std::string typeString() = 0;
};