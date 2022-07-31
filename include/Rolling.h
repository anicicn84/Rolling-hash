#pragma once

#include <string>
#include <vector>
#include "Delta.h"

const uint32_t MOD_ADLER = 65521;

/**
 *@brief Pass the iterators instead of the vec by reference
 @note Because if we want subvector, we avoid copying
 @note it is much easier to work with iterators
 @param begin iterator of vector<uint8_t>
 @param end interator of vector <uint8_t>
 * */
uint64_t adler32(const std::vector<uint8_t>::iterator& begin, const std::vector<uint8_t>::iterator& end);
long long rolling_hash_custom(const std::string& s);
long long rolling_hash_custom_bytes(const std::vector<uint8_t>::const_iterator& begin, const std::vector<uint8_t>::const_iterator& end);

int rabin_karp_strings(const std::string& needle, const std::string& haystack);
int rabin_karp_bytes(const std::vector<uint8_t>& sub, const std::vector<uint8_t>& greater);

Chunks divide_chunks(const std::vector<uint8_t>& data);