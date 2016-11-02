#ifndef SUBOBS_SEQUENTIAL_HEADER_GUARD
#define SUBOBS_SEQUENTIAL_HEADER_GUARD
#include "observationConstructorTypes.h"
#include "subObsConstructorTypes.h"
#include "getSubObservation.hpp"
#include "getObservation.hpp"
#include "subObs.h"
#include <boost/shared_array.hpp>
#include "context.h"
namespace largeComponent
{
	class observationSequential;
	class subObsSequential : public subObs
	{
	public:
		template<class T> friend class ::largeComponent::getSubObservation;
		template<class T> friend class ::largeComponent::getObservation;

		typedef observationSequential observationType;
		typedef observationConstructorTypes::sequentialConstructorType observationConstructorType;

		subObsSequential(subObsSequential&& other);
		subObsSequential& operator=(subObsSequential&& other);
		subObsSequential(context const& contextObj, boost::shared_array<const vertexState> state, ::largeComponent::subObsConstructorTypes::sequentialConstructorType&);
		const mpfr_class& getWeight() const;
		subObsSequential copyWithWeight(mpfr_class newWeight) const;
		bool isLargeComponentPossible() const;
		const std::vector<int>& getTable() const;
		const std::vector<int>& getComponents() const;
	private:
		subObsSequential(context const& contextObj, boost::shared_array<const vertexState> state);
		void getObservation(vertexState* newState, boost::mt19937& randomSource, observationConstructorType& other) const;
		mpfr_class weight;
		bool largeComponentPossible;
		std::vector<int> components;
		std::vector<int> table;
		boost::shared_array<const int> order;
	};
}
#endif
