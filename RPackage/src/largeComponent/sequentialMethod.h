#ifndef SEQUENTIAL_METHOD_R_PACKAGE_HEADER_GUARD
#define SEQUENTIAL_METHOD_R_PACKAGE_HEADER_GUARD
#include <Rcpp.h>
SEXP sequentialMethod(SEXP graph, SEXP probabilities, SEXP n, SEXP seed, SEXP componentSize, SEXP initialRadius, SEXP vertexPositions_sexp);
#endif
