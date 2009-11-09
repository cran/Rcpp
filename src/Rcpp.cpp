// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// Rcpp.cpp: R/C++ interface class library
//
// Copyright (C) 2005 - 2006 Dominick Samperi
// Copyright (C) 2008 - 2009 Dirk Eddelbuettel
//
// This library is free software; you can redistribute it and/or modify it 
// under the terms of the GNU Lesser General Public License as published by 
// the Free Software Foundation; either version 2.1 of the License, or (at 
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but 
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public 
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License 
// along with this library; if not, write to the Free Software Foundation, 
// Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 

#include <Rcpp.h>

RcppFrame::RcppFrame(SEXP df) {
    if (!Rf_isNewList(df))
	throw std::range_error("RcppFrame::RcppFrame: invalid data frame.");
    int ncol = Rf_length(df);
    SEXP names = Rf_getAttrib(df, R_NamesSymbol);
    colNames.resize(ncol);
    SEXP colData = VECTOR_ELT(df,0); // First column of data.
    int nrow = Rf_length(colData);
    if (nrow == 0)
	throw std::range_error("RcppFrame::RcppFrame: zero lenth column.");

    // Allocate storage for table.
    table.resize(nrow);
    for (int r = 0; r < nrow; r++)
	table[r].resize(ncol);
    
    for (int i=0; i < ncol; i++) {
	colNames[i] = std::string(CHAR(STRING_ELT(names,i)));
	SEXP colData = VECTOR_ELT(df,i);
	if (!Rf_isVector(colData) || Rf_length(colData) != nrow)
	    throw std::range_error("RcppFrame::RcppFrame: invalid column.");

	// Check for Date class. Currently R stores the date ordinal in a
	// real value. We check for Date under both Real and Integer values
	// as insurance against future changes.
	bool isDateClass = false;
	SEXP classname = Rf_getAttrib(colData, R_ClassSymbol);
	if (classname != R_NilValue)
	    isDateClass = (strcmp(CHAR(STRING_ELT(classname,0)),"Date") == 0);

	if (Rf_isReal(colData)) {
	    if (isDateClass) {
		for (int j=0; j < nrow; j++) // Column of Date's
		    table[j][i].setDateValue(RcppDate((int)REAL(colData)[j]));
	    }
	    else // Column of REAL's
		for (int j=0; j < nrow; j++)
		    table[j][i].setDoubleValue(REAL(colData)[j]);
	}
	else if (Rf_isInteger(colData)) {
	    if (isDateClass) {
		for (int j=0; j < nrow; j++) // Column of Date's
		    table[j][i].setDateValue(RcppDate(INTEGER(colData)[j]));
	    }
	    else
		for (int j=0; j < nrow; j++)
		    table[j][i].setIntValue(INTEGER(colData)[j]);
	}
	else if (Rf_isString(colData)) { // Non-factor string column
	    for (int j=0; j < nrow; j++)
		table[j][i].setStringValue(std::string(CHAR(STRING_ELT(colData,j))));
	}
	else if (Rf_isFactor(colData)) { // Factor column.
	    SEXP names = Rf_getAttrib(colData, R_LevelsSymbol);
	    int numLevels = Rf_length(names);
	    std::string *levelNames = new std::string[numLevels];
	    for (int k=0; k < numLevels; k++)
		levelNames[k] = std::string(CHAR(STRING_ELT(names,k)));
	    for (int j=0; j < nrow; j++)
		table[j][i].setFactorValue(levelNames, numLevels,
					   INTEGER(colData)[j]);
	    delete [] levelNames;
	}
	else if (Rf_isLogical(colData)) {
	    for (int j=0; j < nrow; j++) {
		table[j][i].setLogicalValue(INTEGER(colData)[j]);
	    }
	}
	else
	    throw std::range_error("RcppFrame::RcppFrame: unsupported data frame column type.");
    }
}

