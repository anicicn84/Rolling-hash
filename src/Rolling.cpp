#include "Rolling.h"

// One of the rolling hash algorithms, just given as an option
uint64_t adler32(const std::vector<uint8_t>::iterator& begin, const std::vector<uint8_t>::iterator& end)
{
    uint64_t a = 1, b = 0;
    
    for (auto it=begin; it < end; ++it)
    {
        a = (a + static_cast<uint64_t>(*it)) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }
    
    return (b << 16) | a;
}

// Another way of doing so

const unsigned PRIME_BASE = 257;
const unsigned PRIME_MOD = 1000000007;

long long rolling_hash_custom(const std::string& s)
{
    long long ret = 0;
    for (int i = 0; i < s.size(); i++)
    {
        ret = ret*PRIME_BASE + s[i];
        ret %= PRIME_MOD; //don't overflow
    }
    return ret;
}

long long rolling_hash_custom_bytes(const std::vector<uint8_t>::const_iterator& begin, const std::vector<uint8_t>::const_iterator& end)
{
    long long ret = 0;
    for (auto it = begin; it < end; it++)
    {
        ret = ret*PRIME_BASE + *it;
        ret %= PRIME_MOD; //don't overflow
    }
    return ret;
}

// Rabin-Karp algo for strings
int rabin_karp_strings(const std::string& substring, const std::string& main_string)
{
    long long hash1 = rolling_hash_custom(substring);
    long long hash2 = 0;

    long long power = 1;
    for (int i = 0; i < substring.size(); i++)
        power = (power * PRIME_BASE) % PRIME_MOD;

    for (int i = 0; i < main_string.size(); i++)
    {
        //add the last letter
        hash2 = hash2*PRIME_BASE + main_string[i];
        hash2 %= PRIME_MOD;

        //remove the first character, if needed
        if (i >= substring.size())
        {
            hash2 -= power * main_string[i-substring.size()] % PRIME_MOD;
            if (hash2 < 0) //negative can be made positive with mod
                hash2 += PRIME_MOD;
        }

        //match?
        if (i >= substring.size()-1 && hash1 == hash2)
            return i - (substring.size()-1);
    }

    return -1;
}

// Rabin-Karp algo for bytes
int rabin_karp_bytes(const std::vector<uint8_t>& first, const std::vector<uint8_t>& second)
{

    // Find the greater of the two, smaller should be a sub of it
    const std::vector<uint8_t>& sub = first.size() > second.size() ? second : first;
    const std::vector<uint8_t>& greater = first.size() > second.size() ? first : second;

    long long hash1 = rolling_hash_custom_bytes(sub.begin(), sub.end());
    long long hash2 = 0;

    long long power = 1;
    for (auto it = sub.begin(); it < sub.end(); it++)
        power = (power * PRIME_BASE) % PRIME_MOD;

    for (auto it = greater.begin(); it < greater.end(); it++)
    {
        size_t idx = it-greater.begin();
        //add the last letter
        hash2 = hash2*PRIME_BASE + static_cast<long long>(*it);
        hash2 %= PRIME_MOD;

        //remove the first character, if needed
        if (idx >= sub.size())
        {
            hash2 -= power * greater[idx-sub.size()] % PRIME_MOD;
            if (hash2 < 0) //negative can be made positive with mod
                hash2 += PRIME_MOD;
        }

        //match?
        if (idx >= sub.size()-1 && hash1 == hash2)
            return idx - (sub.size()-1);
    }

    return -1;
}