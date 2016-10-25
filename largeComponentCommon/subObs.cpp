#include "subObs.h"
#include <boost/random/bernoulli_distribution.hpp>
namespace largeComponent
{
	subObs::subObs(context const& contextObj, boost::shared_array<const vertexState> state)
		: ::largeComponent::observation(contextObj, state)
	{}
	subObs& subObs::operator=(const subObs& other)
	{
		observation::operator=(other);
		return *this;
	}
	subObs::subObs(subObs&& other)
		: ::largeComponent::observation(static_cast< ::largeComponent::observation&&>(other))
	{}
	subObs::subObs(const subObs& other)
		: ::largeComponent::observation(static_cast<const ::largeComponent::observation&>(other))
	{}
	void subObs::getObservation(vertexState* outputState, boost::mt19937& randomSource, observationConstructorType&) const
	{
		std::size_t nVertices = boost::num_vertices(contextObj.getGraph());
		const std::vector<double>& operationalProbabilitiesD = contextObj.getOperationalProbabilitiesD();
		memcpy(outputState, state.get(), sizeof(vertexState)*nVertices);
		//generate a full random grid, which includes the subPoints 
		for(std::size_t i = 0; i < nVertices; i++)
		{
			if(outputState[i].state & UNFIXED_MASK)
			{
				boost::random::bernoulli_distribution<double> vertexDistribution(operationalProbabilitiesD[i]);
				if(vertexDistribution(randomSource))
				{
					outputState[i].state = UNFIXED_ON;
				}
				else outputState[i].state = UNFIXED_OFF;
			}
		}
	}
}