RcppDateVector::RcppDateVector(SEXP vec) {
    int i;
    if (!Rf_isNumeric(vec) || Rf_isMatrix(vec) || Rf_isLogical(vec))
	throw std::range_error("RcppDateVector: invalid numeric vector in constructor");
    int len = Rf_length(vec);
    if (len == 0)
	throw std::range_error("RcppDateVector: null vector in constructor");
    v = new RcppDate[len];
    for (i = 0; i < len; i++)
	v[i] = RcppDate((int)REAL(vec)[i]);
    length = len;
}

RcppDatetimeVector::RcppDatetimeVector(SEXP vec) {
    int i;
    if (!Rf_isNumeric(vec) || Rf_isMatrix(vec) || Rf_isLogical(vec))
	throw std::range_error("RcppDatetimeVector: invalid numeric vector in constructor");
    int len = Rf_length(vec);
    if (len == 0)
	throw std::range_error("RcppDatetimeVector: null vector in constructor");
    v = new RcppDatetime[len];
    for (i = 0; i < len; i++)
	v[i] = RcppDatetime(REAL(vec)[i]);
    length = len;
}

RcppStringVector::RcppStringVector(SEXP vec) {
    int i;
    if (Rf_isMatrix(vec) || Rf_isLogical(vec))
	throw std::range_error("RcppStringVector: invalid numeric vector in constructor");
    if (!Rf_isString(vec))
	throw std::range_error("RcppStringVector: invalid string");
    int len = Rf_length(vec);
    if (len == 0)
	throw std::range_error("RcppStringVector: null vector in constructor");
    v = new std::string[len];
    for (i = 0; i < len; i++)
	v[i] = std::string(CHAR(STRING_ELT(vec,i)));
    length = len;
}

template <typename T>
RcppVector<T>::RcppVector(SEXP vec) {
    int i;

    // The function Rf_isVector returns TRUE for vectors AND
    // matrices, so it does not distinguish. We could
    // check the dim attribute here to be sure that it
    // is not present (i.e., dimAttr == R_NilValue, not 0!).
    // But it is easier to simply check if it is set via
    // Rf_isMatrix (in which case we don't have a vector).
    if (!Rf_isNumeric(vec) || Rf_isMatrix(vec) || Rf_isLogical(vec))
	throw std::range_error("RcppVector: invalid numeric vector in constructor");
    len = Rf_length(vec);
    v = (T *)R_alloc(len, sizeof(T));
    if (Rf_isInteger(vec)) {
	for (i = 0; i < len; i++)
	    v[i] = (T)(INTEGER(vec)[i]);
    }	
    else if (Rf_isReal(vec)) {
	for (i = 0; i < len; i++)
	    v[i] = (T)(REAL(vec)[i]);
    }
}

template <typename T>
RcppVector<T>::RcppVector(int _len) {
    len = _len;
    v = (T *)R_alloc(len, sizeof(T));
    for (int i = 0; i < len; i++)
	v[i] = 0;
}

template <typename T>
T *RcppVector<T>::cVector() {
    T* tmp = (T *)R_alloc(len, sizeof(T));
    for (int i = 0; i < len; i++)
	tmp[i] = v[i];
    return tmp;
}

template <typename T>
std::vector<T> RcppVector<T>::stlVector() {
    std::vector<T> tmp(len);
    for (int i = 0; i < len; i++)
	tmp[i] = v[i];
    return tmp;
}

template <typename T>
RcppMatrix<T>::RcppMatrix(SEXP mat) {

    if (!Rf_isNumeric(mat) || !Rf_isMatrix(mat))
	throw std::range_error("RcppMatrix: invalid numeric matrix in constructor");

    // Get matrix dimensions
    SEXP dimAttr = Rf_getAttrib(mat, R_DimSymbol);
    dim1 = INTEGER(dimAttr)[0];
    dim2 = INTEGER(dimAttr)[1];

    // We guard against  the possibility that R might pass an integer matrix.
    // Can be prevented using R code: temp <- as.double(a), dim(temp) <- dim(a)
    int i,j;
    int isInt = Rf_isInteger(mat);
    T *m = (T *)R_alloc(dim1*dim2, sizeof(T));
    a = (T **)R_alloc(dim1, sizeof(T *));
    for (i = 0; i < dim1; i++)
	a[i] = m + i*dim2;
    if (isInt) {
	for (i=0; i < dim1; i++)
	    for (j=0; j < dim2; j++)
		a[i][j] = (T)(INTEGER(mat)[i+dim1*j]);
    }	
    else {
	for (i=0; i < dim1; i++)
	    for (j=0; j < dim2; j++)
		a[i][j] = (T)(REAL(mat)[i+dim1*j]);
    }	
}

