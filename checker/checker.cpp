#include "checker.hpp"
#include <cstdlib>
#include <cctype>
#include <openssl/evp.h>
#include <openssl/md5.h>

file_checker::file_checker()
{
	ctx = EVP_MD_CTX_new();
	memset(digest, 0, sizeof(digest));
	mdlen = 0;
}

file_checker::file_checker(std::string filename, uint64_t size)
{
	fin.open(filename, std::ios_base::in | std::ios_base::binary);
	file_size = size;
	ctx = EVP_MD_CTX_new();
	memset(digest, 0, sizeof(digest));
	mdlen = 0;
}

file_checker::~file_checker()
{
	EVP_MD_CTX_free(ctx);
	fin.close();
}

void file_checker::md5_digest()
{
	if (!fin.is_open())
	{
		return;
	}
	EVP_DigestInit_ex(ctx, EVP_md5(), NULL);
	char rdbuf[4096];
	while (!fin.eof())
	{
		fin.read(rdbuf, 4096);
		EVP_DigestUpdate(ctx, rdbuf, fin.gcount());
	}
	// fin.close();
	EVP_DigestFinal(ctx, digest, &mdlen);
	// EVP_MD_CTX_free(ctx);
	return;
}

bool file_checker::check_digest(uint8_t *dig, uint32_t dig_len)
{
	return memcmp(dig, digest, dig_len) == 0;
}

bool file_checker::check_digest(std::string dig, uint32_t dig_len)
{
	uint8_t cdig[EVP_MAX_MD_SIZE];
	if (dig.length() & 1)
	{
		return false;
	}
	for (int i = 0; i < dig.length(); i += 2)
	{
		char digstr[3];
		digstr[0] = dig[i];
		digstr[1] = dig[i + 1];
		digstr[2] = 0;
		cdig[i >> 1] = strtol(digstr, NULL, 16);
	}
	return memcmp(cdig, digest, dig_len) == 0;
}
