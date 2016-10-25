#ifndef LARGE_COMPONENT_OBSERVATION_HEADER_GUARD
#define LARGE_COMPONENT_OBSERVATION_HEADER_GUARD
#include <boost/random/mersenne_twister.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "largeComponentCommon/context.h"
namespace largeComponent
{
	class observation : public boost::noncopyable
	{
	public:
		observation(context const&, boost::mt19937& randomSource);
		observation(observation&& other);
		observation& operator=(observation&& other);
		context const& getContext() const;
		const vertexState* getState() const;
	protected:
		observation(const context& contextObj);
		observation& operator=(const observation&);
		observation(const observation& other);
		context const& contextObj;
		boost::shared_array<const vertexState> state;
	};
}
#endif
