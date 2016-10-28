#ifndef SEQUENTIAL_METHOD_HEADER_GUARD
#define SEQUENTIAL_METHOD_HEADER_GUARD
#include "context.h"
#include <boost/random/mersenne_twister.hpp>
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
		std::size_t n, componentSize;
		mpfr_class estimate;
		int initialRadius;
		aliasTemporariesStruct aliasTemporaries;
	};
	void sequentialMethod(sequentialMethodArgs& args);
}
#endif
