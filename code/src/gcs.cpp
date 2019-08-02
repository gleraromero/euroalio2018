//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#include "gcs.h"

using namespace std;
using namespace goc;

namespace euroalio2018
{
GCS::GCS(const VRPInstance& vrp, const Matrix<Variable>& x, const vector<Variable>& y) : vrp(vrp), x(x), y(y)
{ }

vector<Constraint> GCS::Separate(const Valuation& z, int node_number, int count_limit,
											  double node_bound) const
{
	auto& D = vrp.D;
	
	// Add to N the support digraph.
	Digraph N(D.VertexCount());
	for (int i: D.Vertices())
		for (int j: D.Successors(i))
			if (epsilon_bigger(z[x[i][j]], 0.0))
				N.AddArc({i,j});

	// Solve max-flow with k = {1,2,...,n-1}.
	vector<Constraint> violated;
	for (Vertex k: D.Vertices())
	{
		if (k == vrp.o || k == vrp.d) continue;
		if (epsilon_equal(z[y[k]], 0.0)) continue;
		double F;
		STCut ST;
		tie(F, ST) = maxflow_mincut(N, [&](int i, int j) { return z[x[i][j]]; }, k, vrp.d);

		// Flow from k to d should be y_k. Otherwise, there is a subtour.
		// Break it using \sum_{i \in S} \sum_{j \in T} x[i][j] >= y_k.
		if (epsilon_smaller(F, z[y[k]] - 0.1))
			violated.push_back(ESUM(i:ST.S, ESUM(j:ST.T, D.IncludesArc({i,j}) ? x[i][j] : Expression())).GEQ(y[k]));
	}
	return violated;
}
} // namespace euroalio2018