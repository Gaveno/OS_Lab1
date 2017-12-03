#ifndef __MATRIX__
#define __MATRIX__

#include <iostream>

#define PASS true
#define FAIL false

namespace Matrices {

    typedef long long int datatype;
    //const unsigned int TOTAL_THREADS = 16;
    

    class Matrix {
    public:
        static const unsigned int MAX_ROWS;
        static const unsigned int MAX_COLS;
        
        // Construct without allocating array
        Matrix( );
        // Construct with allocating array with dimensions rows x cols
        // Throws DimensionInvalidEx
        Matrix(unsigned int rows, unsigned int cols);
        // Destruct with deallocation
        ~Matrix( );
        
        // Allocate array space
        // Throws DimensionInvalidEx
        void create(unsigned int rows, unsigned int cols);
        
        // set value x at row and col
        // throws DimensionOutOfBoundsEx
        void setValue(datatype v, unsigned int row, unsigned int col);
        
        datatype getValue(unsigned int row, unsigned int col) const;
        
        Matrix &operator=(const Matrix &rhs);
        //Matrix &operator*=(const Matrix &rhs);
        
        // Matrix multiplication
        friend Matrix operator*(const Matrix &lhs, const Matrix &rhs);
        friend void *rowMult(void *threadargs);
        
        // Load two matrices A, and B from a file stream
        // Throws BadDataEx, EndOfFileEx, as well as matrix creation exceptions
        friend void loadMatricesFromFile(std::istream &in, Matrix &A, Matrix &B);
        
        // Print the matrix's data
        friend std::ostream &operator<<(std::ostream &os, const Matrix &m);
        
        // Accessors
        unsigned int getRows( ) const { return __rows; };
        unsigned int getColumns( ) const { return __cols; };
        datatype *getValues( ) const { return __values; };
        
    protected:
        datatype *__values;
        unsigned int __rows, __cols;
        
        bool getValid(std::string str);
        unsigned int countNumbers(std::string str);
    };
    
    Matrix operator*(const Matrix &lhs, const Matrix &rhs);
    void loadMatricesFromFile(std::istream &in, Matrix &A, Matrix &B);
    void *rowMult(void *threadargs);
    
    struct ThreadArgs {
        ThreadArgs(unsigned int index,
                   const Matrix *mA, const Matrix *mB, Matrix *mC,
                   unsigned int rowstart, unsigned int rowend) {
            id = index;
            A = mA;
            B = mB;
            C = mC;
            row_start = rowstart;
            row_end = rowend;
        }
        unsigned int id;
        const Matrix *A;
        const Matrix *B;
        Matrix *C;
        unsigned int row_start;
        unsigned int row_end;
    };
}


#endif