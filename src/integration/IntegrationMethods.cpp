#include "IntegrationMethods.h"

#include "integration/methods/VerletFunctor.h"
#include "io/logger/Logger.h"

const std::unique_ptr<IntegrationFunctor> get_integration_functor(IntegrationMethod method) {
    switch (method) {
        case IntegrationMethod::VERLET:
            return std::make_unique<VerletFunctor>();
    }

    Logger::logger->error("Unknown integration method");
    throw std::runtime_error("Unknown integration method");
}