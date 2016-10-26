#include "observationSequential.h"
#include <boost/random/bernoulli_distribution.hpp>
namespace largeComponent
{
	observationSequential::observationSequential(context const& contextObj, boost::shared_array<const vertexState> state, ::largeComponent::observationConstructorTypes::sequentialConstructorType& otherData)
		: observation(contextObj, state), weight(otherData.weight), importanceProbabilities(otherData.importanceProbabilities)
	{}
	observationSequential::observationSequential(context const& contextObj, boost::mt19937& randomSource, boost::shared_array<const double> importanceProbabilities)
		: observation(contextObj), weight(1), importanceProbabilities(importanceProbabilities)
	{
		std::size_t nVertices = boost::num_vertices(contextObj.getGraph());
		boost::shared_array<vertexState> state(new vertexState[nVertices]);

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
	}
	void observationSequential::getSubObservation(vertexState* newState, int radius, subObsConstructorType& other) const
	{
		observation::getSubObservation(radius, newState);
		std::size_t nVertices = boost::num_vertices(contextObj.getGraph());
		const std::vector<mpfr_class>& opProbabilities = contextObj.getOperationalProbabilities();
		mpfr_class newWeight = 1;
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
			}
		}
		other.weight = newWeight * weight;
	}
	observationSequential::observationSequential(observationSequential&& other)
		: observation(other), weight(other.weight), importanceProbabilities(other.importanceProbabilities)
	{}
	const mpfr_class& observationSequential::getWeight() const
	{
		return weight;
	}
}
