#pragma once
#include "FileReader.h"
#include "Delta.h"
#include <algorithm>

class HashComparer
{
    public:
        HashComparer() = default;
        //StreamSegment compare(const std::shared_ptr<FileReader>& first, const std::shared_ptr<FileReader>& second);
        unsigned long long hash_vector(std::vector<uint8_t> const& vec) const;
        unsigned long long hash_vector_iters(std::vector<uint8_t>::const_iterator begin, std::vector<uint8_t>::const_iterator end) const;
};

class StreamBreaker
{
private:
    int width = 64;  //--> the # of bytes in the window
    long seed = 2273;  //--> a our hash seed
    long mask = ( 1 << 16 ) - 1;  //--> a hash seive: 16 gets you ~64k chunks
    int bufferSize = 64 * 1024;
    HashComparer hash_comparer;

public:
    std::vector<StreamSegment> getSegments(const std::vector<uint8_t>& stream, long length);
};