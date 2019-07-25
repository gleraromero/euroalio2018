//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#ifndef EUROALIO2018_TTBF_H
#define EUROALIO2018_TTBF_H

#include <vector>

#include <goc/goc.h>

#include "model.h"
#include "vrp_instance.h"

namespace euroalio2018
{
// Model TTBF from the article.
class TTBF : public Model
{
public:
	// Initializes a formulation f with the specified parameters.
	TTBF(const VRPInstance& vrp, const std::vector<double>& profits);
	
	~TTBF();
	
	// Serializes the route into a valuation for the formulation.
	virtual goc::Valuation SerializeSolution(const goc::Route& route) const;
	
	// Parses a solution from a solver.
	// Returns: the solution route.
	virtual goc::Route ParseSolution(const goc::Valuation& z) const;
};
} // namespace euroalio2018
#endif //EUROALIO2018_TTBF_H
