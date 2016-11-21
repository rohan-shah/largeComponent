#ifndef LARGE_COMPONENT_OBSERVATION_HEADER_GUARD
#define LARGE_COMPONENT_OBSERVATION_HEADER_GUARD
#include <boost/random/mersenne_twister.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "context.h"
#include "observationConstructorTypes.h"
#include "getObservation.hpp"
namespace largeComponent
{
	class observation : public boost::noncopyable
	{
	public:
		template<class T> friend class ::largeComponent::getObservation;
		friend class boost::serialization::access;
		observation(context const& contextObj, boost::archive::binary_iarchive& archive);
		observation(context const& contextObj, boost::archive::text_iarchive& archive);
		observation(context const&, boost::shared_array<const vertexState> state);
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
		void getSubObservation(int radius, vertexState* outputState) const;
		static void getSubObservation(int radius, vertexState* newState, const context& contextObj, const vertexState* oldStatesPtr);
	private:
		BOOST_SERIALIZATION_SPLIT_MEMBER()
		template<class Archive> void load(Archive& ar, const unsigned int version)
		{
			const context::inputGraph& graph = contextObj.getGraph();
			std::size_t nVertices = boost::num_vertices(graph);
			boost::shared_array<vertexState> state(new vertexState[nVertices]);
			ar & boost::serialization::make_array(state.get(), nVertices);
			this->state = state;
		};
		template<class Archive> void save(Archive& ar, const unsigned int version) const
		{
			const context::inputGraph& graph = contextObj.getGraph();
			std::size_t nVertices = boost::num_vertices(graph);
			ar & boost::serialization::make_array(state.get(), nVertices);
		}
		observation(context const&, boost::shared_array<const vertexState> state, observationConstructorTypes::basicConstructorType&);
	};
	class observationWithContext
	{
	public:
		observationWithContext(boost::archive::text_iarchive& ar)
		{
			ar >> *this;
		}
		observationWithContext(boost::archive::binary_iarchive& ar)
		{
			ar >> *this;
		}
		observationWithContext(const observationWithContext& other);
		friend class boost::serialization::access;
		const observation& getObs() const;
		const context& getContext() const;
	private:
		BOOST_SERIALIZATION_SPLIT_MEMBER()
		template<class Archive> void save(Archive& ar, const unsigned int version) const
		{
			std::string typeString = "largeComponentObsWithContext";
			ar << typeString;
			ar << obs->getContext();
			ar << *obs;
			typeString = "largeComponentObsWithContext_end";
			ar << typeString;
		}
		template<class Archive> void load(Archive& ar, const unsigned int version)
		{
			std::string typeString;
			ar >> typeString;
			if(typeString != "largeComponentObsWithContext")
			{
				throw std::runtime_error("Incorrect type specifier");
			}
			contextObj.reset(new context(ar));
			obs.reset(new observation(*contextObj.get(), ar));
			ar >> typeString;
			if(typeString != "largeComponentObsWithContext_end")
			{
				throw std::runtime_error("Incorrect type specifier");
			}
		}
		boost::shared_ptr<const context> contextObj;
		boost::shared_ptr<observation> obs;
	};}
#endif
