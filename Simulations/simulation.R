library(largeComponent)
if(!dir.exists("results")) dir.create("results", showWarnings = FALSE)

source("./generateScenarios.R")
scenarioIndex <- as.integer(Sys.getenv("SCENARIO_INDEX"))

fileName <- scenarios[scenarioIndex, "fileName"]
method <- scenarios[scenarioIndex, "method"]
thresholdProportion <- scenarios[scenarioIndex, "thresholdProportion"]
graphSize <- scenarios[scenarioIndex, "graphSize"]
thresholdCount <- round(graphSize * graphSize * thresholdProportion)
sampleSize <- scenarios[scenarioIndex, "sampleSize"]

graph <- igraph::make_lattice(dimvector = c(graphSize, graphSize))

set.seed(graphSize)
vertexProbabilities <- runif(graphSize * graphSize)
if(file.exists(file.path("results", fileName)))
{
	load(file.path("results", fileName))
} else
{
	results <- list()
}

if(method == "SIS")
{
	if(length(results) < replications)
	{
		start <- length(results)+1
		for(i in start:replications)
		{
			results[[i]] <- sequentialImportanceSampling(probabilities = vertexProbabilities, n = sampleSize, seed = i, graph = graph, componentSize = thresholdCount, initialRadius = round(graphSize * 0.3))
			if(i %% 100 == 0) save(results, file = file.path("results", fileName))
			cat(i, " ", sep = "")
		}
		save(results, file = file.path("results", fileName))
	}
} else if(method == "importanceUniform")
{
	if(length(results) < replications)
	{
		start <- length(results)+1
		for(i in start:replications)
		{
			results[[i]] <- importanceSampling(probabilities = vertexProbabilities, n = sampleSize, seed = i, graph = graph, componentSize = thresholdCount, importanceProbabilities = rep(thresholdCount / (graphSize * graphSize), graphSize * graphSize))
			if(i %% 100 == 0) save(results, file = file.path("results", fileName))
			cat(i, " ", sep = "")
		}
		save(results, file = file.path("results", fileName))
	}
} else if(method == "conditionalUniform")
{
	if(length(results) < replications)
	{
		start <- length(results)+1
		for(i in start:replications)
		{
			results[[i]] <- conditionalMC(probabilities = vertexProbabilities, n = sampleSize, seed = i, graph = graph, componentSize = thresholdCount, importanceProbabilities = rep(thresholdCount / (graphSize * graphSize), graphSize * graphSize))
			if(i %% 100 == 0) save(results, file = file.path("results", fileName))
			cat(i, " ", sep = "")
		}
		save(results, file = file.path("results", fileName))
	}
} else if(method == "importanceNonUniform")
{
	if(length(results) < replications)
	{
		importanceProbabilities <- .Call("defaultImportanceDensity", vertexProbabilities, thresholdCount, PACKAGE="largeComponent")
		start <- length(results)+1
		for(i in start:replications)
		{
			results[[i]] <- importanceSampling(probabilities = vertexProbabilities, n = sampleSize, seed = i, graph = graph, componentSize = thresholdCount, importanceProbabilities = importanceProbabilities)
			if(i %% 100 == 0) save(results, file = file.path("results", fileName))
			cat(i, " ", sep = "")
		}
		save(results, file = file.path("results", fileName))
	}
} else if(method == "conditionalNonUniform")
{
	if(length(results) < replications)
	{
		importanceProbabilities <- .Call("defaultImportanceDensity", vertexProbabilities, thresholdCount, PACKAGE="largeComponent")
		start <- length(results)+1
		for(i in start:replications)
		{
			results[[i]] <- conditionalMC(probabilities = vertexProbabilities, n = sampleSize, seed = i, graph = graph, componentSize = thresholdCount, importanceProbabilities = importanceProbabilities)
			if(i %% 100 == 0) save(results, file = file.path("results", fileName))
			cat(i, " ", sep = "")
		}
		save(results, file = file.path("results", fileName))
	}
} else
{
	stop("Invalid value for input method")
}
