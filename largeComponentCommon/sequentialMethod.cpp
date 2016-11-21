#include "largeComponentCommon/sequentialMethod.h"
#include "observationSequential.h"
#include "subObsSequential.h"
#include "isLargeComponentPossible.h"
#include "aliasMethod.h"
#include "importanceDensity.h"
#include "empiricalDistribution.cpp"
#include "observationCollection.h"
#include <boost/archive/binary_oarchive.hpp>
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

		std::vector<int>& distinctParticles = args.distinctParticles;
		distinctParticles.resize(args.initialRadius+1);
		std::fill(distinctParticles.begin(), distinctParticles.end(), 0);

		std::vector<double>& levelProbabilities = args.levelProbabilities;
		levelProbabilities.resize(args.initialRadius+1);
		std::fill(levelProbabilities.begin(), levelProbabilities.end(), 0);

		boost::numeric::ublas::matrix<int>& resamplingCounts = args.resamplingCounts;
		resamplingCounts.resize(args.initialRadius, args.n, false);
		std::fill(resamplingCounts.data().begin(), resamplingCounts.data().end(), 0);
		
		boost::shared_array<double> initialImportanceProbabilities(new double[nVertices]);
		importanceDensity(initialImportanceProbabilities, opProbabilitiesD, componentSize);

		for(std::size_t i = 0; i < args.n; i++)
		{
			observations.emplace_back(observationSequential(contextObj, args.randomSource, initialImportanceProbabilities));
		}

		std::vector<subObsSequential> subObservations, resampledSubObservations;
		std::vector<double> remainingImportanceWeight, remainingAuxiliaryWeight;
		
		subObsSequential::observationConstructorType getObservationHelper;
		observationSequential::subObsConstructorType getSubObsHelper;
	
		mpfr_class sumWeights = 0, previousSumWeights = args.n;

		std::vector<int> alreadyOn, unfixed;
		std::vector<double> copyRemainingAuxiliaryWeight;

#ifndef NDEBUG
		observationCollection radiusOneDist(&contextObj, 1);
