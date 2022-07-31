#include "HashComparer.h"

unsigned long long HashComparer::hash_vector(std::vector<uint8_t> const& vec) const {
  std::size_t seed = vec.size();
  for(auto& i : vec) {
    seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
  return seed;
}

unsigned long long HashComparer::hash_vector_iters(std::vector<uint8_t>::const_iterator begin, std::vector<uint8_t>::const_iterator end) const {
  std::size_t seed = end - begin;
  for (auto it = begin; it < end; it++){
      seed ^= *it + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
  return seed;
}


std::vector<StreamSegment> StreamBreaker::getSegments(const std::vector<uint8_t>& stream, long length)
{
      long maxSeed = seed; //--> will be prime^width after initialization (sorta)
      auto buffer = std::vector<uint8_t> {};
      buffer.reserve(bufferSize);

      auto circle = std::vector<uint8_t> {};  //--> the circular queue: input to the hash functions
      circle.reserve(width);
      long hash = 0L;  //--> our rolling hash
      unsigned int circleIndex = 0;  //--> index into circular queue
      long last = 0L;  //--> last place we started a new segment
      long pos = 0L;  //--> the position we're at in the range of stream we're reading

      //--> initialize maxSeed...
      for (unsigned int i = 0; i < width; i++ ) maxSeed *= maxSeed;

      std::vector<StreamSegment> ret_val_segments{};

      long buffer_size = std::min(static_cast<long>(stream.size()), length - pos);
      for (unsigned int i = 0; i < buffer_size; i++ )
      {
          pos++;
          hash = stream[i] + ((hash - ( maxSeed * circle[circleIndex])) * seed);
          circle[circleIndex++] = stream[i];
          if (circleIndex == width) circleIndex = 0;
          if (((hash | mask ) == hash) || (pos == length))  //--> match or EOF
          {
              //--> apply the strong hash to the remainder of the bytes in the circular queue...
              unsigned long long hash_val = hash_comparer.hash_vector_iters(circle.cbegin(), circleIndex == 0 ? circle.cbegin() + width : circle.cbegin() + circleIndex );

              //--> return the results to the caller...
              ret_val_segments.emplace_back(last, pos - last, hash_val);
              last = pos;

              //--> reset the hashes...
              hash = 0;
              for (unsigned int j = 0; j < width; j++ ) circle[j] = 0;
              circleIndex = 0;
          }

          // TODO HERE it needs to be improved, so this is not the way to go, not finished yet.
          
          // else
          // {
          //     if ( circleIndex == 0 ){
          //         unsigned long long hash_val = hash_comparer.hash_vector(circle.cbegin(), circle.cbegin() + width);
          //     }
          // }
      }
      return ret_val_segments;
  }