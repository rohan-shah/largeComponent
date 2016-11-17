#ifndef IMPORTANCE_DENSITY_HEADER_GUARD
#define IMPORTANCE_DENSITY_HEADER_GUARD
#include <boost/shared_array.hpp>
#include <vector>
namespace largeComponent
{
	void importanceDensity(boost::shared_array<double> importanceProbabilities, const std::vector<double>& opProbabilities, std::size_t componentSize);
}
#endif
