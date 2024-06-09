#include <memory>
#include <vector>

#include "LinkedCellBoundary.h"
#include "force/Force.h"

class BoundaryConditionReflection : public LinkedCellBoundary {
 private:
  std::shared_ptr<PairwiseForce> forceMethod;
  std::array<double, 3> surface_normal;
  std::array<double, 3> point_on_plane;

 public:
  ~BoundaryConditionReflection() = default;

  /**
   * \brief
   * Constructor for the Condition, takes all constant values required
   * for the condition.
   * \param haloCellList
   * The halo Cells that are managed by this Condition, for this condition may be empty
   * \param boundaryCellList
   * The boundary cells managed by this condition, particles in here will be affected by the repulsing force
   * \param forcemode
   * The force mode the simulation is run in
   * \param surface_normal_
   * A normalized! normal on the surface from which particles get reflected.
   * This must be the outfacing normal (towards the outer halo ring)
   *
   * The surface is expected to be parallel to one of the XY,XZ or YZ planes.
   * \param point_on_plane_
   * Any point on the plane
   */
  BoundaryConditionReflection(std::vector<std::vector<Particle>>& haloCellList,
                              std::vector<std::vector<Particle>>& boundaryCellList,
                              std::shared_ptr<PairwiseForce> forcemode,
                              std::array<double, 3>& surface_normal_,
                              std::array<double, 3>& point_on_plane_);

  /**
   * \brief
   * Executes the boundary conditions, inflicts a repulsing force on the particles.
   * The force is calculated according to the Stoermer-Verlet provided in the constructer and applied if it repels
   * the particle from the defined surface.
   */
  std::shared_ptr<std::vector<Particle>> executeBoundaryCondition() override;
};
