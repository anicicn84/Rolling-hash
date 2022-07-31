#include "Delta.h"

// Helper method to divide the vector of bytes into smaller Chunks.
Chunks divide_chunks(const std::vector<uint8_t>& data){
    Chunks ret;
    ret.reserve(data.size() / CHUNK_SIZE + 1); // add just one chunk just in case of division rounding

    for (size_t data_chunk_pos = 0; data_chunk_pos < data.size(); data_chunk_pos += CHUNK_SIZE) {
        if (data_chunk_pos + CHUNK_SIZE > data.size()) {
            ret.emplace_back(std::vector<uint8_t>(data.begin() + data_chunk_pos, data.end()));
            break;
        }
        ret.emplace_back(std::vector<uint8_t>(data.begin() + data_chunk_pos, data.begin() + data_chunk_pos + CHUNK_SIZE));
    }
    return ret;
}