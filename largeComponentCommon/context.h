#ifndef CONTEXT_HEADER_GUARD
#define CONTEXT_HEADER_GUARD
#include <boost/graph/adjacency_list.hpp>
#include <vector>
#include <boost/shared_array.hpp>
#include <boost/noncopyable.hpp>
#include "includeMPFRLargeComponent.h"
#include "vertexState.h"
namespace largeComponent
{
	class context : public boost::noncopyable
	{
	public:
		typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_index_t, int> > inputGraph;
		context(boost::shared_ptr<const inputGraph> graph, std::vector<mpfr_class>& opProbabilities, std::size_t componentSize);
		context(context&& other);
		const inputGraph& getGraph() const;
		context& operator=(context&& other);
		const std::vector<mpfr_class>& getOperationalProbabilities() const;
		const std::vector<double>& getOperationalProbabilitiesD() const;
		const int* getShortestDistances() const;
		std::size_t getComponentSize() const;
	private:
		std::vector<mpfr_class> opProbabilities;
		std::vector<double> opProbabilitiesD;
		boost::shared_ptr<const inputGraph> graph;
		boost::shared_array<const int> shortestDistances;
		std::size_t componentSize;
	};
}
#endif
