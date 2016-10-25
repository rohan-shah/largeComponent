#include "isLargeComponentPossible.h"
#include <boost/graph/connected_components.hpp>
#include "depth_first_search_restricted.hpp"
#include "connected_components_restricted.hpp"
namespace largeComponent
{
	bool isLargeComponentPossible(const context::inputGraph& graph, const vertexState* state, std::size_t componentSize, isLargeComponentPossibleTemporaries& temporaries)
	{
		typedef boost::color_traits<boost::default_color_type> Color;
		std::size_t nVertices = boost::num_vertices(graph);

		if(temporaries.colourVector.size() < nVertices) temporaries.colourVector.resize(nVertices);
		std::fill(temporaries.colourVector.begin(), temporaries.colourVector.end(), Color::black());

		temporaries.initialPoints.clear();
		for(std::size_t i = 0; i < nVertices; i++)
		{
			if(state[i].state & ON_MASK)
			{
				temporaries.initialPoints.push_back((int)i);
				temporaries.colourVector[i] = Color::white();
			}
		}
		if(temporaries.connectedComponents.size() < nVertices) temporaries.connectedComponents.resize(nVertices);
		int nComponents = boost::connected_components_restricted(graph, &(temporaries.connectedComponents[0]), &(temporaries.colourVector[0]), temporaries.stack, temporaries.initialPoints);
		if(temporaries.table.size() < (std::size_t)nComponents) temporaries.table.resize(nComponents);
		std::fill(temporaries.table.begin(), temporaries.table.end(), 0);
		for(std::size_t i = 0; i < nVertices; i++)
		{
			if(state[i].state & ON_MASK) temporaries.table[temporaries.connectedComponents[i]]++;
		}
		return *std::max_element(temporaries.table.begin(), temporaries.table.end()) >= (int)componentSize;
	}
}
