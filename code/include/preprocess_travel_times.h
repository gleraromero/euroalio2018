//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#ifndef EUROALIO2018_PREPROCESS_TRAVEL_TIMES_H
#define EUROALIO2018_PREPROCESS_TRAVEL_TIMES_H

#include <goc/goc.h>

namespace euroalio2018
{
// Takes a JSON instance of the vehicle routing problems with the following attributes:
//	- digraph
//	- distances
//	- clusters
//	- cluster_speeds
//	- speed_zones
// Adds the travel_times attribute to a JSON instance with a matrix of piecewise linear functions.
void preprocess_travel_times(nlohmann::json& instance);
} // namespace euroalio2018

#endif //EUROALIO2018_PREPROCESS_TRAVEL_TIMES_H
