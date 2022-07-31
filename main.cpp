#include <iostream>
#include "Rolling.h"
#include "HashComparer.h"
#include <algorithm>
#include <limits>
#include <random>


void init(std::vector<int>& hashes) {
    hashes.reserve(256);

    // We use static in order to instantiate the random engine
    // and the distribution once only.
    // It may provoke some thread-safety issues.

    static std::uniform_int_distribution<int> distribution(
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max());
    static std::default_random_engine generator;

    std::generate(hashes.begin(), hashes.end(), []() { return distribution(generator); });
}

void resetHash(int& currentHash, int blockSize, const std::vector<int>& hashes, const std::vector<uint8_t>& b, int pos) {
    currentHash = 0;
    for (int i = blockSize; i > 0; i--) {
        currentHash = (currentHash << 1 | currentHash >> 31) ^ hashes[b[pos - i] + 128];
    }
}

void hashRoll(uint8_t oldByte, uint8_t newByte, const std::vector<int>& hashes, int& currentHash, int bhRotate, int bhRotateComp) {
    int oldHash = hashes[oldByte + 128];
    currentHash = (currentHash << 1 | currentHash >> 31) ^
            (oldHash << bhRotate | oldHash >> bhRotateComp) ^
            hashes[newByte + 128];
}

// The same approach as with StreamBreaker where we do 
// (hash | mask ) == hash
// in order to see when we have the same hash, that means we can split the files up in chunks since we found the same hash.
// That means the same data is encountered.
bool isToBeSplit(int currentHash) {
    // We want 23 bits to be 0
    return (currentHash & 0b11111111111111111111111) == 0;
}

int main() {
    std::vector<uint8_t> some_other_data(2000, 0); // 1'000'000 zeroes
    some_other_data[10] = 10;
    some_other_data[11] = 11;
    some_other_data[12] = 12;
    some_other_data[13] = 13;

    // Alter some data above and below
    
    some_other_data[101] = 10;
    some_other_data[102] = 11;
    some_other_data[103] = 12;
    some_other_data[104] = 13;

    int BLOCK_SIZE = 63;
    int BH_ROTATE = BLOCK_SIZE % 32;
    int BH_ROTATE_COMP = 32 - BH_ROTATE;
    std::vector<int> HASHES;
    int currentHash;

    init(HASHES);

    // Start at the position where we have enough bytes for BLOCK_SIZE
    int currentPos = BLOCK_SIZE;
    int currentStartPos = 0;
    resetHash(currentHash, BLOCK_SIZE, HASHES, some_other_data, currentPos);

    while (currentPos < some_other_data.size()) {
        hashRoll(some_other_data[currentPos - BLOCK_SIZE], some_other_data[currentPos], HASHES, currentHash, BH_ROTATE, BH_ROTATE_COMP);

        if (isToBeSplit(currentHash)) {
            currentStartPos = currentPos + 1;
            currentPos += BLOCK_SIZE + 1;
            resetHash(currentHash, BLOCK_SIZE, HASHES, some_other_data, currentPos);
        } else {
            currentPos++;
        }
    }
    currentPos = some_other_data.size() - 1;
    std::cout << currentPos << "," << currentPos - currentStartPos + 1 << std::endl;

    return 0;
}
