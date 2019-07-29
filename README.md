# Integer programming formulations for the time-dependent elementary shortest path problem with resource constraints
Source code to replicate the experiments from the article https://doi.org/10.1016/j.endm.2018.07.008

## Abstract
The impact of congestion in transportation has become one of the main concerns regarding urban planing in large cities. Time-Dependent Vehicle Routing Problems (TDVRPs) is the name given to a broad family of VRPs that explicitly incorporate the congestion by considering variable travel times. In this paper we study the Time-Dependent Elementary Shortest Path Problem with Resource Constraints (TDESPPRC), that appears as the pricing sub-problem in column generation-based approaches for TDVRPs. We consider two integer programming formulations which exploit the characteristics of the time-dependent travel time function and are evaluated on benchmark instances. On preliminary computational experiments, the approach is able to effectively solve instances with up to 25 vertices in reasonable times, showing its potential to be used within a Branch and Price algorithm.

## Getting started
The following instructions will guide you through the steps to execute the experiments from the article.

### Prerequisites
- Python >= 3.6 [(more info)](https://www.python.org/)
- CPLEX >= 12.8 [(more info)](https://www.ibm.com/products/ilog-cplex-optimization-studio)
- Boost Graph Library >=1.66 [(more info)](https://www.boost.org/doc/libs/1_66_0/libs/graph/doc/index.html)
    - On Linux: ```sudo apt-get install libboost-all-dev```
- CMake >= 2.8.4 [(more info)](https://cmake.org/)
    - On Linux: ```sudo apt-get install cmake```
- C++14 or higher [(more info)](https://es.wikipedia.org/wiki/C%2B%2B14)

### Built with
- Kaleidoscope: A tool to visualize the outputs of Optimization Problems [(more info)](https://github.com/gleraromero/kaleidoscope)
- Runner: A script to ease the process of running experiments [(more info)](https://github.com/gleraromero/runner)
- GOC lib: A library that includes interfaces for using (Mixed Integer) Linear Programming solvers, and some useful resources [(more info)](https://github.com/gleraromero/goc).

### Running the experiments.
1. Add environment variables with the paths to the libraries.
    1. Add two environment variables to bash with CPLEX include and library paths.
        1. ```export CPLEX_INCLUDE=<path_to_cplex_include_dir>```
            - Usually on Linux: _/opt/ibm/ILOG/CPLEX_Studio\<VERSION\>/cplex/include_
        1. ```export CPLEX_BIN=<path_to_cplex_lib_binary_file>```
            - Usually on Linux: _/opt/ibm/ILOG/CPLEX_Studio\<VERSION\>/cplex/lib/x86-64_linux/static_pic/libcplex.a_
    1. Add two environment variables to bash with BOOST Graph Library include and library paths.
        1. ```export BOOST_INCLUDE=<path_to_boost_include_dir>```
            - Usually on Linux: _/usr/include_
        1. ```export BOOST_BIN=<path_to_boost_lib_binary_file>```
            - Usually on Linux: _/usr/lib/x86_64-linux-gnu/libboost_graph.a_
1. Go to the euroalio2018 root directory.
1. Execute ```python3 runner/runner.py experiments/comparison.json```
1. The execution output will be continually saved to the output folder.

### Visualizing the experiment results.
1. Go to https://gleraromero.github.io/kaleidoscope/euroalio2018
1. Add the output file.
1. Select the experiments (TTBF or CTTBF).
1. Add some attributes to visualize.
1. Click on Refresh.
1. If more details on an experiment are desired click on the + icon in a specific row.

## Built With
* [JSON for Modern C++](https://github.com/nlohmann/json)
* [Boost Graph Library](https://www.boost.org/doc/libs/1_66_0/libs/graph/doc/index.html)

## Authors
- Gonzalo Lera-Romero
- Juan José Miranda-Bront

## License
This project is licensed under the MIT License - see the LICENSE.md file for details
