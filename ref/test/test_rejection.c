#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../kem.h"
#include "../hashing.h"

/* Fixed coins from the original Kyber768 failure: byte 963 XOR 8.
 * The old hash mapped decoded messages f0... and b0... to the same digest. */
static const uint8_t keycoins[64] = {
  0x56,0x5c,0x08,0x30,0xd8,0xa9,0xb7,0xe6,0x8a,0x19,0x88,0x3d,0xee,0x47,0x89,0x5b,
  0x9a,0xf3,0xfe,0xbe,0x40,0xc9,0x22,0x79,0xe6,0x60,0xbc,0xc5,0x98,0x33,0xb0,0x62,
  0x75,0x9a,0x21,0x24,0xe5,0x8d,0x5a,0x7f,0x09,0x23,0x9e,0x20,0xee,0xe7,0x16,0x3c,
  0xce,0xd5,0x8c,0x29,0x5f,0xcf,0xb1,0x41,0x1e,0xb6,0x23,0x5b,0x8b,0xf8,0x06,0xaa
};
static const uint8_t enccoins[32] = {
  0xf0,0x6a,0xc8,0x56,0x28,0xf7,0xe0,0x83,0x9a,0x6a,0xd6,0x1f,0x88,0x92,0x56,0x30,
  0xf7,0xd5,0x4b,0x96,0x79,0x1c,0xcd,0x0a,0xd4,0xec,0x87,0xcf,0x8c,0xf2,0x04,0x9f
};

int main(void)
{
  uint8_t pk[CRYPTO_PUBLICKEYBYTES], sk[CRYPTO_SECRETKEYBYTES];
  uint8_t ct[CRYPTO_CIPHERTEXTBYTES], altered[CRYPTO_CIPHERTEXTBYTES];
  uint8_t expected[CRYPTO_BYTES], actual[CRYPTO_BYTES], rejection[CRYPTO_BYTES];
  if(crypto_kem_keypair_derand(pk, sk, keycoins) ||
     crypto_kem_enc_derand(ct, expected, pk, enccoins) ||
     crypto_kem_dec(actual, ct, sk) || memcmp(actual, expected, sizeof actual)) {
    puts("ERROR fixed valid ciphertext");
    return 1;
  }
  size_t failures = 0;
  for(size_t pos = 0; pos < sizeof ct; pos++) {
    for(unsigned int bit = 0; bit < 8; bit++) {
      memcpy(altered, ct, sizeof ct);
      altered[pos] ^= (uint8_t)(1u << bit);
      crypto_kem_dec(actual, altered, sk);
      kyber_shake256_rkprf(rejection, sk + CRYPTO_SECRETKEYBYTES - KYBER_SYMBYTES, altered);
      if(!memcmp(actual, expected, sizeof actual) || memcmp(actual, rejection, sizeof actual)) {
        if(failures < 5)
          printf("ERROR invalid ciphertext: byte %zu bit %u\n", pos, bit);
        failures++;
      }
    }
  }
  printf("%s: %zu of %zu single-bit mutations failed rejection\n",
         CRYPTO_ALGNAME, failures, sizeof ct * 8);
  return failures != 0;
}
