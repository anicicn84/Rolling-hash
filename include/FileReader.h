#pragma once
#include <vector>
#include <string>


// Read the file into the stream of bytes
class FileReader
{
    public:
        FileReader(const std::string& filename);
        const std::vector<uint8_t>& get_data() const;

    private:
        // This class keeps the read bytes for comparison
        std::vector<uint8_t> m_read_data;
};
