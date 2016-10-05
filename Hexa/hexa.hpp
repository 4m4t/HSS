#pragma once
#include <iostream>
#include <string>

using namespace std;

class hexa{
    public:
        hexa(int numb = 0);
        hexa(hexa const& hex);
        ~hexa();
        static int htoi(char hex);
        static string itoh(int num);
        const void print();
        const string value();
        static int pow16(int i);
        template <typename T1> hexa hexRoot(T1 numb);
        friend bool operator< (const hexa& hex1, const hexa& hex2);
        friend hexa operator+ (const hexa& hex1, const hexa& hex2);
        friend hexa operator- (const hexa& hex1, const hexa& hex2);
        friend ostream& operator<< (ostream& out, const hexa& hex);
        hexa& operator= (const hexa& hex);
        friend bool operator== (const hexa& hex1, const hexa& hex2);
        friend bool operator!= (const hexa& hex1, const hexa& hex2);
        hexa& operator+= (const hexa& hex);
        
    private:
        string m_hexNumb;
        char sign;
        
        void format();
        string push_front(string source, string t);
        string push_back(string source, string t);

};
