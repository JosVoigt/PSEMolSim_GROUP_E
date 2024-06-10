#pragma once

#include <array>

#include "Force.h"

class PlanetForce : public Force {
   public:
    /**
     *  \brief
     *  Default constructor
     */
    PlanetForce() = default;

    /**
     *  \brief
     *  Calculates the relative force between two particles as if they were planets
     *
     *  |F| = (m1*m2)/(d^2) \n
     *  d   :     Distance between the two particles \n
     *  m1  :     Mass of the first particles \n
     *  m2  :     Mass of the second particle
     *
     *  The gravitational constant G is not included for the sake of simplicity
     *
     *  \param forcePartner
     *  The other planet in the equation
     *
     *  \return
     *  The force vector with |F| as described above and pointing from this
     *  planet to the other one
     */
    std::array<double, 3> calculateForce(Particle &p1,
                                         Particle &p2) const override;

    /** \brief
     *  Returns the type of the force
     *
     *  \return
     *  The type of the force as a string
     */
    std::string typeString() override;
};
