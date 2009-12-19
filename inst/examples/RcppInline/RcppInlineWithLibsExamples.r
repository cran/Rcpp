#!/usr/bin/r

suppressMessages(library(Rcpp))

firstExample <- function() {
    ## a really simple C program calling three functions from the GSL
    gslrng <- '
    gsl_rng *r;
    gsl_rng_env_setup();
    double v;

    r = gsl_rng_alloc (gsl_rng_default);

    printf("generator type: %s\\n", gsl_rng_name (r));
    printf("seed = %lu\\n", gsl_rng_default_seed);
    v = gsl_rng_get (r);
    printf("first value = %f\\n", v);
    return R_NilValue;
    '

    ## turn into a function that R can call
    ## compileargs redundant on Debian/Ubuntu as gsl headers are found anyway
    funx <- cfunction(signature(ignored="numeric"), gslrng,
                      includes="#include <gsl/gsl_rng.h>",
                      Rcpp=FALSE,
                      cppargs="-I/usr/include",
                      libargs="-lgsl -lgslcblas")

    cat("Calling first example\n")
    funx(0)
    invisible(NULL)
}

secondExample <- function() {

    ## now use Rcpp to pass down a parameter for the seed
    gslrng <- '
    RcppVector<int> vec(par);
    int seed = vec(0);

    gsl_rng *r;
    gsl_rng_env_setup();
    double v;

    r = gsl_rng_alloc (gsl_rng_default);

    gsl_rng_set (r, (unsigned long) seed);
    v = gsl_rng_get (r);

    #ifndef BeSilent
    printf("generator type: %s\\n", gsl_rng_name (r));
    printf("seed = %d\\n", seed);
    printf("first value = %f\\n", v);
    #endif

    RcppResultSet rs;           // Build result set to be returned as a list to R.
    rs.add("value", v);         // vec as named element with name "vec"
    SEXP rl = rs.getReturnList();    // Get the list to be returned to R.

    return rl;
    '

    ## turn into a function that R can call
    ## compileargs redundant on Debian/Ubuntu as gsl headers are found anyway
    ## use additional define for compile to suppress output
    funx <- cfunction(signature(par="numeric"), gslrng,
                      includes="#include <gsl/gsl_rng.h>",
                      Rcpp=TRUE,
                      cppargs="-I/usr/include",
                      libargs="-lgsl -lgslcblas")
    cat("\n\nCalling second example without -DBeSilent set\n")
    print(funx(0)["value"])

    funx <- cfunction(signature(par="numeric"), gslrng,
                      includes="#include <gsl/gsl_rng.h>",
                      Rcpp=TRUE,
                      cppargs="-I/usr/include -DBeSilent",
                      libargs="-lgsl -lgslcblas")
    cat("\n\nCalling second example with -DBeSilent set\n")
    print(funx(1)["value"])

    invisible(NULL)
}

thirdExample <- function() {

    ## now use Rcpp to pass down a parameter for the seed
    gslrng <- '
    RcppVector<int> vec(par);
    int seed = vec(0);

    gsl_rng *r;
    gsl_rng_env_setup();
    double v;

    r = gsl_rng_alloc (gsl_rng_default);

    gsl_rng_set (r, (unsigned long) seed);
    v = gsl_rng_get (r);

    #ifndef BeSilent
    printf("generator type: %s\\n", gsl_rng_name (r));
    printf("seed = %d\\n", seed);
    printf("first value = %f\\n", v);
    #endif

    RcppResultSet rs;           // Build result set to be returned as a list to R.
    rs.add("value", v);         // vec as named element with name "vec"
    SEXP rl = rs.getReturnList();    // Get the list to be returned to R.

    return rl;
    '

    ## turn into a function that R can call
    ## compileargs redundant on Debian/Ubuntu as gsl headers are found anyway
    ## use additional define for compile to suppress output
    funx <- cfunction(signature(par="numeric"), gslrng,
                      includes="#include <gsl/gsl_rng.h>",
                      Rcpp=TRUE,
                      compileargs="-I/usr/include",
                      libargs="-lgsl -lgslcblas")
    cat("\n\nCalling second example without -DBeSilent set\n")
    print(funx(0)["value"])

    funx <- cfunction(signature(par="numeric"), gslrng,
                      includes="#include <gsl/gsl_rng.h>",
                      Rcpp=TRUE,
                      compileargs="-I/usr/include -DBeSilent",
                      libargs="-lgsl -lgslcblas")
    cat("\n\nCalling second example with -DBeSilent set\n")
    print(funx(1)["value"])

    invisible(NULL)
}

firstExample()
secondExample()
