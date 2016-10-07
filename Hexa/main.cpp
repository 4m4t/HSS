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
    hexa hex1(arg1), hex2(arg2);
    hex1.print();
    hex2.print();
    hexa hex3 = hex1*hex2;
    hex3.print();
    hexa hex4(24426),hex5(390816);
    hex4+=hex5;
    cout << hex4 << ":" << hex3 << endl;
    return 0;
}
