source("generateScenarios.R")

methods <- unique(scenarios[,"method"])
dir.create("results", showWarnings = FALSE)
jobsSubmitted <- 0
sapply(1:nrow(scenarios),
	function(index)
	{
		submit <- TRUE
		if(file.exists(scenarios[index, "fileName"]))
		{
			load(scenarios[index, "fileName"])
			submit <- length(results) != replications
		}
		if(submit)
		{
			subCommand <- paste0("sbatch -p defq --export SCENARIO_INDEX=", index, " --time=", scenarios[index, "time"], " --mem=", scenarios[index, "memory"], " SimulateSlurm")
			if(!exists("maxJobs") || jobsSubmitted < maxJobs)
			{
				jobsSubmitted <<- jobsSubmitted + 1
				cat(subCommand, "\n")
				system(subCommand)
			}
		}
	})
