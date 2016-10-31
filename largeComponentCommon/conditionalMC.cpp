#include "largeComponentCommon/conditionalMC.h"
#include <boost/range/algorithm/random_shuffle.hpp>
#include <boost/random/uniform_real_distribution.hpp>
namespace largeComponent
{
	void conditionalMC(conditionalMCArgs& args)
	{
		args.estimate = 0;
		std::size_t n = args.n;
		std::size_t componentSize = args.componentSize;
		
		const context& contextObj = args.contextObj;
		const context::inputGraph& graph = contextObj.getGraph();
		std::size_t nVertices = boost::num_vertices(graph);
		const std::vector<double>& opProbabilitiesD = contextObj.getOperationalProbabilitiesD();
		const std::vector<mpfr_class>& opProbabilities = contextObj.getOperationalProbabilities();
		std::vector<double>& importanceProbabilities = args.importanceProbabilities;

		if(importanceProbabilities.size() != nVertices)
		{
			throw std::runtime_error("Input importanceProbabilities had the wrong length");
		}

		boost::shared_array<vertexState> state(new vertexState[nVertices]);
		vertexState* statePtr = state.get();

		std::vector<int> components(nVertices, 0);
		std::vector<int> table;
		std::vector<int> indices(nVertices);
		for(std::size_t i = 0; i < nVertices; i++)
		{
			indices[i] = i;
		}

		mpfr_class weight = 1;
		boost::random::uniform_real_distribution<> standardUniform;

		for(std::size_t i = 0; i < n; i++)
		{
			std::fill(statePtr, statePtr + nVertices, vertexState::unfixed_off());
			boost::random_shuffle(indices);
			weight = 1;
			for(std::size_t vertexCounter = 0; vertexCounter < nVertices; vertexCounter++)
			{
				statePtr[indices[vertexCounter]] = vertexState::fixed_off();
				if(!isLargeComponentPossible(graph, statePtr, componentSize, args.temporaries))
				{
					statePtr[indices[vertexCounter]] = vertexState::fixed_on();
					weight *= opProbabilities[indices[vertexCounter]];
				}
				else
				{
					if(args.temporaries.table[args.temporaries.connectedComponents[indices[vertexCounter]]] < componentSize)
					{
						if(standardUniform(args.randomSource) <= opProbabilitiesD[indices[vertexCounter]])
						{
							statePtr[indices[vertexCounter]] = vertexState::fixed_on();
						}
					}
					else
					{
						if(standardUniform(args.randomSource) <= importanceProbabilities[indices[vertexCounter]])
						{
							statePtr[indices[vertexCounter]] = vertexState::fixed_on();
							weight *= opProbabilities[indices[vertexCounter]] / importanceProbabilities[indices[vertexCounter]];
						}
						else
						{
							weight *= (1 - opProbabilities[indices[vertexCounter]]) / (1 - importanceProbabilities[indices[vertexCounter]]);
						}
					}
				}
			}
			args.estimate += weight;
		}
		args.estimate /= n;
	}
}
