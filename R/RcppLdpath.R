
## Use R's internal knowledge of path settings to find the lib/ directory
## plus optinally an arch-specific directory on system building multi-arch
RcppLdPath <- function() {
    if (nzchar(.Platform$r_arch)) {	## eg amd64, ia64, mips
        system.file("lib",.Platform$r_arch,package="Rcpp")
    } else {
        system.file("lib",package="Rcpp")
    }
}

## Provide linker flags -- i.e. -L/path/to/libRcpp -- as well as an
## optional rpath call needed to tell the Linux dynamic linker about the
## location.  This is not needed on OS X where we encode this as library
## built time (see src/Makevars) or Windows where we use a static library
RcppLdFlags <- function(static=FALSE) {
    rcppdir <- RcppLdPath()
    flags <- paste("-L", rcppdir, " -lRcpp", sep="")	## general default
    if (.Platform$OS.type == "unix") {
        if (length(grep("^linux",R.version$os))) {
            if (static==FALSE) {		## on Linux with dyn. linking, use rpath too
                flags <- paste("-lRcpp -Wl,-rpath,", rcppdir, sep="")
            }
        }
    }
    invisible(flags)
}

## Provide compiler flags -- i.e. -I/path/to/Rcpp.h
RcppCxxFlags <- function() {
    paste("-I", RcppLdPath(), sep="")
}
