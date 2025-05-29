#ifndef HELPER_HPP
#define HELPER_HPP

#include <iostream>
#include <fstream>
#include <vector>

// error helper
void error(const char* message);

// bit-wise input
class FancyInputStream {
  const char* FILENAME;
  std::ifstream input_file;
  unsigned char buffer;
  int buffer_index;
public:
  FancyInputStream(const char* filename);
  bool good() const;
  int filesize() const;
  void reset();
  int read_int();
  int read_byte();
  int read_bit();
  void alignToByte();
};

// bit-wise output
class FancyOutputStream {
  std::ofstream output_file;
  unsigned char buffer;
  int buffer_index;
public:
  FancyOutputStream(const char* filename);
  ~FancyOutputStream();
  bool good() const;
  void write_int(int const & num);
  void write_byte(unsigned char const & byte);
  void write_bit(int bit);
  void flush_bitwise();
  void flush();
};

// node for Huffman tree
struct HCNode {
  int count;
  unsigned char symbol;
  HCNode* c0;
  HCNode* c1;
  HCNode* p;
  HCNode(int count, unsigned char symbol);
};

// comparator for priority queue
struct HCNodePtrComp {
  bool operator()(HCNode*& lhs, HCNode*& rhs) const;
};

#endif // HELPER_HPP