#include "Helper.hpp"
#include "HCTree.hpp"
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        error("Usage: decompress <in> <out>");
    }

    FancyInputStream in(argv[1]);
    FancyOutputStream out(argv[2]);

    // dont want to read_int on empty file
    int size = in.filesize();
    if (size == 0) {
        return 0;
    }

    // read original size; if zero, done
    int N = in.read_int();
    if (N == 0) {
        return 0;
    }

    // rebuild tree from header
    HCTree huff;
    huff.readHeader(in);

    // align to next byte boundary before reading bits
    in.alignToByte();

    // decode N symbols
    for (int i = 0; i < N; i++) {
        unsigned char c = huff.decode(in);
        out.write_byte(c);
    }
    out.flush();
    return 0;
}