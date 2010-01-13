all: clean Rcpp-unitTests.html Rcpp-unitTests.pdf

clean:
	rm -fr Rcpp-unitTests.html
	rm -fr Rcpp-unitTests.pdf

Rcpp-unitTests.html: Rcpp-unitTests.R
	Rscript --default-packages="Rcpp,RUnit,utils,tools" Rcpp-unitTests.R

