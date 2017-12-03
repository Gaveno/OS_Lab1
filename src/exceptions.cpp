/*
 * exceptions.cpp
 * created by: Gavin Atkin
 * for lab1
 * Operating Systems 3453-001
 */
 
 #include "exceptions.h"

namespace Matrices {
    
    std::ostream &operator<<(std::ostream &os, const MatrixException &ex) {
        os << ex.getName() << ": ";
        ex.__print_args(os);
        os << std::endl;
        return os;
    }
    
    DimensionEx::DimensionEx(unsigned int rows, unsigned int cols) : __rows(rows), __cols(cols) {
        setName("DimensionEx");
        __errid = 1;
    }
    
    DimensionInvalidEx::DimensionInvalidEx(unsigned int rows, unsigned int cols) : DimensionEx(rows, cols) {
        setName("DimensionInvalidEx");
        __errid = 2;
    }
    
    DimensionMismatchEx::DimensionMismatchEx(unsigned int rows, unsigned int cols) : DimensionEx(rows, cols) {
        setName("DimensionMismatchEx");
        __errid = 3;
    }
    
    DimensionOutOfBoundsEx::DimensionOutOfBoundsEx(unsigned int rows, unsigned int cols, unsigned int maxrows, unsigned int maxcols) : DimensionEx(rows, cols) {
        setName("DimensionOutOfBoundsEx");
        __maxrows = maxrows;
        __maxcols = maxcols;
        __errid = 4;
    }
    
    BadDataEx::BadDataEx(std::string line) {
        __line = line;
        setName("BadDataEx");
        __errid = 5;
    }
    
    void BadDataEx::__print_args(std::ostream &os) const {
        os << "line: " << __line;
    }
    
    void DimensionEx::__print_args(std::ostream &os) const {
        os << "rows: " << __rows << " columns: " << __cols;
    }
    
    void DimensionOutOfBoundsEx::__print_args(std::ostream &os) const {
        os << "row: " << __rows << " column: " << __cols;
        os << " total rows: " << __maxrows << " total columns: " << __maxcols - 1;
    }
}