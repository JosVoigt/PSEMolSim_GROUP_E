#include "VTUWriter.h"

#include <spdlog/fmt/bundled/core.h>

#include <fstream>

VTKFile_t VTUWriter::initializeOutput(int numParticles) {
    VTKFile_t vtu_file("UnstructuredGrid");

    // per point, we add type, position, velocity and force
    PointData point_data;
    DataArray_t mass(type::Float32, "mass", 1);
    DataArray_t velocity(type::Float32, "velocity", 3);
    DataArray_t forces(type::Float32, "force", 3);
    DataArray_t type(type::Int32, "type", 1);
    point_data.DataArray().push_back(mass);
    point_data.DataArray().push_back(velocity);
    point_data.DataArray().push_back(forces);
    point_data.DataArray().push_back(type);

    CellData cell_data;  // we don't have cell data => leave it empty

    // 3 coordinates
    Points points;
    DataArray_t point_coordinates(type::Float32, "points", 3);
    points.DataArray().push_back(point_coordinates);

    Cells cells;  // we don't have cells, => leave it empty
    // for some reasons, we have to add a dummy entry for paraview
    DataArray_t cells_data(type::Float32, "types", 0);
    cells.DataArray().push_back(cells_data);

    PieceUnstructuredGrid_t piece(point_data, cell_data, points, cells, numParticles, 0);
    UnstructuredGrid_t unstructured_grid(piece);
    vtu_file.UnstructuredGrid(unstructured_grid);

    return vtu_file;
}

void VTUWriter::plotParticle(VTKFile_t& vtuFile, const Particle& p) {
    PointData::DataArray_sequence& point_data_sequence = vtuFile.UnstructuredGrid()->Piece().PointData().DataArray();
    PointData::DataArray_iterator data_iterator = point_data_sequence.begin();

    data_iterator->push_back(p.getM());

    data_iterator++;
    data_iterator->push_back(p.getV()[0]);
    data_iterator->push_back(p.getV()[1]);
    data_iterator->push_back(p.getV()[2]);

    data_iterator++;
    data_iterator->push_back(p.getOldF()[0]);
    data_iterator->push_back(p.getOldF()[1]);
    data_iterator->push_back(p.getOldF()[2]);

    data_iterator++;
    data_iterator->push_back(p.getType());

    Points::DataArray_sequence& points_sequence = vtuFile.UnstructuredGrid()->Piece().Points().DataArray();
    Points::DataArray_iterator points_iterator = points_sequence.begin();
    points_iterator->push_back(p.getX()[0]);
    points_iterator->push_back(p.getX()[1]);
    points_iterator->push_back(p.getX()[2]);
}

const std::filesystem::path VTUWriter::writeFile(const SimulationParams& params, size_t iteration,
                                                 const std::vector<Particle>& particles) const {
    auto file_name = params.output_dir_path / fmt::format("MD_VTU_{:04d}.vtu", iteration);

    auto vtu_file = initializeOutput(static_cast<int>(particles.size()));

    for (const Particle& particle : particles) {
        plotParticle(vtu_file, particle);
    }

    std::ofstream file(file_name);
    VTKFile(file, vtu_file);
    file.close();

    return file_name;
}
