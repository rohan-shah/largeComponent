#include "subObsSequential.h"
#include <boost/random/bernoulli_distribution.hpp>
namespace largeComponent
{
	subObsSequential::subObsSequential(context const& contextObj, boost::shared_array<const vertexState> state)
		: subObs(contextObj, state)
	{}
	subObsSequential subObsSequential::copyWithWeight(mpfr_class newWeight) const
	{
		subObsSequential retVal(contextObj, state);
		retVal.weight = newWeight;
		retVal.largeComponentPossible = largeComponentPossible;
		retVal.components = components;
		retVal.table = table;
		retVal.order = order;
		return retVal;
	}
	subObsSequential::subObsSequential(subObsSequential&& other)
		:subObs(other), weight(other.weight), largeComponentPossible(other.largeComponentPossible), components(std::move(other.components)), table(std::move(other.table)), order(std::move(other.order))
	{}
	subObsSequential& subObsSequential::operator=(subObsSequential&& other)
	{
		subObs::operator=(other);
		weight = other.weight;
		largeComponentPossible = other.largeComponentPossible;
		components = std::move(other.components);
		table = std::move(other.table);
		order = other.order;
		return *this;
	}
	subObsSequential::subObsSequential(context const& contextObj, boost::shared_array<const vertexState> state, ::largeComponent::subObsConstructorTypes::sequentialConstructorType& constructorType)
		: subObs(contextObj, state), weight(constructorType.weight), order(constructorType.order)
	{
		largeComponentPossible = ::largeComponent::isLargeComponentPossible(contextObj.getGraph(), state.get(), contextObj.getComponentSize(), constructorType.temporaries);
		components.swap(constructorType.temporaries.connectedComponents);
		table.swap(constructorType.temporaries.table);
	}
	const std::vector<int>& subObsSequential::getTable() const
	{
		return table;
	}
	const std::vector<int>& subObsSequential::getComponents() const
	{
		return components;
	}
	bool subObsSequential::isLargeComponentPossible() const
	{
		return largeComponentPossible;
	}
	const mpfr_class& subObsSequential::getWeight() const
	{
		return weight;
	}
	void subObsSequential::getObservation(vertexState* newState, boost::mt19937& randomSource, observationConstructorType& other) const
	{
		std::size_t nVertices = boost::num_vertices(contextObj.getGraph());
		double* importanceProbabilities = other.importanceProbabilities.get();
		memcpy(newState, state.get(), sizeof(vertexState)*nVertices);
		for(std::size_t i = 0; i < nVertices; i++)
		{
			if(newState[i].state & UNFIXED_MASK)
			{
				boost::random::bernoulli_distribution<double> vertexDistribution(importanceProbabilities[i]);
				if(vertexDistribution(randomSource))
				{
					newState[i].state = UNFIXED_ON;
				}
				else newState[i].state = UNFIXED_OFF;
			}
		}
		other.weight = weight;
		other.order = order;
	}
}
