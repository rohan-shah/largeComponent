#ifndef SUB_OBS_WITH_RADIUS_HEADER_GUARD
#define SUB_OBS_WITH_RADIUS_HEADER_GUARD
#include <boost/shared_array.hpp>
#include "context.h"
#include "subObs.h"
namespace largeComponent
{
	class subObsWithRadius : public subObs
	{
	public:
		subObsWithRadius& operator=(subObsWithRadius&& other);
		int getRadius() const
		{
			return radius;
		}
	protected:
		subObsWithRadius(const context& contextObj, boost::shared_array<const vertexState> state, int radius);
		subObsWithRadius& operator=(const subObsWithRadius&);
		subObsWithRadius(subObsWithRadius&& other);
		subObsWithRadius(const subObsWithRadius& other);
		int radius;
	};
}
#endif
