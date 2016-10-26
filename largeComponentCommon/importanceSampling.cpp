#include "largeComponentCommon/importanceSampling.h"
#include "isLargeComponentPossible.h"
#include "importanceSamplingObservation.h"
namespace largeComponent
{
	void importanceSampling(importanceSamplingArgs& args)
	{
		const context& contextObj = args.contextObj;
		mpfr_class sum = 0, sumSquares = 0;
		isLargeComponentPossibleTemporaries temp;
		for(std::size_t i = 0; i < args.n; i++)
		{
			importanceSamplingObservation obs(contextObj, args.randomSource, args.importanceProbabilities);
			if(isLargeComponentPossible(contextObj.getGraph(), obs.getState(), args.componentSize, temp))
			{
				sum += obs.getWeight();
				sumSquares += obs.getWeight() * obs.getWeight();
			}
		}
		args.estimate = sum / args.n;
		args.varianceEstimate = sumSquares / args.n - args.estimate * args.estimate;
	}
}
