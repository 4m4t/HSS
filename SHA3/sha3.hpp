#include "sha3.cpp"

uint64_t round_constant(int round)
{
    uint64_t rc = 0, RC = 0, mod = 0;
    for (int j = 0; j < 6; ++j)
    {
        rc = 1 << (j + 7 * round);
        while (rc > 0x10)
        {
            mod = 0x171 << ((j + 7 * round) - 8);
            rc ^= mod;
        }
        RC += (rc & 0x10);
    }
    std::cout << std::ios_base::hex << RC << std::endl;
}
