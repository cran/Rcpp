### R code from vignette source 'Rcpp-unitTests.Rnw'

###################################################
### code chunk number 1: Rcpp-unitTests.Rnw:9-12
###################################################
require( Rcpp )
prettyVersion <- packageDescription("Rcpp")$Version
prettyDate <- format(Sys.Date(), "%B %e, %Y")


###################################################
### code chunk number 2: Rcpp-unitTests.Rnw:24-30
###################################################
results <- "unitTests-results/Rcpp-unitTests.txt"
if( file.exists( results ) ){
	writeLines( readLines( results ) )
} else{
	writeLines( "unit test results not available" )
}