template <typename T>
RcppMatrix<T>::RcppMatrix(int _dim1, int _dim2) {
    dim1 = _dim1;
    dim2 = _dim2;
    int i,j;
    T *m = (T *)R_alloc(dim1*dim2, sizeof(T));
    a = (T **)R_alloc(dim1, sizeof(T *));
    for (i = 0; i < dim1; i++)
	a[i] = m + i*dim2;
    for (i=0; i < dim1; i++)
	for (j=0; j < dim2; j++)
	    a[i][j] = 0;
}

template <typename T>
std::vector<std::vector<T> > RcppMatrix<T>::stlMatrix() {
    int i,j;
    std::vector<std::vector<T> > temp;
    for (i = 0; i < dim1; i++) {
	temp.push_back(std::vector<T>(dim2));
    }
    for (i = 0; i < dim1; i++)
	for (j = 0; j < dim2; j++)
	    temp[i][j] = a[i][j];
    return temp;
}

template <typename T>
T **RcppMatrix<T>::cMatrix() {
    int i,j;
    T *m = (T *)R_alloc(dim1*dim2, sizeof(T));
    T **tmp = (T **)R_alloc(dim1, sizeof(T *));
    for (i = 0; i < dim1; i++)
	tmp[i] = m + i*dim2;
    for (i=0; i < dim1; i++)
	for (j=0; j < dim2; j++)
	    tmp[i][j] = a[i][j];
    return tmp;
}

// Explicit instantiation (required for external linkage)
template class RcppVector<int>;
template class RcppVector<double>;
template class RcppMatrix<int>;
template class RcppMatrix<double>;

template <typename T>
RcppVectorView<T>::RcppVectorView(SEXP vec) {
    if (!Rf_isNumeric(vec) || Rf_isMatrix(vec) || Rf_isLogical(vec))
	throw std::range_error("RcppVectorView: invalid numeric vector in constructor");
    len = Rf_length(vec);
    if (Rf_isInteger(vec)) v = (T *)(INTEGER(vec));
    else if (Rf_isReal(vec)) v = (T *)(REAL(vec));
}

template class RcppVectorView<int>;
template class RcppVectorView<double>;

template <typename T>
RcppMatrixView<T>::RcppMatrixView(SEXP mat) {
    if (!Rf_isNumeric(mat) || !Rf_isMatrix(mat))
	throw std::range_error("RcppMatrixView: invalid numeric matrix in constructor");
    // Get matrix dimensions
    SEXP dimAttr = Rf_getAttrib(mat, R_DimSymbol);
    d1 = INTEGER(dimAttr)[0];
    d2 = INTEGER(dimAttr)[1];
    if (Rf_isInteger(mat)) a = (T *)(INTEGER(mat));
    else if (Rf_isReal(mat)) a = (T *)(REAL(mat));
}

template class RcppMatrixView<int>;
template class RcppMatrixView<double>;

RcppStringVectorView::RcppStringVectorView(SEXP vec) {
    //int i;
    if (Rf_isMatrix(vec) || Rf_isLogical(vec))
	throw std::range_error("RcppStringVectorView: invalid numeric vector in constructor");
    if (!Rf_isString(vec))
	throw std::range_error("RcppStringVectorView: invalid string");
    int len = Rf_length(vec);
    if (len == 0)
	throw std::range_error("RcppStringVectorView: null vector in constructor");
    //v = new (char *)[len];
    //for (i = 0; i < len; i++)
    //	v[i] = string(CHAR(STRING_ELT(vec,i)));
    length = len;
    v = vec;
}


