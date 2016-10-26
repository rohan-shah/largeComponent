#ifndef IMPORTANCE_SAMPLING_R_PACKAGE_HEADER_GUARD
#define IMPORTANCE_SAMPLING_R_PACKAGE_HEADER_GUARD
#include <Rcpp.h>
SEXP importanceSampling(SEXP graph, SEXP probabilities, SEXP n, SEXP seed, SEXP componentSize, SEXP importanceSampling);
#endif
