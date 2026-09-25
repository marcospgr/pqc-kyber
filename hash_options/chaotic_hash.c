#include <stdint.h>
#include "chaotic_hash.h"

#define SCALE_VAL 16777216LL
#define SCALE_BIT 24

#define CFOLD(x) ((x) & 0x00FFFFFFLL)

#define MUL_FIXED(a, b) ((int64_t)((((__int128)(a)) * ((__int128)(b))) >> SCALE_BIT))

void ChaoticHashInit (ChaoticHashCtx_t *Ctx, int h_size) {
    Ctx->z_real = (int64_t)(0.4569 * SCALE_VAL);
    Ctx->z_imag = (int64_t)(0.5133 * SCALE_VAL);
    Ctx->c_real = (int32_t)(0.7429 * SCALE_VAL);
    Ctx->c_imag = (int32_t)(0.2142 * SCALE_VAL);
    Ctx->h_size = h_size;

    double a = 10.15 + (double)(h_size)/10000.0;
    Ctx->alpha = (int64_t)(a * SCALE_VAL);
}

void ChaoticHashing (ChaoticHashCtx_t *Ctx, uint8_t *output, const uint8_t *data, size_t data_len) {
    int64_t z_real = Ctx->z_real;
    int64_t z_imag = Ctx->z_imag;
    int64_t c_real = Ctx->c_real;
    int64_t c_imag = Ctx->c_imag;
    int64_t alpha = Ctx->alpha;

    /***********************************************************************************************************
     * COMPUTATIONAL BLOCK 
     * 1 -  z, c and alpha received from the input
     * 2 -  padding is performed on the data so that the data has a multiplier of 8-bytes
     *      reads the data in a sequence of 8-bytes at each step 
     *      (the first 4-bytes are stores in c_real and the last 4-bytes stored in c_imag)
     * 3 -  data is mapped in the range of [0,1]
     *      normalization equation (10)
     * 4 -  the hsize variable value is added to the alpha control parameter
     *      equation (11)
     * 5 -  the complex map is repeated on the whole data with lenght N, which is inserted in the variable c
     *      equation (12)
     * 6 -  the last value obtained for z is saved for use in the next step of the process
     **********************************************************************************************************/

    if(data != NULL && data_len > 0) {
        size_t iterations = (data_len / 4) + ((data_len % 4) > 0 ? 1 : 0);

        for (size_t i = 0; i < iterations; i +=2) {
            uint32_t cd_real = 0, cd_imag = 1;

            for (int j = 0; j < 4; j++) {
                if (i*4 + j < data_len) {
                    cd_real = cd_real | ((uint32_t)data[i*4+j] << (8*j));
                }
            }

            if (i+1 < iterations) {
                cd_imag = 0;
                for (int k = 0; k < 4; k++) {
                    if ((i+1)*4 + k < data_len) {
                        cd_imag = cd_imag | ((uint32_t)data[(i+1)*4+k] << (8*k));
                    }
                }
            }

            int64_t norm_cd_real = (int64_t)(cd_real >> 8);
            int64_t norm_cd_imag = (int64_t)(cd_imag >> 8); 
            //int64_t norm_cd_real = (int64_t)((cd_real >> 8) | 1); -> remove problema do valor totalmente zerado

            int64_t temp_z_real = z_real;
            int64_t temp_z_imag = z_imag;

            z_real = CFOLD(MUL_FIXED(MUL_FIXED(alpha,alpha),(MUL_FIXED(temp_z_real,temp_z_real)-MUL_FIXED(temp_z_imag,temp_z_imag))) + norm_cd_real);
            z_imag = CFOLD(MUL_FIXED(MUL_FIXED(alpha,alpha),(MUL_FIXED(temp_z_real,temp_z_imag) << 1)) + norm_cd_imag);
        }    
    }

    /***********************************************************************************************************
     * ITERATION BLOCK 
     * 1 -  c and alpha received from the input
     * 2 -  z received from the previous block (COMPUTATIONAL BLOCK)
     * 3 -  the last z in the previous step is repeated at (200 + H_size) iterations
     *      to make the chaotic behavior in the system 
     *      equation (7)
     * 4 -  c and alpha is the initial input from the user's chaotic keys and is not extracted from the data
     * 5 -  the final value obtained from z will be used in the next step as the system input to extract the hash code
     **********************************************************************************************************/

    for (int l = 0; l <= (Ctx->h_size + 200); l++) {
        int64_t temp_z_real = z_real;
        int64_t temp_z_imag = z_imag;

        z_real = CFOLD(MUL_FIXED(MUL_FIXED(alpha,alpha),(MUL_FIXED(temp_z_real,temp_z_real)-MUL_FIXED(temp_z_imag,temp_z_imag))) + c_real);
        z_imag = CFOLD(MUL_FIXED(MUL_FIXED(alpha,alpha),(MUL_FIXED(temp_z_real,temp_z_imag) << 1)) + c_imag);
    }

    /***********************************************************************************************************
     * EXTRACTION BLOCK
     * 1 -  c and alpha received from the input
     * 2 -  z received from the previous block (ITERARION BLOCK)
     * 3 -  we repeat the chaotic iterated map to extract 32-bits of hash code in each iteration
     *      equation (7)
     * 4 -  extract 16-bits from the real part and 16-bits from the imaginary part of z in each iteration
     * 5 -  process of extraction defined in equation (13)
     * 6 -  it is sufficient to merge these 32-bit obtained hash codes with previous 32-bit hash codes in each
     *      iteration
     **********************************************************************************************************/

    if (output != NULL) {
        int L = (Ctx->h_size / 32);
        for (int m = 0; m <= L; m++) {
            int64_t temp_z_real = z_real;
            int64_t temp_z_imag = z_imag;

            z_real = CFOLD(MUL_FIXED(MUL_FIXED(alpha,alpha),(MUL_FIXED(temp_z_real,temp_z_real)-MUL_FIXED(temp_z_imag,temp_z_imag))) + c_real);
            z_imag = CFOLD(MUL_FIXED(MUL_FIXED(alpha,alpha),(MUL_FIXED(temp_z_real,temp_z_imag) << 1)) + c_imag);

            uint16_t h1 = (uint16_t)((z_real >> 8) & 0xFFFF);
            uint16_t h2 = (uint16_t)((z_imag >> 8) & 0xFFFF);

            output[m*4] = (uint8_t)(h1 >> 8);
            output[m*4 + 1] = (uint8_t)(h1 >> 8);

            output[m*4 + 2] = (uint8_t)(h2 >> 8);
            output[m*4 + 3] = (uint8_t)(h2 >> 8);
        }
    }
}