//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#include "vrp_instance.h"

using namespace std;
using namespace goc;
using namespace nlohmann;

namespace euroalio2018
{
TimeUnit VRPInstance::ReadyTime(const GraphPath& p, TimeUnit t0) const
{
	CapacityUnit qq = q[0];
	TimeUnit t = t0 + s[p[0]];
	for (int k = 0; k < (int)p.size()-1; ++k)
	{
		Vertex i = p[k], j = p[k+1];
		if (!tau[i][j].Domain().Includes(t)) return INFTY;
		t += tau[i][j](t) + s[j];
		qq += q[j];
	}
	if (epsilon_bigger(qq, Q)) return INFTY;
	return t;
}

bool VRPInstance::IsFeasible(const GraphPath& p, TimeUnit t0) const
{
	return ReadyTime(p, t0) != INFTY;
}

void VRPInstance::Print(ostream& os) const
{
	os << json(*this);
}

void to_json(json& j, const VRPInstance& instance)
{
	j["digraph"] = instance.D;
	j["start_depot"] = instance.o;
	j["end_depot"] = instance.d;
	j["horizon"] = vector<TimeUnit>({0, instance.T});
	j["capacity"] = instance.Q;
	j["demands"] = instance.q;
	j["service_times"] = instance.s;
	j["travel_times"] = instance.tau;
}

void from_json(const json& j, VRPInstance& instance)
{
	int n = j["digraph"]["vertex_count"];
	instance.D = j["digraph"];
	instance.o = j["start_depot"];
	instance.d = j["end_depot"];
	instance.T = j["horizon"][1];
	for (int i = 0; i < n; ++i) instance.s.push_back(j["service_times"][i]);
	instance.Q = j["capacity"];
	for (int i = 0; i < n; ++i) instance.q.push_back(j["demands"][i]);
	instance.tau = j["travel_times"];
}
} // namespace ejor2019