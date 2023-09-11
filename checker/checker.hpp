#ifndef _CHECKER_CPP_
#define _CHECKER_CPP_

#include <fstream>
#include <string>
#include <string.h>
#include <openssl/evp.h>

struct file_checker
{
	std::fstream fin;
	EVP_MD_CTX *ctx;
	uint8_t digest[EVP_MAX_MD_SIZE];
	uint32_t mdlen;
	uint64_t file_size;

	file_checker();

	file_checker(std::string, uint64_t);

	~file_checker();

	void md5_digest();

	bool check_digest(uint8_t *dig, uint32_t dig_len);

	bool check_digest(std::string, uint32_t dig_len);
};

#endif
