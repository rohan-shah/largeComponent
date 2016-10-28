#include "observation.h"
#include <boost/random/uniform_real_distribution.hpp>
namespace largeComponent
{
	observation::observation(const context& contextObj, boost::shared_array<const vertexState> state)
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
	observation::observation(context const&, boost::shared_array<const vertexState> state, observationConstructorTypes::basicConstructorType&)
		: contextObj(contextObj), state(state)
	{}
	void observation::getSubObservation(int radius, vertexState* outputState) const
	{
		getSubObservation(radius, outputState, contextObj, state.get());
	}
	void observation::getSubObservation(int radius, vertexState* newState, const context& contextObj, const vertexState* oldStatesPtr)
	{
		std::size_t nVertices = boost::num_vertices(contextObj.getGraph());
		const int* shortestDistances = contextObj.getShortestDistances();

		std::size_t sourceVertex = 0;

		std::fill(newState, newState+nVertices, vertexState::fixed_off());

		while(sourceVertex < nVertices)
		{
			//is this vertex marked as on, for one reason or another? If so continue from here
			if((oldStatesPtr[sourceVertex].state & ON_MASK) > 0 && newState[sourceVertex].state == FIXED_OFF)
			{
				newState[sourceVertex].state = FIXED_ON;

				//Do we find another vertex in our search that is marked on, and is far enough away from the source?
				//If so retain it, it will be our new starting point. 
				//If no such found, we'll continue from finalSearchVertex+1
				bool found = false;
				std::size_t nextSourceVertex = -1;
				//keep copy of source vertex
				std::size_t copiedSourceVertex = sourceVertex;
				//we want to begin on the NEXT vertex
				sourceVertex++;
				while(sourceVertex < nVertices)
				{
					int previousState = oldStatesPtr[sourceVertex].state;
					if(shortestDistances[copiedSourceVertex + nVertices * sourceVertex] <= radius)
					{
						if(previousState & FIXED_MASK) newState[sourceVertex].state = previousState;
						else newState[sourceVertex].state = UNFIXED_OFF;
					}
					else if(!found && (previousState & ON_MASK) > 0 && newState[sourceVertex].state == FIXED_OFF)
					{
						nextSourceVertex = sourceVertex;
						found = true;
					}
					sourceVertex++;
				}
				//if we found another vertex, continue from there. If no, we're already at finalSearchVertex+1. 
				//Which is where we want to be.
				if(found)
				{
					sourceVertex = nextSourceVertex;
				}
			}
			else sourceVertex++;
		}
	}
}
