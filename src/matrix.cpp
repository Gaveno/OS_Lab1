/*
 * matrix.cpp
 * created by: Gavin Atkin
 * for lab1
 * Operating Systems 3453-001
 */
 
  #include "matrix.h"
 
 #include <iostream>
 #include <sstream>
 #include <pthread.h>
 #include "exceptions.h"
 
 #define min(X, Y) ((X) < (Y) ? (X) : (Y))
 #define max(X, Y) ((X) > (Y) ? (X) : (Y))
 
 extern unsigned int TOTAL_THREADS;

 
 namespace Matrices {
     
    const unsigned int Matrix::MAX_ROWS = 10000;
    const unsigned int Matrix::MAX_COLS = 10000;
     
    // Construct without allocating array
    Matrix::Matrix( ) {
        __values = nullptr;
        __rows = 0;
        __cols = 0;
    }
    
    // Construct with allocating array with dimensions rows x cols
    // Throws DimensionInvalidEx
    Matrix::Matrix(unsigned int rows, unsigned int cols) {
        __values = nullptr;
        create(rows, cols);
    }
    
    // Destruct with deallocation
    Matrix::~Matrix( ) {
        if (__values != nullptr && __values != 0) {
            delete [] __values;
        }
    }
    
    void Matrix::create(unsigned int rows, unsigned int cols) {
        if (rows == 0 || cols == 0) throw DimensionInvalidEx(rows, cols);
        __rows = rows;
        __cols = cols;
        
        if (__values != nullptr && __values != 0) {
            //std::cout << "deleting array" << std::endl;
            delete [] __values;
        }
        __values = new datatype[rows * cols];
    }
    
    // set value x at row and col
    // throws DimensionOutOfBoundsEx
    void Matrix::setValue(datatype v, unsigned int row, unsigned int col) {
        if (row >= __rows || col >= __cols) throw DimensionOutOfBoundsEx(row, col, __rows, __cols);
        
        //__values[col*max(1, __mode * __rows) + row*max(1, abs(__mode - 1) *__cols)] = v;
        //__values[col + row*__cols] = v;
        __values[col*__rows + row] = v;
    }
    
    datatype Matrix::getValue(unsigned int row, unsigned int col) const {
        //return __values[col*max(1, __mode * __rows) + row*max(1, abs(__mode - 1) *__cols)];
        //return __values[col + row*__cols];
        return __values[col*__rows + row];
    }
    
    Matrix &Matrix::operator=(const Matrix &rhs) {
        // Don't set to rhs if rhs is non-initialized
        if (rhs.__rows == 0 || rhs.__cols == 0) return *this;
        
        create(rhs.__rows, rhs.__cols);
        //copy values
        for (unsigned int i = 0; i < __rows; i++) {
            for (unsigned int j = 0; j < __cols; j++) {
                __values[j + i*__cols] = rhs.__values[j + i*__cols];
            }
        }
        return *this;
    }
    
    // Matrix multiplication via the * operator.
    // Distributes workload to the threads.
    Matrix operator*(const Matrix &lhs, const Matrix &rhs) {
        if (lhs.__cols != rhs.__rows) throw DimensionMismatchEx(rhs.__rows, lhs.__cols);
        
        std::cout << "Matrix A: " << std::endl;
        if (lhs.__rows * lhs.__cols <= 100)
            std::cout << lhs;
        else
            std::cout << "Too large to print!\n";
        std::cout << "Matrix B: " << std::endl;
        if (rhs.__rows * rhs.__cols <= 100)
            std::cout << rhs;
        else
            std::cout << "Too large to print!\n";
        std::cout << std::endl;
        
        Matrix c(lhs.__rows, rhs.__cols);
        pthread_t threads[TOTAL_THREADS];
        for (unsigned int i = 0; i < min(TOTAL_THREADS, lhs.__rows); i++) {
            unsigned int rowsperthread = max(1, lhs.__rows / TOTAL_THREADS);
            ThreadArgs *args = new ThreadArgs(i, &lhs, &rhs, &c, i*rowsperthread, (i+1) * rowsperthread);
            if (i == min(TOTAL_THREADS, lhs.__rows) - 1) args->row_end = lhs.__rows;
            
            int err = pthread_create(&threads[i], NULL, rowMult, (void *)args);
            std::cout << "Created worker thread " << threads[i] << " for rows " << args->row_start << "-" << args->row_end << std::endl;
            
            if (err) {
                std::cout << "An error occured when creating a thread: " << err << std::endl;
            }
        }
        for (unsigned int i = 0; i < min(TOTAL_THREADS, lhs.__rows); i++) {
            int err = pthread_join(threads[i], NULL);
            if (err) {
                std::cout << "An error occured when joining a thread: " << err << std::endl;
            }
        }
        
        std::cout << "\nMatrix C: " << std::endl;
        if (c.__rows * c.__cols <= 100)
            std::cout << c;
        else
            std::cout << "Too large to print!\n";
        return c;
    }
    
    void *rowMult(void *threadargs) {
        ThreadArgs *myargs;
        myargs = (ThreadArgs *) threadargs;
        
        for (unsigned int i = myargs->row_start; i < myargs->row_end; i++) {
            for (unsigned int j = 0; j < myargs->B->__cols; j++) {
                datatype sum = 0;
                for (unsigned int k = 0; k < myargs->A->__cols; k++) {
                    sum += myargs->A->getValue(i, k) * myargs->B->getValue(k, j);
                }
                myargs->C->setValue(sum, i, j);
            }
        }
        
        delete myargs;
        pthread_exit(NULL);
    }
    
    // Print out matrix information
    std::ostream &operator<<(std::ostream &os, const Matrix &m) {
        for (unsigned int j = 0; j < m.__rows; j++) {
            for (unsigned int i = 0; i < m.__cols; i++) {
                os << m.getValue(j, i) << " ";
            }
            os << std::endl;
        }
        return os;
    }
    
    // Get data from input stream
    // Throws BadDataEx
    void loadMatricesFromFile(std::istream &in, Matrix &A, Matrix &B) {
        if (in.eof()) throw EndOfFileEx();
        
        // Handle Matrix A
        std::string line;
        std::getline(in, line);
        
        
        if (A.getValid(line) == FAIL || A.countNumbers(line) != 2) throw BadDataEx(line);
        
        datatype rows, cols;
        std::stringstream ss(line);
        ss >> rows;
        ss >> cols;
        
        A = Matrix(rows, cols);
        
        // Read in rows for Matrix A
        for (unsigned int i = 0; i < rows; i++) {
            if (in.eof()) throw EndOfFileEx();
            std::getline(in, line);
            
            if (A.getValid(line) == FAIL || A.countNumbers(line) != cols) throw BadDataEx(line);
            std::stringstream _ss(line);
            
            for (unsigned int j = 0; j < cols; j++) {
                datatype val;
                _ss >> val;
                A.setValue(val, i, j);
            }
        }
        
        // Handle Matrix B
        std::getline(in, line);
        
        if (A.getValid(line) == FAIL || A.countNumbers(line) != 2) throw BadDataEx(line);
        
        std::stringstream __ss(line);
        __ss >> rows;
        __ss >> cols;
        
        B = Matrix(rows, cols);
        
        // Read in rows for Matrix B
        for (unsigned int i = 0; i < rows; i++) {
            if (in.eof()) throw EndOfFileEx();
            std::getline(in, line);
            
            if (A.getValid(line) == FAIL || A.countNumbers(line) != cols) throw BadDataEx(line);
            std::stringstream _ss(line);
            
            for (unsigned int j = 0; j < cols; j++) {
                datatype val;
                _ss >> val;
                B.setValue(val, i, j);
            }
        }
    }
    
    bool Matrix::getValid(std::string str) {
        bool pass = true;
        for (unsigned int i = 0; i < str.length(); i++) {
            if (str[i] != 32 && str[i] != 13 && str[i] != '\n' && (str[i] < 48 || str[i] > 57)
                && str[i] != 45) {
                pass = false;
            }
        }
        return pass;
    }
    
    unsigned int Matrix::countNumbers(std::string str) {
        unsigned int num = 0;
        char previous = 32;
        for (unsigned int i = 0; i < str.length(); i++) {
            if (((str[i] >= 48 && str[i] <= 57) || str[i] == 45) && previous == 32) ++num;
            previous = str[i];
        }
        return num;
    }
     
 }