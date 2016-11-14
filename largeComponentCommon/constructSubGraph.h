#ifndef LARGE_COMPONENT_CONSTRUCT_SUBGRAPH_HEADER_GUARD
#define LARGE_COMPONENT_CONSTRUCT_SUBGRAPH_HEADER_GUARD
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include "context.h"
namespace largeComponent
{
	//The vertex name is the vertex number in the original graph that corresponds to the vertex in the reduced graph. 
	typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::property<boost::vertex_name_t, int>, boost::property<boost::edge_index_t, int> > subGraphType;
	void constructSubGraph(subGraphType& graph, const context& contextObj, const vertexState* stateRef);
}
#endif
