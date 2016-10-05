#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define ROTL8(x, shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8-shift)))

void Init_SBOX(uint8_t sbox[256]);
void Init_RCON(uint8_t rcon[256]);

void SubBytes(int x_size, int y_size, uint8_t mat[x_size*y_size], uint8_t sbox[256]);
void ShiftRows(uint8_t mat[16], int rowN);
void MixColumns(uint8_t mat[16], int colN);
void AddRoundKey(uint8_t mat[16], uint8_t key[16]);
void KeySchedule(uint8_t key[16], uint8_t sbox[256], uint8_t rcon[256], int round);

void Print_Mat(int x_size, int y_size, uint8_t mat[x_size*y_size]);

int main()
{
    uint8_t sbox[256];
    uint8_t rcon[256];
    //uint8_t key[] = {0x2B, 0x28, 0xAB, 0x09, 0x7E, 0xAE, 0xF7, 0xCF, 0x15, 0xD2, 0x15, 0x4F, 0x16, 0xA6, 0x88, 0x3C};
    //uint8_t mat[] = {0x19, 0xA0, 0x9A, 0xE9, 0x3D, 0xF4, 0xC6, 0xF8, 0xE3, 0xE2, 0x8D, 0x48, 0xBE, 0x2B, 0x2A, 0x08};
    //uint8_t state[] = {0x32, 0x88, 0x31, 0xE0, 0x43, 0x5A, 0x31, 0x37, 0xF6, 0x30, 0x98, 0x07, 0xA8, 0x8D, 0xA2, 0x34};
    uint8_t state[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t key[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    Init_SBOX(sbox);
    Print_Mat(16, 16, sbox);
    Init_RCON(rcon);
    Print_Mat(16, 16, rcon);
    Print_Mat(4, 4, state);
    AddRoundKey(state, key);
    Print_Mat(4, 4, state);
    //Print_Mat(4, 4, mat);
    for(int i = 1; i < 10; i++)
    {
        SubBytes(4, 4, state, sbox);
        Print_Mat(4, 4, state);
        ShiftRows(state, 1);
        Print_Mat(4, 4, state);
        MixColumns(state, 0);
        Print_Mat(4, 4, state);
        KeySchedule(key, sbox, rcon, i);
        Print_Mat(4, 4, key);
        AddRoundKey(state, key);
        Print_Mat(4, 4, state);
    }
    SubBytes(4, 4, state, sbox);
    Print_Mat(4, 4, state);
    ShiftRows(state, 1);
    Print_Mat(4, 4, state);
    KeySchedule(key, sbox, rcon, 10);
    Print_Mat(4, 4, key);
    AddRoundKey(state, key);
    Print_Mat(4, 4, state);

    return 0;
}

void Init_SBOX(uint8_t sbox[256])
{
    printf("Generating S-BOX...\n");
    uint8_t p = 1, q = 1;
    uint8_t progress = 0;
    do{
        printf("\r[");
        for(int i = 0; i < 42; i++)
        {
            if(i < progress*42/252)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
            if(i == 41)
                printf("]");
        }
        printf("%d%%", progress*100/252);

        p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);
        q ^= q << 1;
        q ^= q << 2;
        q ^= q << 4;
        q ^= q & 0x80 ? 0x09 : 0;
        sbox[p] = 0x63 ^ q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);

        progress++;
    } while( p!=1 );
    sbox[0] = 0x63;
}

void Init_RCON(uint8_t rcon[256])
{
    printf("\nGenerating Rcon... \n");
    rcon[0] = 0x8D;
    uint8_t highBit;
    uint8_t progress = 0;
    for(int i = 1; i < 256; i++)
    {
        printf("\r[");
        for(int i = 0; i < 42; i++)
        {
            if(i < progress*42/252)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
            if(i == 41)
                printf("]");
        }
        printf("%d%%", progress*100/252);

        highBit = (unsigned char)((signed char)rcon[i-1] >> 7);
        rcon[i] = rcon[i-1] << 1;
        rcon[i] ^= 0x1B & highBit;
        progress++;
    }
}

