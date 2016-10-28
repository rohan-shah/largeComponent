#ifndef IS_LARGE_COMPONENT_POSSIBLE_HEADER_GUARD
#define IS_LARGE_COMPONENT_POSSIBLE_HEADER_GUARD
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/shared_ptr.hpp>
#include "context.h"
#include "depth_first_search_restricted.hpp"
namespace largeComponent
{
	struct isLargeComponentPossibleTemporaries
	{
		std::vector<int> connectedComponents;
		boost::detail::depth_first_visit_restricted_impl_helper<context::inputGraph>::stackType stack;

		std::vector<context::inputGraph::vertex_descriptor> initialPoints;
		std::vector<boost::default_color_type> colourVector;
		std::vector<int> table;
	};
	bool isLargeComponentPossible(const context::inputGraph& graph, const vertexState* state, std::size_t componentSize, isLargeComponentPossibleTemporaries& temp);
	bool isLargeComponentOn(const context::inputGraph& graph, const vertexState* state, std::size_t componentSize, isLargeComponentPossibleTemporaries& temporaries);
}
#endif
