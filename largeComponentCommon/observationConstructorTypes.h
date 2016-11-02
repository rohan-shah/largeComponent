#ifndef OBS_CONSTRUCTOR_TYPES_HEADER_GUARD
#define OBS_CONSTRUCTOR_TYPES_HEADER_GUARD
#include <vector>
#include "depth_first_search_restricted.hpp"
#include "context.h"
#include <boost/shared_array.hpp>
namespace largeComponent
{
	namespace observationConstructorTypes
	{
		struct basicConstructorType
		{
		};
		struct sequentialConstructorType
		{
		public:
			mpfr_class weight;
			boost::shared_array<double> importanceProbabilities;
			boost::shared_array<const int> order;
		};
	}
}
#endif
