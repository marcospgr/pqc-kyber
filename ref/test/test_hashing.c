#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../hashing.h"

/* Golden outputs from the original repository core, not the floating-point paper. */
static const uint8_t expected256[32] = {
  0x37,0x6e,0x5f,0xc1,0x06,0x00,0x05,0xe0,0x3b,0xe0,0x5d,0x7a,0x9a,0x83,0xfd,0xfc,
  0x30,0xa6,0xac,0x05,0xa4,0xdc,0x46,0x6d,0x6f,0xe8,0x21,0x3e,0x59,0xad,0xb4,0xd8,
};
static const uint8_t expected512[64] = {
  0xd4,0x39,0x4a,0x7b,0xae,0x93,0x70,0xf5,0x0b,0x0b,0x9c,0x4a,0x55,0xa5,0x0e,0xa8,
  0x36,0x82,0x78,0xea,0xa1,0x4a,0x6d,0xa8,0xc5,0x9a,0xc5,0x16,0x93,0x65,0x4f,0x89,
  0x24,0xbe,0xb3,0x2c,0x7c,0x05,0x67,0x34,0x30,0xb3,0x52,0x3a,0xd8,0x5d,0xb9,0x5f,
  0x2f,0x18,0x39,0x7b,0x67,0x47,0x5b,0x90,0x12,0x9f,0x12,0x54,0x3f,0xf1,0x65,0x54,
};

static int test_original_vectors(void)
{
  uint8_t input[64], digest256[32], digest512[64];
  for(size_t i = 0; i < sizeof input; i++) input[i] = (uint8_t)i;
  _sha3_256(digest256, input, sizeof input);
  _sha3_512(digest512, input, sizeof input);
  if(memcmp(digest256, expected256, sizeof digest256) ||
     memcmp(digest512, expected512, sizeof digest512)) {
    puts("ERROR hashing differs from the original repository vectors");
    return 1;
  }
  return 0;
}

static int test_partial_squeeze(void)
{
  uint8_t seed[KYBER_SYMBYTES] = {0};
  uint8_t whole[176], split[176];
  const size_t chunks[] = {1, 3, 5, 167};
  hash_state state;
  kyber_shake128_absorb(&state, seed, 0, 0);
  _shake128_squeeze(whole, sizeof whole, &state);
  kyber_shake128_absorb(&state, seed, 0, 0);
  memset(split, 0xa5, sizeof split);
  size_t offset = 0;
  for(size_t i = 0; i < sizeof chunks / sizeof chunks[0]; i++) {
    _shake128_squeeze(split + offset, chunks[i], &state);
    offset += chunks[i];
  }
  if(memcmp(whole, split, sizeof whole)) {
    puts("ERROR XOF partial output differs from continuous output");
    return 1;
  }
  return 0;
}

int main(void)
{
  int result = test_original_vectors();
  result |= test_partial_squeeze();
  if(!result) puts("Chaotic hashing regression tests: OK");
  return result;
}
