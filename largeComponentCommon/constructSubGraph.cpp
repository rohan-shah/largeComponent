#include "constructSubGraph.h"
namespace largeComponent
{
	void constructSubGraph(subGraphType& graph, const context& contextObj, const vertexState* stateRef)
	{
		const context::inputGraph& originalGraph = contextObj.getGraph();
		std::size_t nVertices = boost::num_vertices(originalGraph);

		//graphVertices[i] is the vertex in the global graph that corresponds to vertex i in the returned graph
		//inverse is the inverse mapping, with 0's everywhere else. 
		int* inverse = (int*)alloca(nVertices * sizeof(int));
		//0 is used to identify a point which is not going to be contained within the returned graph
		memset(inverse, 0, sizeof(int)*nVertices);

		int vertexCounter = 0;
		for(std::size_t i = 0; i < nVertices; i++)
		{
			if(stateRef[i].state & (UNFIXED_MASK | ON_MASK)) vertexCounter++;
		}

		graph.clear();
		graph.m_vertices.resize(vertexCounter);
		vertexCounter = 0;
		for(std::size_t i = 0; i < nVertices; i++)
		{
			if(stateRef[i].state & (UNFIXED_MASK | ON_MASK))
			{
				boost::put(boost::vertex_name, graph, vertexCounter, i);
				vertexCounter++;
				inverse[i] = vertexCounter;
			}
		}
		for(int i = 0; i < vertexCounter; i++)
		{
			context::inputGraph::out_edge_iterator start, end;
			boost::tie(start, end) = boost::out_edges(boost::get(boost::vertex_name, graph, i), originalGraph);
			while(start != end)
			{
				if(inverse[start->m_target] > 0 && (int)i < inverse[start->m_target] - 1)
				{
					boost::add_edge((int)i, inverse[start->m_target] - 1, graph);
				}
				start++;
			}
		}
	}
}
