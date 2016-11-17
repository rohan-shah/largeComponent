#include "observationSequential.h"
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/range/algorithm/random_shuffle.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/random/random_number_generator.hpp>
#include "constructSubGraph.h"
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/connected_components.hpp>
namespace largeComponent
{
	observationSequential::observationSequential(context const& contextObj, boost::shared_array<const vertexState> state, ::largeComponent::observationConstructorTypes::sequentialConstructorType& otherData)
		: observation(contextObj, state), weight(otherData.weight), importanceProbabilities(otherData.importanceProbabilities), order(otherData.order)
	{}
	observationSequential::observationSequential(context const& contextObj, boost::mt19937& randomSource, boost::shared_array<const double> importanceProbabilities)
		: observation(contextObj), weight(1), importanceProbabilities(importanceProbabilities)
	{
		std::size_t nVertices = boost::num_vertices(contextObj.getGraph());
		boost::shared_array<vertexState> state(new vertexState[nVertices]);
		boost::shared_array<int> order(new int[nVertices]);
		for(std::size_t i = 0; i < nVertices; i++) order[i] = (int)i;
		boost::iterator_range<int*> range = boost::make_iterator_range(order.get(), order.get() + nVertices);
		boost::random_number_generator<boost::mt19937> generator(randomSource);
		boost::random_shuffle(range, generator);

		for(std::size_t i = 0; i < nVertices; i++)
		{
			boost::random::bernoulli_distribution<double> vertexDistribution(importanceProbabilities[i]);
			if(vertexDistribution(randomSource))
			{
				state[i].state = UNFIXED_ON;
			}
			else
			{
				state[i].state = UNFIXED_OFF;
			}
		}
		this->state = state;
		this->order = order;
	}
	struct countVisitor : public boost::default_dfs_visitor
	{
	public:
		template<typename Vertex, typename Graph> void discover_vertex(Vertex u, const Graph& g)
		{
			count += boost::get(boost::vertex_name, g, u);
		}
		int count;
	};
	void observationSequential::getSubObservation(vertexState* newState, int radius, subObsConstructorType& other) const
	{
		observation::getSubObservation(radius, newState);
		std::size_t nVertices = boost::num_vertices(contextObj.getGraph());
		const std::vector<mpfr_class>& opProbabilities = contextObj.getOperationalProbabilities();
		mpfr_class newWeight = 1;
		std::size_t nNewFixed = 0;
		for(std::size_t i = 0; i < nVertices; i++)
		{
			if(newState[i].state != state[i].state && (newState[i].state & FIXED_MASK))
			{
				if(newState[i].state == FIXED_ON)
				{
					newWeight *= opProbabilities[i] / importanceProbabilities[i];
				}
				else
				{
					newWeight *= (1 - opProbabilities[i]) / (1 - importanceProbabilities[i]);
				}
				nNewFixed++;
			}
		}
		if(radius == 1)
		{
			int componentSize = (int)contextObj.getComponentSize();

			subGraphType reducedGraph;
			constructSubGraph(reducedGraph, contextObj, newState);
			{
				//assign edge indices
				subGraphType::edge_iterator start, end;
				int edgeIndex = 0;
				boost::property_map<subGraphType, boost::edge_index_t>::type edgePropertyMap = boost::get(boost::edge_index, reducedGraph);
				boost::tie(start, end) = boost::edges(reducedGraph);
				for(; start != end; start++) boost::put(edgePropertyMap, *start, edgeIndex++);
			}

			typedef std::vector<boost::graph_traits<subGraphType>::edges_size_type> component_storage_t;
			typedef boost::iterator_property_map<component_storage_t::iterator, boost::property_map<subGraphType, boost::edge_index_t>::type> component_map_t;
			component_storage_t biconnectedIds(boost::num_edges(reducedGraph), -1);
			component_map_t componentMap(biconnectedIds.begin(), boost::get(boost::edge_index, reducedGraph));
			//articulation vertices is the vertex ids of the articulation vertices in *reducedGraph*, not the original graph. 
			std::vector<int> articulationVertices;
			boost::biconnected_components(reducedGraph, componentMap, std::back_inserter(articulationVertices));
			std::vector<bool> isArticulation(boost::num_vertices(reducedGraph), false), alreadyCounted(boost::num_vertices(reducedGraph), false);
			for(std::vector<int>::iterator i = articulationVertices.begin(); i != articulationVertices.end(); i++) alreadyCounted[*i] = isArticulation[*i] = true;

			int nBiconnectedComponents = *std::max_element(biconnectedIds.begin(), biconnectedIds.end())+1;
			int nComponentsGraphVertices =  nBiconnectedComponents + (int)articulationVertices.size();
			std::vector<int> biconnectedComponentSizes(nBiconnectedComponents, 0);

			//Count the sizes of every component (ignoring the articulation vertices, which are contained in multiple components. 
			subGraphType::edge_iterator current, end;
			boost::tie(current, end) = boost::edges(reducedGraph);
			for(; current != end; current++)
			{
				int source = boost::source(*current, reducedGraph), target = boost::target(*current, reducedGraph);
				int biconnectedID = biconnectedIds[boost::get(boost::edge_index, reducedGraph, *current)];
				if(!alreadyCounted[source]) biconnectedComponentSizes[biconnectedID]++;
				if(!alreadyCounted[target]) biconnectedComponentSizes[biconnectedID]++;
				alreadyCounted[source] = alreadyCounted[target] = true;
			}

			//Ok, now we create an even *more* reduced graph, containing the articulation vertices and the biconnected components. The vertex names are the values that we take the sum of, in order to work out the number of vertices that *might* be in that part of the graph. The ordering of the vertex *IDS* is that the first nBiconnectedComponents vertices represent the biconnected components, and the remaining vertices represent the articulation vertices, in the order given in articulationVertices.  
			typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::property<boost::vertex_name_t, int>, boost::no_property> componentsGraphType;
			componentsGraphType componentsGraph(nComponentsGraphVertices);
			for(int i = 0; i < nBiconnectedComponents; i++)
			{
				boost::put(boost::vertex_name, componentsGraph, i, biconnectedComponentSizes[i]);
			}
			for(int i = 0; i < (int)articulationVertices.size(); i++)
			{
				boost::put(boost::vertex_name, componentsGraph, nBiconnectedComponents + i, 1);
			}
			//Now we put in the edges of the reduced graph
			for(int i = 0; i < (int)articulationVertices.size(); i++)
			{
				subGraphType::out_edge_iterator currentOut, endOut;
				boost::tie(currentOut, endOut) = boost::out_edges(articulationVertices[i], reducedGraph);
				for(; currentOut != endOut; currentOut++)
				{
					subGraphType::edge_descriptor currentEdge = *currentOut;
					int biconnectedComponentId = biconnectedIds[boost::get(boost::edge_index, reducedGraph, currentEdge)];
					boost::add_edge(nBiconnectedComponents + i, biconnectedComponentId, componentsGraph);
				}
			}
			//visitor used to accumulate the vertex_name attributes over the different parts
			countVisitor accumulator;
			typedef boost::color_traits<boost::default_color_type> Color;
			//Colour vector for depth-first visit of componentsGraph
			std::vector<boost::default_color_type> colourVector(nComponentsGraphVertices, Color::white());
			std::vector<int> mustBeOn;
			if(componentSize <= (int)nVertices / 2)
			{
				//If there is more than one (connected) component which has the potential to be large, then we can't really do any conditioning. In theory we could do some inclusion / exclusion calculation, but in practice it probably won't be worth it. 
				std::vector<int> componentIDs(nComponentsGraphVertices, -1);
				int nConnectedComponents = boost::connected_components(componentsGraph, &(componentIDs[0]), boost::color_map(&(colourVector[0])));
				//Table of the number of vertices in every component
				std::vector<int> table(nConnectedComponents, 0);
				for(int i = 0; i < nComponentsGraphVertices; i++)
				{
					table[componentIDs[i]]++;
				}
				int possibleLarge = 0;
				for(int i = 0; i < nConnectedComponents; i++) possibleLarge += (table[i] >= componentSize);
				if(possibleLarge > 1) goto skipConditioning;
			}

			//Ok, now iterate through the articulation vertices and see which ones divide the graph into two parts *BOTH* of which are too small. 
			//Here the colour vector is used to break the biconnected components apart at the articulation vertices. 
			for(int i = 0; i < (int)articulationVertices.size(); i++)
			{
				componentsGraphType::out_edge_iterator currentOut, endOut;
				boost::tie(currentOut, endOut) = boost::out_edges(nBiconnectedComponents + i, componentsGraph);
				std::fill(colourVector.begin(), colourVector.end(), Color::white());
				//Is the presence / absence of this articulation vertex still uncertain?
				//If not, skip it. 
				int originalVertex = boost::get(boost::vertex_name, reducedGraph, articulationVertices[i]);
				if(newState[originalVertex].state & FIXED_MASK) continue;
				//Mark the articulation vertex as being removed. 
				colourVector[nBiconnectedComponents + i] = Color::black();
				int currentComponentMaxSize = 0;
				for(; currentOut != endOut; currentOut++)
				{
					componentsGraphType::edge_descriptor edge = *currentOut;
					int startingVertex = boost::target(edge, componentsGraph);
					if(startingVertex == articulationVertices[i]) startingVertex = boost::source(edge, componentsGraph);
					if(colourVector[startingVertex] == Color::white())
					{
						accumulator.count = 0;
						boost::depth_first_visit(componentsGraph, startingVertex, accumulator, &(colourVector[0]));
						//In this case we don't need this articulation vertex to be present, in order to have a large component
						if(accumulator.count >= componentSize) goto skipCurrentArticulationVertex;
						currentComponentMaxSize += accumulator.count;
					}
				}
				currentComponentMaxSize++;
				if(currentComponentMaxSize >= componentSize)
				{
					//If we get to here then we really need this vertex. 
					newState[originalVertex] = vertexState::fixed_on();
					newWeight *= opProbabilities[originalVertex];
					nNewFixed++;
				}
skipCurrentArticulationVertex:
				;
			}
skipConditioning:
			other.weight = newWeight * weight;
			other.geometricMeanAdditional = pow(newWeight.convert_to<double>(), 1.0 / nNewFixed);
			other.order = order;
		}
		else
		{
			other.weight = newWeight * weight;
			other.order = order;
			other.geometricMeanAdditional = pow(newWeight.convert_to<double>(), 1.0 / nNewFixed);
		}
	}
	observationSequential::observationSequential(observationSequential&& other)
		: observation(other), weight(other.weight), importanceProbabilities(other.importanceProbabilities), order(other.order)
	{}
	const mpfr_class& observationSequential::getWeight() const
	{
		return weight;
	}
	const int* observationSequential::getOrder() const
	{
		return order.get();
	}
}
