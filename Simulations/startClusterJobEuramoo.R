source("generateScenarios.R")

methods <- unique(scenarios[,"method"])
dir.create("results", showWarnings = FALSE)
jobsSubmitted <- 0
sapply(1:nrow(scenarios),
	function(index)
	{
		submit <- TRUE
		if(file.exists(file.path("results", scenarios[index, "fileName"])))
		{
			load(file.path("results", scenarios[index, "fileName"]))
			submit <- length(results) != replications
		}
		if(submit)
		{
			if(!exists("maxJobs") || jobsSubmitted < maxJobs)
			{
				jobsSubmitted <<- jobsSubmitted + 1
				system2(command = "qsub", args = "SimulateEuramoo", env = paste0("SCENARIO_INDEX=", index), wait=TRUE)
			}
		}
	})
