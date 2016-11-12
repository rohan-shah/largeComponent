#include "observationSequential.h"
#include <boost/random/bernoulli_distribution.hpp>
#include <boost/range/algorithm/random_shuffle.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/random/random_number_generator.hpp>
namespace largeComponent
{
	observationSequential::observationSequential(context const& contextObj, boost::shared_array<const vertexState> state, ::largeComponent::observationConstructorTypes::sequentialConstructorType& otherData)
		: observation(contextObj, state), weight(otherData.weight), importanceProbabilities(otherData.importanceProbabilities), order(otherData.order)
	{}
	observationSequential::observationSequential(context const& contextObj, boost::mt19937& randomSource, boost::shared_array<const double> importanceProbabilities)
		: observation(contextObj), weight(1), importanceProbabilities(importanceProbabilities)
	{
		std::size_t nVertices = boost::num_vertices(contextObj.getGraph());
		boost::shared_array<vertexState> state(new vertexState[nVertices]);
		boost::shared_array<int> order(new int[nVertices]);
		for(std::size_t i = 0; i < nVertices; i++) order[i] = (int)i;
		boost::iterator_range<int*> range = boost::make_iterator_range(order.get(), order.get() + nVertices);
		boost::random_number_generator<boost::mt19937> generator(randomSource);
		boost::random_shuffle(range, generator);

		for(std::size_t i = 0; i < nVertices; i++)
		{
			boost::random::bernoulli_distribution<double> vertexDistribution(importanceProbabilities[i]);
			if(vertexDistribution(randomSource))
			{
				state[i].state = UNFIXED_ON;
			}
			else
			{
				state[i].state = UNFIXED_OFF;
			}
		}
		this->state = state;
		this->order = order;
	}
	void observationSequential::getSubObservation(vertexState* newState, int radius, subObsConstructorType& other) const
	{
		observation::getSubObservation(radius, newState);
		std::size_t nVertices = boost::num_vertices(contextObj.getGraph());
		const std::vector<mpfr_class>& opProbabilities = contextObj.getOperationalProbabilities();
		mpfr_class newWeight = 1;
		std::size_t nNewFixed = 0;
		for(std::size_t i = 0; i < nVertices; i++)
		{
			if(newState[i].state != state[i].state && (newState[i].state & FIXED_MASK))
			{
				if(newState[i].state == FIXED_ON)
				{
					newWeight *= opProbabilities[i] / importanceProbabilities[i];
				}
				else
				{
					newWeight *= (1 - opProbabilities[i]) / (1 - importanceProbabilities[i]);
				}
				nNewFixed++;
			}
		}
		other.weight = newWeight * weight;
		other.order = order;
		other.geometricMeanAdditional = boost::multiprecision::pow(newWeight, 1.0 / nNewFixed);
	}
	observationSequential::observationSequential(observationSequential&& other)
		: observation(other), weight(other.weight), importanceProbabilities(other.importanceProbabilities), order(other.order)
	{}
	const mpfr_class& observationSequential::getWeight() const
	{
		return weight;
	}
	const int* observationSequential::getOrder() const
	{
		return order.get();
	}
}
