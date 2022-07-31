#include "FileReader.h"
#include <iostream>
#include <fstream>
#include <ios>
#include <filesystem>
#include <stdexcept>

FileReader::FileReader(const std::string& filename){
    std::ifstream file(filename.c_str(), std::ios::binary);

    // Stop eating new lines in binary mode
    file.unsetf(std::ios::skipws);
    
    // Safe in this case since we have only vector of bytes stored in this class
    if (!file){
        throw std::invalid_argument( "Invalid path to file given\n" );
    }

    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    m_read_data.reserve(fileSize);

    m_read_data.insert(m_read_data.begin(),
               std::istream_iterator<uint8_t>(file),
               std::istream_iterator<uint8_t>());
}


const std::vector<uint8_t>& FileReader::get_data() const {
    return m_read_data;
}
