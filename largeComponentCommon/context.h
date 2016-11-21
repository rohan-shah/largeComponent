#ifndef CONTEXT_HEADER_GUARD
#define CONTEXT_HEADER_GUARD
#include <boost/graph/adjacency_list.hpp>
#include <vector>
#include <boost/shared_array.hpp>
#include <boost/noncopyable.hpp>
#include "includeMPFRLargeComponent.h"
#include "vertexState.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include "serializeGMP.hpp"
namespace largeComponent
{
	class context : public boost::noncopyable
	{
	public:
		friend class boost::serialization::access;
		typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_index_t, int> > inputGraph;
		typedef std::pair<float, float> vertexPosition;
		context(boost::shared_ptr<const inputGraph> graph, std::vector<mpfr_class>& opProbabilities, boost::shared_ptr<std::vector<vertexPosition> > vertexPositions, std::size_t componentSize);
		context(context&& other);
		context(boost::archive::text_iarchive& ar)
		{
			ar >> *this;
		}
		context(boost::archive::binary_iarchive& ar)
		{
			ar >> *this;
		}
		template<class Archive> void save(Archive& ar, const unsigned int version) const
		{
			ar & *graph;
			ar & opProbabilities;
			ar & componentSize;

			std::size_t nVertices = boost::num_vertices(*graph);
			ar & boost::serialization::make_array(shortestDistances.get(), nVertices * nVertices);

			ar & *vertexPositions.get();
		};
		template<class Archive> void load(Archive& ar, const unsigned int version)
		{
			{
				boost::shared_ptr<inputGraph> graph(new inputGraph());
				ar & *graph;
				this->graph = graph;
			}
			ar & opProbabilities;
			ar & componentSize;

			std::size_t nVertices = boost::num_vertices(*graph);
			{
				boost::shared_array<int> shortestDistances(new int[nVertices * nVertices]);
				ar & boost::serialization::make_array(shortestDistances.get(), nVertices * nVertices);
				this->shortestDistances = shortestDistances;
			}
			{
				vertexPositions.reset(new std::vector<vertexPosition>());
				ar & *vertexPositions;
			}

			opProbabilitiesD.clear();
			std::transform(opProbabilities.begin(), opProbabilities.end(), std::back_inserter(opProbabilitiesD), std::bind(&mpfr_class::convert_to<double>, std::placeholders::_1));
		};
		BOOST_SERIALIZATION_SPLIT_MEMBER()
		const inputGraph& getGraph() const;
		context& operator=(context&& other);
		const std::vector<vertexPosition>& getVertexPositions() const;
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
		boost::shared_ptr<std::vector<vertexPosition> > vertexPositions;
	};
}
#endif
