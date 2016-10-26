#ifndef GET_OBSERVATION_HEADER_GUARD
#define GET_OBSERVATION_HEADER_GUARD
#include <boost/random/mersenne_twister.hpp>
#include "context.h"
#include <boost/shared_array.hpp>
#include <boost/noncopyable.hpp>
namespace largeComponent
{
	template<class T> class getObservation : public boost::noncopyable
	{
		public:
			static typename T::observationType get(const T& input, boost::random::mt19937& randomSource)
			{
				const context& contextObj = input.getContext();
				std::size_t nVertices = boost::num_vertices(contextObj.getGraph());
				boost::shared_array<vertexState> state(new vertexState[nVertices]);
				typename T::observationConstructorType otherData;
				input.getObservation(state.get(), randomSource, otherData);
				typename T::observationType returnVal(input.getContext(), state, otherData);
				return returnVal;
			}
			template<typename U> static typename T::observationType get(const T& input, boost::random::mt19937& randomSource, U& aux)
			{
				const context& contextObj = input.getContext();
				std::size_t nVertices = boost::num_vertices(contextObj.getGraph());
				boost::shared_array<vertexState> state(new vertexState[nVertices]);
				typename T::observationConstructorType otherData(aux);
				input.getObservation(state.get(), randomSource, otherData);
				typename T::observationType returnVal(input.getContext(), state, otherData);
				return returnVal;
			}
		private:
			getObservation();
	};
}
#endif
