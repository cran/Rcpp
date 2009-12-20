#!/usr/bin/r -t

suppressMessages(library(Rcpp))

cat("===Doubles\n")
foo <- '
        double d = RcppSexp(x).asDouble();
	std::cout << "Returning twice the value of " << d << " : ";
	return(RcppSexp( 2*d ).asSexp());
        '
funx <- cfunction(signature(x="numeric"), foo, Rcpp=TRUE, verbose=FALSE)
cat(funx(x=2.123), "\n")
cat(funx(x=2), "\n")
##funx(x='2')  ## throws as expected
##funx(x=2:3)  ## throws as expected


cat("\n===Int\n")
foo <- '
        int i = RcppSexp(x).asInt();
	std::cout << "Returning twice the value of " << i << " : ";
	return(RcppSexp( 2*i ).asSexp());
        '
funx <- cfunction(signature(x="numeric"), foo, Rcpp=TRUE, verbose=FALSE)
cat(funx(x=2), "\n")
cat(funx(x=2.2), "\n")

cat("\n===String\n")
foo <- '
        std::string s = RcppSexp(x).asStdString();
	std::cout << "Returning twice the value of " << s << " : ";
	return(RcppSexp( s+s ).asSexp());
        '
funx <- cfunction(signature(x="character"), foo, Rcpp=TRUE, verbose=FALSE)
cat(funx(x="abc"), "\n")

cat("\n===Int Vector via RcppResultSet.getSEXP\n")
foo <- '
        std::vector<int> iv = RcppSexp(x).asStdVectorInt();
	std::cout << "Returning twice the value of vector : ";
        for (size_t i=0; i<iv.size(); i++) {
            iv[i] = 2*iv[i];
        }
        RcppResultSet rs;
        rs.add("", iv);
        return(rs.getSEXP());
        '
funx <- cfunction(signature(x="numeric"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=2:5))

cat("\n===Int Vector\n")
foo <- '
        std::vector<int> iv = RcppSexp(x).asStdVectorInt();
	std::cout << "Returning twice the value of vector : ";
        for (size_t i=0; i<iv.size(); i++) {
            iv[i] = 2*iv[i];
        }
	return(RcppSexp( iv ).asSexp());
        '
funx <- cfunction(signature(x="numeric"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=2:5))


cat("\n===Double Vector\n")
foo <- '
        std::vector<double> iv = RcppSexp(x).asStdVectorDouble();
	std::cout << "Returning twice the value of vector : ";
        for (size_t i=0; i<iv.size(); i++) {
            iv[i] = 2*iv[i];
        }
 	return(RcppSexp( iv ).asSexp());
        '
funx <- cfunction(signature(x="numeric"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=0.1+2:5))


cat("\n===String Vector\n")
foo <- '
        std::vector<std::string> iv = RcppSexp(x).asStdVectorString();
	std::cout << "Returning twice the value of vector : ";
        for (size_t i=0; i<iv.size(); i++) {
            iv[i] = iv[i] + iv[i];
        }
 	return(RcppSexp( iv ).asSexp());
        '
funx <- cfunction(signature(x="character"), foo, Rcpp=TRUE, verbose=FALSE)
print(funx(x=c("foo", "bar")))
