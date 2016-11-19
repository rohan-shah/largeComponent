graphSize <- c(10L, 12L, 14L)
thresholdProportion <- c(0.5, 0.6, 0.7)
sampleSize <- 10000L
methods <- c("SIS", "importanceUniform", "conditionalUniform", "importanceNonUniform", "conditionalNonUniform")
replications <- 1000L

scenarios <- expand.grid(method = methods, sampleSize = sampleSize, thresholdProportion = thresholdProportion, graphSize = graphSize, stringsAsFactors=FALSE)
scenarios$fileName <- paste0(scenarios$method, "-", scenarios$sampleSize, "-", scenarios$thresholdProportion, "-", scenarios$graphSize, ".RData")
