#ifndef SEQUENTIAL_METHOD_HEADER_GUARD
#define SEQUENTIAL_METHOD_HEADER_GUARD
#include "context.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/numeric/ublas/matrix.hpp>
namespace largeComponent
{
	struct aliasTemporariesStruct
	{
		std::vector<std::ptrdiff_t> temporary1, temporary2;
		std::vector<std::pair<double, std::ptrdiff_t> > temporary3;
	};
	struct sequentialMethodArgs
	{
		sequentialMethodArgs(const context& contextObj, boost::random::mt19937& randomSource);
		const context& contextObj;
		boost::random::mt19937& randomSource; 
		std::size_t n;
		mpfr_class estimate;
		int initialRadius;
		aliasTemporariesStruct aliasTemporaries;
		std::vector<double> levelProbabilities;
		std::vector<int> distinctParticles;
		boost::numeric::ublas::matrix<int> resamplingCounts;
	};
	void sequentialMethod(sequentialMethodArgs& args);
}
#endif
