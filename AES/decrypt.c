#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define ROTL8(x, shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8-shift)))

void Init_SBOX(uint8_t sbox[256]);
void Init_Inv_SBOX(uint8_t i_sbox[256], uint8_t sbox[256]);
void Init_RCON(uint8_t rcon[256]);

void SubBytes(int x_size, int y_size, uint8_t mat[x_size*y_size], uint8_t sbox[256]);
void KeySchedule(uint8_t key[16], uint8_t sbox[256], uint8_t rcon[256], int round);

void InvSubBytes(int x_size, int y_size, uint8_t mat[x_size*y_size], uint8_t sbox[256]);
void InvShiftRows(uint8_t mat[16], int rowN);
void InvMixColumns(uint8_t mat[16], int colN);
void AddRoundKey(uint8_t mat[16], uint8_t key[16]);
void InvKeySchedule(uint8_t key[16], uint8_t sbox[256], uint8_t rcon[256], int round);

void Print_Mat(int x_size, int y_size, uint8_t mat[x_size*y_size]);

void testMul()
{
    static const uint8_t modTableF[8] = {0, 0x1B, 0x2D, 0x36, 0x41, 0x5A, 0x6C, 0x6C};
    static const uint8_t modTableE[8] = {0, 0x1B, 0x36, 0x2D, 0x6C, 0x6C, 0x41, 0x5A};
    static const uint8_t modTableT[8] = {0, 0x1B, 0x2D, 0x36, 0x5A, 0x41, 0x77, 0x6C};
    uint8_t p = 0x01, q = 0xd8;
    p = q << 3 ^ q << 1 ^ q;//11
    //p = q<<3 ^ q<<2^q;//13
    //p = q<<3^q;//9
    //p = q<<3^q<<2^q<<1;//14
    //p^=(q&0x80?0x6C:0)^(q&0x40?0x36:0)^(q&0x20?0x1B:0);
    printf("\n\n\t0x%2.2X\n",(0xE9^0x0F^0x7D^0x76));
    
    p ^= modTableE[q>>5];
    printf("\n\t0x%2.2X\n",modTableE[q>>5]);
    printf("\n\t0x%2.2X\t%2.2X\n\n",p,q);    
}
//26FDB42A7DBA1FEBAC7B64B93194FB5B
//CE2AD677DBD8DFEF134FCF99654FA58A