void RcppResultSet::add(std::string name, RcppDate& date) {
    SEXP value = PROTECT(Rf_allocVector(REALSXP, 1));
    numProtected++;
    REAL(value)[0] = date.getJDN() - RcppDate::Jan1970Offset;
    SEXP dateclass = PROTECT(Rf_allocVector(STRSXP,1));
    numProtected++;
    SET_STRING_ELT(dateclass, 0, Rf_mkChar("Date"));
    Rf_setAttrib(value, R_ClassSymbol, dateclass); 
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, RcppDatetime& datetime) {
    SEXP value = PROTECT(Rf_allocVector(REALSXP, 1));
    numProtected++;
    REAL(value)[0] = datetime.getFractionalTimestamp();
    SEXP datetimeclass = PROTECT(Rf_allocVector(STRSXP,2));
    numProtected++;
    SET_STRING_ELT(datetimeclass, 0, Rf_mkChar("POSIXt"));
    SET_STRING_ELT(datetimeclass, 1, Rf_mkChar("POSIXct"));
    Rf_setAttrib(value, R_ClassSymbol, datetimeclass); 
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, double x) {
    SEXP value = PROTECT(Rf_allocVector(REALSXP, 1));
    numProtected++;
    REAL(value)[0] = x;
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, int i) {
    SEXP value = PROTECT(Rf_allocVector(INTSXP, 1));
    numProtected++;
    INTEGER(value)[0] = i;
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, std::string strvalue) {
    SEXP value = PROTECT(Rf_allocVector(STRSXP, 1));
    numProtected++;
    SET_STRING_ELT(value, 0, Rf_mkChar(strvalue.c_str()));
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, double *vec, int len) {
    if (vec == 0)
	throw std::range_error("RcppResultSet::add: NULL double vector");
    SEXP value = PROTECT(Rf_allocVector(REALSXP, len));
    numProtected++;
    for (int i = 0; i < len; i++)
	REAL(value)[i] = vec[i];
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, RcppDateVector& datevec) {
    SEXP value = PROTECT(Rf_allocVector(REALSXP, datevec.size()));
    numProtected++;
    for (int i = 0; i < datevec.size(); i++) {
	REAL(value)[i] = datevec(i).getJDN() - RcppDate::Jan1970Offset;
    }
    SEXP dateclass = PROTECT(Rf_allocVector(STRSXP,1));
    numProtected++;
    SET_STRING_ELT(dateclass, 0, Rf_mkChar("Date"));
    Rf_setAttrib(value, R_ClassSymbol, dateclass); 
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, RcppDatetimeVector &dtvec) {
    SEXP value = PROTECT(Rf_allocVector(REALSXP, dtvec.size()));
    numProtected++;
    for (int i = 0; i < dtvec.size(); i++) {
	REAL(value)[i] = dtvec(i).getFractionalTimestamp();
    }
    SEXP datetimeclass = PROTECT(Rf_allocVector(STRSXP,2));
    numProtected++;
    SET_STRING_ELT(datetimeclass, 0, Rf_mkChar("POSIXt"));
    SET_STRING_ELT(datetimeclass, 1, Rf_mkChar("POSIXct"));
    Rf_setAttrib(value, R_ClassSymbol, datetimeclass); 
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, RcppStringVector& stringvec) {
    int len = (int)stringvec.size();
    SEXP value = PROTECT(Rf_allocVector(STRSXP, len));
    numProtected++;
    for (int i = 0; i < len; i++)
        SET_STRING_ELT(value, i, Rf_mkChar(stringvec(i).c_str()));
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, int *vec, int len) {
    if (vec == 0)
	throw std::range_error("RcppResultSet::add: NULL int vector");
    SEXP value = PROTECT(Rf_allocVector(INTSXP, len));
    numProtected++;
    for (int i = 0; i < len; i++)
	INTEGER(value)[i] = vec[i];
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, double **mat, int nx, int ny) {
    if (mat == 0)
	throw std::range_error("RcppResultSet::add: NULL double matrix");
    SEXP value = PROTECT(Rf_allocMatrix(REALSXP, nx, ny));
    numProtected++;
    for (int i = 0; i < nx; i++)
	for (int j = 0; j < ny; j++)
	    REAL(value)[i + nx*j] = mat[i][j];
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, int **mat, int nx, int ny) {
    if (mat == 0)
	throw std::range_error("RcppResultSet::add: NULL int matrix");
    SEXP value = PROTECT(Rf_allocMatrix(INTSXP, nx, ny));
    numProtected++;
    for (int i = 0; i < nx; i++)
	for (int j = 0; j < ny; j++)
	    INTEGER(value)[i + nx*j] = mat[i][j];
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, std::vector<std::string>& vec) {
    if (vec.size() == 0)
	throw std::range_error("RcppResultSet::add; zero length vector<string>");
    int len = (int)vec.size();
    SEXP value = PROTECT(Rf_allocVector(STRSXP, len));
    numProtected++;
    for (int i = 0; i < len; i++)
        SET_STRING_ELT(value, i, Rf_mkChar(vec[i].c_str()));
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, std::vector<int>& vec) {
    if (vec.size() == 0)
	throw std::range_error("RcppResultSet::add; zero length vector<int>");
    int len = (int)vec.size();
    SEXP value = PROTECT(Rf_allocVector(INTSXP, len));
    numProtected++;
    for (int i = 0; i < len; i++)
	INTEGER(value)[i] = vec[i];
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, std::vector<double>& vec) {
    if (vec.size() == 0)
	throw std::range_error("RcppResultSet::add; zero length vector<double>");
    int len = (int)vec.size();
    SEXP value = PROTECT(Rf_allocVector(REALSXP, len));
    numProtected++;
    for (int i = 0; i < len; i++)
	REAL(value)[i] = vec[i];
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, std::vector<std::vector<int> >& mat) {
    if (mat.size() == 0)
	throw std::range_error("RcppResultSet::add: zero length vector<vector<int> >");
    else if (mat[0].size() == 0)
	throw std::range_error("RcppResultSet::add: no columns in vector<vector<int> >");
    int nx = (int)mat.size();
    int ny = (int)mat[0].size();
    SEXP value = PROTECT(Rf_allocMatrix(INTSXP, nx, ny));
    numProtected++;
    for (int i = 0; i < nx; i++)
	for (int j = 0; j < ny; j++)
	    INTEGER(value)[i + nx*j] = mat[i][j];
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, std::vector<std::vector<double> >& mat) {
    if (mat.size() == 0)
	throw std::range_error("RcppResultSet::add: zero length vector<vector<double> >");
    else if (mat[0].size() == 0)
	throw std::range_error("RcppResultSet::add: no columns in vector<vector<double> >");
    int nx = (int)mat.size();
    int ny = (int)mat[0].size();
    SEXP value = PROTECT(Rf_allocMatrix(REALSXP, nx, ny));
    numProtected++;
    for (int i = 0; i < nx; i++)
	for (int j = 0; j < ny; j++)
	    REAL(value)[i + nx*j] = mat[i][j];
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, RcppVector<int>& vec) {
    int len = vec.size();
    int *a = vec.cVector();
    SEXP value = PROTECT(Rf_allocVector(INTSXP, len));
    numProtected++;
    for (int i = 0; i < len; i++)
	INTEGER(value)[i] = a[i];
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, RcppVector<double>& vec) {
    int len = vec.size();
    double *a = vec.cVector();
    SEXP value = PROTECT(Rf_allocVector(REALSXP, len));
    numProtected++;
    for (int i = 0; i < len; i++)
	REAL(value)[i] = a[i];
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, RcppMatrix<int>& mat) {
    int nx = mat.getDim1();
    int ny = mat.getDim2();
    int **a = mat.cMatrix();
    SEXP value = PROTECT(Rf_allocMatrix(INTSXP, nx, ny));
    numProtected++;
    for (int i = 0; i < nx; i++)
	for (int j = 0; j < ny; j++)
	    INTEGER(value)[i + nx*j] = a[i][j];
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, RcppMatrix<double>& mat) {
    int nx = mat.getDim1();
    int ny = mat.getDim2();
    double **a = mat.cMatrix();
    SEXP value = PROTECT(Rf_allocMatrix(REALSXP, nx, ny));
    numProtected++;
    for (int i = 0; i < nx; i++)
	for (int j = 0; j < ny; j++)
	    REAL(value)[i + nx*j] = a[i][j];
    values.push_back(make_pair(name, value));
}

