#ifndef SUBOBS_CONSTRUCTOR_TYPES_HEADER_GUARD
#define SUBOBS_CONSTRUCTOR_TYPES_HEADER_GUARD
#include <vector>
#include "depth_first_search_restricted.hpp"
#include "context.h"
#include "isLargeComponentPossible.h"
namespace largeComponent
{
	namespace subObsConstructorTypes
	{
		struct sequentialConstructorType
		{
		public:
			isLargeComponentPossibleTemporaries temporaries;
	 		mpfr_class weight, geometricMeanAdditional;
			boost::shared_array<const int> order;
		};
	}
}
#endif
