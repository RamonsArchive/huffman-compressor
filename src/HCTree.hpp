#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <queue>
#include <vector>
#include "Helper.hpp"

class HCTree {
private:
    HCNode* root;
    std::vector<HCNode*> leaves;

    static void writeTree(HCNode* node, FancyOutputStream* out);
    HCNode* readTree(FancyInputStream & in);

public:
    HCTree();
    ~HCTree();

    void build(const std::vector<int>& freqs);
    void encode(unsigned char symbol, FancyOutputStream & out) const;
    unsigned char decode(FancyInputStream & in) const;

    void writeHeader(FancyOutputStream& out) const;
    void readHeader(FancyInputStream & in);
};

#endif // HCTREE_HPP