void SubBytes(int x_size, int y_size, uint8_t mat[x_size*y_size], uint8_t sbox[256])
{
    printf("\nSubBytes... \n");
    for(int i = 0; i < x_size; i++)
    {
        for(int j = 0; j < y_size; j++)
        {
            uint8_t tmp = mat[4*i+j];
            mat[4*i+j] = sbox[tmp];
        }
    }
}

void ShiftRows(uint8_t mat[16], int rowN)
{
    if(rowN > 3)
        return;
    printf("\nShiftRow %d... \n", rowN);
    uint8_t tmp[rowN];
    for(int j = 0; j < 4; j++)
    {
        if(j < rowN)
            tmp[j] = mat[4*rowN+j];
        if(j < (4 - rowN)) {
            mat[4*rowN+j] = mat[5*rowN+j];
        }
        else {
            mat[4*rowN+j] = tmp[rowN+j-4];
        }
    }
    ShiftRows(mat, ++rowN);
}

void MixColumns(uint8_t mat[16], int colN)
{
    if(colN > 3)
        return;
    printf("\nMixColumns %d... \n", colN);
    uint8_t mat_Cpy_ColN[4];
    uint8_t mat_Dbl[4];
    uint8_t highBit;
    for(int i = 0; i < 4; i++)
    {
        mat_Cpy_ColN[i] = mat[colN+4*i];
        highBit =(unsigned char)((signed char)mat[colN+4*i] >> 7);
        mat_Dbl[i] = mat[colN+4*i] << 1;
        mat_Dbl[i] ^= 0x1B & highBit;
    }
    mat[colN] = mat_Dbl[0] ^ mat_Dbl[1] ^ mat_Cpy_ColN[1] ^ mat_Cpy_ColN[2] ^ mat_Cpy_ColN[3];

    mat[colN+4] = mat_Cpy_ColN[0] ^ mat_Dbl[1] ^ mat_Dbl[2] ^ mat_Cpy_ColN[2] ^ mat_Cpy_ColN[3];
    
    mat[colN+8] = mat_Cpy_ColN[0] ^ mat_Cpy_ColN[1] ^ mat_Dbl[2] ^ mat_Dbl[3] ^ mat_Cpy_ColN[3];

    mat[colN+12] = mat_Dbl[0] ^ mat_Cpy_ColN[0] ^ mat_Cpy_ColN[1] ^ mat_Cpy_ColN[2] ^ mat_Dbl[3];

    MixColumns(mat, ++colN);
}

void AddRoundKey(uint8_t mat[16], uint8_t key[16])
{
    printf("\nAddRoundKey... \n");
    for(int i = 0; i < 16; i++)
    {
        mat[i] ^= key[i];
    }
}

void KeySchedule(uint8_t key[16], uint8_t sbox[256], uint8_t rcon[256], int round)
{
    printf("\nKeySchedule... \n");
    uint8_t key_C[4];
    key_C[0] = key[7];
    key_C[1] = key[11];
    key_C[2] = key[15];
    key_C[3] = key[3];
    SubBytes(1, 4, key_C, sbox);
    key[0] ^= key_C[0] ^ rcon[round];
    key[4] ^= key_C[1];
    key[8] ^= key_C[2];
    key[12] ^= key_C[3];
    for(int i = 1; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            key[4*j+i] ^= key[4*j+i-1]; 
        }
    }
}

void Print_Mat(int x_size, int y_size, uint8_t mat[x_size*y_size])
{

    printf("\n");
    for(int i = 0; i < x_size; i++)
    {
        for(int j = 0; j < y_size; j++)
        {
            printf(" 0x%2.2X ",mat[x_size*i+j]);
        }
        printf("\n");
    }
}
