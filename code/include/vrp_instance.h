//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#ifndef EUROALIO2018_VRP_INSTANCE_H
#define EUROALIO2018_VRP_INSTANCE_H

#include <vector>
#include <goc/goc.h>

namespace euroalio2018
{
typedef double TimeUnit; // Represents time.
typedef double CapacityUnit; // Represents the capacity.
typedef double ProfitUnit; // Represents the profit of vertices.

// This class represents an instance of a vehicle routing problem.
// Considerations:
// 	- It considers two depots (origin and destination).
//	- The number of vertices n includes both depots.
class VRPInstance : public goc::Printable
{
public:
	int n; // number of vertices.
	goc::Digraph D; // digraph representing the network.
	goc::Vertex o, d; // origin and destination depot.
	TimeUnit T; // end of planning horizon ([0,T]).
	CapacityUnit Q; // vehicle capacity.
	std::vector<CapacityUnit> q; // demand of customers (q[i] = demand of customer i).
	std::vector<TimeUnit> s; // service time of customers (s[i] = service time of customer i).
	goc::Matrix<goc::PWLFunction> tau; // tau[i][j](t) = travel time function of arc (i, j) if departing from i at t.
	
	// Returns: the time we finish visiting the last vertex if departing at t0.
	// If infeasible, returns INFTY.
	TimeUnit ReadyTime(const goc::GraphPath& p, TimeUnit t0=0) const;
	
	// Returns: if traversing path p departing at t0 is feasible.
	bool IsFeasible(const goc::GraphPath& p, TimeUnit t0=0) const;
	
	// Prints the JSON representation of the instance.
	virtual void Print(std::ostream& os) const;
};

// Serializes the instance.
void to_json(nlohmann::json& j, const VRPInstance& instance);

// Parses an instance.
void from_json(const nlohmann::json& j, VRPInstance& instance);
} // namespace euroalio2018

#endif //EUROALIO2018_VRP_INSTANCE_H
