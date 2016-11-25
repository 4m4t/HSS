#include <iostream>
#include <sstream>
#include "hexa.hpp"

using namespace std;

int main(int argc, char * argv[])
{
    if(argc != 3)
        return 1;
    istringstream ss(argv[1]);
    istringstream ss1(argv[2]);
    int arg1, arg2;
    ss >> arg1;
    ss1 >> arg2;
    //cout << "arg2:" << argv[2] << endl;
    cout << "HEXA" << endl;
    hexa hex1(arg1), hex2(arg2), hexDiv(0);
    cout << hex1 << ":" << hex2 << endl;
    hexDiv = hex1 * hex2;
    hexDiv.print();
    cout << hex1 << "*" << hex2  << "=" << hexDiv << endl;
    return 0;
}
