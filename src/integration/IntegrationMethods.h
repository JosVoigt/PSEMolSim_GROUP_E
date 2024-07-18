#pragma once

#include <map>
#include <memory>

#include "integration/IntegrationFunctor.h"

/**
 * @brief Enum class to specify the integration method
 */
enum class IntegrationMethod { VERLET };

/**
 * @brief Returns the corresponding integration functor for the given integration method
 *
 * @param method Integration method to get the functor for
 * @return std::unique_ptr<IntegrationFunctor> Integration functor for the given integration method
 */
const std::unique_ptr<IntegrationFunctor> get_integration_functor(IntegrationMethod method);