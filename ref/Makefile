CC ?= /usr/bin/cc
PYTHON ?= python3
CFLAGS += -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls \
  -Wshadow -Wpointer-arith -O3 -fomit-frame-pointer -z noexecstack
NISTFLAGS += -Wno-unused-result -O3 -fomit-frame-pointer
RM = /bin/rm

SOURCES = kem.c indcpa.c polyvec.c poly.c ntt.c cbd.c reduce.c verify.c
SOURCESCHAOTIC = $(SOURCES) hashing.c ../hash_options/chaotic_hash.c
HEADERS = params.h kem.h indcpa.h polyvec.h poly.h ntt.h cbd.h reduce.h verify.h symmetric.h randombytes.h
HEADERSCHAOTIC = $(HEADERS) hashing.h ../hash_options/chaotic_hash.h

.PHONY: all test check speed shared nistkat clean

all: test speed shared nistkat

test: \
  test/test_hashing \
  test/test_rejection512 \
  test/test_rejection768 \
  test/test_rejection1024 \
  test/test_kyber512 \
  test/test_kyber768 \
  test/test_kyber1024 \
  test/test_vectors512 \
  test/test_vectors768 \
  test/test_vectors1024 \

speed: \
  test/test_speed512 \
  test/test_speed768 \
  test/test_speed1024 \

check: test
	$(PYTHON) test/run_checks.py

shared: \
  lib/libpqcrystals_kyber512_ref.so \
  lib/libpqcrystals_kyber768_ref.so \
  lib/libpqcrystals_kyber1024_ref.so \

nistkat: \
	nistkat/PQCgenKAT_kem512 \
	nistkat/PQCgenKAT_kem768 \
	nistkat/PQCgenKAT_kem1024 \


lib/libpqcrystals_kyber512_ref.so: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) randombytes.c
	mkdir -p lib
	$(CC) -shared -fPIC $(CFLAGS) -DKYBER_K=2 $(SOURCESCHAOTIC) randombytes.c -o $@

lib/libpqcrystals_kyber768_ref.so: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) randombytes.c
	mkdir -p lib
	$(CC) -shared -fPIC $(CFLAGS) -DKYBER_K=3 $(SOURCESCHAOTIC) randombytes.c -o $@

lib/libpqcrystals_kyber1024_ref.so: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) randombytes.c
	mkdir -p lib
	$(CC) -shared -fPIC $(CFLAGS) -DKYBER_K=4 $(SOURCESCHAOTIC) randombytes.c -o $@

test/test_hashing: hashing.c ../hash_options/chaotic_hash.c $(HEADERSCHAOTIC) test/test_hashing.c
	$(CC) $(CFLAGS) hashing.c ../hash_options/chaotic_hash.c test/test_hashing.c -o $@

test/test_rejection512: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) test/test_rejection.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESCHAOTIC) randombytes.c test/test_rejection.c -o $@

test/test_rejection768: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) test/test_rejection.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESCHAOTIC) randombytes.c test/test_rejection.c -o $@

test/test_rejection1024: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) test/test_rejection.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESCHAOTIC) randombytes.c test/test_rejection.c -o $@

test/test_kyber512: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) test/test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESCHAOTIC) randombytes.c test/test_kyber.c -o $@

test/test_kyber768: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) test/test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESCHAOTIC) randombytes.c test/test_kyber.c -o $@

test/test_kyber1024: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) test/test_kyber.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESCHAOTIC) randombytes.c test/test_kyber.c -o $@

test/test_vectors512: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) test/test_vectors.c
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESCHAOTIC) test/test_vectors.c -o $@

test/test_vectors768: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) test/test_vectors.c
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESCHAOTIC) test/test_vectors.c -o $@

test/test_vectors1024: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) test/test_vectors.c
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESCHAOTIC) test/test_vectors.c -o $@

test/test_speed512: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) test/cpucycles.h test/cpucycles.c test/speed_print.h test/speed_print.c test/test_speed.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=2 $(SOURCESCHAOTIC) randombytes.c test/cpucycles.c test/speed_print.c test/test_speed.c -o $@

test/test_speed768: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) test/cpucycles.h test/cpucycles.c test/speed_print.h test/speed_print.c test/test_speed.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=3 $(SOURCESCHAOTIC) randombytes.c test/cpucycles.c test/speed_print.c test/test_speed.c -o $@

test/test_speed1024: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) test/cpucycles.h test/cpucycles.c test/speed_print.h test/speed_print.c test/test_speed.c randombytes.c
	$(CC) $(CFLAGS) -DKYBER_K=4 $(SOURCESCHAOTIC) randombytes.c test/cpucycles.c test/speed_print.c test/test_speed.c -o $@

nistkat/PQCgenKAT_kem512: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) nistkat/PQCgenKAT_kem.c nistkat/rng.c nistkat/rng.h
	$(CC) $(NISTFLAGS) -DKYBER_K=2 -o $@ $(SOURCESCHAOTIC) nistkat/rng.c nistkat/PQCgenKAT_kem.c $(LDFLAGS) -lcrypto

nistkat/PQCgenKAT_kem768: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) nistkat/PQCgenKAT_kem.c nistkat/rng.c nistkat/rng.h
	$(CC) $(NISTFLAGS) -DKYBER_K=3 -o $@ $(SOURCESCHAOTIC) nistkat/rng.c nistkat/PQCgenKAT_kem.c $(LDFLAGS) -lcrypto

nistkat/PQCgenKAT_kem1024: $(SOURCESCHAOTIC) $(HEADERSCHAOTIC) nistkat/PQCgenKAT_kem.c nistkat/rng.c nistkat/rng.h
	$(CC) $(NISTFLAGS) -DKYBER_K=4 -o $@ $(SOURCESCHAOTIC) nistkat/rng.c nistkat/PQCgenKAT_kem.c $(LDFLAGS) -lcrypto

clean:
	-$(RM) -f test/test_hashing test/test_rejection512 test/test_rejection768 test/test_rejection1024
	-$(RM) -f *.gcno *.gcda *.lcov *.o *.so
	-$(RM) -f test/test_kyber512
	-$(RM) -f test/test_kyber768
	-$(RM) -f test/test_kyber1024
	-$(RM) -f test/test_vectors512
	-$(RM) -f test/test_vectors768
	-$(RM) -f test/test_vectors1024
	-$(RM) -f test/test_speed512
	-$(RM) -f test/test_speed768
	-$(RM) -f test/test_speed1024
	-$(RM) -f nistkat/PQCgenKAT_kem512
	-$(RM) -f nistkat/PQCgenKAT_kem768
	-$(RM) -f nistkat/PQCgenKAT_kem1024
	-$(RM) -f nistkat/*.req
	-$(RM) -f nistkat/*.rsp
	-$(RM) -rf lib/
