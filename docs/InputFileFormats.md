<!-- markdownlint-disable-next-line -->
@~english
\page InputFileFormats Input File Formats

## Overview

Our program supports multiple file formats as input files, which are interpreted differently. The file format is automatically determined by the file extension.
The following file extensions are supported:

- `.xml`: Contains data for particle generation and allows to specify all the input parameters for the simulation as well

In the following sections, the file formats are explained in detail.

## File Formats Explained

### .xml

The `.xml` file format can be used to specify all the input parameters for the simulation at once. Therefore, it is preferred over the other file formats.

Its definition is based on the [simulation_schema.xsd](simulation_schema.xsd) file, which is used to validate the input file. The file contains a single root element `<configuration>` with the following child elements:

- Simulation Settings:
  - `<fps>`: The number of frames per second to be rendered
  - `<video_length>`: The length of the video in seconds
  - `<delta_t>`: The time step size
  - `<end_time>`: The time at which the simulation should end
  - `<particle_container>` Which particle container implementation should be used
  
- Particle Data:
  - `<cuboid_spawner>`: Input data for a cuboid spawner, which generates particles in a cuboid shape (2 or 3 dimensional)
  - `<sphere_spawner>`: Input data for a sphere spawner, which generates particles in a spherical shape (2 or 3 dimensional)
  - `<single_particle_spawner>`: Input data for a single particle, which is placed at the specified position

An example file could look like this:

```xml
<?xml version="1.0"?>
<configuration xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:noNamespaceSchemaLocation="simulation_schema.xsd">

    <settings>
        <fps>24</fps>
        <video_length>10</video_length>
        <delta_t>0.014</delta_t>
        <end_time>10.0</end_time>
        <particle_container>
            <directsum_container />
        </particle_container>
    </settings>

    <particles>
        <cuboid_spawner>
            <lower_left_front_corner>
                <x>0.0</x>
                <y>0.0</y>
                <z>0.0</z>
            </lower_left_front_corner>
            <grid_dim>
                <x>40</x>
                <y>8</y>
                <z>1</z>
            </grid_dim>
            <grid_spacing>1.1225</grid_spacing>
            <temperature>0.1</temperature>
            <mass>1</mass>
            <velocity>
                <x>0.0</x>
                <y>0.0</y>
                <z>0.0</z>
            </velocity>
            <type>0</type>
        </cuboid_spawner>
    </particles>

</configuration>
```
