#include "sha3.h"

#include <cmath>
#include <iomanip>

uint64_t sha3::RoundConstant(uint64_t round)
{
    uint64_t rc = 0x0, RC = 0x0, mod = 0x0;
    const uint64_t modConst = 0x171;
    const uint64_t unit = 0x1;
    for (int j = 0; j < 6; ++j)
    {
        rc = RotL(unit, (j + 0x7 * round));
        std::cout << "j:" << j << ";rc:" << std::hex << rc << std::endl;
        int i = 55;
        while (i >= 0)
        {
            //mod = modConst << i;
            mod = RotL(modConst, static_cast<uint64_t>(i));
            if (mod <= rc)
            {
                std::cout << "i:" << i << ",rc:" << std::hex << rc << ",mod:" << mod << ",rc^mod:" << (rc^mod) << std::endl;
                rc ^= mod;
            }
            --i;
        }
        int shift = std::exp2(j - 1);
        RC += (rc & 0x1) << shift; 
    }
    std::cout << "RC=0x" << std::hex << RC << std::endl;
    return RC;
}

