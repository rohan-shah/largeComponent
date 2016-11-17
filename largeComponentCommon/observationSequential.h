#ifndef OBS_SEQUENTIAL_HEADER_GUARD
#define OBS_SEQUENTIAL_HEADER_GUARD
#include "subObsConstructorTypes.h"
#include "getSubObservation.hpp"
#include "getObservation.hpp"
#include "observation.h"
#include <boost/shared_array.hpp>
#include "context.h"
#include <boost/random/mersenne_twister.hpp>
#include "observationConstructorTypes.h"
namespace largeComponent
{
	class subObsSequential;
	class observationSequential : public observation
	{
	public:
		template<class T> friend class ::largeComponent::getSubObservation;
		template<class T> friend class ::largeComponent::getObservation;

		typedef subObsSequential subObservationType;
		typedef subObsConstructorTypes::sequentialConstructorType subObsConstructorType;

		observationSequential(context const& contextObj, boost::mt19937& randomSource, boost::shared_array<double const> importanceProbabilities);
		observationSequential(observationSequential&& other);
		observationSequential& operator=(observationSequential&& other);
		observationSequential(context const& contextObj, boost::shared_array<const vertexState> state, ::largeComponent::observationConstructorTypes::sequentialConstructorType&);
		const mpfr_class& getWeight() const;
		const int* getOrder() const;
	private:
		void getSubObservation(vertexState* newState, int radius, subObsConstructorType& other) const;
		mpfr_class weight;
		boost::shared_array<const double> importanceProbabilities;
		boost::shared_array<const int> order;
	};
}
#endif
