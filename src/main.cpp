/*
 * main.cpp
 * created by: Gavin Atkin
 * for lab1
 * Operating Systems 3453-001
 */
 
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include "matrix.h"
#include "exceptions.h"

const unsigned int MAX_THREADS = 20;
unsigned int TOTAL_THREADS = 1;

int main( ) {
    Matrices::Matrix A, B, C;
    
    std::string fname = "";
    std::fstream infile;
    int error = 0;
    std::chrono::duration<double> totalTime;
    bool should_exit = false;
    
    for (unsigned int i = 1; (i <= MAX_THREADS && error == 0 && !should_exit); i++) {
        
        do {
            std::cout << "Next runthrough will have " << i << " thread(s).\n";
            std::cout << "Enter filename with data for two Matrices or 'exit' to quit: " << std::endl;
            std::getline(std::cin, fname);
            if (fname == "exit" || fname == "Exit" || fname == "EXIT") {
                should_exit = true;
            }
        
            infile.open(fname);
            if (!infile) {
                fname = "";
                std::cout << "\n\nError: Could not open file.\n\n";
            }
        }
        while (!infile && !should_exit);
        
        if (!should_exit) {
            try {
                Matrices::loadMatricesFromFile(infile, A, B);
                TOTAL_THREADS = i;
                std::chrono::high_resolution_clock::time_point timeStart = std::chrono::high_resolution_clock::now();
                C = A * B;
                std::chrono::high_resolution_clock::time_point timeEnd = std::chrono::high_resolution_clock::now();
                totalTime = std::chrono::duration_cast<std::chrono::duration<double>>(timeEnd - timeStart);
            } catch (Matrices::MatrixException &ex) {
                std::cerr << "Caught Exception: " << ex << std::endl;
                infile.close();
                error = 1000 + ex.getErrID();
            } catch (...) {
                std::cerr << "Unknown exception caught" << std::endl;
            }
        
        
        
            infile.close();
            
            if (!error) {
                std::cout << "\nMatrix multiplication took " << totalTime.count() * 1000 << " milliseconds ";
                std::cout << std::setprecision(8)<< "for " << TOTAL_THREADS << " threads\n";
            }
            else {
                std::cout << "Exiting with error code: " << error << std::endl;
            }
        }
    }
    
    return error;
}