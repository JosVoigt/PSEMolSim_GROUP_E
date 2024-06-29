MolSim - Group E
===

The Molecular Dynamics teaching code from the chair of scientific computing at Technical University Munich.
This is the implementation of the set tasks of Group E in the summer semester 2024.

The code base can be found here: https://github.com/TUM-I5/MolSim

The goal is to create a simple simulator for molecular dynamics.
This is done by using Störmer-Verlet to solve the differential equation for the system.

Currently there are two supported modes for particle simulation.
First, Planet simulation for a basic understanding of Störmer-Verlet.
Second, Molecules using the Lennard-Jones-Potential for the actual molecule simulation.

For a documentation please refer to the Doxygen build information and locations further down.

Quick build
---
The suggested build is:
```bash
    mkdir build && cd build && cmake .. -DDOWNLOAD_DEPENDENCIES=OFF -DCMAKE_BUILD_TYPE=release && cmake --build .
```
You will require all dependencies listed below, just install them with the provided apt command.

Required libraries
---
- boost
- xerces-c
- googletest
- spdlog (spdlog requires fmt, but that will be installed by apt or by cmake)

To install all of them using apt, run this command (requires *SUDO*):
```bash
    apt install libboost-dev libboost-program-options-dev libxerces-c-dev libgtest-dev libspdlog-dev
```
To then prevent the dowload via the CMake add to the cmake prompt:
```bash
    -DDOWNLOAD_DEPENDENCIES=off
```
It is strongly recommended to download them prior as a complete fetch may be upwards off 2 minutes (longest were 20 minutes on WSL2 Ubuntu).

Building with cmake
---
Create a build folder and start the build from there:
Every command listed here should be executed in this folder if not stated otherwise.

```bash
    mkdir {folder-name}
    cd {folder-name}
```

To build release:
```bash
    cmake -DCMAKE_BUILD_TYPE=release .. 
```
For a non-optimized version with debug options:
```bash
    cmake -DCMAKE_BUILD_TYPE=debug .. 
```
For a release version with debug options:
```bash
    cmake -DCMAKE_BUILD_TYPE=relwithdebinfo .. 
```
For a minimal size release (not implemented):
```bash
    cmake -DCMAKE_BUILD_TYPE=minsizerel .. 
```

If the -DCMAKE_BUILD_TYPE flag is not set it will default to Release.

This will generate a Makefile in {folder-name}.
To compile and link with the makefile use either make or --build:
```bash
    make
```
```bash
    cmake --build ..
```

Due to the size making use of Make's -j flag is advised.
This may be memory and computational power consuming and should be used at your own risk.

The resulting executable is located at the folder build from.
It is named "MolSim".
Additionally the tests will be built along side it, they are named "MolSimTest".

To disable the Doxygen build add the flag BUILD_DOC flag:
```bash
    cmake -DCMAKE_BUILD_TYPE={type} -DBUILD_DOC=OFF .
```
To create the documentation run the custom make target.
```bash
	make doc_doxygen
```
The doxygen files will be created in the folder PSEMolSim_GROUP_E/doxys_documentation.


To disable file output via the C preprocessor disable the output flag (or compile with -D NO_OUT_FILE):
```bash
    cmake -DCMAKE_BUILD_TYPE={type} -DOUTPUT=off ..
```
To disable non critical logging disable the logging flag:
```bash
    cmake -DLOGGING=OFF ..
```

Options
---
These are the availabe command for the generated executable.

|Long name      |Short name |Values         			    | Defaults  	| Description												                                                                                                                |
|---------------|-----------|-------------------------------|---------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|--help         | -h        |               			    |           	|Prints out the help message										                                                                                                        |
|--xml          | -X        |filepath                       |               |Extracts all required information from the provided XML file
|--delta	    | -d	    |double				            | 1e-5  		|Sets the stepsize for the simulation									                                                                                                    |
|--frequency    | -f        |int            			    | 10        	|Sets the output frequency, every nth step a file will be generated					                                                                                        |
|--start        | -s        |double         			    | 0         	|Sets the first point at which output is generated							                                                                                                |
|--end          | -e        |double         			    | 1         	|Sets the endpoint for the simulation. After reaching will terminate					                                                                                    |
|--file         | -F        |filepath(s)    			    |           	|Sets the input file(s) that describe the initial state of the system					                                                                                    |
|--outformat    | -O        |vtk,xyz       			        | vtk       	|Set the output method											                                                                                                            |
|--outfile      | -o        |string         			    | simulation	|Sets the prefix for the output files									                                                                                                    |
|--cuboid       | -c        |string                         |               |Accepts multiple cuboids, in the form [velocity,corner,distance,mass,x,y,z,meanBrownianMotion] sperated by comma. Velocity and corner are 3D-vectors of the form [a,b,c]   |
|--planet       |           |               			    |           	|Sets the particle type to planets and uses planet force calculation					                                                                                    |
|--lenjonesmol  |           |epsilon (double) sigma(double)	|		        |Set the particle mode to molcule while using Lennard-Jones with the provided epsilon and sigma values	                                                                    |
  

An example to calculate the path of Halley's comet using the provided data in input/:
This would be run from the toplevel folder.
If one would like to change the execution directory the only thing required to be adapted would be the relative path to the input data.
```bash
	./MolSim --planet --start 10 --end 1000 --delta 1 --frequency 10 --outformat vtk --outfile halley --file input/eingabe-sonne.txt
```

This is the same command, but it makes use of the shorter flags and default values.
The arguments are provided in the same order as above.
```bash
	./MolSim --planet -s 10 -e 1000 -d 1 -o halley -F input/eingabe-sonne.txt
```

The logs are written to the executing directory into the file logs/MolSim_[currentTime].log.

Setting loglevel
----------------

The loglevel can be set easily by setting the SPDLOG enviroment variable.
For detailed information refer here: https://github.com/gabime/spdlog?tab=readme-ov-file#load-log-levels-from-the-env-variable-or-argv.

To disable all non-critical logging at compile refer to the -DLOGGING section of cmake.

The available loggers currently available are:
- "file": a general file logger
- "console": a logger used mainly for outputting errors and critical information to the console

The settings provided are:
- off - no logging
- critical - only the most essential logging, usally program crashes
- error - non-fatal errors to inform the user
- info - general informations on the program state
- debug - detailed information on the program state, intended for debugging
- trace - extreme information on the program state, intended for debugging extreme bugs "debug" cannot detect. WARNING: This will generate large files on your machine depending on settings!


A possible example could look like this:
```bash
    export SPDLOG_LOGLEVEL=info,console=critical,file=debug
```
This way the console displays fatal error messages to not clutter the console (e.g. during gdb) while maintaining a reasonable info state in the file logger.
