#ifndef CONDITIONAL_MC_HEADER_GUARD
#define CONDITIONAL_MC_HEADER_GUARD
#include "context.h"
#include <boost/random/mersenne_twister.hpp>
#include "isLargeComponentPossible.h"
namespace largeComponent
{
	struct conditionalMCArgs
	{
	public:
		conditionalMCArgs(const context& contextObj, boost::random::mt19937& randomSource)
			: contextObj(contextObj), randomSource(randomSource)
		{}
		const context& contextObj;
		boost::random::mt19937& randomSource;
		std::size_t n, componentSize;
		mpfr_class estimate, varianceEstimate;
		isLargeComponentPossibleTemporaries temporaries;
		std::vector<double> importanceProbabilities;
	};
	void conditionalMC(conditionalMCArgs& args);
}
#endif
