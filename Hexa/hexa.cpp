#include "hexa.hpp"

hexa::hexa(int numb)
{
    m_sign = '\0';
    if(numb < 0)
    {
        numb *= -1;
        m_sign = '-';
    }
    
    m_hexNumb = itoh(numb);
}

hexa::hexa(hexa const& hexa):m_hexNumb(hexa.m_hexNumb), m_sign(hexa.m_sign)
{
    //cout << "cpy_hex:" << m_sign << m_hexNumb << endl;;
}

hexa::~hexa()
{
}

int hexa::pow16(int i)
{
    int pow = 1;
    for(int k = 0; k < i; k++)
        pow *= 16;
    return pow;
}

const void hexa::print()
{   
    cout << m_sign << "0x" << m_hexNumb << endl;;
}

const string hexa::value()
{
    return m_sign+m_hexNumb;
}

int hexa::htoi(char hex)
{
    static const string hexCode = "0123456789ABCDEF";
    return hexCode.find(toupper(hex));

}

string hexa::itoh(int num)
{
    static const string hexCode = "0123456789ABCDEF";
    string hex;
    int i = 0, cpy_num = num;
    if(num <= 15 && num >=0)
        return string(1, hexCode[num]);
    while(cpy_num != 0)
    {
        while(pow16(i) <= cpy_num )
            i++;
        i--;
        if(hex.length() < i+1)
            hex.resize(i+1,'0');
        hex[hex.length()-(i+1)] = hexCode[cpy_num/pow16(i)];
        cpy_num -= (cpy_num/pow16(i))*pow16(i);
        i = 0;
    }
    return hex;
}

string hexa::push_front(string source, string t)
{
    return t+source;
}

string hexa::push_back(string source, string t)
{
    return source+t;
}

void hexa::format()
{
    int i = 0;
    string formated;
    while(m_hexNumb[i]=='0')
        i++;
    if(i == m_hexNumb.length())
    {
        m_hexNumb = "0";
        return;
    }
    //cout << m_hexNumb.length  << ":format:" << i << endl;
    for(; i < m_hexNumb.length(); ++i)
        formated += m_hexNumb[i];
    m_hexNumb = formated;
}

template <typename T1> hexa hexa::hexRoot(T1 numb)
{
    m_hexNumb = to_string(numb);
    hexa hexRoot;

    return hexRoot;
}

bool operator< (const hexa& hex1, const hexa& hex2)
{
    int h1_length = hex1.m_hexNumb.length()-1, h2_length = hex2.m_hexNumb.length()-1;

    if(hex1 == hex2)
        return false;

    if(hex1.m_sign  == '-' && hex2.m_sign == '\0')
        return true;
    if(hex1.m_sign == '\0' && hex2.m_sign == '-')
        return false;

    if (h1_length > h2_length)
        return false;
    else if (h1_length < h2_length)
        return true;
    else
    {
        for(int i = 0; i < h1_length; i++)
        {
            if (hex1.m_hexNumb[i] > hex2.m_hexNumb[i])
                return false;
            else if (hex1.m_hexNumb[i] < hex2.m_hexNumb[i])
                return true;
        }
    }
}
        
bool operator> (const hexa& hex1, const hexa& hex2)
{
    return (hex2 < hex1);
}

