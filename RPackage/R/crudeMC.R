#' @export
crudeMC <- function(probabilities, n, seed, graph, componentSize, vertexPositions = matrix(data=vector(length=0),nrow=0))
{
	if(missing(graph))
	{
		stop("Input graph cannot be missing")
	}
	if(missing(probabilities))
	{
		stop("Input probabilities cannot be missing")
	}
	if(missing(n))
	{
		stop("Input n cannot be missing")
	}
	if(missing(seed))
	{
		stop("Input seed cannot be missing")
	}
	if(missing(componentSize))
	{
		stop("Input componentSize cannot be missing")
	}
	if(length(componentSize) != 1 || mode(componentSize) != "numeric")
	{
		stop("Input `componentSize' must be a single number")
	}
	if(abs(componentSize - round(componentSize)) > 1e-3)
	{
		stop("Input `componentSize' must be an integer")
	}
	if(length(n) != 1 || mode(n) != "numeric")
	{
		stop("Input `n' must be a single number")
	}
	if(abs(n - round(n)) > 1e-3)
	{
		stop("Input `n' must be an integer")
	}
	if(mode(probabilities) != "numeric")
	{
		stop("Input `probabilities' must be a numeric vector")
	}
	if(any(probabilities < 0 | probabilities > 1))
	{
		stop("Input `probabilities' must be between 0 and 1")
	}
	if(class(graph) %in% c("igraph", "graphNEL", "graphAM"))
	{
		start <- Sys.time()
		estimate <- .Call("crudeMC", graph, probabilities, n, seed, componentSize, vertexPositions, PACKAGE="largeComponent")
		end <- Sys.time()
	}
	else 
	{
		stop("Input graph must have class \"igraph\", \"graphAM\" or \"graphNEL\"")
	}
	call <- match.call()
	return(new("monteCarloResult", start = start, end = end, call = call, estimate = estimate))
}