void RcppResultSet::add(std::string name, RcppFrame& frame) {
    std::vector<std::string> colNames = frame.getColNames();
    std::vector<std::vector<ColDatum> > table = frame.getTableData();
    int ncol = colNames.size();
    int nrow = table.size();
    SEXP rl = PROTECT(Rf_allocVector(VECSXP,ncol));
    SEXP nm = PROTECT(Rf_allocVector(STRSXP,ncol));
    numProtected += 2;
    for (int i=0; i < ncol; i++) {
	SEXP value, names;
	if (table[0][i].getType() == COLTYPE_DOUBLE) {
	    value = PROTECT(Rf_allocVector(REALSXP,nrow));
	    numProtected++;
	    for (int j=0; j < nrow; j++)
		REAL(value)[j] = table[j][i].getDoubleValue();
	} else if (table[0][i].getType() == COLTYPE_INT) {
	    value = PROTECT(Rf_allocVector(INTSXP,nrow));
	    numProtected++;
	    for (int j=0; j < nrow; j++)
		INTEGER(value)[j] = table[j][i].getIntValue();
	} else if (table[0][i].getType() == COLTYPE_FACTOR) {
	    value = PROTECT(Rf_allocVector(INTSXP,nrow));
	    numProtected++;
	    int levels = table[0][i].getFactorNumLevels();
	    names = PROTECT(Rf_allocVector(STRSXP,levels));
	    numProtected++;
	    std::string *levelNames = table[0][i].getFactorLevelNames();
	    for (int k=0; k < levels; k++)
		SET_STRING_ELT(names, k, Rf_mkChar(levelNames[k].c_str()));
	    for (int j=0; j < nrow; j++) {
		int level = table[j][i].getFactorLevel();
		INTEGER(value)[j] = level;
	    }
	    Rf_setAttrib(value, R_LevelsSymbol, names);
	    SEXP factorclass = PROTECT(Rf_allocVector(STRSXP,1));
	    numProtected++;
	    SET_STRING_ELT(factorclass, 0, Rf_mkChar("factor"));
	    Rf_setAttrib(value, R_ClassSymbol, factorclass); 
	} else if (table[0][i].getType() == COLTYPE_STRING) {
	    value = PROTECT(Rf_allocVector(STRSXP,nrow));
	    numProtected++;
	    for (int j=0; j < nrow; j++) {
		SET_STRING_ELT(value, j, Rf_mkChar(table[j][i].getStringValue().c_str()));
	    }
	} else if (table[0][i].getType() == COLTYPE_LOGICAL) {
	    value = PROTECT(Rf_allocVector(LGLSXP,nrow));
	    numProtected++;
	    for (int j=0; j < nrow; j++) {
		LOGICAL(value)[j] = table[j][i].getLogicalValue();
	    }
	} else if (table[0][i].getType() == COLTYPE_DATE) {
	    value = PROTECT(Rf_allocVector(REALSXP,nrow));
	    numProtected++;
	    for (int j=0; j < nrow; j++)
		REAL(value)[j] = table[j][i].getDateRCode();
	    SEXP dateclass = PROTECT(Rf_allocVector(STRSXP,1));
	    numProtected++;
	    SET_STRING_ELT(dateclass, 0, Rf_mkChar("Date"));
	    Rf_setAttrib(value, R_ClassSymbol, dateclass); 
	} else if (table[0][i].getType() == COLTYPE_DATETIME) {
	    value = PROTECT(Rf_allocVector(REALSXP,nrow));
	    numProtected++;
	    for (int j=0; j < nrow; j++) {
		// we could access the seconds as the internal double via getDouble but it's
		// more proper to use the proper accessor (and if we ever added code ...)
		REAL(value)[j] = table[j][i].getDatetimeValue().getFractionalTimestamp();
	    }
	    SEXP dateclass = PROTECT(Rf_allocVector(STRSXP,2));
	    numProtected++;
	    SET_STRING_ELT(dateclass, 0, Rf_mkChar("POSIXt"));
	    SET_STRING_ELT(dateclass, 1, Rf_mkChar("POSIXct"));
	    Rf_setAttrib(value, R_ClassSymbol, dateclass); 
	} else {
	    throw std::range_error("RcppResultSet::add invalid column type");
	}
	SET_VECTOR_ELT(rl, i, value);
	SET_STRING_ELT(nm, i, Rf_mkChar(colNames[i].c_str()));
    }
    Rf_setAttrib(rl, R_NamesSymbol, nm);
    values.push_back(make_pair(name, rl));
}

