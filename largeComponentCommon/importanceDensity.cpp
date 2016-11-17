#include "importanceDensity.h"
namespace largeComponent
{
	void importanceDensity(boost::shared_array<double> importanceProbabilities, const std::vector<double>& opProbabilities, std::size_t componentSize)
	{
		std::size_t nVertices = opProbabilities.size();
		double sumOpProbabilities = 0;
		for(int i = 0; i < (int)opProbabilities.size(); i++)
		{
			sumOpProbabilities += opProbabilities[i];
		}
		if(sumOpProbabilities < componentSize)
		{
			double remaining = componentSize - sumOpProbabilities;
			for(int i = 0; i < (int)opProbabilities.size(); i++)
			{
				importanceProbabilities[i] = opProbabilities[i] + ((1 - opProbabilities[i]) / (nVertices - sumOpProbabilities))*remaining;
			}
		}
		else
		{
			std::copy(opProbabilities.begin(), opProbabilities.end(), importanceProbabilities.get());
		}
	}
}
