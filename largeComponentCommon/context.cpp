#include "context.h"
#include "contextImpl.h"
#include <boost/graph/johnson_all_pairs_shortest.hpp>
namespace largeComponent
{
	context::context(boost::shared_ptr<const inputGraph> graph, std::vector<mpfr_class>& opProbabilities, boost::shared_ptr<std::vector<vertexPosition> > vertexPositions, std::size_t componentSize)
		:opProbabilities(opProbabilities), graph(graph), componentSize(componentSize), vertexPositions(vertexPositions)
	{
		std::size_t nVertices = boost::num_vertices(*graph.get());
		if(vertexPositions->size() != 0 && vertexPositions->size() != nVertices)
		{
			throw std::runtime_error("Input vertexPositions must have an entry for every vertex, or zero entries");
		}
		if(opProbabilities.size() == 1 && opProbabilities.size() != nVertices)
		{
			this->opProbabilities.insert(this->opProbabilities.end(), nVertices - 1, opProbabilities[0]);
		}
		std::transform(opProbabilities.begin(), opProbabilities.end(), std::back_inserter(opProbabilitiesD), [](mpfr_class& x){return x.convert_to<double>();});

		boost::shared_array<int> shortestDistances = boost::shared_array<int>(new int[nVertices*nVertices]);

		contextImpl::twoDArray tmp;
		tmp.base = shortestDistances.get();
		tmp.dim = nVertices;

		contextImpl::constant_property_map<context::inputGraph::edge_descriptor, 1> edgeWeights;
		boost::johnson_all_pairs_shortest_paths(*graph.get(), tmp, boost::weight_map(edgeWeights));
		this->shortestDistances = shortestDistances;
	}
	context::context(context&& other)
		: opProbabilities(std::move(other.opProbabilities)), opProbabilitiesD(std::move(other.opProbabilitiesD)), graph(other.graph), shortestDistances(other.shortestDistances), componentSize(other.componentSize)
	{}
	const context::inputGraph& context::getGraph() const
	{
		return *graph.get();
	}
	context& context::operator=(context&& other)
	{
		if(graph.get() != other.graph.get())
		{
			throw std::runtime_error("Internal error");
		}
		opProbabilities = std::move(other.opProbabilities);
		opProbabilitiesD = std::move(other.opProbabilitiesD);
		shortestDistances = other.shortestDistances;
		componentSize = other.componentSize;
		vertexPositions = other.vertexPositions;
		return *this;
	}
	const std::vector<mpfr_class>& context::getOperationalProbabilities() const
	{
		return opProbabilities;
	}
	const std::vector<double>& context::getOperationalProbabilitiesD() const
	{
		return opProbabilitiesD;
	}
	const int* context::getShortestDistances() const
	{
		return shortestDistances.get();
	}
	std::size_t context::getComponentSize() const
	{
		return componentSize;
	}
	const std::vector<context::vertexPosition>& context::getVertexPositions() const
	{
		return *vertexPositions.get();
	}
}
