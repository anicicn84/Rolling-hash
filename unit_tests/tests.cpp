#include <gtest/gtest.h>
#include "Rolling.h"
#include "HashComparer.h"


std::string my_name= "Nikola Anicic";
std::vector<uint8_t> data(my_name.begin(), my_name.end());

std::string my_name1 = "1Nikola Anicic1";
std::vector<uint8_t> data1(my_name1.begin(), my_name1.end());

TEST(RollingTests, AdlerTest) {
    uint64_t adler_data_expected = 557843654;
    uint64_t adler_data1_expected = 689374504;
    EXPECT_EQ(adler32(data.begin(), data.end()), adler_data_expected); 
    EXPECT_EQ(adler32(data1.begin(), data1.end()), adler_data1_expected); 

}

TEST(RollingTests, RabinKarpStringTest){
    int expected_waldo = 31;
    EXPECT_EQ(rabin_karp_strings("waldo", "willy werther warhol wendy --> waldo <--"), expected_waldo);

    int expected_my_name_pos = 1;
    EXPECT_EQ(rabin_karp_strings("ikola anicic", "nikola anicic"), expected_my_name_pos);
}

TEST(RollingTests, RabinKarpBinaryTest){
    int expected_my_name_pos_bytes = 1;
    EXPECT_EQ(rabin_karp_bytes(data, data1), expected_my_name_pos_bytes);
}

TEST(DeltaTests, DivideChunks){
    std::vector<uint8_t> data(1000000, 1); // a million ones
    Chunks _chunks = divide_chunks(data);
    size_t expected_size = 1000000/CHUNK_SIZE + 1;
    EXPECT_EQ(_chunks.size(), expected_size);
    EXPECT_EQ(_chunks[0].size(), CHUNK_SIZE);

    size_t last_chunk_size_expected = 1000000 % CHUNK_SIZE;
    EXPECT_EQ((*(_chunks.rbegin())).size(), last_chunk_size_expected);
    EXPECT_EQ((*(_chunks.rbegin()))[0], 1); // last chunk's 1st element should still be 1, like initialized
}

TEST(HashComparer, VectorHashMethodTest){
  HashComparer hk;
  unsigned long long hashed_vec_data = hk.hash_vector(data);
  unsigned long long expected_hash_for_data = 13032904574851241750ULL;
  EXPECT_EQ(hashed_vec_data, expected_hash_for_data);

  unsigned long long hashed_vec_data1 = hk.hash_vector(data1);
  unsigned long long expected_hash_for_data1 = 15695718975171976206ULL;
  EXPECT_EQ(hashed_vec_data1, expected_hash_for_data1);
}

TEST(Main, GetSegmentsTest) {
    StreamBreaker sb;
    auto result = sb.getSegments(data, data.size());
    EXPECT_EQ(result.size(), 1);

    std::vector<uint8_t> some_other_data(20, 0); // 1'000'000 zeroes
    some_other_data[10] = 10;
    some_other_data[11] = 11;
    some_other_data[12] = 12;
    some_other_data[13] = 13;
    auto result_other = sb.getSegments(some_other_data, some_other_data.size());
    EXPECT_EQ(result_other.size(), 1);
}