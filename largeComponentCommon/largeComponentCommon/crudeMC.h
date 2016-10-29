#ifndef CRUDE_MC_HEADER_GUARD
#define CRUDE_MC_HEADER_GUARD
#include "context.h"
#include <boost/random/mersenne_twister.hpp>
namespace largeComponent
{
	struct crudeMCArgs
	{
	public:
		crudeMCArgs(const context& contextObj, boost::random::mt19937& randomSource)
			: contextObj(contextObj), randomSource(randomSource)
		{}
		const context& contextObj;
		boost::random::mt19937& randomSource;
		std::size_t n;
	};
	std::size_t crudeMC(crudeMCArgs& args);
}
#endif
