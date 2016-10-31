#ifndef CONDITIONAL_MC_R_PACKAGE_HEADER_GUARD
#define CONDITIONAL_MC_R_PACKAGE_HEADER_GUARD
#include <Rcpp.h>
SEXP conditionalMC(SEXP graph, SEXP probabilities, SEXP n, SEXP seed, SEXP componentSize, SEXP importanceProbabilities);
#endif
