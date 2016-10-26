#ifndef LARGE_COMPONENT_OBSERVATION_HEADER_GUARD
#define LARGE_COMPONENT_OBSERVATION_HEADER_GUARD
#include <boost/random/mersenne_twister.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "context.h"
#include "observationConstructorTypes.h"
#include "getObservation.hpp"
namespace largeComponent
{
	class observation : public boost::noncopyable
	{
	public:
		template<class T> friend class ::largeComponent::getObservation;
		observation(context const&, boost::shared_array<const vertexState> state);
		observation(context const&, boost::mt19937& randomSource);
		observation(observation&& other);
		observation& operator=(observation&& other);
		context const& getContext() const;
		const vertexState* getState() const;
	protected:
		observation(const context& contextObj);
		observation& operator=(const observation&);
		observation(const observation& other);
		context const& contextObj;
		boost::shared_array<const vertexState> state;
		void getSubObservation(int radius, vertexState* outputState) const;
		static void getSubObservation(int radius, vertexState* newState, const context& contextObj, const vertexState* oldStatesPtr);
	private:
		observation(context const&, boost::shared_array<const vertexState> state, observationConstructorTypes::basicConstructorType&);
	};
}
#endif