void RcppResultSet::add(std::string name, RcppList &list) {
    // we let RcppList export itself as a SEXP and send it along
    values.push_back(make_pair(name, list.getList()));
}

void RcppResultSet::add(std::string name, SEXP sexp, bool isProtected) {
    values.push_back(make_pair(name, sexp));
    if (isProtected)
	numProtected++;
}

SEXP RcppResultSet::getReturnList() {
    int nret = (int)values.size();
    SEXP rl = PROTECT(Rf_allocVector(VECSXP,nret));
    SEXP nm = PROTECT(Rf_allocVector(STRSXP,nret));
    std::list<std::pair<std::string,SEXP> >::iterator iter = values.begin();
    for (int i = 0; iter != values.end(); iter++, i++) {
	SET_VECTOR_ELT(rl, i, iter->second);
	SET_STRING_ELT(nm, i, Rf_mkChar(iter->first.c_str()));
    }
    Rf_setAttrib(rl, R_NamesSymbol, nm);
    UNPROTECT(numProtected+2);
    return rl;
}

SEXP RcppFunction::listCall() {
    if (names.size() != (unsigned)listSize)
	throw std::range_error("RcppFunction::listCall: no. of names != no. of items");
    if (currListPosn != listSize)
	throw std::range_error("RcppFunction::listCall: list has incorrect size");
    SEXP nm = PROTECT(Rf_allocVector(STRSXP,listSize));
    numProtected++;
    for (int i=0; i < listSize; i++)
	SET_STRING_ELT(nm, i, Rf_mkChar(names[i].c_str()));
    Rf_setAttrib(listArg, R_NamesSymbol, nm);
    SEXP R_fcall;
    PROTECT(R_fcall = Rf_lang2(fn, R_NilValue));
    numProtected++;
    SETCADR(R_fcall, listArg);
    SEXP result = Rf_eval(R_fcall, R_NilValue);
    names.clear();
    listSize = currListPosn = 0; // Ready for next call.
    return result;
}

