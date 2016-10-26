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
		public:
			basicConstructorType(std::vector<int>& components, boost::detail::depth_first_visit_restricted_impl_helper<context::inputGraph>::stackType& stack);
			std::vector<int>& components;
			boost::detail::depth_first_visit_restricted_impl_helper<context::inputGraph>::stackType& stack;
		};
		struct sequentialConstructorType
		{
		public:
			mpfr_class weight;
			boost::shared_array<double> importanceProbabilities;
			int radius;
		};
	}
}
#endif
