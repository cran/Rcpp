#!/usr/bin/r -t
#
# Copyright (C) 2010	Dirk Eddelbuettel and Romain Francois
#
# This file is part of Rcpp.
#
# Rcpp is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Rcpp is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Rcpp.  If not, see <http://www.gnu.org/licenses/>.

.setUp <- function(){
    suppressMessages( require( inline ) )
}

test.RcppMatrix.int <- function() {
    src <- 'RcppMatrix<int> m(x);
            RcppResultSet rs;
            rs.add("dim1",  m.getDim1());
            rs.add("dim2",  m.getDim2());
            rs.add("rows",  m.rows());
            rs.add("cols",  m.cols());
            rs.add("p22",   m(1,1));
            std::vector<std::vector<int> > mm = m.stlMatrix();
            rs.add("m",     mm);
	    return rs.getReturnList();';
    funx <- cfunction(signature(x="numeric"), src, Rcpp=TRUE)
    M <- matrix(1:6,2,3,byrow=TRUE)
    checkEquals(funx(x=M), list(dim1=2, dim2=3, rows=2, cols=3, p22=5, m=M),
                     msg = "RcppMatrix.int")
}

test.RcppMatrix.double <- function() {
    src <- 'RcppMatrix<double> m(x);
            RcppResultSet rs;
            rs.add("dim1",  m.getDim1());
            rs.add("dim2",  m.getDim2());
            rs.add("rows",  m.rows());
            rs.add("cols",  m.cols());
            rs.add("p22",   m(1,1));
            std::vector<std::vector<double> > mm = m.stlMatrix();
            rs.add("m",     mm);
	    return rs.getReturnList();';
    funx <- cfunction(signature(x="numeric"), src, Rcpp=TRUE)
    M <- matrix(1:6,2,3,byrow=TRUE)
    checkEquals(funx(x=M), list(dim1=2, dim2=3, rows=2, cols=3, p22=5, m=M),
                     msg = "RcppMatrix.double")
}















