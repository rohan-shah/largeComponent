#include "graphInterface.h"
#include "igraphInterface.h"
#include "graphAMInterface.h"
#include "graphNELInterface.h"
#include "graphConvert.h"
largeComponent::context graphInterface(SEXP graph_sexp, SEXP probabilities_sexp)
{
	//Convert probability
	std::vector<largeComponent::mpfr_class> probabilities;
	Rcpp::NumericVector probabilities_R;
	try
	{
		probabilities_R = Rcpp::as<Rcpp::NumericVector>(probabilities_sexp);
		std::transform(probabilities_R.begin(), probabilities_R.end(), std::back_inserter(probabilities), [](double x){return largeComponent::mpfr_class(x); });
	}
	catch(Rcpp::not_compatible&)
	{
		throw std::runtime_error("Unable to convert input probability to a number");
	}

	boost::shared_ptr<largeComponent::context::inputGraph> boostGraph(new largeComponent::context::inputGraph());
	graphConvert(graph_sexp, *boostGraph.get()); 
	std::size_t nVertices = boost::num_vertices(*boostGraph);
	if(probabilities.size() != nVertices && probabilities.size() != 1)
	{
		throw std::runtime_error("Input probabilities must contain a single value, or a value for each vertex");
	}

	largeComponent::context contextObj(boostGraph, probabilities);
	return contextObj;
}
