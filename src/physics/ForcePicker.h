#pragma once

#include <map>
#include <memory>
#include <string>

#include "physics/pairwiseforces/PairwiseForceSource.h"
#include "physics/simpleforces/SimpleForceSource.h"

const std::map<std::string, std::shared_ptr<SimpleForceSource>> get_supported_simple_forces();

const std::map<std::string, std::shared_ptr<PairwiseForceSource>> get_supported_pairwise_forces();