#endif
		for(int currentRadius = args.initialRadius; currentRadius >= 0; currentRadius--)
		{
			subObservations.clear();
			remainingImportanceWeight.clear();
			remainingImportanceWeight.reserve(args.n);

			remainingAuxiliaryWeight.clear();
			remainingAuxiliaryWeight.reserve(args.n);
			sumWeights = 0;
			mpfr_class sumAuxiliaryWeight = 0;
			for(std::vector<observationSequential>::iterator i = observations.begin(); i != observations.end(); i++)
			{
				subObsSequential sub = getSubObservation<observationSequential>::get(*i, currentRadius, getSubObsHelper);
				if(sub.isLargeComponentPossible())
				{
					mpfr_class auxiliaryWeight = sub.getGeometricMeanAdditional();
					subObservations.emplace_back(std::move(sub));
					sumWeights += subObservations.back().getWeight();
					sumAuxiliaryWeight += auxiliaryWeight;
					remainingAuxiliaryWeight.push_back(auxiliaryWeight.convert_to<double>());
					remainingImportanceWeight.push_back(subObservations.back().getWeight().convert_to<double>());
					distinctParticles[args.initialRadius - currentRadius]++;
				}
			}
			levelProbabilities[args.initialRadius - currentRadius] = mpfr_class(sumWeights / previousSumWeights).convert_to<double>();
			previousSumWeights = sumWeights;
			mpfr_class meanAuxiliaryWeight = sumAuxiliaryWeight / args.n;
			if(currentRadius == 0) break;
#ifndef NDEBUG
			if(currentRadius == 1)
			{
				for(std::vector<subObsSequential>::iterator i = subObservations.begin(); i != subObservations.end(); i++)
				{
					radiusOneDist.add(*i);
				}
			}
#endif
			if(sumWeights == 0)
			{
				args.estimate = 0;
				return;
			}
			//stratified resampling
			std::size_t remaining = args.n;
			resampledSubObservations.clear();
			for(std::vector<subObsSequential>::iterator i = subObservations.begin(); i != subObservations.end(); i++)
			{
				double& currentRemainingAuxiliaryWeight = remainingAuxiliaryWeight[std::distance(subObservations.begin(), i)];
				std::size_t multiple = (std::size_t)mpfr_class(currentRemainingAuxiliaryWeight / meanAuxiliaryWeight).convert_to<double>();
				mpfr_class weightOfCopy = (meanAuxiliaryWeight / currentRemainingAuxiliaryWeight) * i->getWeight();
				for(std::size_t j = 0; j < multiple; j++)
				{
					resampledSubObservations.push_back(i->copyWithWeight(weightOfCopy));
				}
				resamplingCounts(args.initialRadius - currentRadius, std::distance(subObservations.begin(), i)) += (int)multiple;
				remaining -= multiple;
				currentRemainingAuxiliaryWeight -= mpfr_class(meanAuxiliaryWeight * multiple).convert_to<double>();
				sumAuxiliaryWeight -= meanAuxiliaryWeight * multiple;
				remainingImportanceWeight[std::distance(subObservations.begin(), i)] -= mpfr_class(weightOfCopy * multiple).convert_to<double>();
				sumWeights -= weightOfCopy * multiple;
			}
			//leftovers
			copyRemainingAuxiliaryWeight = remainingAuxiliaryWeight;
			aliasMethod::aliasMethod alias(copyRemainingAuxiliaryWeight, sumAuxiliaryWeight.convert_to<double>(), args.aliasTemporaries.temporary1, args.aliasTemporaries.temporary2, args.aliasTemporaries.temporary3);
			//Sum of the weights for the randomly chosen units (the non-stratified ones)
			mpfr_class randomWeightsSum = 0;
			for(std::size_t j = 0; j < remaining; j++)
			{
				int index = (int)alias(args.randomSource);
				mpfr_class newWeight = remainingImportanceWeight[index] / remainingAuxiliaryWeight[index];
				randomWeightsSum += newWeight;
				resampledSubObservations.push_back(subObservations[index].copyWithWeight(newWeight));
				resamplingCounts(args.initialRadius - currentRadius, index)++;
			}
			for(std::size_t j = 0; j < remaining; j++)
			{
				subObsSequential& current = resampledSubObservations[resampledSubObservations.size() - 1 - j];
				current.setWeight(current.getWeight() * sumWeights / randomWeightsSum);
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
				int countLargeComponents = 0, largeComponentId = 0;
				for(std::vector<int>::const_iterator j = table.begin(); j != table.end(); j++)
				{
					if(*j >= (int)componentSize) 
					{
						countLargeComponents++;
						largeComponentId = std::distance(table.begin(), j);
					}
				}
				std::copy(opProbabilitiesD.begin(), opProbabilitiesD.end(), importanceProbabilities);
				if(countLargeComponents == 1 && alreadyOn[largeComponentId] < (int)componentSize)
				{
					boost::shared_array<double> importanceProbabilitiesSubsetVertices(new double[unfixed[largeComponentId]]);
					std::vector<double> originalProbabilitiesSubsetVertices;
					for(std::size_t j = 0; j < nVertices; j++)
					{
						if(currentSubObsVertexState[j].state & UNFIXED_MASK && components[j] == largeComponentId)
						{
							originalProbabilitiesSubsetVertices.push_back(opProbabilitiesD[j]);
						}
					}
					importanceDensity(importanceProbabilitiesSubsetVertices, originalProbabilitiesSubsetVertices, componentSize - alreadyOn[largeComponentId]);
					int counter = 0;
					for(std::size_t j = 0; j < nVertices; j++)
					{
						if(currentSubObsVertexState[j].state & UNFIXED_MASK && components[j] == largeComponentId)
						{
							importanceProbabilities[j] = std::max(opProbabilitiesD[j], importanceProbabilitiesSubsetVertices[counter]);
							counter++;
						}
					}
				}
				observationSequential obs = getObservation<subObsSequential>::get(*i, args.randomSource, getObservationHelper);
				observations.emplace_back(std::move(obs));
			}
		}
		args.estimate = sumWeights / args.n;
#ifndef NDEBUG
		std::ofstream outputStream("./radiusOneDist", std::ios_base::binary);
		boost::archive::binary_oarchive outputArchive(outputStream);
		outputArchive << radiusOneDist;
#endif
	}
}
