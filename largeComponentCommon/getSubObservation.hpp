#ifndef GET_SUB_OBSERVATION_HEADER_GUARD
#define GET_SUB_OBSERVATION_HEADER_GUARD
#include "context.h"
#include <boost/noncopyable.hpp>
namespace largeComponent
{
	template<class T> class getSubObservation : public boost::noncopyable
	{
		public:
			static typename T::subObservationType get(const T& input, int radius)
			{
				const context& contextObj = input.getContext();
				typename T::subObservationConstructorType otherData;
				boost::shared_array<vertexState> newState(new vertexState[boost::num_vertices(contextObj.getGraph())]);
				input.getSubObservation(newState.get(), radius, otherData);

				typename T::subObservationType retVal(contextObj, newState, otherData);
				return retVal;
			}
			template<typename U> static typename T::subObservationType get(const T& input, int radius, U& aux)
			{
				const context& contextObj = input.getContext();
				boost::shared_array<vertexState> newState(new vertexState[boost::num_vertices(contextObj.getGraph())]);
				input.getSubObservation(newState.get(), radius, aux);

				typename T::subObservationType retVal(contextObj, newState, aux);
				return retVal;
			}
		private:
			getSubObservation();
	};
}
#endif

