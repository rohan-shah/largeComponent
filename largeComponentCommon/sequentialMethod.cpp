#include "largeComponentCommon/sequentialMethod.h"
#include "observationSequential.h"
#include "subObsSequential.h"
#include "isLargeComponentPossible.h"
#include "aliasMethod.h"
namespace largeComponent
{
	sequentialMethodArgs::sequentialMethodArgs(const context& contextObj, boost::random::mt19937& randomSource)
		:contextObj(contextObj), randomSource(randomSource), initialRadius(-1)
	{}
	void sequentialMethod(sequentialMethodArgs& args)
	{
		if(args.initialRadius < 0)
		{
			throw std::runtime_error("Input initialRadius must be non-negative");
		}
		isLargeComponentPossibleTemporaries temp;
		std::vector<observationSequential> observations;
	
		const context& contextObj = args.contextObj;
		const std::vector<double>& opProbabilitiesD = contextObj.getOperationalProbabilitiesD();
		std::size_t componentSize = contextObj.getComponentSize();
		std::size_t nVertices = boost::num_vertices(contextObj.getGraph());
		boost::shared_array<double> initialImportanceProbabilities(new double[nVertices]);
		for(std::size_t i = 0; i < nVertices; i++)
		{
			initialImportanceProbabilities[i] = (double)componentSize / (double)nVertices;
		}

		for(std::size_t i = 0; i < args.n; i++)
		{
			observations.emplace_back(observationSequential(contextObj, args.randomSource, initialImportanceProbabilities));
		}

		std::vector<subObsSequential> subObservations, resampledSubObservations;
		std::vector<double> resamplingProbabilities;
		
		subObsSequential::observationConstructorType getObservationHelper;
		observationSequential::subObsConstructorType getSubObsHelper;
	
		mpfr_class sumWeights = 0;

		std::vector<int> alreadyOn, unfixed;
		for(int currentRadius = args.initialRadius; currentRadius >= 0; currentRadius--)
		{
			subObservations.clear();
			resamplingProbabilities.clear();
			sumWeights = 0;
			for(std::vector<observationSequential>::iterator i = observations.begin(); i != observations.end(); i++)
			{
				subObsSequential sub = getSubObservation<observationSequential>::get(*i, currentRadius, getSubObsHelper);
				if(sub.isLargeComponentPossible())
				{
					subObservations.emplace_back(std::move(sub));
					sumWeights += subObservations.back().getWeight();
					resamplingProbabilities.push_back(subObservations.back().getWeight().convert_to<double>());
				}
			}
			if(currentRadius == 0) break;
			if(sumWeights == 0)
			{
				args.estimate = 0;
				return;
			}
			//stratified resampling
			mpfr_class averageWeight = sumWeights / args.n;
			std::size_t remaining = args.n;
			resampledSubObservations.clear();
			for(std::vector<subObsSequential>::iterator i = subObservations.begin(); i != subObservations.end(); i++)
			{
				std::size_t multiple = (std::size_t)mpfr_class(i->getWeight() / averageWeight).convert_to<double>();
				for(std::size_t j = 0; j < multiple; j++)
				{
					resampledSubObservations.push_back(i->copyWithWeight(averageWeight));
				}
				remaining -= multiple;
				resamplingProbabilities[std::distance(subObservations.begin(), i)] -= mpfr_class(averageWeight * multiple).convert_to<double>();
				sumWeights -= averageWeight * multiple;
			}
			//leftovers
			aliasMethod::aliasMethod alias(resamplingProbabilities, sumWeights.convert_to<double>(), args.aliasTemporaries.temporary1, args.aliasTemporaries.temporary2, args.aliasTemporaries.temporary3);
			for(std::size_t j = 0; j < remaining; j++)
			{
				int index = (int)alias(args.randomSource);
				resampledSubObservations.push_back(subObservations[index].copyWithWeight(averageWeight));
			}
			observations.clear();
			for(std::vector<subObsSequential>::iterator i = resampledSubObservations.begin(); i != resampledSubObservations.end(); i++)
			{
				getObservationHelper.importanceProbabilities.reset(new double[nVertices]);
				double* importanceProbabilities = getObservationHelper.importanceProbabilities.get();
				std::fill(importanceProbabilities, importanceProbabilities + nVertices, -1);
				const vertexState* currentSubObsVertexState = i->getState();
				const std::vector<int>& table = i->getTable();
				const std::vector<int>& components = i->getComponents();

				alreadyOn.resize(table.size());
				unfixed.resize(table.size());
				std::fill(alreadyOn.begin(), alreadyOn.end(), 0);
				std::fill(unfixed.begin(), unfixed.end(), 0);

				for(std::size_t j = 0; j < nVertices; j++)
				{ 
					if(currentSubObsVertexState[j].state == FIXED_ON)
					{
						alreadyOn[components[j]]++;
					}
					else if(currentSubObsVertexState[j].state & UNFIXED_MASK)
					{
						unfixed[components[j]]++;
					}
				}
				for(std::size_t j = 0; j < nVertices; j++)
				{
					if(!(currentSubObsVertexState[j].state & FIXED_MASK))
					{
						if(table[components[j]] >= (int)componentSize)
						{
							importanceProbabilities[j] = (double)(componentSize - alreadyOn[components[j]]) / (double)unfixed[components[j]];
						}
						else
						{
							importanceProbabilities[j] = opProbabilitiesD[j];
						}
					}
				}
				observationSequential obs = getObservation<subObsSequential>::get(*i, args.randomSource, getObservationHelper);
				observations.emplace_back(std::move(obs));
			}
		}
		args.estimate = sumWeights / args.n;
	}
}
