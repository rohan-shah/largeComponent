#include "defaultImportanceDensity.h"
#include "importanceDensity.h"
SEXP defaultImportanceDensity(SEXP opProbabilities_sexp, SEXP componentSize_sexp)
{
BEGIN_RCPP
	Rcpp::NumericVector opProbabilities_rcpp;
	try
	{
		opProbabilities_rcpp = Rcpp::as<Rcpp::NumericVector>(opProbabilities_sexp);
	}
	catch(...)
	{
		throw std::runtime_error("Input opProbabilities must be a numeric vector");
	}
	std::vector<double> opProbabilities = Rcpp::as<std::vector<double> >(opProbabilities_rcpp);
	int componentSize_tmp;
	try
	{
		componentSize_tmp = Rcpp::as<int>(componentSize_sexp);
	}
	catch(...)
	{
		throw std::runtime_error("Input componentSize must be an integer");
	}
	if(componentSize_tmp <= 0 || componentSize_tmp >= (int)opProbabilities.size())
	{
		throw std::runtime_error("Input componentSize was too big or too small");
	}
	std::size_t componentSize = (std::size_t)componentSize_tmp;
	boost::shared_array<double> importanceProbabilities(new double[opProbabilities.size()]);
	largeComponent::importanceDensity(importanceProbabilities, opProbabilities, componentSize);

	Rcpp::NumericVector result(opProbabilities.size());
	std::copy(importanceProbabilities.get(), importanceProbabilities.get() + opProbabilities.size(), &(result(0)));
	return result;
END_RCPP
}
