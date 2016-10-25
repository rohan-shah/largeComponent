#include "largeComponentCommon/context.h"
namespace largeComponent
{
	context::context(const inputGraph& graph, std::vector<mpfr_class>& opProbabilities)
		:opProbabilities(opProbabilities), graph(graph)
	{
		std::transform(opProbabilities.begin(), opProbabilities.end(), std::back_inserter(opProbabilitiesD), [](mpfr_class& x){return x.convert_to<double>();});
	}
	context::context(context&& other)
		: opProbabilities(std::move(opProbabilities)), opProbabilitiesD(std::move(opProbabilitiesD)), graph(other.graph)
	{}
	const context::inputGraph& context::getGraph() const
	{
		return graph;
	}
	context& context::operator=(context&& other)
	{
		if(&graph != &other.graph)
		{
			throw std::runtime_error("Internal error");
		}
		opProbabilities = std::move(other.opProbabilities);
		opProbabilitiesD = std::move(other.opProbabilitiesD);
		return *this;
	}
	const std::vector<mpfr_class>& context::getOperationalProbabilities() const
	{
		return opProbabilities;
	}
	const std::vector<double>& context::getOperationalProbabilitiesD() const
	{
		return opProbabilitiesD;
	}
}
