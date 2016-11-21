#ifndef RPACKAGE_GRAPH_INTERFACE_HEADER_GUARD
#define RPACKAGE_GRAPH_INTERFACE_HEADER_GUARD
#include "context.h"
#include <Rcpp.h>
largeComponent::context graphInterface(SEXP graph_sexp, SEXP probabilities_sexp, std::size_t componentSize, boost::shared_ptr<std::vector<largeComponent::context::vertexPosition> > vertexPositions);
#endif
