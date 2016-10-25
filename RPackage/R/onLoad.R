#' @export
.onLoad <- function(libname, pkgname)
{
	library.dynam(package="largeComponent", chname="largeComponent", lib.loc = .libPaths())
}
