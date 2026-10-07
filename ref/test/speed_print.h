#ifndef PRINT_SPEED_H
#define PRINT_SPEED_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void print_results(const char *s, uint64_t *t, size_t tlen);

#ifdef __cplusplus
}
#endif

#endif
