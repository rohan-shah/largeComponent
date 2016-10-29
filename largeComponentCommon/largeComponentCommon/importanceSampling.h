#ifndef IMPORTANCE_SAMPLING_HEADER_GUARD
#define IMPORTANCE_SAMPLING_HEADER_GUARD
#include "context.h"
#include <boost/random/mersenne_twister.hpp>
namespace largeComponent
{
	struct importanceSamplingArgs
	{
	public:
		importanceSamplingArgs(const context& contextObj, boost::random::mt19937& randomSource)
			: contextObj(contextObj), randomSource(randomSource)
		{}
		const context& contextObj;
		boost::random::mt19937& randomSource;
		std::size_t n;
		std::vector<double> importanceProbabilities;
		mpfr_class estimate, varianceEstimate;
	};
	void importanceSampling(importanceSamplingArgs& args);
}
#endif
