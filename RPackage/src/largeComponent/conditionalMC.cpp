#include "largeComponentCommon/conditionalMC.h"
#include "Rcpp.h"
#include "conditionalMC.h"
#include "graphInterface.h"
#include "graphConvert.h"
#include <boost/random/mersenne_twister.hpp>
SEXP conditionalMC(SEXP graph_sexp, SEXP probabilities_sexp, SEXP n_sexp, SEXP seed_sexp, SEXP componentSize_sexp, SEXP importanceProbabilities_sexp, SEXP vertexPositions_sexp)
{
BEGIN_RCPP
	//convert number of samples
	double n_double;
	try
	{
		n_double = Rcpp::as<double>(n_sexp);
	}
	catch(...)
	{
		throw std::runtime_error("Unable to convert input n to a number");
	}
	long n;
	if(std::abs(n_double - std::round(n_double)) > 1e-3)
	{
		throw std::runtime_error("Input n must be an integer");
	}
	n = (long)std::round(n_double);

	//convert seed
	int seed;
	try
	{
		seed = Rcpp::as<int>(seed_sexp);
	}
	catch(...)
	{
		throw std::runtime_error("Input seed must be an integer");
	}

	//Convert importance sampling probabilities
	std::vector<double> importanceProbabilities;
	try
	{
		importanceProbabilities = Rcpp::as<std::vector<double> >(importanceProbabilities_sexp);
	}
	catch(...)
	{
		throw std::runtime_error("Input importanceProbabilities must be a numeric vector");
	}

	//convert componentSize
	int componentSize;
	try
	{
		componentSize = Rcpp::as<int>(componentSize_sexp);
	}
	catch(...)
	{
		throw std::runtime_error("Input componentSize must be an integer");
	}
	if(componentSize <= 1)
	{
		throw std::runtime_error("Input componentSize must be at least 2");
	}

	//Convert vertexPositions
	boost::shared_ptr<std::vector<largeComponent::context::vertexPosition> > vertexPositions(new std::vector<largeComponent::context::vertexPosition>());
	try
	{
		Rcpp::NumericMatrix vertexPositions_Rcpp = Rcpp::as<Rcpp::NumericMatrix>(vertexPositions_sexp);
		for(int i = 0; i < vertexPositions_Rcpp.nrow(); i++)
		{
			vertexPositions->push_back(largeComponent::context::vertexPosition(vertexPositions_Rcpp(i, 0), vertexPositions_Rcpp(i, 1)));
		}
	}
	catch(...)
	{}

	largeComponent::context::inputGraph graph;
	graphConvert(graph_sexp, graph);

	std::size_t nVertices = boost::num_vertices(graph);
	if(importanceProbabilities.size() != nVertices)
	{
		throw std::runtime_error("Input importanceProbabilities had the wrong length");
	}

	largeComponent::context contextObj = graphInterface(graph_sexp, probabilities_sexp, componentSize, vertexPositions);
	boost::mt19937 randomSource;
	randomSource.seed(seed);
	largeComponent::conditionalMCArgs args(contextObj, randomSource);
	args.n = n;
	args.importanceProbabilities = importanceProbabilities;
	args.componentSize = componentSize;

	largeComponent::conditionalMC(args);

	std::string estimate_string = args.estimate.str(10, std::ios_base::dec);
	std::string variance_string = args.varianceEstimate.str(10, std::ios_base::dec);
	Rcpp::List result = Rcpp::List::create(Rcpp::Named("estimate") = Rcpp::wrap(estimate_string), Rcpp::Named("variance") = Rcpp::wrap(variance_string));
	return result;
END_RCPP
}