SEXP RcppFunction::vectorCall() {
    if (vectorArg == R_NilValue)
	throw std::range_error("RcppFunction::vectorCall: vector has not been set");
    SEXP R_fcall;
    PROTECT(R_fcall = Rf_lang2(fn, R_NilValue));
    numProtected++;
    SETCADR(R_fcall, vectorArg);
    SEXP result = Rf_eval(R_fcall, R_NilValue);
    vectorArg = R_NilValue; // Ready for next call.
    return result;
}

void RcppFunction::setRVector(std::vector<double>& v) {
    vectorArg = PROTECT(Rf_allocVector(REALSXP,v.size()));
    numProtected++;
    for (int i=0; i < (int)v.size(); i++)
	REAL(vectorArg)[i] = v[i];
}

void RcppFunction::setRListSize(int n) {
    listSize = n;
    listArg = PROTECT(Rf_allocVector(VECSXP, n));
    numProtected++;
}

void RcppFunction::appendToRList(std::string name, double value) {
    if (currListPosn < 0 || currListPosn >= listSize)
	throw std::range_error("RcppFunction::appendToRList(double): list posn out of range");
    SEXP valsxp = PROTECT(Rf_allocVector(REALSXP,1));
    numProtected++;
    REAL(valsxp)[0] = value;
    SET_VECTOR_ELT(listArg, currListPosn++, valsxp);
    names.push_back(name);
}