int main()
{
    uint8_t sbox[256];
    uint8_t i_sbox[256];
    uint8_t rcon[256];
    Init_SBOX(sbox);
    Print_Mat(16, 16, sbox);
    Init_Inv_SBOX(i_sbox, sbox);
    Print_Mat(16, 16, i_sbox);
    Init_RCON(rcon);
    Print_Mat(16, 16, rcon);
    //uint8_t test[] = {0xCE, 0xDB, 0x13, 0x65, 0x2A, 0xD8, 0x4F, 0x4F, 0xD6, 0xDF, 0xCF, 0xA5, 0x77, 0xEF, 0x99, 0x8A}; 
    //uint8_t result[] = {0x26, 0x7D, 0xAC, 0x31, 0xFD, 0xBA, 0x7B, 0x94, 0xB4, 0x1F, 0x64, 0xFB, 0x2A, 0xEB, 0xB9, 0x5B};
    //uint8_t mat1[] = {0x19, 0xA0, 0x9A, 0xE9, 0x3D, 0xF4, 0xC6, 0xF8, 0xE3, 0xE2, 0x8D, 0x48, 0xBE, 0x2B, 0x2A, 0x08};
    uint8_t state[] = {0x66, 0xEF, 0x88, 0xCA, 0xE9, 0x8A, 0x4C, 0x34, 0x4B, 0x2C, 0xFA, 0x2B, 0xD4, 0x3B, 0x59, 0x2E};
    //uint8_t state[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t key[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    for(int i = 0; i < 10; i++)
    {
        KeySchedule(key, sbox, rcon, i+1);        
    }
    Print_Mat(4, 4, key);
    Print_Mat(4, 4, state);
    //InvKeySchedule(key, sbox, rcon, 11);
    //Print_Mat(4, 4, key);
    AddRoundKey(state, key);
    //Print_Mat(4, 4, state);
    InvShiftRows(state, 1);
    //Print_Mat(4, 4, state);
    InvSubBytes(4, 4, state, i_sbox);
    //Print_Mat(4, 4, state); 
    for(int i = 1; i < 10; i++)
    {
        InvKeySchedule(key, sbox, rcon, 11-i);
        //printf("\n\t%2.2X\n",rcon[11-i]);
        //Print_Mat(4, 4, key);
        AddRoundKey(state, key);
        Print_Mat(4, 4, state);
        InvMixColumns(state, 0);
        Print_Mat(4, 4, state); 
        InvShiftRows(state, 1);
        Print_Mat(4, 4, state);
        InvSubBytes(4, 4, state, i_sbox);
        Print_Mat(4, 4, state);        
    }
    InvKeySchedule(key, sbox, rcon, 1);
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

void Init_Inv_SBOX(uint8_t i_sbox[256], uint8_t sbox[256]) //Calc from sbox
{
    for(int i = 0; i < 256 ;i++)
    {
        i_sbox[sbox[i]] = i;
    }
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

void InvSubBytes(int x_size, int y_size, uint8_t mat[x_size*y_size], uint8_t i_sbox[256])
{
    printf("\nInvert SubBytes... \n");
    for(int i = 0; i < x_size; i++)
    {
        for(int j = 0; j < y_size; j++)
        {
            uint8_t tmp = mat[4*i+j];
            mat[4*i+j] = i_sbox[tmp];
        }
    }
}

void InvShiftRows(uint8_t mat[16], int rowN)
{
    if(rowN > 3)
        return;
    printf("\nInvert ShiftRow %d... \n", rowN);
    uint8_t tmp[4-rowN];
    for(int j = 0; j < 4; j++)
    {
        if(j < (4 - rowN))
            tmp[j] = mat[4*rowN+j];
        if(j < rowN) {
            mat[4*rowN+j] = mat[3*rowN+j+4];
        }
        else {
            mat[4*rowN+j] = tmp[j-rowN];
        }
    }
    InvShiftRows(mat, ++rowN);
}

void InvMixColumns(uint8_t mat[16], int colN)
{   
    if(colN > 3)
        return;

    static const uint8_t modTableF[8] = {0, 0x1B, 0x2D, 0x36, 0x41, 0x5A, 0x6C, 0x77};
    static const uint8_t modTableE[8] = {0, 0x1B, 0x36, 0x2D, 0x77, 0x6C, 0x41, 0x5A};
    static const uint8_t modTableT[8] = {0, 0x1B, 0x2D, 0x36, 0x5A, 0x41, 0x77, 0x6C};

    printf("\nInvert MixColumns %d... \n", colN);
    uint8_t mat_Cpy_ColN[4];
    uint8_t mat_F[4], mat_E[4], mat_T[4], mat_N[4];
    uint8_t highBit;
    for(int i = 0; i < 4; i++)
    {
        mat_Cpy_ColN[i] = mat[colN+4*i];
        //printf("\ni:%d\t%2.2X\n",i,mat_Cpy_ColN[i]);
        mat_F[i] = (mat_Cpy_ColN[i] << 1 ^ mat_Cpy_ColN[i] << 2 ^ mat_Cpy_ColN[i] << 3);
        mat_F[i] ^= modTableF[mat_Cpy_ColN[i] >> 5];
        printf("\n\tmat14:%2.2X\t::%2.2X\n",mat_F[i], mat_Cpy_ColN[i]);
        //printf("\n\tmat14:%2.2X\n",mat_F[i]);
        mat_E[i] = (mat_Cpy_ColN[i] ^ mat_Cpy_ColN[i] << 1 ^ mat_Cpy_ColN[i] << 3);
        mat_E[i] ^= modTableE[mat_Cpy_ColN[i] >> 5];
        printf("\n\tmat11:%2.2X\t::%2.2X\n",mat_E[i], mat_Cpy_ColN[i]);
        //printf("\n\tmat11:%2.2X\n",mat_E[i]);
        mat_T[i] = (mat_Cpy_ColN[i] ^ mat_Cpy_ColN[i] << 2 ^ mat_Cpy_ColN[i] << 3);
        mat_T[i] ^= modTableT[mat_Cpy_ColN[i] >> 5];
        printf("\n\tmat13:%2.2X\t::%2.2X\n",mat_T[i], mat_Cpy_ColN[i]);
        //printf("\n\tmat13:%2.2X\n",mat_T[i]);
        mat_N[i] = (mat_Cpy_ColN[i] ^ mat_Cpy_ColN[i] << 3);
        mat_N[i] ^= (mat_Cpy_ColN[i]&0x80?0x6C:0)^(mat_Cpy_ColN[i]&0x40?0x36:0)^(mat_Cpy_ColN[i]&0x20?0x1B:0);
        printf("\n\tmat9:%2.2X\t\t::%2.2X\n",mat_N[i], mat_Cpy_ColN[i]);
        //printf("\n\tmat9:%2.2X\n",mat_N[i]);
        printf("____________");
        
    }
    mat[colN] = mat_F[0] ^ mat_E[1] ^ mat_T[2] ^ mat_N[3];
    //printf("\n0x%2.2X\t0x%2.2X\t0x%2.2X\t0x%2.2X\n", mat_F[0],  mat_E[1],  mat_T[2],  mat_N[3]);

    mat[colN+4] = mat_N[0] ^ mat_F[1] ^ mat_E[2] ^ mat_T[3];
    //printf("0x%2.2X\n", mat[colN+4]);

    mat[colN+8] = mat_T[0] ^ mat_N[1] ^ mat_F[2] ^ mat_E[3];
    //printf("0x%2.2X\n", mat[colN+8]);

    mat[colN+12] = mat_E[0] ^ mat_T[1] ^ mat_N[2] ^ mat_F[3];
    //printf("0x%2.2X\n", mat[colN+12]);
    printf("\n0x%2.2X\t0x%2.2X\t0x%2.2X\t0x%2.2X\n", mat_T[0],  mat_N[1],  mat_F[2],  mat_E[3]);

    InvMixColumns(mat, ++colN);
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

void InvKeySchedule(uint8_t key[16], uint8_t sbox[256], uint8_t rcon[256], int round)
{
    printf("\nInvert KeySchedule... \n");
    uint8_t key_C[4];
    //Print_Mat(4,4,key);
    for(int j = 0; j < 4; j++)
    {
        key[4*j+3] ^= key[4*j+2];
        key[4*j+2] ^= key[4*j+1];
        key[4*j+1] ^= key[4*j];
        if(j == 0)
        {
            key_C[3-j] = key[4*j+3];
        }
        else
        {
            key_C[j-1] = key[4*j+3];
        }
    }
    //Print_Mat(4,4,key);
    //Print_Mat(1,4,key_C);
    SubBytes(1, 4, key_C, sbox);
    key_C[0] ^= rcon[round];
    for(int i = 0; i < 4; i++)
    {
        key[4*i] ^= key_C[i]; 
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
