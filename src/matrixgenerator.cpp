/*
 * Program to build large matrix files
 */
 
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int matrixsize = 1000;

int main() {
    ofstream outfile;
    string fname;
    cout << "Enter name of file to generate (without extension):\n";
    getline(cin, fname);
    fname += ".txt";
    outfile.open(fname);
    
    // Matrix A
    outfile << matrixsize << " " << matrixsize << endl;
    for (int i = 0; i < matrixsize; i++) {
        for (int j = 0; j < matrixsize; j++) {
            outfile << i + j << " ";
        }
        outfile << endl;
    }
    
    // Matrix B
    outfile << matrixsize << " " << matrixsize << endl;
    for (int i = 0; i < matrixsize; i++) {
        for (int j = 0; j < matrixsize; j++) {
            outfile << i + j << " ";
        }
        outfile << endl;
    }
    outfile.close();
    return 0;
}