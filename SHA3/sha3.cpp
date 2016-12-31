//cpp
#pragma once

#include <iostream>
#include <cstdint>

class sha3
{
    sha3();
    ~sha3();
    public uint64_t round_constant(int round);
};
