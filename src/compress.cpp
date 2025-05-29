#include "Helper.hpp"
#include "HCTree.hpp"
#include <vector>
using namespace std;

void getFrequency(FancyInputStream & in, vector<int>& freqs) {
    in.reset();
    int b;
    while ((b = in.read_byte()) != -1) {
        freqs[b]++;
    }
    in.reset();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        error("Usage: compress <in> <out>");
    }

    FancyInputStream in(argv[1]);
    FancyOutputStream out(argv[2]);

    // write original size; if zero, nothing more to do
    int originalSize = in.filesize();
    if (originalSize == 0) {
        return 0;
    }
    out.write_int(originalSize);

    // build frequency table
    vector<int> freqs(256, 0);
    getFrequency(in, freqs);

    // build Huffman tree
    HCTree huff;
    huff.build(freqs);

    // write tree header
    huff.writeHeader(out);

    // encode body
    int b;
    while ((b = in.read_byte()) != -1) {
        huff.encode((unsigned char)b, out);
    }
    out.flush();
    return 0;
}