MolSim - Group E
===

The Molecular Dynamics teaching code from the chair of scientific computing at Technical University Munich.

Required libraries
---
- boost
- xerces-c

Building with cmake
---
The cmake file is located in the build folder.
Execute all commands below there.

To build:
```bash
    cmake -DCMAKE_BUILD_TYPE=release . 
    cmake --build
```
For a non-optimized version with debug options:
```bash
    cmake -DCMAKE_BUILD_TYPE=debug . 
    cmake --build
```
For a release version with debug options:
```bash
    cmake -DCMAKE_BUILD_TYPE=relwithdebinfo . 
    cmake --build
```
For a minimal size release (not implemented):
```bash
    cmake -DCMAKE_BUILD_TYPE=minsizerel . 
    cmake --build
```
The resulting executable is located at **PSEMolSim_GROUP_E/{version}/MolSim**

To disable the Doxygen build:
```bash
    cmake -DCMAKE_BUILD_TYPE={type} -DBUILD_DOC=OFF .
    cmake --build
```

Options
---

\-\-help:     displays the option help

\-\-delta:    difference between two time steps (default 1e-5) <br>
\-\-end:      end point (default: 1) <br>
\-\-file:     path to the file containing the starting values of the molecues
