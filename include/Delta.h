#pragma once
#include <vector>
#include <iostream>

using ChunkData = std::vector<uint8_t>;
using Chunks = std::vector<ChunkData>;
using ChunkHashesList = std::vector<uint32_t>;


// used for the 1st (unfinished) solution
struct StreamSegment
{
    StreamSegment(long _offset, long _length, unsigned long long _hash = 0) :
         offset(_offset)
       , length(_length)
       , hash(_hash) 
    {}
    // How far into the strem we found the chunk
    long offset;

    // The length of the chunk
    long length;

    /// Strong hash for the chunk
    unsigned long long hash;
};

inline std::ostream& operator<<(std::ostream& out, const StreamSegment& strSegment) {
    return out << "offset: " << strSegment.offset << ", length:" << strSegment.length <<
     ", hash: " << strSegment.hash << std::endl;
}



constexpr size_t CHUNK_SIZE = ( 1 << 16 ) - 1; // 64k chunks
Chunks divide_chunks(const std::vector<uint8_t>& data);