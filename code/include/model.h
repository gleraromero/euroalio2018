//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#ifndef EUROALIO2018_MODEL_H
#define EUROALIO2018_MODEL_H

#include <goc/goc.h>

#include "vrp_instance.h"

namespace euroalio2018
{
// Abstraction to represent the models CTTBF and TTBF.
// Both models must implement the Model interface.
class Model
{
public:
	VRPInstance vrp;
	std::vector<goc::Variable> y; // y_i: vertex i is selected.
	goc::Matrix<goc::Variable> x; // x_ij: arc ij is selected.
	goc::Matrix<std::vector<goc::Variable>> xx; // x_ijm: arc ij is traversed during travel time zone m.
	std::vector<goc::Variable> t; // t_i: departure time from i, if y_i = 1, 0 otherwise.
	goc::Matrix<std::vector<goc::Variable>> tt; // t_ijm: t_i if x_ijm = 1, 0 otherwise.
	goc::Formulation* f;
	
	// Serializes the route into a valuation for the formulation.
	virtual goc::Valuation SerializeSolution(const goc::Route& route) const = 0;
	
	// Parses a solution from a solver.
	// Returns: the solution route.
	virtual goc::Route ParseSolution(const goc::Valuation& z) const = 0;
};
} // namespace euroalio2018

#endif //EUROALIO2018_MODEL_H
