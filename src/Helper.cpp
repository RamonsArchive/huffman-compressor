#include "Helper.hpp"

void error(const char* message) {
    std::cerr << "ERROR: " << message << std::endl;
    std::exit(1);
}

// FancyInputStream implementations
FancyInputStream::FancyInputStream(const char* filename)
  : FILENAME(filename),
    input_file(filename, std::ios::binary),
    buffer(0),
    buffer_index(8)
{}

bool FancyInputStream::good() const {
    return input_file.good();
}

int FancyInputStream::filesize() const {
    return std::ifstream(FILENAME, std::ios::ate | std::ios::binary).tellg();
}

void FancyInputStream::reset() {
    input_file.clear();
    input_file.seekg(0);
    buffer = 0;
    buffer_index = 8;
}

int FancyInputStream::read_int() {
    if (buffer_index != 8) error("Attempting to read int when bitwise buffer is not empty");
    int num;
    input_file.read((char*)&num, sizeof(num));
    if (input_file.eof()) error("Not enough bytes to read the next int");
    return num;
}

int FancyInputStream::read_byte() {
    return input_file.get();
}

int FancyInputStream::read_bit() {
    if (buffer_index == 8) {
        int tmp = read_byte();
        if (tmp == -1) return -1;
        buffer = tmp;
        buffer_index = 0;
    }
    return (buffer >> (7 - buffer_index++)) & 1;
}

void FancyInputStream::alignToByte() {
    buffer = 0;
    buffer_index = 8;
}

// FancyOutputStream implementations
FancyOutputStream::FancyOutputStream(const char* filename)
  : output_file(filename, std::ios::binary),
    buffer(0),
    buffer_index(0)
{}

FancyOutputStream::~FancyOutputStream() {
    flush();
}

bool FancyOutputStream::good() const {
    return output_file.good();
}

void FancyOutputStream::write_int(int const & num) {
    if (buffer_index != 0) error("Attempting to write int when bitwise buffer is not empty");
    output_file.write((char*)&num, sizeof(num));
}

void FancyOutputStream::write_byte(unsigned char const & byte) {
    if (buffer_index != 0) error("Attempting to write byte when bitwise buffer is not empty");
    output_file.put(byte);
}

void FancyOutputStream::write_bit(int bit) {
    if (bit != 0 && bit != 1) error("Trying to write invalid bit");
    buffer |= (bit << (7 - buffer_index++));
    if (buffer_index == 8) flush_bitwise();
}

void FancyOutputStream::flush_bitwise() {
    if (buffer_index != 0) {
        output_file.put(buffer);
        buffer = 0;
        buffer_index = 0;
    }
}

void FancyOutputStream::flush() {
    flush_bitwise();
    output_file.flush();
}

// HCNode implementations
HCNode::HCNode(int count_, unsigned char symbol_)
  : count(count_), symbol(symbol_), c0(nullptr), c1(nullptr), p(nullptr)
{}

bool HCNodePtrComp::operator()(HCNode*& lhs, HCNode*& rhs) const {
    if (lhs->count != rhs->count) return lhs->count > rhs->count;
    return lhs->symbol > rhs->symbol;
}