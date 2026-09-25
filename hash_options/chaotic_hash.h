#ifndef CHAOTIC_HASH_H
#define CHAOTIC_HASH_H

#include <stdint.h>
#include <stddef.h> 

typedef struct {
    int64_t z_real;
    int64_t z_imag;
    int64_t c_real;
    int64_t c_imag;
    int64_t alpha;
    int h_size;
} ChaoticHashCtx_t;

void ChaoticHashInit (ChaoticHashCtx_t *Ctx, int h_size);
/***************************************************************************
 * Ctx - estrutura de contexto para a funcao de hash caotica
 * h_size - tamanho do hash pertencente ao conjunto [32,64,128,160,256,512,1024,2048]
***************************************************************************/

void ChaoticHashing (ChaoticHashCtx_t *Ctx, uint8_t *output, const uint8_t *data, size_t data_len);
/****************************************************************************
 * Ctx - valores iniciais para a funcao de hash caotica
 * output - ponteiro para o buffer de saida do hash (1-byte para cada elemento)
 * data - ponteiro para o buffer de entrada de dados (1-byte para cada elemento)
 * data_len - tamanho do buffer de entrada de dados (em bytes)
****************************************************************************/

#endif //CHAOTIC_HASH_H