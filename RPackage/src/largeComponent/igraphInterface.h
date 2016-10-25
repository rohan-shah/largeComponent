#ifndef IGRAPH_INTERFACE_HEADER_GUARD
#define IGRAPH_INTERFACE_HEADER_GUARD
#include <Rcpp.h>
#include "context.h"
void igraphConvert(SEXP graph_sexp, largeComponent::context::inputGraph& outputGraph);
#endif
