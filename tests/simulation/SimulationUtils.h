#pragma once

#include "data/FileLoader.h"
#include "physics/pairwiseforces/GravitationalForce.h"
#include "physics/pairwiseforces/LennardJonesForce.h"
#include "simulation/SimulationParams.h"
#include "simulation/interceptors/frame_writer/FrameWriterInterceptor.h"

const std::shared_ptr<FrameWriterInterceptor> video_writer = std::make_shared<FrameWriterInterceptor>(OutputFormat::VTU, 24, 10);

const std::shared_ptr<GravitationalForce> gravitational_force = std::make_shared<GravitationalForce>();

const std::shared_ptr<LennardJonesForce> lennard_jones_force = std::make_shared<LennardJonesForce>();

const SimulationParams TEST_DEFAULT_PARAMS_GRAVITY(FileLoader::get_input_file_path("empty.xml"), 0.002, 5,
                                                   SimulationParams::DirectSumType{}, {video_writer}, {}, {gravitational_force}, false,
                                                   true);

const SimulationParams TEST_DEFAULT_PARAMS_LENNARD_JONES(FileLoader::get_input_file_path("empty.xml"), 0.002, 5,
                                                         SimulationParams::DirectSumType{}, {video_writer}, {}, {lennard_jones_force},
                                                         false, true);