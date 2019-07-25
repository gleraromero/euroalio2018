//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#ifndef GOC_GRAPH_PATH_FINDING_H
#define GOC_GRAPH_PATH_FINDING_H

#include "goc/graph/digraph.h"
#include "goc/graph/graph_path.h"
#include "goc/graph/vertex.h"

namespace goc
{
// Precondition: D is a DAG. s, t \in Vertices(D).
// Returns: Longest path between s and t.
GraphPath longest_path(const Digraph& D, Vertex s, Vertex t);
} // namespace goc

#endif //GOC_GRAPH_PATH_FINDING_H
