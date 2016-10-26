#' @include monteCarloResult-class.R
setClass("importanceSamplingResult", slots = list(estimate = "mpfrOrNumeric", variance = "mpfrOrNumeric", call = "call", start = "POSIXct", end = "POSIXct"))
setMethod(f = "show", signature = "importanceSamplingResult", definition = function(object)
{
	cat("Monte Carlo estimate of ", as.numeric(object@estimate), " generated in ", as.numeric(object@end - object@start, unit="secs"), " seconds, by\n", sep="")
	print(object@call)
})
