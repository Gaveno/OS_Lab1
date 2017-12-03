#ifndef __EXCEPTIONS__
#define __EXCEPTIONS__

#include <iostream>

namespace Matrices {
    
    class MatrixException {
    public:
        std::string getName( ) const { return __name; };
        int getErrID( ) const { return __errid; };
        friend std::ostream &operator<<(std::ostream &os, const MatrixException &ex);

    protected:
        std::string __name;
        int __errid;
        void setName(std::string name) { __name = name; };
        virtual void __print_args(std::ostream &os) const = 0;
    };
    
    class DimensionEx : public MatrixException {
    public:
        DimensionEx(unsigned int rows, unsigned int cols);
        
    protected:
        unsigned int __rows, __cols;
        void __print_args(std::ostream &os) const override;
    };
    
    class DimensionMismatchEx : public DimensionEx {
    public:
        DimensionMismatchEx(unsigned int rows, unsigned int cols);
    /*protected:
        void __print_args(std::ostream &os) const override;*/
    };
    
    class DimensionOutOfBoundsEx : public DimensionEx {
    public:
        DimensionOutOfBoundsEx(unsigned int rows, unsigned int cols, unsigned int maxrows, unsigned int maxcols);
    protected:
        unsigned int __maxrows, __maxcols;
        void __print_args(std::ostream &os) const override;
    };
    
    class DimensionInvalidEx : public DimensionEx {
        public:
            DimensionInvalidEx(unsigned int rows, unsigned int cols);
    };
    
    class BadDataEx : public MatrixException {
    public:
        BadDataEx(std::string line);
    protected:
        void __print_args(std::ostream &os) const override;
        std::string __line;
    };
    
    class EndOfFileEx : public MatrixException {
    public:
        EndOfFileEx() {
            setName("EndOfFileEx");
            __errid = 6;
        };
    protected:
        void __print_args(std::ostream &os) const override {
            os << "pemature end of file reached. Corrupt data file.";
        }
    };
    
}

#endif