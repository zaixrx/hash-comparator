#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <openssl/md5.h>
#include <openssl/sha.h>

#define ERROR(...) \
	do { \
		fprintf(stderr, __VA_ARGS__); \
	} while(0)

#define ERROR_OUT(...) \
	do { \
		ERROR(__VA_ARGS__); \
		exit(69); \
	} while(0)

typedef enum {
	HASH_SHA1,
	HASH_SHA256,
	HASH_MD5,
} HashFn;

typedef unsigned char *Hash;

static char __hash_str__[64];
char *get_str_from_hash(Hash hash, size_t hash_size) {
	memset(__hash_str__, '\0', sizeof(__hash_str__));
	for (size_t i = 0; i < hash_size; ++i) {
		sprintf(__hash_str__, "%s%02x", __hash_str__, hash[i]);
	}
	return __hash_str__;
}

bool cmp_hash(unsigned char *string, char *hashed, HashFn fn) {
	Hash hash = NULL;
	size_t hash_size = 0;
	switch (fn) {
		case HASH_SHA1: {
			hash_size = SHA_DIGEST_LENGTH;
			hash = malloc(hash_size);
			SHA1(string, strlen((char*)string), hash);
		} break;
		case HASH_MD5: {
			hash_size = MD5_DIGEST_LENGTH;
			hash = malloc(hash_size);
			MD5(string, strlen((char*)string), hash);
		} break;
		case HASH_SHA256: {
			hash_size = SHA256_DIGEST_LENGTH;
			hash = malloc(hash_size);
			SHA256(string, strlen((char*)string), hash);
		} break;
		default: {
			ERROR_OUT("invalid hash function");
		}
	}

	char *hash_hex = get_str_from_hash(hash, hash_size);

	bool ret = false;
	if (strcmp(hashed, hash_hex) == 0) {
		ret = true;
	}

	free(hash);
	return ret;
}

typedef struct {
	char *plain_src_path;
	char *hashed;
	HashFn fn;
} Args;

Args read_args(int argc, char **argv) {
	if (argc < 3) {
	error:
		ERROR_OUT("usage: %s <plain_src_path> <hashed> <SHA1 | SHA256 | MD5>(SHA1)\n", argv[0]);
	}
	HashFn fn = HASH_SHA1;
	if (argc > 3) {
		char *fn_str = argv[3];
		if (strcmp(fn_str, "SHA1") == 0) {
			fn = HASH_SHA1;
		} else if (strcmp(fn_str, "SHA256") == 0) {
			fn = HASH_SHA256;
		} else if (strcmp(fn_str, "MD5") == 0) {
			fn = HASH_MD5;
		} else {
			ERROR("invalid hash function, got %s\n", fn_str); 
			goto error;
		}
	}
	return (Args){
		.plain_src_path = argv[1],
		.hashed = argv[2],
		.fn = fn,
	};
}

#define BUFFER_SIZE (1 << 10)
static unsigned char buffer[BUFFER_SIZE];

int main(int argc, char **argv) {
	Args args = read_args(argc, argv);
	FILE *stream = fopen(args.plain_src_path, "rb+");
	if (!stream) {
		ERROR_OUT("failed to open file: %s\n", args.plain_src_path);
	}
	unsigned char *result = NULL;
	for (;;) {
		memset(buffer, '\0', BUFFER_SIZE);
		if (!fgets((char*)buffer, BUFFER_SIZE, stream)) {
			break;
		}
		buffer[strcspn((char*)buffer, "\n")] = '\0';
		if (cmp_hash(buffer, args.hashed, args.fn)) {
			result = buffer;
			break;
		}
	}
	fclose(stream);
	if (!result) {
		printf("Found it: %s\n", result);
	} else {
		printf("Couldn't find it!\n");
	}
}
