#include "subObsWithRadius.h"
namespace largeComponent
{
	subObsWithRadius& subObsWithRadius::operator=(const subObsWithRadius& other)
	{
		subObs::operator=(other);
		radius = other.radius;
		return *this;
	}
	subObsWithRadius::subObsWithRadius(context const& contextObj, boost::shared_array<const vertexState> state, int radius)
		: ::largeComponent::subObs(contextObj, state), radius(radius)
	{}
	subObsWithRadius::subObsWithRadius(subObsWithRadius&& other)
		: ::largeComponent::subObs(static_cast< ::largeComponent::subObs&&>(other)), radius(other.radius)
	{}
	subObsWithRadius::subObsWithRadius(const subObsWithRadius& other)
		: ::largeComponent::subObs(static_cast<const ::largeComponent::subObs&>(other)), radius(other.radius)
	{}
}
