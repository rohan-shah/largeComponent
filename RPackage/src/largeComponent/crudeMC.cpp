#include "largeComponentCommon/crudeMC.h"
#include "Rcpp.h"
#include "crudeMC.h"
#include "graphInterface.h"
#include "graphConvert.h"
#include <boost/random/mersenne_twister.hpp>
SEXP crudeMC(SEXP graph_sexp, SEXP probabilities_sexp, SEXP n_sexp, SEXP seed_sexp, SEXP componentSize_sexp, SEXP vertexPositions_sexp)
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

	largeComponent::context contextObj = graphInterface(graph_sexp, probabilities_sexp, componentSize, vertexPositions);
	boost::mt19937 randomSource;
	randomSource.seed(seed);
	largeComponent::crudeMCArgs args(contextObj, randomSource);
	args.n = n;

	std::size_t result = largeComponent::crudeMC(args);
	return Rcpp::wrap((double)result / (double)n);
END_RCPP
}
