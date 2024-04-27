MolSim - Group E
===

The Molecular Dynamics teaching code from the chair of scientific computing at Technical University Munich.

Required libraries
---
- boost
- xerces-c

Building with cmake
---
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
The resulting executable is located at **{version}/MolSim**

Options
---

\-\-help:     displays the option help

\-\-delta:    difference between two time steps (default 1e-5) <br>
\-\-end:      end point (default: 1) <br>
\-\-file:     path to the file containing the starting values of the molecues
