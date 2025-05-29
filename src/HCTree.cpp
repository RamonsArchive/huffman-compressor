#include "HCTree.hpp"
#include "Helper.hpp"
#include <algorithm>

// Helper to delete entire tree recursively
static void deleteAll(HCNode* node) {
    if (!node) return;
    deleteAll(node->c0);
    deleteAll(node->c1);
    delete node;
}

HCTree::HCTree()
  : root(nullptr), leaves(256, nullptr)
{}

HCTree::~HCTree() {
    deleteAll(root);
}

void HCTree::build(const std::vector<int>& freqs) {
    std::priority_queue<HCNode*, std::vector<HCNode*>, HCNodePtrComp> pq;
    for (int sym = 0; sym < 256; sym++) {
        if (freqs[sym] > 0) {
            leaves[sym] = new HCNode(freqs[sym], (unsigned char)sym);
            pq.push(leaves[sym]);
        }
    }
    if (pq.empty()) return;
    if (pq.size() == 1) {
        HCNode* only = pq.top(); pq.pop();
        root = new HCNode(only->count, only->symbol);
        root->c0 = only;
        only->p = root;
        return;
    }
    while (pq.size() > 1) {
        HCNode* a = pq.top(); pq.pop();
        HCNode* b = pq.top(); pq.pop();
        HCNode* parent = new HCNode(a->count + b->count,
                                    std::min(a->symbol, b->symbol));
        parent->c0 = a; parent->c1 = b;
        a->p = parent; b->p = parent;
        pq.push(parent);
    }
    root = pq.top();
}

void HCTree::encode(unsigned char symbol, FancyOutputStream& out) const {
    HCNode* node = leaves[symbol];
    if (!node) error("HCTree::encode symbol not in tree");
    std::vector<int> bits;
    while (node->p) {
        bits.push_back(node->p->c0 == node ? 0 : 1);
        node = node->p;
    }
    for (auto it = bits.rbegin(); it != bits.rend(); ++it) {
        out.write_bit(*it);
    }
}

unsigned char HCTree::decode(FancyInputStream& in) const {
    HCNode* node = root;
    if (!node) error("HCTree::decode on empty tree");
    while (node->c0 && node->c1) {
        int bit = in.read_bit();
        if (bit < 0) error("HCTree::decode unexpected EOF");
        node = (bit == 0 ? node->c0 : node->c1);
    }
    return node->symbol;
}

void HCTree::writeHeader(FancyOutputStream& out) const {
    writeTree(root, &out);
    out.flush_bitwise();
}

void HCTree::writeTree(HCNode* node, FancyOutputStream* out) {
    if (!node) return;
    if (!node->c0 && !node->c1) {
        out->write_bit(1);
        for (int b = 7; b >= 0; --b)
            out->write_bit((node->symbol >> b) & 1);
    } else {
        out->write_bit(0);
        writeTree(node->c0, out);
        writeTree(node->c1, out);
    }
}

void HCTree::readHeader(FancyInputStream& in) {
    root = readTree(in);
}

HCNode* HCTree::readTree(FancyInputStream& in) {
    int bit = in.read_bit();
    if (bit < 0) error("Unexpected EOF in tree header");
    if (bit == 1) {
        unsigned char sym = 0;
        for (int b = 7; b >= 0; --b) {
            int x = in.read_bit();
            if (x < 0) error("EOF in reading symbol bits");
            sym |= (x << b);
        }
        return new HCNode(0, sym);
    } else {
        HCNode* left = readTree(in);
        HCNode* right = readTree(in);
        HCNode* parent = new HCNode(0, std::min(left->symbol, right->symbol));
        parent->c0 = left; parent->c1 = right;
        left->p = parent; right->p = parent;
        return parent;
    }
}