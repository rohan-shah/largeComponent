#include "subObsSequential.h"
#include <boost/random/bernoulli_distribution.hpp>
namespace largeComponent
{
	subObsSequential::subObsSequential(context const& contextObj, boost::shared_array<const vertexState> state, mpfr_class newWeight)
		: subObs(contextObj, state), weight(newWeight)
	{}
	subObsSequential subObsSequential::copyWithWeight(mpfr_class newWeight) const
	{
		subObsSequential retVal(contextObj, state, newWeight);
		return retVal;
	}
	subObsSequential::subObsSequential(subObsSequential&& other)
		:subObs(other), weight(other.weight)
	{}
	subObsSequential& subObsSequential::operator=(subObsSequential&& other)
	{
		subObs::operator=(other);
		weight = other.weight;
		return *this;
	}
	subObsSequential::subObsSequential(context const& contextObj, boost::shared_array<const vertexState> state, ::largeComponent::subObsConstructorTypes::sequentialConstructorType& constructorType)
		: subObs(contextObj, state), weight(constructorType.weight)
	{}
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
	}
}
