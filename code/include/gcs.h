//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#ifndef EUROALIO2018_GCS_H
#define EUROALIO2018_GCS_H

#include <goc/goc.h>
#include "vrp_instance.h"

namespace euroalio2018
{
// The Generalized Cut Set inequalities are the following:
//	Given a set S \subset V - {o,d}, k \in S.
// 	\sum_{i \in S}\sum{j \not\in S} x_ij >= y_k.
//
// The separation routine fixes a k, and then solves a max-flow from k to d.
// If the flow is < y_k then it means there is a subtour passing through k.
// The S-T min-cut associated to the max-flow contains exactly the S we need to write the
// inequality.
class GCS : public goc::SeparationRoutine
{
public:
	GCS(const VRPInstance& vrp, const goc::Matrix<goc::Variable>& x, const std::vector<goc::Variable>& y);
	
	virtual std::vector<goc::Constraint> Separate(const goc::Valuation& z, int node_number, int count_limit,
		double node_bound) const;
private:
	VRPInstance vrp;
	goc::Matrix<goc::Variable> x;
	std::vector<goc::Variable> y;
};
} // namespace euroalio2018

#endif //EUROALIO2018_GCS_H
