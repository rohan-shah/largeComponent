#ifndef RPACKAGE_GRAPH_INTERFACE_HEADER_GUARD
#define RPACKAGE_GRAPH_INTERFACE_HEADER_GUARD
#include "largeComponentCommon/context.h"
#include <Rcpp.h>
largeComponent::context graphInterface(SEXP graph_sexp, SEXP probabilities_sexp);
#endif
