#include "ForcePicker.h"

#include <memory>
#include <numeric>

#include "io/logger/Logger.h"
#include "physics/pairwiseforces/GravitationalForce.h"
#include "physics/pairwiseforces/LennardJonesForce.h"
#include "physics/simpleforces/GlobalDownwardsGravity.h"
#include "physics/simpleforces/UpwardsForce.h"
#include "physics/pairwiseforces/MembraneLenJonesForce.h"

const std::map<std::string, std::shared_ptr<SimpleForceSource>> get_supported_simple_forces() {
    std::map<std::string, std::shared_ptr<SimpleForceSource>> force_names;

    auto global_downwards_gravity = std::make_shared<GlobalDownwardsGravity>(0);

    force_names.insert({std::string(*global_downwards_gravity), global_downwards_gravity});

    return force_names;
}

const std::map<std::string, std::shared_ptr<PairwiseForceSource>> get_supported_pairwise_forces() {
    std::map<std::string, std::shared_ptr<PairwiseForceSource>> force_names;

    auto lennardjones = std::make_shared<LennardJonesForce>();
    auto gravitational = std::make_shared<GravitationalForce>();
	auto membrane = std::make_shared<MembraneLenJonesForce>(1,1);
    auto global_downwards_gravity = std::make_shared<GlobalDownwardsGravity>(0);

    force_names.insert({std::string(*lennardjones), lennardjones});
    force_names.insert({std::string(*gravitational), gravitational});
	force_names.insert({std::string(*membrane), membrane});

    return force_names;
}

const std::map<std::string, std::shared_ptr<UpwardsForce>> get_supported_upwards_forces() {
    std::map<std::string, std::shared_ptr<UpwardsForce>> force_names;

    auto upwards_force = std::make_shared<UpwardsForce>(std::array<double, 3>{0, 0, 0}, std::vector<int>{}, 0);

    force_names.insert({std::string(*upwards_force), upwards_force});

    return force_names;
}
