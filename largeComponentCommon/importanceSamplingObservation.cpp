#include "importanceSamplingObservation.h"
#include <boost/random/uniform_real_distribution.hpp>
namespace largeComponent
{
	importanceSamplingObservation::importanceSamplingObservation(context const& contextObj, boost::mt19937& randomSource, std::vector<double>& importanceProbabilities)
		: observation(contextObj), weight(1)
	{
		std::size_t nVertices = boost::num_vertices(contextObj.getGraph());
		if(nVertices != importanceProbabilities.size())
		{
			throw std::runtime_error("Internal error");
		}

		boost::shared_array<vertexState> state(new vertexState[nVertices]);
		const std::vector<double>& opProbabilitiesD = contextObj.getOperationalProbabilitiesD();
		boost::random::uniform_real_distribution<> uniform;
		for(std::size_t i = 0; i < nVertices; i++)
		{
			if(uniform(randomSource) <= importanceProbabilities[i])
			{
				state[i] = vertexState::fixed_on();
				weight *= opProbabilitiesD[i] / importanceProbabilities[i];
			}
			else
			{
				state[i] = vertexState::fixed_off();
				weight *= (1 - opProbabilitiesD[i]) / (1 - importanceProbabilities[i]);
			}
		}
		this->state = state;
	}
	mpfr_class importanceSamplingObservation::getWeight() const
	{
		return weight;
	}
	importanceSamplingObservation::importanceSamplingObservation(importanceSamplingObservation&& other)
		: observation(other), weight(other.weight)
	{}
	importanceSamplingObservation& importanceSamplingObservation::operator=(importanceSamplingObservation&& other)
	{
		observation::operator=(other);
		weight = other.weight;
		return *this;
	}
	importanceSamplingObservation::importanceSamplingObservation(const context& contextObj)
		: observation(contextObj)
	{
	}
	importanceSamplingObservation& importanceSamplingObservation::operator=(const importanceSamplingObservation& other)
	{
		observation::operator=(other);
		weight = other.weight;
		return *this;
	}
	importanceSamplingObservation::importanceSamplingObservation(const importanceSamplingObservation& other)
		: observation(other), weight(other.weight)
	{}
}
