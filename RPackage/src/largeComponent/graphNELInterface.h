#ifndef GRAPH_NEL_INTERFACE_HEADER_GUARD
#define GRAPH_NEL_INTERFACE_HEADER_GUARD
#include <Rcpp.h>
#include "context.h"
void graphNELConvert(SEXP graph_sexp, largeComponent::context::inputGraph& outputGraph);
#endif
