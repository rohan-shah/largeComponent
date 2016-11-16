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
	if(length(results) < 1000)
	{
		start <- length(results)+1
		for(i in start:1000)
		{
			results[[i]] <- sequentialImportanceSampling(probabilities = vertexProbabilities, n = sampleSize, seed = i, graph = graph, componentSize = thresholdCount, initialRadius = round(graphSize * 0.75))
			if(i %% 100 == 0) save(results, file = file.path(results, fileName))
			cat(i, " ", sep = "")
		}
		save(results, file = file.path(results, fileName))
	}
} else if(method == "importance")
{
	if(length(results) < 1000)
	{
		start <- length(results)+1
		for(i in start:1000)
		{
			results[[i]] <- importanceSampling(probabilities = vertexProbabilities, n = sampleSize, seed = i, graph = graph, componentSize = thresholdCount, importanceProbabilities = rep(thresholdCount / (graphSize * graphSize), graphSize * graphSize))
			if(i %% 100 == 0) save(results, file = file.path(results, fileName))
			cat(i, " ", sep = "")
		}
		save(results, file = file.path(results, fileName))
	}
} else if(method == "conditional")
{
	if(length(results) < 1000)
	{
		start <- length(results)+1
		for(i in start:1000)
		{
			results[[i]] <- conditionalMC(probabilities = vertexProbabilities, n = sampleSize, seed = i, graph = graph, componentSize = thresholdCount, importanceProbabilities = rep(thresholdCount / (graphSize * graphSize), graphSize * graphSize))
			if(i %% 100 == 0) save(results, file = file.path(results, fileName))
			cat(i, " ", sep = "")
		}
		save(results, file = file.path(results, fileName))
	}
} else
{
	stop("Invalid value for input method")
}
