#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus 
extern "C" {
#endif

struct chacha20_context
{
	uint32_t keystream32[16];
	size_t position;

	uint8_t key[32];
	uint8_t nonce[12];
	uint64_t counter;

	uint32_t state[16];
};

void chacha20_init_context(struct chacha20_context *ctx, uint8_t key[], uint8_t nounc[], uint64_t counter);

void chacha20_xor(struct chacha20_context *ctx, uint8_t *bytes, size_t n_bytes);

void chacha20_use(uint8_t *bytes, uint64_t n_bytes, uint8_t *key, uint8_t *nonce, uint64_t counter);

#ifdef __cplusplus 
}
#endif 