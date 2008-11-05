\name{Rcpp-package}
\alias{Rcpp-package}
\alias{Rcpp}
\docType{package}
\title{R / C++ interface}
\description{The \pkg{Rcpp} package provides C++ classes that
  greatly facilitate interfacing C or C++ code in \R packages using
  the \code{.Call} interface provided by \R.

  Detailed information is provided in the vignette \sQuote{RcppAPI} that
  can be invoked from \R via \code{vignette("RcppAPI")}.
}
\section{Overview}{
  \pkg{Rcpp} provides matching C++ classes for a large number of
  basic \R data types. Hence, a package author can keep his data in
  normal R data structure without having to worry about translation or
  transfer to C++. At the same time, the data structures can be accessed
  as easily at the C++ level, and used in the normal manner.

  The mapping of data types works in both directions. It is as
  straightforward to pass data from R to C++, as it is it return data
  from C++ to \R.  The following two sections list supported data types.
}
\section{Transfer from R to C++}{
  Standard \R datatypes that are understood are
  \enumerate{
    \item named lists containing numeric (i.e. floating point), integer,
    character, logical (i.e. boolean) or Date and Datetime
    (i.e. POSIXct) arguments;
    \item data frames containing numeric, integer, logical, character,
    Date, Datetime or Factor columns;
    \item named vectors containing numeric or integer values,
    \item vectors and matrices of different values
    \item character strings
  }
}
\section{Transfer from C++ to R}{
  Standard C++ datatypes can be returned to \R in a named list, the most
  general data type in \R.  Permissible components of the returns list
  are the following C++ types:
  \enumerate{
    \item double (scalar as well as vectors and vectors of vectors),
    \item int (scalar as well as vectors and vectors of vectors),
    \item string,
    \item STL vector types and vector<vector> types of int and double
    \item STL vector of strings
    \item internal Rcpp types RcppDate, RcppDateVector, RcppDatetime,
  RcppDatetimeVector, RcppStringVector, RcppVector of int or doube,
  RcppMatrix of int or double, RcppFrame}
}
\section{Usage for package building}{
  \pkg{Rcpp} provides a header file and a library inside the installed
  package in the directory \code{lib}. From within \R, you can compute
  the directory location via \code{system.file("lib", "Rcpp.h", 
    package="Rcpp")}. For Windows, it will be a static library
  \sQuote{Rcpp.a}. For Linux and Mac OS X, it will be \sQuote{libRcpp.so}.

  To use \code{Rcpp} in another package, you need to include the header
  during compilation. This typically requires use of the \code{-I} to
  provide the location as in \code{-I/usr/local/lib/R/site-library/Rcpp/lib}.
  Similarly, for linking we need to provide the location of the library
  via \code{-L} as well as the actual library. An example for Linux
  would be \code{-L/usr/local/lib/R/site-library/Rcpp/lib -lRcpp}.

  But that still requires telling the dynamic linker about the library
  locations -- whereas a link in, say, \code{/usr/local/lib} frees us
  from the \code{-L} switch and the need to tell the dynamic linker.
  Therefore, on Linux, I found it convenient to provide these files via
  copies or links from the usual locations like
  \sQuote{/usr/local/include} and \sQuote{/usr/local/lib} which
  alleviates the need for explicit location flags like \sQuote{-I} or
  \sQuote{-L}. Remember to also run \code{ldconfig} after creating the
  link for the library.  Alternatively, you can hardcode the dynamic 
  library location using the \code{rpath} linker directive:

  \preformatted{
  # We need to know where libRcpp.so and Rcpp.h reside:
  RCPP_DIR     = $(shell Rscript -e 'cat(system.file("lib", package="Rcpp"))')

  # pass header file diretory to R
  PKG_CPPFLAGS = -I$(RCPP_DIR)

  # pass dynamic library to R, and strip
  PKG_LIBS     = -L$(RCPP_DIR) -Wl,-rpath,$(RCPP_DIR) -lRcpp
  } 

  On  Windows, we simply include the static library with its full path
  as in \code{C:/R/library/Rcpp/lib/Rcpp.a} (provided one uses
  \code{R:/R/library} for add-on \R packages). This be automated in a file
  \code{src/Makevars.win} provided \code{Rscript} is in the path:

  \preformatted{
  # We need to know where Rcpp.a and Rcpp.h reside:
  RCPP_DIR     = $(shell Rscript -e 'cat(system.file("lib", package="Rcpp"))')

  # pass header file diretory to R
  PKG_CPPFLAGS = -I$(RCPP_DIR) -DBUILDING_DLL=1

  # pass static library to R, and strip
  PKG_LIBS     = $(RCPP_DIR)/Rcpp.a -s
  }
}
\author{Dominick Samperi wrote most of Rcpp during 2005 and 2006.  Dirk
  Eddelbuettel made some additions, and became maintainer in 2008.}
\keyword{programming}
\keyword{interface}