void RcppFunction::appendToRList(std::string name, int value) {
    if (currListPosn < 0 || currListPosn >= listSize)
	throw std::range_error("RcppFunction::appendToRlist(int): posn out of range");
    SEXP valsxp = PROTECT(Rf_allocVector(INTSXP,1));
    numProtected++;
    INTEGER(valsxp)[0] = value;
    SET_VECTOR_ELT(listArg, currListPosn++, valsxp);
    names.push_back(name);
}

void RcppFunction::appendToRList(std::string name, std::string value) {
    if (currListPosn < 0 || currListPosn >= listSize)
	throw std::range_error("RcppFunction::appendToRlist(string): posn out of range");
    SEXP valsxp = PROTECT(Rf_allocVector(STRSXP,1));
    numProtected++;
    SET_STRING_ELT(valsxp, 0, Rf_mkChar(value.c_str()));
    SET_VECTOR_ELT(listArg, currListPosn++, valsxp);
    names.push_back(name);
}

void RcppFunction::appendToRList(std::string name, RcppDate& date) {
    if (currListPosn < 0 || currListPosn >= listSize)
	throw std::range_error("RcppFunction::appendToRlist(RcppDate): list posn out of range");
    SEXP valsxp = PROTECT(Rf_allocVector(REALSXP,1));
    numProtected++;
    REAL(valsxp)[0] = date.getJDN() - RcppDate::Jan1970Offset;
    SEXP dateclass = PROTECT(Rf_allocVector(STRSXP, 1));
    numProtected++;
    SET_STRING_ELT(dateclass, 0, Rf_mkChar("Date"));
    Rf_setAttrib(valsxp, R_ClassSymbol, dateclass);
    SET_VECTOR_ELT(listArg, currListPosn++, valsxp);
    names.push_back(name);
}

void RcppFunction::appendToRList(std::string name, RcppDatetime& datetime) {
    if (currListPosn < 0 || currListPosn >= listSize)
	throw std::range_error("RcppFunction::appendToRlist(RcppDatetime): list posn out of range");
    SEXP valsxp = PROTECT(Rf_allocVector(REALSXP,1));
    numProtected++;
    REAL(valsxp)[0] = datetime.getFractionalTimestamp();
    SEXP datetimeclass = PROTECT(Rf_allocVector(STRSXP, 2));
    numProtected++;
    SET_STRING_ELT(datetimeclass, 0, Rf_mkChar("POSIXt"));
    SET_STRING_ELT(datetimeclass, 1, Rf_mkChar("POSIXct"));
    Rf_setAttrib(valsxp, R_ClassSymbol, datetimeclass);
    SET_VECTOR_ELT(listArg, currListPosn++, valsxp);
    names.push_back(name);
}

#include <cstring>

// Paul Roebuck has observed that the memory used by an exception message
// is not reclaimed if error() is called inside of a catch block (due to
// a setjmp() call), and he suggested the following work-around.
char *copyMessageToR(const char* const mesg) {
    char* Rmesg;
    const char* prefix = "Exception: ";
    void* Rheap = R_alloc(strlen(prefix)+strlen(mesg)+1,sizeof(char));
    Rmesg = static_cast<char*>(Rheap);
    strcpy(Rmesg, prefix);
    strcat(Rmesg, mesg);
    return Rmesg;
}

