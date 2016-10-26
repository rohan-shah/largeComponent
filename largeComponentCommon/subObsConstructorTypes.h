#ifndef SUBOBS_CONSTRUCTOR_TYPES_HEADER_GUARD
#define SUBOBS_CONSTRUCTOR_TYPES_HEADER_GUARD
#include <vector>
#include "depth_first_search_restricted.hpp"
#include "context.h"
namespace largeComponent
{
	namespace subObsConstructorTypes
	{
		struct sequentialConstructorType
		{
		public:
			sequentialConstructorType(mpfr_class weight)
				:weight(weight)
			{}
			mpfr_class weight;
		};
	}
}
#endif
