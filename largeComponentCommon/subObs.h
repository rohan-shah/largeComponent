#ifndef LARGE_COMPONENT_SUB_OBS_HEADER_GUARD
#define LARGE_COMPONENT_SUB_OBS_HEADER_GUARD
#include <boost/shared_array.hpp>
#include "context.h"
#include "observation.h"
#include "observationConstructorTypes.h"
#include "getObservation.hpp"
namespace largeComponent
{
	class subObs : public ::largeComponent::observation
	{
	public:
		template<class T> friend class ::largeComponent::getObservation;
		typedef ::largeComponent::observation observationType;
		typedef ::largeComponent::obs::basicConstructorType observationConstructorType;

		subObs& operator=(subObs&& other);
		subObs(const context& contextObj, boost::shared_array<const vertexState> state);
		subObs(subObs&& other);
	protected:
		subObs(const subObs& other);
		subObs& operator=(const subObs& other);
	private:
		void getObservation(vertexState* state, boost::mt19937& randomSource, observationConstructorType&) const;
	};
}
#endif
