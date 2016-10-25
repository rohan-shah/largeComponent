#ifndef GRAPH_AM_INTERFACE_HEADER_GUARD
#define GRAPH_AM_INTERFACE_HEADER_GUARD
#include <Rcpp.h>
#include "context.h"
void graphAMConvert(SEXP graph_sexp, largeComponent::context::inputGraph& outputGraph);
#endif
