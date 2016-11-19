source("./generateScenarios.R")
library(Rmpfr)
variance <- mean <- re <- wnrv <- vector(mode = "numeric", length = nrow(scenarios))
for(i in 1:nrow(scenarios))
{
	load(file.path("results", scenarios[i, "fileName"]))
	estimates <- unlist(lapply(results, function(x) as.numeric(x@estimate)))
	times <- unlist(lapply(results, function(x) difftime(x@end, x@start, units = "secs")))
	variance[i] <- var(estimates)
	mean[i] <- mean(estimates)
	re[i] <- sd(estimates) / mean[i]
	wnrv[i] <- variance[i] * mean(times) / mean[i]^2
}
scenarios$fileName <- NULL
