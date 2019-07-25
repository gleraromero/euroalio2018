//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#include "goc/graph/path_finding.h"

#include <limits.h>

#include "goc/collection/collection_utils.h"

using namespace std;

namespace goc
{
GraphPath longest_path(const Digraph& D, Vertex s, Vertex t)
{
	// Calculate topological order.
	vector<Vertex> topo = range(0, D.VertexCount());
	sort(topo.begin(), topo.end(), [&] (Vertex v1, Vertex v2) { return D.IncludesArc({v2, v1}); });
	
	// Calculate max distance and parents.
	vector<int> max_dist(D.VertexCount(), -INT_MAX);
	vector<Vertex> parent(D.VertexCount(), -1);
	parent[t] = -1;
	max_dist[t] = 0;
	for (auto& v: topo)
	{
		for (Vertex w: D.Successors(v))
		{
			if (max_dist[w]+1 > max_dist[v])
			{
				max_dist[v] = max_dist[w]+1;
				parent[v] = w;
			}
		}
	}
	
	// Reconstruct the longest path.
	GraphPath L;
	for (Vertex v = s; v != -1; v = parent[v])
		L.push_back(v);
	
	return L;
}
} // namespace goc