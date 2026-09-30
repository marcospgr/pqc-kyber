#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "params.h"
#include "hashing.h"
#include "../hash_options/chaotic_hash.h"


void _sha3_256(uint8_t h[32], const uint8_t *in, size_t inlen){
        hash_state state;
    	ChaoticHashInit(&state, 256);
    	ChaoticHashing(&state, h, in, inlen);
}

void _sha3_512(uint8_t h[64], const uint8_t *in, size_t inlen){
        hash_state state;
    	ChaoticHashInit(&state, 512);
    	ChaoticHashing(&state, h, in, inlen);

}

void _shake128_squeeze(uint8_t *out, size_t outlen, hash_state *state){
        int original_h_size = state->h_size;
    	state->h_size = (int)(outlen * 8);
    	ChaoticHashing(state, out, NULL, 0);
    	state->h_size = original_h_size;
}

void _shake128_squeezeblocks(uint8_t *out, size_t nblocks, hash_state *state){
        size_t total_bytes = nblocks * 168;    
    	int original_h_size = state->h_size;
    	state->h_size = (int)(total_bytes * 8);
    	ChaoticHashing(state, out, NULL, 0);
    	state->h_size = original_h_size;
}

void kyber_shake128_absorb(hash_state *state,const uint8_t seed[KYBER_SYMBYTES],uint8_t x,uint8_t y){
	uint8_t in[34];
    	memcpy(in, seed, 32);
    	in[32] = x; 
    	in[33] = y;
    	ChaoticHashInit(state, 128);
    	ChaoticHashing(state, NULL, in, 34);
}

void kyber_shake256_prf(uint8_t *out, size_t outlen, const uint8_t key[KYBER_SYMBYTES], uint8_t nonce){
	hash_state state;
        ChaoticHashInit(&state, 256);
        uint8_t in[KYBER_SYMBYTES + 1];
        memcpy(in, key, KYBER_SYMBYTES);
        in[KYBER_SYMBYTES] = nonce;
        size_t inlen = KYBER_SYMBYTES + 1;
        state.h_size = (int)(outlen * 8);
        ChaoticHashing(&state, out, in, inlen);
}

void kyber_shake256_rkprf(uint8_t out[KYBER_SSBYTES], const uint8_t key[KYBER_SYMBYTES], const uint8_t input[KYBER_CIPHERTEXTBYTES]){
	hash_state state;
        ChaoticHashInit(&state, 256);
        ChaoticHashing(&state, NULL, key, KYBER_SYMBYTES);
        ChaoticHashing(&state, NULL, input, KYBER_CIPHERTEXTBYTES);
        state.h_size = (int)(KYBER_SSBYTES * 8);
        ChaoticHashing(&state, out, NULL, 0);
}