hexa operator+ (const hexa& hex1, const hexa& hex2)
{
    hexa hexSum(0), h1(hex1), h2(hex2);
    h1.m_sign = '\0';
    h2.m_sign = '\0';
    
    if(hex1.m_sign == '-' && hex2.m_sign == '-')
        hexSum.m_sign = '-';
    if(hex1.m_sign == '-' && hex2.m_sign == '\0')
        return h2 - h1;
    if(hex1.m_sign == '\0' && hex2.m_sign == '-')
        return h1 - h2;

    int carry = 0, tmp_sum = 0, tmp_h1 = 0, tmp_h2 = 0;
    int h1_length = hex1.m_hexNumb.length()-1, h2_length = hex2.m_hexNumb.length()-1;
    int max_length = max(h1_length, h2_length);
    hexSum.m_hexNumb.resize(max_length+1);
    for(int i = 0; i <= max_length; i++)
    {
        if(i <= h1_length)
            tmp_h1 = hexa::htoi(hex1.m_hexNumb[h1_length-i]);
        else
            tmp_h1 = 0;
        if(i <= h2_length)
            tmp_h2 = hexa::htoi(hex2.m_hexNumb[h2_length-i]);
        else
            tmp_h2 = 0;
        tmp_sum = tmp_h1 + tmp_h2 + carry;
        if(tmp_sum > 15)
        {
            carry = 1;
            tmp_sum -= 16;
        }
        else
            carry = 0;
        hexSum.m_hexNumb[max_length-i] = hexa::itoh(tmp_sum)[0];
    }
    if(carry != 0)
    {
        //hexSum.m_hexNumb.resize(max_length+2);
        hexSum.m_hexNumb = "1" + hexSum.m_hexNumb;
    }
    return hexSum;
}

hexa operator- (const hexa& hex1, const hexa& hex2)
{
    hexa hexDiff(0), h1(hex1), h2(hex2);
    h1.m_sign = '\0';
    h2.m_sign = '\0';
    if (h1 < h2 && hex1.m_sign == hex2.m_sign) //abs
    {
        hexDiff = h2 - h1;
        if (hex1.m_sign == '-')
            hexDiff.m_sign = '\0';
        else
            hexDiff.m_sign = '-';
        return hexDiff;
    }

    if (hex1.m_sign != hex2.m_sign)
    {
        hexDiff = h1 + h2;
        hexDiff.m_sign = hex1.m_sign;
        return hexDiff;
    }
    else
    {
        hexDiff.m_sign = hex1.m_sign;
    }


    int carry = 0, tmp_diff = 0, tmp_h1 = 0, tmp_h2 = 0;
    int h1_length = hex1.m_hexNumb.length()-1, h2_length = hex2.m_hexNumb.length()-1;
    int max_length = max(h1_length, h2_length);
    hexDiff.m_hexNumb.resize(max_length+1);
    for(int i = 0; i <= max_length; i++)
    {
        if(i <= h1_length)
            tmp_h1 = hexa::htoi(hex1.m_hexNumb[h1_length-i]);
        else
            tmp_h1 = 0;
        if(i <= h2_length)
            tmp_h2 = hexa::htoi(hex2.m_hexNumb[h2_length-i]);
        else
            tmp_h2 = 0;
        tmp_h1 -= carry;
        tmp_diff = tmp_h1 - tmp_h2;
        if(tmp_diff < 0)
        {
            tmp_h1 += 16;
            tmp_diff = tmp_h1 - tmp_h2;
            carry = 1;
        }
        else
            carry = 0;
        hexDiff.m_hexNumb[max_length-i] = hexa::itoh(tmp_diff)[0];
    }
    if(carry != 0)
    {
        hexDiff.m_sign = '-';
        cout << "carry:tmp_diff::" << carry << ":" << tmp_diff << endl;
    }
    hexDiff.format();
    return hexDiff;
    
}

ostream& operator<< (ostream& out, const hexa& hex)
{
    out << hex.m_hexNumb;
    return out;
}

hexa& hexa::operator= (const hexa& hex)
{
    m_hexNumb = hex.m_hexNumb;
    return *this;
}

bool operator== (const hexa& hex1, const hexa& hex2)
{
    int h1_length = hex1.m_hexNumb.length()-1, h2_length = hex2.m_hexNumb.length()-1;
    if (h1_length != h2_length)
        return false;
    else
    {
        for(int i = 0; i < h1_length; ++i)
            if (hex1.m_hexNumb[i] != hex2.m_hexNumb[i]) 
                return false;
    }
    return true;
}

bool operator!= (const hexa& hex1, const hexa& hex2)
{
    return !(hex1 == hex2);
}

hexa& hexa::operator+= (const hexa& hex)
{
    *this = hex + *this;
    return *this;
}
        
hexa& hexa::operator-= (const hexa& hex)
{
    *this = hex - *this;
    return *this;
}
