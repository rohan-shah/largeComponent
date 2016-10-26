#ifndef LARGE_COMPONENT_IMPORTANCE_SAMPLING_OBSERVATION_HEADER_GUARD
#define LARGE_COMPONENT_IMPORTANCE_SAMPLING_OBSERVATION_HEADER_GUARD
#include <boost/random/mersenne_twister.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "context.h"
#include "observation.h"
namespace largeComponent
{
	class importanceSamplingObservation : public observation
	{
	public:
		importanceSamplingObservation(context const&, boost::mt19937& randomSource, std::vector<double>& importanceProbabilities);
		importanceSamplingObservation(importanceSamplingObservation&& other);
		importanceSamplingObservation& operator=(importanceSamplingObservation&& other);
		mpfr_class getWeight() const;
	protected:
		importanceSamplingObservation(const context& contextObj);
		importanceSamplingObservation& operator=(const importanceSamplingObservation&);
		importanceSamplingObservation(const importanceSamplingObservation& other);
	private:
		mpfr_class weight;
	};
}
#endif
