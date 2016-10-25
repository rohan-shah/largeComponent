#include "observation.h"
#include <boost/random/uniform_real_distribution.hpp>
namespace largeComponent
{
	observation::observation(context const&, boost::shared_array<const vertexState> state)
		: contextObj(contextObj), state(state)
	{}
	observation::observation(context const& contextObj, boost::mt19937& randomSource)
		: contextObj(contextObj)
	{
		std::size_t nVertices = boost::num_vertices(contextObj.getGraph());
		boost::shared_array<vertexState> state(new vertexState[nVertices]);
		const std::vector<double>& opProbabilitiesD = contextObj.getOperationalProbabilitiesD();
		boost::random::uniform_real_distribution<> uniform;
		for(std::size_t i = 0; i < nVertices; i++)
		{
			if(uniform(randomSource) <= opProbabilitiesD[i])
			{
				state[i] = vertexState::unfixed_on();
			}
			else state[i] = vertexState::unfixed_off();
		}
		this->state = state;
	}
	observation::observation(observation&& other)
		: contextObj(other.contextObj), state(std::move(other.state))
	{}
	observation& observation::operator=(observation&& other)
	{
		if(&contextObj != &other.contextObj)
		{
			throw std::runtime_error("Internal error");
		}
		state = other.state;
		other.state.reset();
		return *this;
	}
	context const& observation::getContext() const
	{
		return contextObj;
	}
	const vertexState* observation::getState() const
	{
		return state.get();
	}
	observation::observation(const context& contextObj)
		:contextObj(contextObj)
	{
	}
	observation& observation::operator=(const observation& other)
	{
		if(&contextObj != &other.contextObj)
		{
			throw std::runtime_error("Internal error");
		}
		state = other.state;
		return *this;
	}
	observation::observation(const observation& other)
		: contextObj(other.contextObj), state(other.state)
	{}
	observation::observation(context const&, boost::shared_array<const vertexState> state, obs::basicConstructorType&)
		: contextObj(contextObj), state(state)
	{}
}
