#include <memory>
#include <vector>

#include "LinkedCellBoundary.h"
#include "force/Force.h"

class BoundaryConditionReflection : public LinkedCellBoundary {
 private:
  std::shared_ptr<Force> forceMethod;
  std::array<double, 3> surface_normal;
  std::array<double, 3> point_on_plane;

 public:
  ~BoundaryConditionReflection() = default;

  BoundaryConditionReflection(std::vector<std::vector<Particle>>& haloCellList,
                              std::vector<std::vector<Particle>>& boundaryCellList,
                              std::shared_ptr<Force> forcemode,
                              std::array<double, 3>& surface_normal_,
                              std::array<double, 3>& point_on_plane_);

  std::shared_ptr<std::vector<Particle>> executeBoundaryCondition() override;
};
