#' @export
#' @include monteCarloResult-class.R
setClass("sequentialImportanceResult", slots = list(estimate = "mpfrOrNumeric", distinctParticles = "integer", levelProbabilities = "numeric", call = "call", start = "POSIXct", end = "POSIXct"))
setMethod(f = "show", signature = "sequentialImportanceResult", definition = function(object)
{
	cat("Monte Carlo estimate of ", as.numeric(object@estimate), " generated in ", as.numeric(object@end - object@start, unit="secs"), " seconds, by\n", sep="")
	print(object@call)
})
