#include <stddef.h>
#include <stdint.h>
#include "params.h"
#include "hash_options\chaotic_hash.h"

typedef ChaoticHashCtx_t hash_state;

void _sha3_256(uint8_t h[32], const uint8_t *in, size_t inlen);
/***********************************************************************************************************
* uint8_t h[32]        -> buffer de saida - 32-bytes/256-bits.
* const uint8_t *in    -> ponteiro de dados de entrada (a mensagem que sofrera o hashing).
* size_t inlen         -> o tamanho em bytes da mensagem de entrada.
***********************************************************************************************************/

void _sha3_512(uint8_t h[64], const uint8_t *in, size_t inlen);
/***********************************************************************************************************
* uint8_t h[64]        -> buffer de saida - 64-bytes/512-bits.
* const uint8_t *in    -> ponteiro de dados de entrada (a mensagem que sofrera o hashing).
* size_t inlen         -> o tamanho em bytes da mensagem de entrada. 
***********************************************************************************************************/

void _shake128_squeezeblocks(uint8_t *out, size_t nblocks, hash_state *state);
/***********************************************************************************************************
* uint8_t *out         -> ponteiro para o buffer de saida. 
* size_t nblocks       -> quantidade de blocos (depende do tamanho do bloco).
* hash_state *state    -> estrutura que guarda o estado interno da "esponja", 
*                         já foi previamente inicializado e "seedado" por uma função correspondente (shake128_absorb).
***********************************************************************************************************/
 
void _shake128_squeeze(uint8_t *out, size_t outlen, hash_state *state);
/***********************************************************************************************************
* uint8_t *out         -> ponteiro para o buffer de saida. 
* size_t outlen        -> quantidade de bytes solicitada nesta chamada específica.
* hash_state *state    -> estrutura que guarda o estado interno da "esponja", 
*                         já foi previamente inicializado e "seedado" por uma função correspondente (shake128_absorb).
***********************************************************************************************************/

#define kyber_shake128_absorb KYBER_NAMESPACE(kyber_shake128_absorb)
void kyber_shake128_absorb(hash_state *state,const uint8_t seed[KYBER_SYMBYTES],uint8_t x,uint8_t y);
/***********************************************************************************************************
* hash_state *state                    -> ponteiro para a estrutura que guarda o estado interno da função de hash. 
* const uint8_t seed[KYBER_SYMBYTES]   -> é a seed pública rho, gerada aleatoriamente por quem iniciou a comunicação.
* uint8_t x                            -> são as coordenadas da matriz pública A, x representa a linha (row).
* uint8_t y                            -> são as coordenadas da matriz pública A, y a coluna (col).
***********************************************************************************************************/

#define kyber_shake256_prf KYBER_NAMESPACE(kyber_shake256_prf)
void kyber_shake256_prf(uint8_t *out, size_t outlen, const uint8_t key[KYBER_SYMBYTES], uint8_t nonce);
/***********************************************************************************************************
* uint8_t *out                         -> ponteiro para o buffer onde a função vai gravar o ruído pseudoaleatório gerado. 
* size_t outlen                        -> a quantidade de bytes de ruído solicitada.
* const uint8_t key[KYBER_SYMBYTES]    -> a semente secreta sigma de 32 bytes, ela atua como a chave da PRF.
* uint8_t nonce                        -> um contador de 1 byte. "nonce" vem de Number Used Once.
***********************************************************************************************************/

#define kyber_shake256_rkprf KYBER_NAMESPACE(kyber_shake256_rkprf)
void kyber_shake256_rkprf(uint8_t out[KYBER_SSBYTES], const uint8_t key[KYBER_SYMBYTES], const uint8_t input[KYBER_CIPHERTEXTBYTES]);
/***********************************************************************************************************
* uint8_t out[KYBER_SSBYTES]                   -> buffer de saída. 
* const uint8_t key[KYBER_SYMBYTES]            -> a chave de rejeição secreta z.
* const uint8_t input[KYBER_CIPHERTEXTBYTES]   -> o texto cifrado (c) recebido pela rede.
***********************************************************************************************************/