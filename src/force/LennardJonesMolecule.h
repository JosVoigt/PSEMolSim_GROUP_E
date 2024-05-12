#include "Force.h"

/** \brief
 *  The Lennard-Jones potential is a pairwise potential that describes the
 *  interaction between two particles. It is used to model the interaction
 *  between atoms or molecules.
 *  The Lennard-Jones potential is a pairwise potential, so the force is only
 *  calculated between two particles.
 */
class LennardJonesMolecule : public Force {
   private:
    /** \brief
     *  The epsilon parameter of the Lennard-Jones potential
     */
    double epsilon;

    /** \brief
     *  The sigma parameter of the Lennard-Jones potential
     */
    double sigma;

    /** \brief
     *  The epsilon parameter times 24
     */
    double epsilon_24;

   public:
    /** \brief
     *  Constructor for the LennardJonesMolecule class
     *
     *  \param epsilon
     *  The epsilon parameter of the Lennard-Jones potential
     *
     *  \param sigma
     *  The sigma parameter of the Lennard-Jones potential
     */
    LennardJonesMolecule(double epsilon, double sigma);

    /** \brief
     *  Destructor for the LennardJonesMolecule class
     */
    ~LennardJonesMolecule() override = default;

    /** \brief
     *  Calculates the force between two particles according to the Lennard-Jones
     *  potential
     *
     *  \param p1
     *  The first particle
     *
     *  \param p2
     *  The second particle
     *
     *  \return
     *  The force between the two particles
     */
    std::array<double, 3> calculateForce(Particle& p1,
                                         Particle& p2) const override;

    /** \brief
     *  Returns the type of the force
     *
     *  \return
     *  The type of the force
     */
    std::string typeString() override;
};
